#include <iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackCandidateModule.hh"
#include "Tracking/include/TrackCandidateStrategy2X1SAS.hh"
#include "Tracking/include/TrackCandidateStrategy2X1SASML.hh"
#include "Services/include/Config.hh"
#include "Services/include/Exception.hh"

fc::TrackCandidateModule::TrackCandidateModule(int debugLevel,
	const std::string& inputHitsLabel, const std::string& outputTracksLabel, const std::string& trackCandidateStrategy,
        const Config& config, const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _inHitsLabel(inputHitsLabel),
    _outTracksLabel(outputTracksLabel),
    _trackCandidateStrategy(trackCandidateStrategy),
    _config(config),
    _detectorGeometry(detectorGeometry) {
}

void fc::TrackCandidateModule::processEvent(Event& event)
{

    Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);

    std::unique_ptr<TrackSet> recoTrackCandidateSet { new TrackSet };

    findTrackCandidates(*recoHitSet,*recoTrackCandidateSet);

    if (_debugLevel>=2) {
        std::cout << "Candidate track set" << std::endl;
        recoTrackCandidateSet->print(std::cout);
    }


    event.put(_outTracksLabel,std::move(recoTrackCandidateSet) );
}

void fc::TrackCandidateModule::findTrackCandidates(const HitSet& recoHitSet,
        TrackSet & recoTrackCandidateSet) const {

   FastTrackSetContainer trackCandidateSet;


  if (_trackCandidateStrategy=="trackCandidateStrategy2X1SAS"){

    TrackCandidateStrategy2X1SAS candStrategy(_debugLevel,_detectorGeometry,
            _config.getMinCandPTCut());
    candStrategy.findTrackCandidates(recoHitSet,trackCandidateSet);


  } else if  (_trackCandidateStrategy=="trackCandidateStrategy2X1SASML"){

    TrackCandidateStrategy2X1SASML candStrategy(_debugLevel,_detectorGeometry,
            _config.getMinCandPTCut());
    candStrategy.findTrackCandidates(recoHitSet,trackCandidateSet);

  } else {
    throw Exception("TrackCandidateModule::findTrackCandidates strategy not recongnized");
  }


    for (auto& track : trackCandidateSet) {
        recoTrackCandidateSet.insertTrack(std::move(track));
    }


}
