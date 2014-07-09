#include<iostream>
#include<vector>
#include "DetectorGeometry.hh"
#include "HitSet.hh"
#include "Track.hh"
#include "TrackFit.hh"
#include "TrackSet.hh"
#include "TrackRecoModule.hh"

fc::TrackRecoModule::TrackRecoModule(int debugLevel, const DetectorGeometry & myDetectorGeometry):
  _debugLevel(debugLevel),
  _myDetectorGeometry(myDetectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _myDetectorGeometry.getNSensors();

}

void fc::TrackRecoModule::processEvent(TrackSet & myTrackSet, const HitSet& myHitSet)
{

  recoTracks(myTrackSet,myHitSet);

  if (_debugLevel >= 2) myHitSet.print();

}

void fc::TrackRecoModule::recoTracks(TrackSet & myTrackSet, const HitSet& myHitSet)
{
  // !!!!! This will eventually pass a track hit candidates strategy 
  // !!!!! Do I want to make a special object for the track hit candidates?
  std::vector<std::vector<int>> trackHitCandidates;


  //findTrack2HitCandidates(trackHitCandidates,myHitSet);
  //TrackSet trackCandidateSet(-1,1,_myDetectorGeometry);
  //build2HitTrackCandidates(trackCandidateSet,trackHitCandidates,myHitSet);

  findTrack5HitCandidates(trackHitCandidates,myHitSet);
  TrackSet trackCandidateSet(-1,1,_myDetectorGeometry);
  buildPerfectTrackCandidates(trackCandidateSet,trackHitCandidates,myHitSet);



  // !!!!! This will eventually pass a tracking algorithm
  //findTracks(myTrackCandidateSet,myHitSet);
  
}

void fc::TrackRecoModule::findTrack5HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet){

  int hitNumberO = 0;


  // Form 4-3-2-1-0 hit candidates
  for (std::vector<Hit>::const_iterator hitIterO = myHitSet.getHitVector().begin(); hitIterO != myHitSet.getHitVector().end(); ++hitIterO,++hitNumberO) {

    if (hitIterO->getLayer() == _nLayers-1) {

      int hitNumberI = 0;

      for (std::vector<Hit>::const_iterator hitIterI = myHitSet.getHitVector().begin(); hitIterI != myHitSet.getHitVector().end(); ++hitIterI,++hitNumberI) {

	if (hitIterI->getLayer() == _nLayers-2) {

	  int hitNumberII = 0;

	  for (std::vector<Hit>::const_iterator hitIterII = myHitSet.getHitVector().begin(); hitIterII != myHitSet.getHitVector().end(); ++hitIterII,++hitNumberII) {

	    if (hitIterII->getLayer() == _nLayers-3) {


	      int hitNumberIII = 0;

	      for (std::vector<Hit>::const_iterator hitIterIII = myHitSet.getHitVector().begin(); hitIterIII != myHitSet.getHitVector().end(); ++hitIterIII,++hitNumberIII) {

		if (hitIterIII->getLayer() == _nLayers-4) {

		  int hitNumberIIII = 0;

		  for (std::vector<Hit>::const_iterator hitIterIIII = myHitSet.getHitVector().begin(); hitIterIIII != myHitSet.getHitVector().end(); ++hitIterIIII,++hitNumberIIII) {

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



void fc::TrackRecoModule::findTrack3HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet){

  int hitNumberO = 0;


  // Form 4-3-2 hit candidates
  for (std::vector<Hit>::const_iterator hitIterO = myHitSet.getHitVector().begin(); hitIterO != myHitSet.getHitVector().end(); ++hitIterO,++hitNumberO) {

    int hitNumberI = 0;

    //!!!!! change to outer layer at some point
    if (hitIterO->getLayer() == _nLayers-1) {

      for (std::vector<Hit>::const_iterator hitIterI = myHitSet.getHitVector().begin(); hitIterI != myHitSet.getHitVector().end(); ++hitIterI,++hitNumberI) {

	int hitNumberII = 0;

	if (hitIterI->getLayer() == _nLayers-2) {

	  for (std::vector<Hit>::const_iterator hitIterII = myHitSet.getHitVector().begin(); hitIterII != myHitSet.getHitVector().end(); ++hitIterII,++hitNumberII) {

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

void fc::TrackRecoModule::findTrack2HitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet){

  int hitNumberO = 0;


  // Form 4-3 hit candidates
  for (std::vector<Hit>::const_iterator hitIterO = myHitSet.getHitVector().begin(); hitIterO != myHitSet.getHitVector().end(); ++hitIterO,++hitNumberO) {

    int hitNumberI = 0;

    //!!!!! change to outer layer at some point
    if (hitIterO->getLayer() == _nLayers-1) {

      for (std::vector<Hit>::const_iterator hitIterI = myHitSet.getHitVector().begin(); hitIterI != myHitSet.getHitVector().end(); ++hitIterI,++hitNumberI) {

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



void fc::TrackRecoModule::build2HitTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet){

  TVector3 primaryVertex(0.0,0.0,0.0);
  for (std::vector<std::vector<int>>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    //Track trackCandidate(primaryVertex,myHitSet.getHitVector()[(*trackHitCandidateIter)[2]].getHitPosition(),myHitSet.getHitVector()[(*trackHitCandidateIter)[4]].getHitPosition(),primaryVertex,_myDetectorGeometry,_debugLevel);

    // !!!!! insert hit via constructor
    //trackCandidate.insertHit((*trackHitCandidateIter)[0],4);
    //trackCandidate.insertHit((*trackHitCandidateIter)[1],3);
    std::cout << "Track before fit" << std::endl;

    //if (_debugLevel >=2) trackCandidate.print();

    //trackCandidateSet.insertTrack(trackCandidate);
  }

}


void fc::TrackRecoModule::buildPerfectTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & myHitSet){

  TVector3 primaryVertex(0.0,0.0,0.0);
  for (std::vector<std::vector<int>>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    std::vector<int> trackHitCandidate = *trackHitCandidateIter;
    Track trackCandidate(myHitSet,trackHitCandidate,primaryVertex,_myDetectorGeometry,_debugLevel);

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




    //trackCandidate.FitToHelix(myHitSet,_myDetectorGeometry,false);

   //testTrackFit1.FitToHelix(myHitSet,_myDetectorGeometry,false);

    if (_debugLevel ==2) {
      std::cout << "Track after fit" << std::endl;
      trackCandidate.print();

      //std::cout << "Track from TrackFit after fit" << std::endl;
      //testTrackFit1.print();
    }


   trackCandidateSet.insertTrack(trackCandidate);
   }

}
