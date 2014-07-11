#include "CountEventsSource.hh"
#include "Event.hh"

bool fc::CountEventsSource::fillNextEvent( Event& event) {
  std::unique_ptr<bool> genDataPtr( new bool{_genData} );
  event.put("genData", std::move(genDataPtr) );

  return _numberOfEventsProcessed++ < _numberOfEventsToProcess;
}
