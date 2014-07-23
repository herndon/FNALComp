#include<iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackRecoModule.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Algorithms/include/InitializeHelix.hh"


fc::TrackRecoModule::TrackRecoModule(int debugLevel, 
				     const std::string& inputHitsLabel, const std::string& outputTracksLabel,
				     const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inHitsLabel(inputHitsLabel),
  _outTracksLabel(outputTracksLabel),
  _detectorGeometry(detectorGeometry),
  _candidatePTThreshold(1.0) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _detectorGeometry.getNSensors();

}

void fc::TrackRecoModule::processEvent(Event& event)
{

  Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);
  
  std::unique_ptr<TrackSet> recoTrackSet{ new TrackSet(_detectorGeometry) };

  recoTracks(*recoTrackSet,*recoHitSet);


  event.put(_outTracksLabel,std::move(recoTrackSet) );
}

void fc::TrackRecoModule::recoTracks(TrackSet & trackSet, const HitSet& hitSet)
{
  // !!!!! This will eventually pass a track hit candidates strategy 
  // !!!!! Do I want to make a special object for the track hit candidates?
  std::vector<std::vector<int>> trackHitCandidates;


  findTrack2X1SASHitCandidates(trackHitCandidates,hitSet);
  build2X1SASHitTrackCandidates(trackSet,trackHitCandidates,hitSet);
  perform2X1SASTracking(trackSet,hitSet);
 
  cleanTrackSet(trackSet);


  if (_debugLevel>=2){
    std::cout << "Reconstructed track set" << std::endl;
    trackSet.print();
  }

}



void fc::TrackRecoModule::findTrack3X2SASHitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

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

