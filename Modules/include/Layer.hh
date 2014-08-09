#ifndef Modules_Layer_hh
#define Modules_Layer_hh

#include <vector>
#include "Modules/include/Strip.hh"

namespace fc {
  struct Layer {
    explicit Layer(int lyr) : strips(), layer(lyr) { }
    std::vector<Strip>   strips;
    unsigned short const layer;
  };
}

#endif
