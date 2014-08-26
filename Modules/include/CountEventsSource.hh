#ifndef Modules_CountEventsSource_hh
#define Modules_CountEventsSource_hh

#include <iostream>
#include <fstream>
#include "Framework/include/Source.hh"

namespace fc {
class CountEventsSource : public Source {
public:
  CountEventsSource(int debugLevel, std::ofstream & debugfile,
		    unsigned int numberOfEventsToProcess,
                      bool genData):
    _debugLevel(debugLevel),_debugfile(debugfile),
    _numberOfEventsToProcess {numberOfEventsToProcess},
                             _genData {genData}
    {}

    std::unique_ptr<Event> getNextEvent() override;

private:
    int _debugLevel;
  std::ofstream & _debugfile;
    const unsigned int _numberOfEventsToProcess;
    unsigned int _numberOfEventsProcessed = 0;
    bool _genData;

};
}

#endif //Modules_CountEventsSource_hh
