#ifndef EventProcessor_hh
#define EventProcessor_hh

#include <memory>
#include <vector>
#include "Framework/include/Source.hh"
#include "Framework/include/Module.hh"

class TFile;

namespace fc {
  class Module;

  class EventProcessor {
    std::vector<std::unique_ptr<Module>> _modules;
    std::unique_ptr<Source> _source;
    TFile* _rootFile;
    void endRoot();
  public:
    explicit EventProcessor(Source*iSource, TFile*rootFile):
      _source(iSource),
      _rootFile(rootFile)
    {}
    EventProcessor( const EventProcessor& ) = delete;

    //takes ownership of the module
    void addModule(Module*);

    void processEvents();
    void endJob();
  };

}

#endif //EventProcessor_hh
