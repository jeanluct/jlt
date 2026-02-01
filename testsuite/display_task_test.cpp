//
// Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
//
// See the file LICENSE for copying permission.
//

//
// Examples demonstrating jlt::display_task usage.
//

#include "jlt/display_task.hpp"
#include <chrono>
#include <thread>

// Simulate work
void do_work(int ms = 100) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
  // Create display_task writing to stdout
  jlt::display_task disp(std::cout);

  //
  // Example 1: Basic begin/end usage
  //
  disp << "\n=== Example 1: Basic begin/end ===\n\n";

  disp.begin("Loading configuration");
  do_work(50);
  disp.end();

  disp.begin("Initializing subsystems");
  do_work(100);
  disp.end();

  //
  // Example 2: Interrupted output (reprints message for alignment)
  //
  disp << "\n=== Example 2: Interrupted output ===\n\n";

  disp.begin("Processing data");
  do_work(50);
  disp << "  [info] processed 1000 records\n";
  disp << "  [info] found 42 matches\n";
  do_work(50);
  disp.end();  // reprints "Processing data..." before aligned "ok"

  disp.begin("Without interruption");
  do_work(50);
  disp.end();  // normal aligned "ok"

  //
  // Example 3: RAII scope guard (automatic end)
  //
  disp << "\n=== Example 3: RAII scope guard ===\n\n";

  {
    jlt::scoped_task task(disp, "Processing data");
    do_work(150);
    // end() called automatically when task goes out of scope
  }

  {
    jlt::scoped_task task(disp, "Computing results");
    do_work(50);
    task.end();  // Can also end early
    // Destructor won't call end() again
  }

  //
  // Example 4: Custom end status
  //
  disp << "\n=== Example 4: Custom status ===\n\n";

  disp.begin("Checking network connection");
  do_work(50);
  disp.end_with_status("connected", jlt::display_task::colors::green);

  disp.begin("Validating input data");
  do_work(50);
  disp.end_with_status("skipped", jlt::display_task::colors::yellow);

  {
    jlt::scoped_task task(disp, "Optional feature check");
    do_work(30);
    task.end_with_status("disabled", jlt::display_task::colors::magenta);
  }

  //
  // Example 5: Log levels
  //
  disp << "\n=== Example 5: Log levels ===\n\n";

  disp.info("This is an informational message");
  disp.warn("This is a warning message");
  disp.error("This is an error message");
  disp.debug("This debug message won't show (level is Info)");

  disp << "\nSetting level to Debug:\n";
  disp.set_level(jlt::LogLevel::Debug);
  disp.debug("Now this debug message is visible");

  disp << "\nSetting level to Warn (Info suppressed):\n";
  disp.set_level(jlt::LogLevel::Warn);
  disp.info("This info message won't show");
  disp.warn("But warnings still show");

  // Reset to Info
  disp.set_level(jlt::LogLevel::Info);

  //
  // Example 5: Color control
  //
  disp << "\n=== Example 5: Color control ===\n\n";

  disp.begin("With colors enabled");
  do_work(30);
  disp.end();

  disp.set_color_enabled(false);
  disp.begin("With colors disabled");
  do_work(30);
  disp.end();

  disp.set_color_enabled(true);  // Re-enable

  //
  // Example 6: Muting
  //
  disp << "\n=== Example 6: Muting ===\n\n";

  disp.begin("This task is visible");
  do_work(30);
  disp.end();

  disp.mute();
  disp.begin("This task is muted (you won't see this)");
  do_work(30);
  disp.end();
  disp.unmute();

  disp.begin("Back to visible");
  do_work(30);
  disp.end();

  //
  // Example 7: Free-form output (inherited from std::ostream)
  //
  disp << "\n=== Example 7: Stream output ===\n\n";
  disp << "You can use display_task as a regular ostream.\n";
  disp << "Values: " << 42 << ", " << 3.14159 << ", " << "hello" << '\n';

  //
  // Example 8: Writing to a string stream
  //
  disp << "\n=== Example 8: String stream capture ===\n\n";

  std::ostringstream oss;
  jlt::display_task string_disp(oss, false, false);  // no auto-detect, no color
  string_disp.begin("Captured task");
  string_disp.end();
  string_disp.warn("Captured warning");

  disp << "Captured output:\n" << oss.str();

  disp << "\n=== Done ===\n\n";

  return 0;
}
