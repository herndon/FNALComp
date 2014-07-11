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

  TrackGenModule(int debugLevel, int numberOfTracks, const DetectorGeometry &,Random &);
  ~TrackGenModule() {};

  void processEvent(Event& ) override;


private:

  int _debugLevel;
  int _numberOfTracks;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  // Random numbers
  Random &  _myRandom;

  double _curvatureC;

  Track generateTrack();

};

} // end namespace fc

#endif // TrackGenModule_hh
