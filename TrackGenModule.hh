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

class Track;
class TrackSet;

///
/// Class TrackGenModule: Module for generating tracks
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

namespace fc {

class TrackGenModule {

public:

  TrackGenModule(int,const DetectorGeometry &,Random &);
  ~TrackGenModule() {};

  void processEvent(TrackSet &,int);


private:

  int _debugLevel;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  // Random numbers
  Random &  _myRandom;

  double _curvatureC;

  Track generateTrack();

};

} // end namespace fc

#endif // TrackGenModule_hh
