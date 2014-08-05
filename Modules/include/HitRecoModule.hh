#ifndef Modules_HitRecoModule_hh
#define Modules_HitRecoModule_hh
//============================================================================
// HitRecoModule.hh
// Module for reconstructing hits from strip infomation
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-11
//============================================================================

#include "Framework/include/Module.hh"
#include "DataObjects/include/StripSet.hh"
#include "TVector3.h"
#include <vector>
#include <string>


namespace fc {

class DetectorGeometry;
class Hit;
class HitSet;
  struct SensorDescriptor;


///
/// Class HitRecoModule  Module for reconstructing hits from strip infomation
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class HitRecoModule : public Module {

public:

    HitRecoModule(int, const std::string& iInputStripsLabel,
                  const std::string& iOutputHitsLabel, const DetectorGeometry &);

    void processEvent(Event& ) override;


private:

    int _debugLevel;
    const std::string _inStripsLabel;
    const std::string _outHitsLabel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;
  int _num;

  void recoHits(const StripSet &,HitSet &) const;
  void makeHits(int layer, LayerStripMap const& strips, HitSet& hits) const;
  void makeHit(int layer, int start_strip, 
	       std::vector<int> const& adcs, 
	       SensorDescriptor const& desc,
	       HitSet& hits) const;


};
} // end namespace fc

#endif // Modules_HitRecoModule_hh
