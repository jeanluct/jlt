//
// Copyright (c) 2004-2026 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

#ifndef JLT_DISPLAY_TASK_HPP
#define JLT_DISPLAY_TASK_HPP

//
// display_task.hpp
//

// Helper class to display messages at the beginning and end of a task,
// with support for log levels, color control, and RAII scope guards.

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

// Check for std::source_location support (C++20)
#if __has_include(<version>)
#include <version>
#endif
#if defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907L
#include <source_location>
#define JLT_HAS_SOURCE_LOCATION 1
#else
#define JLT_HAS_SOURCE_LOCATION 0
#endif

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

namespace jlt {

// Log levels for filtering output
enum class LogLevel { Silent, Error, Warn, Info, Debug };

//
// Custom streambuf that wraps another streambuf.
// When tracking is enabled, inserts a newline before the first write
// and sets a flag to indicate output was interrupted.
//
class tracking_streambuf : public std::streambuf {
  std::streambuf* wrapped;
  bool* interrupted_flag;
  bool* first_write_flag;  // true until first write after begin()

  // Insert newline before first write if we're in a task
  void check_first_write() {
    if (first_write_flag && *first_write_flag) {
      *first_write_flag = false;
      if (interrupted_flag) {
        *interrupted_flag = true;
      }
      wrapped->sputc('\n');  // newline before intermediate output
    }
  }

protected:
  int overflow(int c) override {
    if (c != EOF) {
      check_first_write();
      return wrapped->sputc(c);
    }
    return c;
  }

  std::streamsize xsputn(const char* s, std::streamsize n) override {
    if (n > 0) {
      check_first_write();
    }
    return wrapped->sputn(s, n);
  }

  int sync() override {
    return wrapped->pubsync();
  }

public:
  tracking_streambuf(std::streambuf* buf)
    : wrapped(buf), interrupted_flag(nullptr), first_write_flag(nullptr) {}

  void start_tracking(bool* interrupted, bool* first_write) {
    interrupted_flag = interrupted;
    first_write_flag = first_write;
  }

  void stop_tracking() {
    interrupted_flag = nullptr;
    first_write_flag = nullptr;
  }
};

class display_task : public std::ostream {
  static constexpr int default_align_length = 70;

  tracking_streambuf tracking_buf;
  int align_length;
  int mesg_length;
  bool muted;
  bool color_enabled;
  bool interrupted;   // true if output occurred after begin()
  bool first_write;   // true until first write after begin()
  bool show_source;   // true to show source location in error/warn
  LogLevel level;

public:
  // ANSI color codes
  struct colors {
    static constexpr auto
      normal  = "\x1B[0m",
      red     = "\x1B[31m",
      green   = "\x1B[32m",
      yellow  = "\x1B[33m",
      blue    = "\x1B[34m",
      magenta = "\x1B[35m",
      cyan    = "\x1B[36m",
      white   = "\x1B[37m";
  };

  // For backward compatibility
  static constexpr auto normal  = colors::normal;
  static constexpr auto red     = colors::red;
  static constexpr auto green   = colors::green;
  static constexpr auto yellow  = colors::yellow;
  static constexpr auto blue    = colors::blue;
  static constexpr auto magenta = colors::magenta;
  static constexpr auto cyan    = colors::cyan;
  static constexpr auto white   = colors::white;

  // Configurable output strings
  struct strings {
#if __cplusplus >= 201703L
    static inline std::string
#else
    static constexpr auto
#endif
      task_suffix  = "...",
      task_done    = "ok",
      error_prefix = "Error: ",
      warn_prefix  = "Warning: ",
      debug_prefix = "[debug] ";
  };

  //
  // Constructor
  //
  // ostr_: output stream (default: std::cerr)
  // muted_: if true, suppress all output
  // auto_detect_color: if true, enable colors only if output is a TTY
  //
  explicit display_task(std::ostream& ostr_ = std::cerr,
                        bool muted_ = false,
                        bool auto_detect_color = true)
    : std::ostream(&tracking_buf),
      tracking_buf(ostr_.rdbuf()),
      align_length(default_align_length),
      mesg_length(-1),
      muted(muted_),
      color_enabled(true),
      interrupted(false),
      first_write(false),
      show_source(true),
      level(LogLevel::Info)
  {
    if (auto_detect_color) {
      // Detect if output is a TTY
      if (&ostr_ == &std::cout) {
        color_enabled = isatty(fileno(stdout));
      } else if (&ostr_ == &std::cerr) {
        color_enabled = isatty(fileno(stderr));
      } else {
        // For other streams (like ostringstream), disable color
        color_enabled = false;
      }
    }
  }

  //
  // Task begin/end methods
  //

  void begin(const std::string& mesg) {
    if (muted || level == LogLevel::Silent) return;

    // Print the task message first (without tracking)
    *this << color(colors::cyan) << mesg << strings::task_suffix << color(colors::normal) << std::flush;

    // In case there are newlines, split into lines and remember
    // length of final line only.
    std::vector<std::string> lines = split_lines(mesg);
    mesg_length = static_cast<int>(lines.back().length()
#if __cplusplus >= 201703L
				   + strings::task_suffix.length());
#else
				   + std::string(strings::task_suffix).length());
#endif

    // Now enable tracking for subsequent output
    interrupted = false;
    first_write = true;
    tracking_buf.start_tracking(&interrupted, &first_write);
  }

