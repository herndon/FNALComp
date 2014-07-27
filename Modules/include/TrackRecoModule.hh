#ifndef TrackRecoModule_hh
#define TrackRecoModule_hh
//============================================================================
// TrackRecoModule.hh
// Module for reconstructing hits from strip infomation
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-11
//============================================================================

#include "Framework/include/Module.hh"
#include <vector>


namespace fc {

  class Config;
  class DetectorGeometry;
  class HitSet;
  class TrackSet;


///
/// Class TrackRecoModule  Module for reconstructing hits from strip infomation
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class TrackRecoModule : public Module {

public:

  TrackRecoModule(int, const std::string& inputHitsLabel, const std::string& outputTracksLabel, 
		  const Config&, const DetectorGeometry &);
  ~TrackRecoModule() {};

  void processEvent(Event&) override;


private:

  int _debugLevel;

  const std::string _inHitsLabel;
  const std::string _outTracksLabel;

  // Configuration object for tracking parameters
  const Config& _config;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  // Tracking parameters

  // !!!!! initialize through configuration
  double _candidatePTThreshold;


  void recoTracks(TrackSet & trackSet, const HitSet& hitSet) const;

};
} // end namespace fc

#endif // TrackRecoModule_hh
