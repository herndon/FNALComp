#ifndef PerfectTrackRecoModule_hh
#define PerfectTrackRecoModule_hh
//============================================================================
// PerfectTrackRecoModule.hh
// Module for reconstructing hits from strip infomation
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-11
//============================================================================

#include "Framework/include/Module.hh"


namespace fc {

class DetectorGeometry;
class HitSet;

///
/// Class PerfectTrackRecoModule  Module for reconstructing track using correct Hits
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class PerfectTrackRecoModule : public Module {

public:

  PerfectTrackRecoModule(int, const std::string& inputHitsLabel, const std::string& inputGenHitsLabel, const std::string& outputTracksLabel, 
		  const DetectorGeometry &);
  ~PerfectTrackRecoModule() {};

  void processEvent(Event&) override;


private:

  int _debugLevel;

  const std::string _inHitsLabel;
  const std::string _inGenHitsLabel;
  const std::string _outTracksLabel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  int _nLayers;

  void recoTracks(TrackSet & trackSet, const HitSet& recoHitSet, const HitSet& genHitSet);
  void findTrackPerfectCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & recoHitSet, const HitSet& genHitSet);
  double compareHitPositions(const Hit & genHit, const Hit& recoHit);


  void buildPerfectTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet);




};
} // end namespace fc

#endif // PerfectTrackRecoModule_hh
