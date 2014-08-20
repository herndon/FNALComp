#ifndef Framework_EventProcessor_hh
#define Framework_EventProcessor_hh

#include <memory>
#include <vector>
#include "Framework/include/Source.hh"
#include "Framework/include/Module.hh"

class TFile;

namespace fc {
class Module;
class Config;

class EventProcessor {
public:
    EventProcessor(Source*iSource, std::shared_ptr<TFile> rootFile):
        _source(iSource),
        _rootFile(rootFile)
    {}
    EventProcessor( const EventProcessor& ) = delete;

    //takes ownership of the module
    void addModule(Module*);

    void processEvents(const Config&);
    void endJob();

private:
    void endRoot();
    std::vector<std::unique_ptr<Module>> _modules;
    std::unique_ptr<Source> _source;
    std::shared_ptr<TFile> _rootFile;
};
}

#endif //Framework_EventProcessor_hh
