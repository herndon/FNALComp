#include <iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackRecoModule.hh"
#include "Tracking/include/TrackCandidateStrategy2X1SAS.hh"
#include "Tracking/include/TrackRecoStrategy2X1SAS.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "Services/include/Config.hh"


fc::TrackRecoModule::TrackRecoModule(int debugLevel, 
				     const std::string& inputHitsLabel, const std::string& outputTracksLabel,
				     const Config& config, const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inHitsLabel(inputHitsLabel),
  _outTracksLabel(outputTracksLabel),
  _config(config),
  _detectorGeometry(detectorGeometry){
}

void fc::TrackRecoModule::processEvent(Event& event)
{

  Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);
  
  std::unique_ptr<TrackSet> recoTrackSet{ new TrackSet };

  recoTracks(*recoTrackSet,*recoHitSet);

  event.put(_outTracksLabel,std::move(recoTrackSet) );
}

void fc::TrackRecoModule::recoTracks(TrackSet & recoTrackSet, const HitSet& recoHitSet) const {


  TrackCandidateStrategy2X1SAS candStrategy(_debugLevel,_detectorGeometry,_config.getMinCandPTCut());
  TrackRecoStrategy2X1SAS recoStrategy(_debugLevel,_detectorGeometry,_config.getMinPTCut(),_config.getMaxChi2NDofCut());

  trackSet trackCandidateSet;
  
  candStrategy.findTrackCandidates(trackCandidateSet,recoHitSet);
  recoStrategy.recoTracks(trackCandidateSet,recoHitSet);

  for (trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter!= trackCandidateSet.end(); ++trackIter){
    recoTrackSet.insertTrack(*trackIter);
  }

  if (_debugLevel>=2){
    std::cout << "Reconstructed track set" << std::endl;
    recoTrackSet.print(std::cout);
  }

}
