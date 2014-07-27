#ifndef Modules_CountEventsSource_hh
#define Modules_CountEventsSource_hh

#include "Framework/include/Source.hh"

namespace fc {
  class CountEventsSource : public Source {
  public:
    CountEventsSource(unsigned int iNumberOfEventsToProcess, bool genData):
      _numberOfEventsToProcess{iNumberOfEventsToProcess},
      _genData{genData}
    {}

    std::unique_ptr<Event> getNextEvent() override;

  private:
    const unsigned int _numberOfEventsToProcess;
    unsigned int _numberOfEventsProcessed = 0;
    bool _genData;

  };
}

#endif //Modules_CountEventsSource_hh
