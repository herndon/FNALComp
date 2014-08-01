#include <iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackCandidateModule.hh"
#include "Tracking/include/TrackCandidateStrategy2X1SAS.hh"
#include "Services/include/Config.hh"


fc::TrackCandidateModule::TrackCandidateModule(int debugLevel, 
				     const std::string& inputHitsLabel, const std::string& outputTracksLabel,
				     const Config& config, const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inHitsLabel(inputHitsLabel),
  _outTracksLabel(outputTracksLabel),
  _config(config),
  _detectorGeometry(detectorGeometry){
}

void fc::TrackCandidateModule::processEvent(Event& event)
{

  Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);
  
  std::unique_ptr<TrackSet> recoTrackCandidateSet{ new TrackSet };

  findTrackCandidates(*recoTrackCandidateSet,*recoHitSet);

  event.put(_outTracksLabel,std::move(recoTrackCandidateSet) );
}

void fc::TrackCandidateModule::findTrackCandidates(TrackSet & recoTrackCandidateSet, const HitSet& recoHitSet) const {


  TrackCandidateStrategy2X1SAS candStrategy(_debugLevel,_detectorGeometry,_config.getMinCandPTCut());

  trackSet trackCandidateSet;
  
  candStrategy.findTrackCandidates(trackCandidateSet,recoHitSet);

  for (trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter!= trackCandidateSet.end(); ++trackIter){
    recoTrackCandidateSet.insertTrack(std::move(*trackIter));
  }

  if (_debugLevel>=2){
    //std::cout << "Reconstructed track set" << std::endl;
    //recoTrackSet.print(std::cout);
  }

}
