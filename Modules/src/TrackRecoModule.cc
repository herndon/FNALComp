#include<iostream>
#include<vector>
#include "DetectorGeometry.hh"
#include "HitSet.hh"
#include "Track.hh"
#include "TrackFit.hh"
#include "TrackSet.hh"
#include "TrackRecoModule.hh"

fc::TrackRecoModule::TrackRecoModule(int debugLevel, 
				     const std::string& inputHitsLabel, const std::string& outputTracksLabel,
				     const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inHitsLabel(inputHitsLabel),
  _outTracksLabel(outputTracksLabel),
  _detectorGeometry(detectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _detectorGeometry.getNSensors();

}

void fc::TrackRecoModule::processEvent(Event& event)
{

  Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);
  
  std::unique_ptr<TrackSet> recoTrackSet{ new TrackSet(_detectorGeometry) };

  recoTracks(*recoTrackSet,*recoHitSet);

  if (_debugLevel >= 2) recoHitSet->print();

  event.put(_outTracksLabel,std::move(recoTrackSet) );
}

void fc::TrackRecoModule::recoTracks(TrackSet & trackSet, const HitSet& hitSet)
{
  // !!!!! This will eventually pass a track hit candidates strategy 
  // !!!!! Do I want to make a special object for the track hit candidates?
  std::vector<std::vector<int>> trackHitCandidates;


  //findTrack2HitCandidates(trackHitCandidates,hitSet);
  //TrackSet trackCandidateSet(-1,1,_detectorGeometry);
  //build2HitTrackCandidates(trackCandidateSet,trackHitCandidates,hitSet);

  findTrack5HitCandidates(trackHitCandidates,hitSet);
  TrackSet trackCandidateSet(-1,1,_detectorGeometry);
  buildPerfectTrackCandidates(trackCandidateSet,trackHitCandidates,hitSet);



  // !!!!! This will eventually pass a tracking algorithm
  //findTracks(trackCandidateSet,hitSet);
  
}

void fc::TrackRecoModule::findTrack5HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  int hitNumberO = 0;


  // Form 4-3-2-1-0 hit candidates
  for (hitSet::const_iterator hitIterO = hitSet.getHits().begin(); hitIterO != hitSet.getHits().end(); ++hitIterO,++hitNumberO) {

    if (hitIterO->getLayer() == _nLayers-1) {

      int hitNumberI = 0;

      for (hitSet::const_iterator hitIterI = hitSet.getHits().begin(); hitIterI != hitSet.getHits().end(); ++hitIterI,++hitNumberI) {

	if (hitIterI->getLayer() == _nLayers-2) {

	  int hitNumberII = 0;

	  for (hitSet::const_iterator hitIterII = hitSet.getHits().begin(); hitIterII != hitSet.getHits().end(); ++hitIterII,++hitNumberII) {

	    if (hitIterII->getLayer() == _nLayers-3) {


	      int hitNumberIII = 0;

	      for (hitSet::const_iterator hitIterIII = hitSet.getHits().begin(); hitIterIII != hitSet.getHits().end(); ++hitIterIII,++hitNumberIII) {

		if (hitIterIII->getLayer() == _nLayers-4) {

		  int hitNumberIIII = 0;

		  for (hitSet::const_iterator hitIterIIII = hitSet.getHits().begin(); hitIterIIII != hitSet.getHits().end(); ++hitIterIIII,++hitNumberIIII) {

		    if (hitIterIIII->getLayer() == _nLayers-4) {


		      std::vector<int> trackHitCandidate;
		      trackHitCandidate.push_back(hitNumberO);
		      trackHitCandidate.push_back(hitNumberI);
		      trackHitCandidate.push_back(hitNumberII);
		      trackHitCandidate.push_back(hitNumberIII);
		      trackHitCandidate.push_back(hitNumberIIII);

		      trackHitCandidates.push_back(trackHitCandidate);
		    }
		  }
		}

	      }

	    }

	  }

	}

      }

    }

  }

}



