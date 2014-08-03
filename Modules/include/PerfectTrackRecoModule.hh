#ifndef Modules_PerfectTrackRecoModule_hh
#define Modules_PerfectTrackRecoModule_hh
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
class GenHitSet;
class HitSet;

///
/// Class PerfectTrackRecoModule  Module for reconstructing track using correct Hits
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class PerfectTrackRecoModule : public Module {

public:

  PerfectTrackRecoModule(int, const std::string& inputHitsLabel, const std::string& inputGenHitsLabel, const std::string& outputTracksLabel, 
		  const DetectorGeometry &);

  void processEvent(Event&) override;


private:

  int _debugLevel;

  const std::string _inHitsLabel;
  const std::string _inGenHitsLabel;
  const std::string _outTracksLabel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;
  
  void recoTracks(const HitSet&, const GenHitSet&,TrackSet&) const;

  void findTrackPerfectCandidates(const HitSet & recoHitSet, const GenHitSet& genHitSet,
				  std::vector<std::vector<int>> & trackHitCandidates) const;
  double compareHitPositions(const GenHit & genHit, const Hit& recoHit) const;

  void buildPerfectTrackCandidates(const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet,
				   TrackSet & trackCandidateSet) const;




};
} // end namespace fc

#endif // Modules_PerfectTrackRecoModule_hh
