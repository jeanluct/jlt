#ifndef JLT_PROMPT_HPP
#define JLT_PROMPT_HPP

namespace jlt {

template<class T>
T read_number(const std::string& prompt,
	      const T& nmin, const T& nmax, const T& ndef,
	      std::istream& strm = std::cin);

bool yesno(const std::string& prompt,
	   const bool def = true, std::istream& strm = std::cin);


template<class T>
T read_number(const std::string& prompt,
	      const T& nmin, const T& nmax, const T& ndef,
	      std::istream& strm)
{
  const std::string err = "Out of range.  Try again.";
  std::string str;

  if (nmin == nmax) return nmin;

  if (nmin > nmax)
    {
      std::cerr << "Error in read_number: nmin > nmax.\n";
      exit(-1);
    }

  bool def = true;
  if (ndef < nmin || ndef > nmax) def = false;

  bool goodinput = true;
  T n = 0;
  do
    {
      if (!goodinput) std::cerr << err << std::endl;
      std::cout << prompt << " (" << nmin << "-" << nmax << ")";
      if (def) std::cout << " [" << ndef << "]";
      std::cout << ": ";
      std::getline(strm,str);
      if (str == "") { n = ndef; break; }
      std::istringstream istr(str,std::istringstream::in);
      istr >> n;
      if (n >= nmin && n <= nmax)
	goodinput = true;
      else
	goodinput = false;
    }
  while (!goodinput);

  return n;
}

bool yesno(const std::string& prompt, const bool def, std::istream& strm)
{
  const std::string err = "I'm not sure what you mean.  Try again.";
  std::string yn;

  bool goodinput = true;
  do
    {
      if (!goodinput) std::cerr << err << std::endl;
      std::cout << prompt << " (y/n) ";
      std::cout << "[" << (def ? "Y" : "N") << "]? ";
      std::getline(strm,yn);
      if (yn == "") yn = (def ? "y" : "n");
      std::transform(yn.begin(),yn.end(),yn.begin(),tolower);
      if (yn == "y" || yn == "n" || yn == "yes" || yn == "no")
	goodinput = true;
      else
	goodinput = false;
    }
  while (!goodinput);

  return (yn == "y" || yn == "yes");
}

} // namespace jlt

#endif // JLT_PROMPT_HPP
