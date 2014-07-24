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

class DetectorGeometry;
class HitSet;
class TrackSet;

namespace fc {

///
/// Class TrackRecoModule  Module for reconstructing hits from strip infomation
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class TrackRecoModule : public Module {

public:

  TrackRecoModule(int, const std::string& inputHitsLabel, const std::string& outputTracksLabel, 
		  const DetectorGeometry &);
  ~TrackRecoModule() {};

  void processEvent(Event&) override;


private:

  int _debugLevel;

  const std::string _inHitsLabel;
  const std::string _outTracksLabel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  int _nLayers;

  // Tracking parameters

  // !!!!! initialize through configuration
  double _candidatePTThreshold;


  void recoTracks(TrackSet & trackSet, const HitSet& hitSet);
  void findTrack3X2SASHitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet);

};
} // end namespace fc

#endif // TrackRecoModule_hh
