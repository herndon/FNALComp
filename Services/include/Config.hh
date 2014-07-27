#ifndef Config_hh
#define Config_hh
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

  Config(std::ifstream&,int);
 ~Config() {};

  int getSeed() const {return _seed;}; //!< Returns random number seed
  int getNumberEvents() const {return _numberEvents;}; //!< Returns number of events to generate
  int getNumberTracks() const {return _numberTracks;}; //!< Returns number of tracks to generate per event

  std::string const& getRootFileName() const { return _rootFileName; } //!< Returns the name of the root file to be created.


  double getMinCandPTCut() const {return _minCandPTCut;}
  double getMinPTCut() const {return _minPTCut;}
  double getMaxChi2NDofCut() const {return _maxChi2NDofCut;}

  void printConfig(std::ostream& out) const;


private:

  bool _genData;
  int _numberEvents;
  int _numberTracks;
  int _seed;
  std::string _rootFileName;
  double _minCandPTCut;
  double _minPTCut;
  double _maxChi2NDofCut;
  
  void _initConfig(std::ifstream&);

};
} // end namespace fc

#endif // Config_hh
