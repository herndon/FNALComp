#ifndef EventProcessor_hh
#define EventProcessor_hh

#include <memory>
#include <vector>
#include "Source.hh"

namespace fc {
  class Module;

  class EventProcessor {
    std::vector<std::unique_ptr<Module>> _modules;
    std::unique_ptr<Source> _source;
  public:
    explicit EventProcessor(Source*iSource):
      _source(iSource)
    {}
    EventProcessor( const EventProcessor& ) = delete;

    //takes ownership of the module
    void addModule(Module*);

    void processEvents();
  };

}

#endif //EventProcessor_hh
