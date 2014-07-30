#include<iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Algorithms/include/BuildTrack.hh"
#include "Modules/include/PerfectTrackRecoModule.hh"

fc::PerfectTrackRecoModule::PerfectTrackRecoModule(int debugLevel, 
						   const std::string& inputHitsLabel, const std::string& inputGenHitsLabel,
						   const std::string& outputTracksLabel,
						   const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inHitsLabel(inputHitsLabel),
  _inGenHitsLabel(inputGenHitsLabel),
  _outTracksLabel(outputTracksLabel),
  _detectorGeometry(detectorGeometry) {
}

void fc::PerfectTrackRecoModule::processEvent(Event& event)
{

  Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);
  Handle<HitSet> genHitSet = event.get<HitSet>(_inGenHitsLabel);
  
  std::unique_ptr<TrackSet> perfectRecoTrackSet{ new TrackSet };

  recoTracks(*perfectRecoTrackSet,*recoHitSet,*genHitSet);

  if (_debugLevel>=2){
    std::cout << "Perfect reconstructed tracks" << std::endl;
    perfectRecoTrackSet->print(std::cout);
  }

  event.put(_outTracksLabel,std::move(perfectRecoTrackSet) );
}

void fc::PerfectTrackRecoModule::recoTracks(TrackSet & perfectRecoTrackSet, const HitSet& recoHitSet, const HitSet& genHitSet) const {
  // !!!!! This will eventually pass a track hit candidates strategy 
  // !!!!! Do I want to make a special object for the track hit candidates?
  std::vector<std::vector<int>> trackHitCandidates;


  findTrackPerfectCandidates(trackHitCandidates,recoHitSet,genHitSet);
  buildPerfectTrackCandidates(perfectRecoTrackSet,trackHitCandidates,recoHitSet);

  
}



void fc::PerfectTrackRecoModule::findTrackPerfectCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & recoHitSet,const HitSet & genHitSet) const{

  int trackNumber = (genHitSet.getHits().empty() ? 0 : genHitSet.getHits().front().getTrackNumber());

  std::vector<int> trackHitCandidate;

  // Form all hit candidates

  for (auto genHit : genHitSet.getHits()) {

    double deltaPosition = 999.0;
    int recoHitNumber = 0;
    int bestRecoHitNumber = -1;

    for (hitSet::const_iterator recoHitIter = recoHitSet.getHits().begin(); recoHitIter != recoHitSet.getHits().end(); ++recoHitIter,++recoHitNumber) {

      if (genHit.getLayer()==recoHitIter->getLayer()) {
	double tempDeltaPosition = compareHitPositions(genHit,*recoHitIter);
	if (std::abs(tempDeltaPosition) < std::abs(deltaPosition)) {
	  deltaPosition = tempDeltaPosition;
	  bestRecoHitNumber = recoHitNumber;
	}
      }

    } // end reco Hit loop
    if (genHit.getTrackNumber() == trackNumber && bestRecoHitNumber>-1) {

      trackHitCandidate.push_back(bestRecoHitNumber);

    } else if (genHit.getTrackNumber() != trackNumber){
      trackHitCandidates.push_back(trackHitCandidate);
      trackHitCandidate.clear();
      trackHitCandidate.push_back(bestRecoHitNumber);
      trackNumber = genHit.getTrackNumber();
    }
  } // end gen Hit loop
  trackHitCandidates.push_back(std::move(trackHitCandidate));


}


// !!!!! move to utility function
double fc::PerfectTrackRecoModule::compareHitPositions(const Hit & genHit, const Hit& recoHit) const{

  return recoHit.getHitPosition()*_detectorGeometry.getSensor(recoHit.getLayer())._measurementDirection
    - genHit.getHitPosition()*_detectorGeometry.getSensor(genHit.getLayer())._measurementDirection;

}


void fc::PerfectTrackRecoModule::buildPerfectTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet) const{

  for (auto const& trackHitCandidate : trackHitCandidates) {
    Track trackCandidate(buildTrack(hitSet,trackHitCandidate,_detectorGeometry,_debugLevel));

    if (_debugLevel ==2) {
      std::cout << "Track after fit" << std::endl;
      trackCandidate.print(std::cout);
    }

    trackCandidateSet.insertTrack(std::move(trackCandidate));

  }

}
