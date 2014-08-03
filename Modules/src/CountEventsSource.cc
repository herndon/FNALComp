#include <iostream>
#include "Modules/include/CountEventsSource.hh"
#include "Framework/include/Event.hh"

std::unique_ptr<fc::Event> fc::CountEventsSource::getNextEvent() {
    const unsigned int eventNumber = _numberOfEventsProcessed++;
    if( _numberOfEventsProcessed <= _numberOfEventsToProcess) {

        if ((_debugLevel >=2) || (_debugLevel>=1
                                  && (eventNumber % 1000) == 0)) std::cout << "Event Number: " << eventNumber <<
                                              std::endl;

        std::unique_ptr<Event> event { new Event(eventNumber) };

        std::unique_ptr<bool> genDataPtr( new bool {_genData} );
        event->put("genData", std::move(genDataPtr) );
        return std::move(event);
    }

    return std::unique_ptr<Event>();
}
