#ifndef Module_hh
#define Module_hh

#include "Event.hh"

namespace fc {
  class Module {
  public:
    Module() = default;
    Module( const Module&) = delete;
    virtual ~Module();

    virtual void processEvent( Event& ) = 0;

    virtual void endJob();
  };
}

#endif //Module_hh
