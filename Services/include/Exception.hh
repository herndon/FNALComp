#ifndef Exception_hh
#define Exception_hh
#include<stdexcept>

namespace fc {

///
/// Structure Exception 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-05
/// Wrapper for std library exception functions
///

struct Exception : public std::exception
{
   std::string s;
   Exception(std::string ss) : s(ss) {}
   ~Exception() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};
} // end namespace fc

#endif // Exception_hh
