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

#include "StripSet.hh"
#include "Module.hh"
#include <vector>

class DetectorGeometry;
class HitSet;

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
  const DetectorGeometry & _myDetectorGeometry;

  int _nLayers;

  void recoTracks(TrackSet & myTrackSet, const HitSet& myHitSet);
  void findTrack2HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet);
  void findTrack3HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet);
  void findTrack5HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet);

  void build2HitTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet);
  void buildPerfectTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet);




};
} // end namespace fc

#endif // TrackRecoModule_hh
