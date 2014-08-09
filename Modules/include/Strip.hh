#ifndef Modules_Strip_hh
#define Modules_Strip_hh

namespace fc {
  // An instance of Strip represents a strip with a signal. We use small
  // integers for data members because their range is small. Our c'tor
  // has to use what the surrounding code uses.
  struct Strip {
    Strip(int i, int adc) : id(i), cnt(adc) { }
    unsigned short id;
    unsigned short cnt;
  };
}

#endif
