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

#include "Module.hh"
#include "StripSet.hh"
#include <vector>
#include <string>

class DetectorGeometry;
class HitSet;

namespace fc {

///
/// Class HitRecoModule  Module for reconstructing hits from strip infomation
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class HitRecoModule : public Module {

public:

  HitRecoModule(int, const std::string& iInputStripsLabel, const std::string& iOutputHitsLabel, const DetectorGeometry &);
  ~HitRecoModule() {};

  void processEvent(Event& ) override;


private:

  int _debugLevel;
  const std::string _inStripsLabel;
  const std::string _outHitsLabel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  int _nLayers;

  void recoHits(HitSet &, const StripSet &);

  void recoHitsLayer(HitSet &, const StripSet &, int, int &);

  void findCluster(int &,int, std::vector<int> &,layerStripMap::const_iterator &,layerStripMap::const_iterator &,const StripSet &);

  double calculateStripHitPosition(int, const std::vector<int> &);

  void calculateHitPosition(int, double, double *);


};
} // end namespace fc

#endif // HitRecoModule_hh
