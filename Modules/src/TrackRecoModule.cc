#include <iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackRecoModule.hh"
#include "Tracking/include/TrackRecoStrategy2X1SAS.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "Services/include/Config.hh"


fc::TrackRecoModule::TrackRecoModule(int debugLevel,
                                     const std::string& inputHitsLabel,
                                     const std::string& inputTracksLabel, const std::string& outputTracksLabel,
                                     const Config& config, const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _inHitsLabel(inputHitsLabel),
    _inTracksLabel(inputTracksLabel),
    _outTracksLabel(outputTracksLabel),
    _config(config),
    _detectorGeometry(detectorGeometry) {
}

void fc::TrackRecoModule::processEvent(Event& event)
{

    Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);

    Handle<TrackSet> inputTrackCandidateSet = event.get<TrackSet>(_inTracksLabel);

    std::unique_ptr<TrackSet> recoTrackSet { new TrackSet };

    recoTracks(*inputTrackCandidateSet,*recoHitSet,*recoTrackSet);

    event.put(_outTracksLabel,std::move(recoTrackSet) );
}

void fc::TrackRecoModule::recoTracks(const TrackSet& inputTrackCandidateSet,
                                     const HitSet& recoHitSet,TrackSet & recoTrackSet) const {


    TrackRecoStrategy2X1SAS recoStrategy(_debugLevel,_detectorGeometry,
                                         _config.getMinPTCut(),_config.getMaxChi2NDofCut());

    TrackSetContainer trackCandidateSet(inputTrackCandidateSet.getTracks()) ;

    recoStrategy.recoTracks(recoHitSet,trackCandidateSet);

    for (auto& track : trackCandidateSet) {
        recoTrackSet.insertTrack(std::move(track));
    }

    if (_debugLevel>=2) {
        std::cout << "Reconstructed track set" << std::endl;
        recoTrackSet.print(std::cout);
    }

}
