#include "EventProcessor.hh"
#include "Event.hh"
#include "Module.hh"
#include "Exception.hh"
#include <iostream>

void fc::EventProcessor::addModule(Module* iModule) {
  _modules.emplace_back(iModule);
}

void fc::EventProcessor::processEvents(int iNumberOfEvents) {
  try {
    for (int ii_event = 0; ii_event < iNumberOfEvents; ++ii_event) {
      fc::Event event(ii_event);
      {
	std::unique_ptr<bool> genDataPtr( new bool{_genData} );
	event.put("genData", std::move(genDataPtr) );
      }
      
      for(auto& module : _modules) {
	module->processEvent(event);
      }
    }
  } catch( const Exception& iException) {
    std::cerr<<"caught exception: "<<iException.what();
  } catch( const std::exception& iException) {
    std::cerr<<"caught std::exception: "<<iException.what();
  }

}
