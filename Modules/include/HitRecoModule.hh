#ifndef HitRecoModule_hh
#define HitRecoModule_hh
//============================================================================
// HitRecoModule.hh
// Module for reconstructing hits from strip infomation
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-11
//============================================================================

#include "StripSet.hh"
#include <vector>

class DetectorGeometry;
class HitSet;

namespace fc {

///
/// Class HitRecoModule  Module for reconstructing hits from strip infomation
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class HitRecoModule {

public:

  HitRecoModule(int, const DetectorGeometry &);
  ~HitRecoModule() {};

  void processEvent(HitSet &,const StripSet &);


private:

  int _debugLevel;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  int _nLayers;

  void recoHits(HitSet &, const StripSet &);

  void recoHitsLayer(HitSet &, const StripSet &, int, int &);

  void findCluster(int &,int, std::vector<int> &,layerStripMap::const_iterator &,layerStripMap::const_iterator &,const StripSet &);

  double calculateStripHitPosition(int, const std::vector<int> &);

  void calculateHitPosition(int, double, double *);


};
} // end namespace fc

#endif // HitRecoModule_hh
