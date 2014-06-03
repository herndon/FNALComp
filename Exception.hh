#ifndef Exception_hh
#define Exception_hh
#include<stdexcept>

struct Exception : public std::exception
{
   std::string s;
   Exception(std::string ss) : s(ss) {}
   ~Exception() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};
#endif // Exception_hh
