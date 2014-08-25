#ifndef Services_Config_hh
#define Services_Config_hh
#include <fstream>
#include <iostream>
//============================================================================
// Config.hh
// header for Config class
// General config class for loading config information for fc project programs
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-17
//============================================================================
#include <fstream>
#include <iostream>
//#include <stream>

namespace fc {

///
/// Class Config
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-17
/// General config class for loading config information for fc project programs
///

class Config {


public:

  Config(std::ifstream&,int genData,int eventNumberForEventDisplay);

    int debugLevel() const {
        return _debugLevel;   //!< Returns debug level: 0 nothing, 1 progress infrmation; 2 data object printout; 3-5 basic-detailed debug
    }

  bool runGenHistogrammingModule() const {
    return _runGenHistogrammingModule;
  }
  bool runHitRecoModule() const {
    return _runHitRecoModule;
  }
  bool runHitCompareModule() const {
    return _runHitCompareModule;
  }
  bool runPerfectTrackRecoModule() const {
    return _runPerfectTrackRecoModule;
  }
  bool runPerfectTrackCompareWithGenModule() const {
    return _runPerfectTrackCompareWithGenModule;
  }
  bool runTrackSeedModule() const {
    return _runTrackSeedModule;
  }
  bool runTrackSeedCompareModule() const {
    return _runTrackSeedCompareModule;
  }
  bool runTrackRecoModule() const {
    return _runTrackRecoModule;
  }
  bool runTrackCompareWithPerfectModule() const {
    return _runTrackCompareWithPerfectModule;
  }
  bool runRecoTrackCompareWithGenModule() const {
    return _runRecoTrackCompareWithGenModule;
  }
  bool runEventDisplayModule() const {
    return _runEventDisplayModule;
  }

    int seed() const {
        return _seed;
    }; //!< Returns random number seed
    int numberEvents() const {
        return _numberEvents;
    }; //!< Returns number of events to generate
    int numberTracks() const {
        return _numberTracks;
    }; //!< Returns number of tracks to generate per event

    std::string const& rootFileName() const {
        return _rootFileName;    //!< Returns the name of the root file to be created.
    }


    double minCandPTCut() const {
        return _minCandPTCut;
    }
    double minPTCut() const {
        return _minPTCut;
    }
    double dRCut() const {
        return _dRCut;
    }
    double dZCut() const {
        return _dZCut;
    }
    double maxChi2NDofCut() const {
        return _maxChi2NDofCut;
    }

    int eventNumberForEventDisplay() const {
        return _eventNumberForEventDisplay;
    }


    void printConfig(std::ostream& out) const;


private:

  int _debugLevel;

  bool _runGenHistogrammingModule = false;
  bool _runHitRecoModule = false;
  bool _runHitCompareModule = false;
  bool _runPerfectTrackRecoModule = false;
  bool _runPerfectTrackCompareWithGenModule = false;
  bool _runTrackSeedModule = false;
  bool _runTrackSeedCompareModule = false;
  bool _runTrackRecoModule = false;
  bool _runTrackCompareWithPerfectModule = false;
  bool _runRecoTrackCompareWithGenModule = false;
  bool _runEventDisplayModule = false;


  bool _genData;
  int _numberEvents;
  int _numberTracks;
  int _seed;

  std::string _rootFileName;

  double _minCandPTCut;
  double _minPTCut;
  double _dRCut;
  double _dZCut;
  double _maxChi2NDofCut;

  int _eventNumberForEventDisplay;

  void _initConfig(std::ifstream&);

};
} // end namespace fc

#endif // Services_Config_hh
