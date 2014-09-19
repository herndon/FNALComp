#ifndef EventDisplay_EventDisplayModule_hh
#define EventDisplay_EventDisplayModule_hh
//============================================================================
// EventDisplayModule.hh
// header with class definition for an root EVE event display
//
// Author Matt Herndon, University of Wisconsin,
//
// 2014-08-13
//============================================================================

#include "Framework/include/Module.hh"

class TApplication;
class TEveTrackList;
class TEveElementList;
class TEveStraightLineSet;
 
namespace fc {

  class Config;
  class DetectorGeometry;
  class TrackSet;
  class GenTrackSet;
  class HitSet;

///
/// Class EventDisplayModule
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-08-13
///

class EventDisplayModule : public Module {


public:


  EventDisplayModule(int debugLevel, const std::string& inputGenHitsLabel,const std::string& inputGenTracksLabel,
		     const std::string& inputRecoHitsLabel,const std::string& inputRecoTracksLabel, 
		     const Config&, const DetectorGeometry&);

    void processEvent(Event&) override;

    void displayGeometry();

private:

    int _debugLevel;

    const std::string _genHitsLabel;
    const std::string _genTracksLabel;
    const std::string _recoHitsLabel;
    const std::string _recoTracksLabel;

  unsigned int _eventNumber;

    // Configuration object for event display parameters
    const Config& _config;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

  TApplication* _tApp = nullptr;

    bool            _drawGenTracks;
    bool            _drawGenHits;
    bool            _drawRecoTracks;
    bool            _drawSeedRecoTracks;
    bool            _drawRecoHits;
    bool            _genDataOnly; 

  void fillGenTrackList(const GenTrackSet& genTrackSet,const TrackSet& recoTrackSet,TEveTrackList& trackList);
  void fillRecoTrackList(const TrackSet& recoTrackSet,const GenTrackSet& genTrackSet,const HitSet& recoHitSet,
			 TEveTrackList& trackList,TEveStraightLineSet& recoHitFakeTrackLineSet);
  void fillGenHitList(const GenHitSet& genHitSet,TEveElementList& hitList,TEveElementList& sasZHitList);
  void fillHitList(const HitSet& hitSet,TEveStraightLineSet& recoHitLineSet);



};
} // end namespace fc

#endif // EventDisplay_EventDisplayModule_hh
