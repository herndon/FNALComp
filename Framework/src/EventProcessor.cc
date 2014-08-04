#include "Framework/include/EventProcessor.hh"
#include "Framework/include/Event.hh"
#include "Framework/include/Module.hh"
#include "Framework/include/Source.hh"
#include "Services/include/Exception.hh"

#include "TFile.h"

#include <iostream>

void fc::EventProcessor::addModule(Module* iModule) {
    _modules.emplace_back(iModule);
}

void fc::EventProcessor::processEvents() {
    try {
        while(true) {
            std::unique_ptr<fc::Event> event {_source->getNextEvent()};
            if(not event) {
                break;
            }
            for(auto& module : _modules) {
                module->processEvent(*event);
            }
        }
    } catch( const Exception& iException) {
        std::cerr<<"caught exception: "<<iException.what()<<"\n";
    } catch( const std::exception& iException) {
        std::cerr<<"caught std::exception: "<<iException.what()<<"\n";
    }
}

void fc::EventProcessor::endJob() {
    for(auto& module : _modules) {
        try {
            module->endJob();
        } catch( const Exception& iException) {
            std::cerr<<"caught exception: "<<iException.what()<<"\n";
        } catch( const std::exception& iException) {
            std::cerr<<"caught std::exception: "<<iException.what()<<"\n";
        }
    }
    endRoot();
}

void fc::EventProcessor::endRoot() {
    _rootFile->Write();
    _rootFile->Close();
}
