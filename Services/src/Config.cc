#include <string>
#include "Services/include/Exception.hh"
#include "Services/include/Config.hh"

fc::Config::Config(std::ifstream & configfile,int genData):
    _debugLevel(1),
    _genData(genData),
    _numberEvents(128),
    _numberTracks(1),
    _seed(1),
    _minCandPTCut(1.0),
    _minPTCut(1.0),
    _maxChi2NDofCut(10.0),
    _eventNumberForEventDisplay(0) {

    _initConfig(configfile);
    if (_debugLevel >=1 )printConfig(std::cout);

}

void fc::Config::_initConfig(std::ifstream & configfile) {


    if (!configfile) {
        throw Exception("Config::_initConfig: can't open configfile.txt file");
    }

    std::string configString;


    while(configfile) {
        configfile >> configString;

        if (_genData) {
            if (configString == "DebugLevel") {
                configfile >> _debugLevel;
            } else if (configString == "NumberEvents") {
                configfile >> _numberEvents;
            } else if (configString == "NumberTracks") {
                configfile >> _numberTracks;
            } else if (configString == "Seed") {
                configfile >> _seed;
            } else if (configString == "RootFileName") {
                configfile >> _rootFileName;
            } else {
                throw Exception("Config::_initConfig: Unrecognized parameter in configfile.txt");
            }
        } else {
            if (configString == "DebugLevel") {
                configfile >> _debugLevel;
            } else if (configString == "RootFileName") {
                configfile >> _rootFileName;
           } else if (configString == "RunGenHistogrammingModule") {
                configfile >> _runGenHistogrammingModule;
           } else if (configString == "RunHitRecoModule") {
                configfile >> _runHitRecoModule;
           } else if (configString == "RunHitCompareModule") {
                configfile >> _runHitCompareModule;
           } else if (configString == "RunPerfectTrackRecoModule") {
                configfile >> _runPerfectTrackRecoModule;
           } else if (configString == "RunPerfectTrackCompareWithGenModule") {
                configfile >> _runPerfectTrackCompareWithGenModule;
           } else if (configString == "RunTrackCandidateModule") {
                configfile >> _runTrackCandidateModule;
           } else if (configString == "RunCandidateCompareModule") {
                configfile >> _runCandidateCompareModule;
           } else if (configString == "RunTrackRecoModule") {
                configfile >> _runTrackRecoModule;
           } else if (configString == "RunTrackCompareWithPerfectModule") {
                configfile >> _runTrackCompareWithPerfectModule;
           } else if (configString == "RunRecoTrackCompareWithGenModule") {
                configfile >> _runRecoTrackCompareWithGenModule;
           } else if (configString == "RunEventDisplayModule") {
                configfile >> _runEventDisplayModule;
            } else if (configString == "minCandPTCut") {
                configfile >> _minCandPTCut;
            } else if (configString == "minPTCut") {
                configfile >> _minPTCut;
            } else if (configString == "dRCut") {
                configfile >> _dRCut;
            } else if (configString == "dZCut") {
                configfile >> _dZCut;
            } else if (configString == "maxChi2NDofCut") {
                configfile >> _maxChi2NDofCut;
           } else if (configString == "EventNumberForEventDisplay") {
                configfile >> _eventNumberForEventDisplay;
            } else {
                throw Exception("Config::_initConfig: Unrecognized parameter in configfile.txt");
            }

        }

    } // endl while loop through configfile

}



void fc::Config::printConfig(std::ostream& out) const {

    out << "Program configuration information" << std::endl;

    if (_genData) {
        out << "gen Data configuration" << std::endl;
        out << "Debug Level: " << _debugLevel << std::endl;
        out << "Number Events: " << _numberEvents << std::endl;
        out << "Number Tracks " << _numberTracks << std::endl;
        out << "Random number seed " << _seed << std::endl;
        out << "Root file name " << _rootFileName << std::endl;
    } else {
        out << "reco Data configuration" << std::endl;
        out << "Debug Level: " << _debugLevel << std::endl;
        out << "Root file name " << _rootFileName << std::endl;
    }

}
