#ifndef Services_Exception_hh
#define Services_Exception_hh
#include<stdexcept>

namespace fc {

///
/// Structure Exception 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-05
/// Wrapper for std library exception functions
///

class Exception : public std::exception
{
public:
   explicit Exception(const std::string& message) : _what(message) {}
   const char* what() const noexcept override { return _what.c_str(); }
private:
   std::string _what;
};
} // end namespace fc

#endif // Services_Exception_hh
