#ifndef HitStripGenModule_hh
#define HitStripGenModule_hh
//============================================================================
// HitStripGenModule.hh
// Module for generating associated hit and strip data from tracks on the 
// sensors of a 5 layer planor silicon detector.
// See detectorGeometry.pdf for detector description
// Strip data is stored in bianary format
// See  dataFormat.pdf for data format information
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================

#include "Random.hh"

class DetectorGeometry;
class TrackSet;
class HitSet;
class StripSet;

namespace fc {

///
/// Class HitStripGenModule Module for generating associated hit and strip data from tracks
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

class HitStripGenModule {

public:

  HitStripGenModule(int, const DetectorGeometry &, Random &);
  ~HitStripGenModule() {};

  void processEvent(TrackSet &, HitSet &,StripSet &);


private:

  int _debugLevel;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  // Random numbers
  Random &  _myRandom;

  int _nLayers;

  void makeHitsStrips(HitSet &, StripSet &, Track &, int, int &);
  void calculateTrackSensorIntersection(Track &, int, double *);

};
} // end namespace fc

#endif // HitStripGenModule_hh
