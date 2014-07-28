#ifndef Framework_Module_hh
#define Framework_Module_hh

#include "Framework/include/Event.hh"

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

#endif //Framework_Module_hh