void fc::TrackRecoModule::findTrack2X1SASHitCandidates(std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  int hitNumberO = 0;

  if (hitSet.getHits().size() < 10) return;

  // Form 4-3,9 hit candidates
  for (hitSet::const_iterator hitIterO = hitSet.getHits().begin(); hitIterO != hitSet.getHits().end(); ++hitIterO,++hitNumberO) {


    if (hitIterO->getLayer() == 4) {

      int hitNumberI = 0;
 
      for (hitSet::const_iterator hitIterI = hitSet.getHits().begin(); hitIterI != hitSet.getHits().end(); ++hitIterI,++hitNumberI) {

	if (hitIterI->getLayer() == 3) {

	  int hitNumberOSAS = 0;

	  for (hitSet::const_iterator hitIterOSAS = hitSet.getHits().begin(); hitIterOSAS != hitSet.getHits().end(); ++hitIterOSAS,++hitNumberOSAS) {

	    if (hitIterOSAS->getLayer() == 9) {

	      TVector3 zIntersection;
	      bool goodIntersection = intersectStrips(*hitIterO,*hitIterOSAS,zIntersection,_detectorGeometry);
	      if (goodIntersection) {

                TVector3 primaryVertex(0.0,0.0,0.0);
		Helix helix = initializeHelix(primaryVertex,hitIterO->getHitPosition(),hitIterI->getHitPosition(),zIntersection,_detectorGeometry);
                TVectorD helixParameters = helix.getHelix();
		// !!!!! change to function goodCandidate
		if (helix.getPT() > _candidatePTThreshold) {
		  std::vector<int> trackHitCandidate;

		  trackHitCandidate.push_back(hitNumberO);
		  trackHitCandidate.push_back(hitNumberI);
		  trackHitCandidate.push_back(hitNumberOSAS);

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



void fc::TrackRecoModule::build2X1SASHitTrackCandidates(TrackSet & trackCandidateSet, const std::vector<std::vector<int>> & trackHitCandidates,const HitSet & hitSet){

  TVector3 primaryVertex(0.0,0.0,0.0);
  for (std::vector<std::vector<int>>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    Track trackCandidate(hitSet,*trackHitCandidateIter,primaryVertex,_detectorGeometry,_debugLevel);
    trackCandidateSet.insertTrack(trackCandidate);
  }

}

void fc::TrackRecoModule::layerTracking(TrackSet & trackCandidateSet,const HitSet & recoHitSet,int layer,int expNHits) {


  trackSet newTracks;

  for (trackSet::iterator trackSetIter = trackCandidateSet.getTracks().begin(); trackSetIter!= trackCandidateSet.getTracks().end(); ++trackSetIter){

    trackSet foundTracks;
    if (trackSetIter->getHits().size() < (expNHits-2)) continue;

    bool hasLayerHit = false;
    for (trackHitSet::const_iterator hitIter = trackSetIter->getHits().begin(); hitIter!=trackSetIter->getHits().end(); ++hitIter){
      if (recoHitSet.getHits()[*hitIter].getLayer() == layer){
	hasLayerHit = true;
      }
    }
    if (hasLayerHit) continue;


      double uncer = expectedMeasurementUncertianty1D(trackSetIter->getHelix(), trackSetIter->getCovMatrix(), layer, _detectorGeometry);
      double maxResidual = 5.0*uncer;
      double residual;

      int hitNumber = 0;
      for (hitSet::const_iterator hitSetIter = recoHitSet.getHits().begin(); hitSetIter != recoHitSet.getHits().end(); ++hitSetIter,++hitNumber){

	if (hitSetIter->getLayer()==layer){
	  // !!!!! Replace with residual function
	  residual = std::abs(expectedMeasurementVectorX(trackSetIter->getHelix(),hitSetIter->getLayer(),_detectorGeometry)(0,0) - measurementVector1D(hitSetIter->getHitPosition(),hitSetIter->getLayer(),_detectorGeometry)(0,0));
	  if ( residual < maxResidual) {

	  std::vector<int> trackHitCandidate;
	  for (trackHitSet::const_iterator hitIter = trackSetIter->getHits().begin(); hitIter!=trackSetIter->getHits().end(); ++hitIter){
	    trackHitCandidate.push_back(*hitIter);
	  }
	    trackHitCandidate.push_back(hitNumber);

	    TVector3 primaryVertex(0.0,0.0,0.0);
	  Track newTrack(recoHitSet,trackHitCandidate,primaryVertex,_detectorGeometry, _debugLevel);

	    // !!!!! replace with good candidate call

            if (newTrack.getHelix().getPT() > 1.0 && newTrack.getChi2()< 100.0 && std::abs(newTrack.getHelix().getDr()) < 0.05 && std::abs(newTrack.getHelix().getDz()) < 0.05 && newTrack.getHits().size() > (expNHits-2)) {
	      foundTracks.push_back(newTrack);


	    }

	  }

	}

      } // End loop over hits
 
      // keep the two best or any less than chi2()/ndof 1
      double bestChi2 = 999.0;
      double secondBestChi2 = 999.0;
      int bestTrack = -1;
      int secondBestTrack = -1;
      int foundTrackN = 0;

	for (trackSet::const_iterator foundTracksIter = foundTracks.begin(); foundTracksIter != foundTracks.end(); ++foundTracksIter,++foundTrackN){
	  if (foundTracksIter->getNDof() > 0 && foundTracksIter->getChi2() < bestChi2) {
	    secondBestChi2 = bestChi2;
            secondBestTrack = bestTrack;
	    bestChi2 = foundTracksIter->getChi2();
            bestTrack = foundTrackN;
	  } else if (foundTracksIter->getNDof() > 0 && foundTracksIter->getChi2() < secondBestChi2){
	    secondBestChi2 = foundTracksIter->getChi2();
            secondBestTrack = foundTrackN;
	  }

	}
	if (foundTracks.size()>0) {
	  if (foundTracks.begin()->getNDof() <1 ){
	    for (trackSet::const_iterator foundTracksIter = foundTracks.begin(); foundTracksIter != foundTracks.end(); ++foundTracksIter,++foundTrackN){
	      newTracks.push_back(*foundTracksIter);
	    }
	  } else {
	    if (bestTrack > -1)  newTracks.push_back(foundTracks[bestTrack]);
	    if (secondBestTrack > -1)  newTracks.push_back(foundTracks[secondBestTrack]);

	  }
	}

	if (trackSetIter->getNDof()> 0 && secondBestTrack > -1 && secondBestChi2/foundTracks[secondBestTrack].getNDof() <  trackSetIter->getChi2()/trackSetIter->getNDof()) {
	  trackCandidateSet.getTracks().erase(trackSetIter);
          trackSetIter--;
	}



   } // End loop over tracks



  for (std::vector<Track>::const_iterator newTrackIter = newTracks.begin(); newTrackIter != newTracks.end(); ++newTrackIter){

    trackCandidateSet.insertTrack(*newTrackIter);

  }

 
  

  for (trackSet::iterator trackIter = trackCandidateSet.getTracks().begin(); trackIter != trackCandidateSet.getTracks().end(); ++trackIter){

    if (trackIter->getHits().size() < expNHits-1) {
 	  trackCandidateSet.getTracks().erase(trackIter);
	  trackIter--;
    }

  }

}


void fc::TrackRecoModule::perform2X1SASTracking(TrackSet & trackSet, const HitSet& hitSet){


  layerTracking(trackSet,hitSet,8,4);


  layerTracking(trackSet,hitSet,2,5);
  layerTracking(trackSet,hitSet,1,6);
  layerTracking(trackSet,hitSet,0,7);


  layerTracking(trackSet,hitSet,7,8);
  layerTracking(trackSet,hitSet,6,9);
  layerTracking(trackSet,hitSet,5,10);

}


void fc::TrackRecoModule::cleanTrackSet(TrackSet & trackCandidateSet) {


  for (trackSet::iterator trackIter = trackCandidateSet.getTracks().begin(); trackIter != trackCandidateSet.getTracks().end(); ++trackIter){
 
    trackSet::iterator trackIter2 = trackIter;
    trackIter2++;

    for ( ; trackIter2 != trackCandidateSet.getTracks().end(); ++trackIter2){

 
      int matchedHits = 0;
      for (trackHitSet::const_iterator hitIter = trackIter->getHits().begin(); hitIter!=trackIter->getHits().end(); ++hitIter){
 

	for (trackHitSet::const_iterator hitIter2 =  trackIter2->getHits().begin(); hitIter2!=trackIter2->getHits().end(); ++hitIter2){
 
	  if (*hitIter == *hitIter2) ++matchedHits;
	}

      }

      if (matchedHits>4) {
 	if ((trackIter->getHits().size() <= trackIter2->getHits().size() && trackIter->getChi2()/trackIter->getNDof()> trackIter2->getChi2()/trackIter2->getNDof())||
	    trackIter->getHits().size() < trackIter2->getHits().size()) {
	  trackCandidateSet.getTracks().erase(trackIter);
 	  trackIter--;
	  break;
	} else if ((trackIter2->getHits().size() <= trackIter->getHits().size() && trackIter2->getChi2()/trackIter2->getNDof()> trackIter->getChi2()/trackIter->getNDof())||
		   trackIter2->getHits().size() <= trackIter->getHits().size()){
	  trackCandidateSet.getTracks().erase(trackIter2);
	  trackIter2--;
	} else if (trackIter2->getHits().size() != trackIter->getHits().size()){





	}

      }
    }
  }

}
