#include<iostream>
#include<vector>
#include "DetectorGeometry.hh"
#include "HitSet.hh"
#include "Track.hh"
#include "TrackFit.hh"
#include "TrackSet.hh"
#include "PerfectTrackRecoModule.hh"

fc::PerfectTrackRecoModule::PerfectTrackRecoModule(int debugLevel, 
				     const std::string& inputHitsLabel, const std::string& outputTracksLabel,
				     const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inHitsLabel(inputHitsLabel),
  _outTracksLabel(outputTracksLabel),
  _detectorGeometry(detectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _detectorGeometry.getNSensors();

}

void fc::PerfectTrackRecoModule::processEvent(Event& event)
{

  Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);
  
  std::unique_ptr<TrackSet> perfectRecoTrackSet{ new TrackSet(_detectorGeometry) };

  recoTracks(*perfectRecoTrackSet,*recoHitSet);

  if (_debugLevel >= 2) recoHitSet->print();

  event.put(_outTracksLabel,std::move(perfectRecoTrackSet) );
}

void fc::PerfectTrackRecoModule::recoTracks(TrackSet & perfectRecoTrackSet, const HitSet& hitSet)
{
  // !!!!! This will eventually pass a track hit candidates strategy 
  // !!!!! Do I want to make a special object for the track hit candidates?
  std::vector<std::vector<int>> trackHitCandidates;


  findTrackPerfectCandidates(trackHitCandidates,hitSet);
  buildPerfectTrackCandidates(perfectRecoTrackSet,trackHitCandidates,hitSet);

  
}



void fc::PerfectTrackRecoModule::findTrackPerfectCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  int hitNumber = 0;

  std::vector<int> trackHitCandidate;

  // Form all hit candidates

  //if (hitSet.getHits().size() < 10) return;

  for (hitSet::const_iterator hitIterO = hitSet.getHits().begin(); hitIterO != hitSet.getHits().end(); ++hitIterO,++hitNumber) {

    trackHitCandidate.push_back(hitNumber);
  }

  trackHitCandidates.push_back(trackHitCandidate);


}





void fc::PerfectTrackRecoModule::buildPerfectTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  TVector3 primaryVertex(0.0,0.0,0.0);
  for (std::vector<std::vector<int>>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    std::vector<int> trackHitCandidate = *trackHitCandidateIter;
    Track trackCandidate(hitSet,trackHitCandidate,primaryVertex,_detectorGeometry,_debugLevel);


    if (_debugLevel ==2) {
      std::cout << "Track after fit" << std::endl;
      trackCandidate.print();
    }


   trackCandidateSet.insertTrack(trackCandidate);
   }

}