  void end() {
    if (muted || level == LogLevel::Silent) return;

    if (mesg_length < 0) {
      throw std::logic_error("display_task::end() called without begin()");
    }

    // Stop tracking before we print the status
    tracking_buf.stop_tracking();

    // If output occurred after begin(), status goes on its own line
    // Otherwise it stays on the same line as the message
    int padding = interrupted ? align_length : std::max(1, align_length - mesg_length);
    *this << std::string(padding, ' ');
    *this << color(colors::cyan) << strings::task_done << color(colors::normal) << '\n';

    mesg_length = -1;
    interrupted = false;
    first_write = false;
  }

  void end_with_status(const std::string& status, const char* status_color = colors::cyan) {
    if (muted || level == LogLevel::Silent) return;

    if (mesg_length < 0) {
      throw std::logic_error("display_task::end_with_status() called without begin()");
    }

    // Stop tracking before we print the status
    tracking_buf.stop_tracking();

    // If output occurred after begin(), status goes on its own line
    // Otherwise it stays on the same line as the message
    int padding = interrupted ? align_length : std::max(1, align_length - mesg_length);
    *this << std::string(padding, ' ');
    *this << color(status_color) << status << color(colors::normal) << '\n';

    mesg_length = -1;
    interrupted = false;
    first_write = false;
  }

  //
  // Log level methods
  //

  void error(const std::string& mesg) {
    if (muted || level == LogLevel::Silent) return;
    *this << color(colors::red) << strings::error_prefix << color(colors::normal) << mesg << '\n';
  }

  void warn(const std::string& mesg) {
    if (muted || level < LogLevel::Warn) return;
    *this << color(colors::yellow) << strings::warn_prefix << color(colors::normal) << mesg << '\n';
  }

  void info(const std::string& mesg) {
    if (muted || level < LogLevel::Info) return;
    *this << mesg << '\n';
  }

  void debug(const std::string& mesg) {
    if (muted || level < LogLevel::Debug) return;
    *this << color(colors::magenta) << strings::debug_prefix << color(colors::normal) << mesg << '\n';
  }

#if JLT_HAS_SOURCE_LOCATION
  // Overloads with automatic source location (C++20)
  void error(const std::string& mesg,
             const std::source_location& loc) {
    if (muted || level == LogLevel::Silent) return;
    *this << color(colors::red) << strings::error_prefix << color(colors::normal);
    if (show_source) *this << loc.function_name() << ": ";
    *this << mesg << '\n';
  }

  void warn(const std::string& mesg,
            const std::source_location& loc) {
    if (muted || level < LogLevel::Warn) return;
    *this << color(colors::yellow) << strings::warn_prefix << color(colors::normal);
    if (show_source) *this << loc.function_name() << ": ";
    *this << mesg << '\n';
  }

  void debug(const std::string& mesg,
             const std::source_location& loc) {
    if (muted || level < LogLevel::Debug) return;
    *this << color(colors::magenta) << strings::debug_prefix << color(colors::normal);
    if (show_source) *this << loc.function_name() << ": ";
    *this << mesg << '\n';
  }
#endif

  //
  // Configuration methods
  //

  void set_level(LogLevel lvl) { level = lvl; }
  LogLevel get_level() const { return level; }

  void set_color_enabled(bool enabled) { color_enabled = enabled; }
  bool get_color_enabled() const { return color_enabled; }

  void set_show_source(bool enabled) { show_source = enabled; }
  bool get_show_source() const { return show_source; }

  void set_align_length(int len) { align_length = len; }
  int get_align_length() const { return align_length; }

  void mute() { muted = true; }
  void unmute() { muted = false; }
  bool is_muted() const { return muted; }

private:
  // Return color code if colors enabled, otherwise empty string
  const char* color(const char* code) const {
    return color_enabled ? code : "";
  }

  // Utility function for splitting a string at newlines
  static std::vector<std::string> split_lines(const std::string& str) {
    std::vector<std::string> lines;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line, '\n')) {
      lines.push_back(line);
    }
    if (lines.empty()) {
      lines.push_back("");
    }
    return lines;
  }
};


//
// RAII scope guard for automatic end() on scope exit
//
// Usage:
//   {
//     jlt::scoped_task task(disp, "Loading data");
//     // ... do work ...
//   } // automatically calls disp.end()
//
class scoped_task {
  display_task& dt;
  bool ended;

public:
  scoped_task(display_task& d, const std::string& mesg)
    : dt(d), ended(false)
  {
    dt.begin(mesg);
  }

  // End early with default "ok" status
  void end() {
    if (!ended) {
      dt.end();
      ended = true;
    }
  }

  // End early with custom status
  void end_with_status(const std::string& status,
                       const char* color = display_task::colors::cyan) {
    if (!ended) {
      dt.end_with_status(status, color);
      ended = true;
    }
  }

  ~scoped_task() {
    if (!ended) {
      try {
        dt.end();
      } catch (...) {
        // Suppress exceptions in destructor
      }
    }
  }

  // Non-copyable
  scoped_task(const scoped_task&) = delete;
  scoped_task& operator=(const scoped_task&) = delete;
};

} // namespace jlt

//
// Convenience macros for automatic source location capture.
// Usage: JLT_ERROR(disp, "message") or JLT_WARN(disp, "message")
//
#if JLT_HAS_SOURCE_LOCATION
#define JLT_ERROR(disp, msg) (disp).error((msg), std::source_location::current())
#define JLT_WARN(disp, msg)  (disp).warn((msg), std::source_location::current())
#define JLT_DEBUG(disp, msg) (disp).debug((msg), std::source_location::current())
#else
#define JLT_ERROR(disp, msg) (disp).error(msg)
#define JLT_WARN(disp, msg)  (disp).warn(msg)
#define JLT_DEBUG(disp, msg) (disp).debug(msg)
#endif

#endif // JLT_DISPLAY_TASK_HPP
