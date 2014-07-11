#ifndef CountEventsSource_hh
#define CountEventsSource_hh

#include "Source.hh"

namespace fc {
  class CountEventsSource : public Source {
    const unsigned int _numberOfEventsToProcess;
    unsigned int _numberOfEventsProcessed = 0;
    bool _genData;

  public:
    CountEventsSource(unsigned int iNumberOfEventsToProcess, bool genData):
      _numberOfEventsToProcess{iNumberOfEventsToProcess},
      _genData{genData}
    {}

    bool fillNextEvent(Event&) override;

  };
}

#endif //CountEventsSource_hh
