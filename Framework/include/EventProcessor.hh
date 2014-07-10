#ifndef EventProcessor_hh
#define EventProcessor_hh

#include <memory>
#include <vector>

namespace fc {
  class Module;

  class EventProcessor {
    std::vector<std::unique_ptr<Module>> _modules;
    bool _genData;
  public:
    explicit EventProcessor(bool genData):
      _genData(genData) {}
    EventProcessor( const EventProcessor& ) = delete;

    //takes ownership of the module
    void addModule(Module*);

    void processEvents(int iNumberOfEvents);
  };

}

#endif //EventProcessor_hh
