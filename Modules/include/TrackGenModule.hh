#ifndef TrackGenModule_hh
#define TrackGenModule_hh
//============================================================================
// TrackGenModule.hh
// Module for generating tracks 
// Track data is stored in bianary format
// See  dataFormat.pdf for data format information
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================

#include <fstream>
#include <iostream>
#include <string>
#include "Random.hh"
#include "Module.hh"

class Track;
class TrackSet;

///
/// Class TrackGenModule: Module for generating tracks
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

namespace fc {

class TrackGenModule : public Module {

public:

  TrackGenModule(int debugLevel, int numberOfTracks,
		 const std::string& iTracksLabel, //label used for the generated tracks
		 const DetectorGeometry &,Random &);
  ~TrackGenModule() {};

  void processEvent(Event& ) override;


private:

  int _debugLevel;
  int _numberOfTracks;

  const std::string _tracksLabel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  // Random numbers
  Random &  _random;

  double _curvatureC;

  Track generateTrack();

};

} // end namespace fc

#endif // TrackGenModule_hh