void fc::TrackRecoModule::findTrack3HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  int hitNumberO = 0;


  // Form 4-3-2 hit candidates
  for (hitSet::const_iterator hitIterO = hitSet.getHits().begin(); hitIterO != hitSet.getHits().end(); ++hitIterO,++hitNumberO) {

    int hitNumberI = 0;

    //!!!!! change to outer layer at some point
    if (hitIterO->getLayer() == _nLayers-1) {

      for (hitSet::const_iterator hitIterI = hitSet.getHits().begin(); hitIterI != hitSet.getHits().end(); ++hitIterI,++hitNumberI) {

	int hitNumberII = 0;

	if (hitIterI->getLayer() == _nLayers-2) {

	  for (hitSet::const_iterator hitIterII = hitSet.getHits().begin(); hitIterII != hitSet.getHits().end(); ++hitIterII,++hitNumberII) {

	    if (hitIterII->getLayer() == _nLayers-3) {



	      std::vector<int> trackHitCandidate;
	      trackHitCandidate.push_back(hitNumberO);
	      trackHitCandidate.push_back(hitNumberI);
	      trackHitCandidate.push_back(hitNumberII);

	      trackHitCandidates.push_back(trackHitCandidate);

	    }

	  }

	}

      }

    }

  }

}

void fc::TrackRecoModule::findTrack2HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  int hitNumberO = 0;


  // Form 4-3 hit candidates
  for (hitSet::const_iterator hitIterO = hitSet.getHits().begin(); hitIterO != hitSet.getHits().end(); ++hitIterO,++hitNumberO) {

    int hitNumberI = 0;

    //!!!!! change to outer layer at some point
    if (hitIterO->getLayer() == _nLayers-1) {

      for (hitSet::const_iterator hitIterI = hitSet.getHits().begin(); hitIterI != hitSet.getHits().end(); ++hitIterI,++hitNumberI) {

	if (hitIterI->getLayer() == _nLayers-2) {

	  std::vector<int> trackHitCandidate;
          trackHitCandidate.push_back(hitNumberO);
          trackHitCandidate.push_back(hitNumberI);

          trackHitCandidates.push_back(trackHitCandidate);
	}

      }

    }

  }

}



void fc::TrackRecoModule::build2HitTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  TVector3 primaryVertex(0.0,0.0,0.0);
  for (std::vector<std::vector<int>>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    //Track trackCandidate(primaryVertex,hitSet.getHits()[(*trackHitCandidateIter)[2]].getHitPosition(),hitSet.getHits()[(*trackHitCandidateIter)[4]].getHitPosition(),primaryVertex,_detectorGeometry,_debugLevel);

    // !!!!! insert hit via constructor
    //trackCandidate.insertHit((*trackHitCandidateIter)[0],4);
    //trackCandidate.insertHit((*trackHitCandidateIter)[1],3);
    std::cout << "Track before fit" << std::endl;

    //if (_debugLevel >=2) trackCandidate.print();

    //trackCandidateSet.insertTrack(trackCandidate);
  }

}


void fc::TrackRecoModule::buildPerfectTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  TVector3 primaryVertex(0.0,0.0,0.0);
  for (std::vector<std::vector<int>>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    std::vector<int> trackHitCandidate = *trackHitCandidateIter;
    Track trackCandidate(hitSet,trackHitCandidate,primaryVertex,_detectorGeometry,_debugLevel);

    // !!!!! insert hit via constructor
//     trackCandidate.insertHit((*trackHitCandidateIter)[0],4);
//     trackCandidate.insertHit((*trackHitCandidateIter)[1],3);
//     trackCandidate.insertHit((*trackHitCandidateIter)[2],2);
//     trackCandidate.insertHit((*trackHitCandidateIter)[3],1);
//     trackCandidate.insertHit((*trackHitCandidateIter)[4],0);




    if (_debugLevel ==2) {
      //std::cout << "Track before fit" << std::endl;
      //trackCandidate.print();

      //std::cout << "Track from TrackFit" << std::endl;
      //testTrackFit1.print();
    }




    //trackCandidate.FitToHelix(hitSet,_detectorGeometry,false);

   //testTrackFit1.FitToHelix(hitSet,_detectorGeometry,false);

    if (_debugLevel ==2) {
      std::cout << "Track after fit" << std::endl;
      trackCandidate.print();

      //std::cout << "Track from TrackFit after fit" << std::endl;
      //testTrackFit1.print();
    }


   trackCandidateSet.insertTrack(trackCandidate);
   }

}
