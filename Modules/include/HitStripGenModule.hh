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

#include <vector>
#include "Random.hh"
#include "Module.hh"


class DetectorGeometry;
class TrackSet;
class HitSet;
class StripSet;
class TVector3;

namespace fc {

///
/// Class HitStripGenModule Module for generating associated hit and strip data from tracks
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

class HitStripGenModule : public Module{

public:

  HitStripGenModule(int, const DetectorGeometry &, Random &);
  ~HitStripGenModule() {};

  void processEvent(Event&) override;


private:

  int _debugLevel;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  // Random numbers
  Random &  _myRandom;

  int _nLayers;

  void makeHitsStrips(HitSet &, StripSet &, Track &, int, int &);

  void calculateTrackSensorIntersection(const Track &, int, TVector3 &);

  void storeHitInfo(HitSet &,Track &,int,int &,TVector3 &,int);

  void storeStripInfo(StripSet &,const TVector3 &,int);

  double calculateStripHitPosition(const TVector3 &, int) const;
  void generateCluster(double, int &, std::vector<int> &);
  void storeCluster(StripSet &, int, int, const std::vector<int> &);


};
} // end namespace fc

#endif // HitStripGenModule_hh
