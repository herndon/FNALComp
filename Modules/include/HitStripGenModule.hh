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
#include <string>
#include "Framework/include/Module.hh"


class TVector3;

namespace fc {

class DetectorGeometry;
class GenTrackSet;
class GenTrack;
class HitSet;
class StripSet;
class Random;


///
/// Class HitStripGenModule Module for generating associated hit and strip data from tracks
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

class HitStripGenModule : public Module{

public:

  HitStripGenModule(int, 
		    const std::string& iInputTracksLabel,
		    const std::string& iOutputHitsLabel,
		    const std::string& iOutputStripsLabel,
		    const DetectorGeometry &, Random &);
  ~HitStripGenModule() {};

  void processEvent(Event&) override;


private:

  std::string const _inTracksLabel;
  std::string const _outTracksLabel;
  std::string const _outHitsLabel;
  std::string const _outStripsLabel;
  int _debugLevel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  // Random numbers
  Random &  _random;

  void makeHitsStrips(HitSet &, StripSet &, const GenTrack &, int, int &) const;

  void storeHitInfo(HitSet &,int,int &,TVector3 &,int) const;

  void storeStripInfo(StripSet &,const TVector3 &,int) const;

  void generateClusterFromStripHitPosition(double, int &, std::vector<int> &) const;
  void storeCluster(StripSet &, int, int, const std::vector<int> &) const;


};
} // end namespace fc

#endif // HitStripGenModule_hh
