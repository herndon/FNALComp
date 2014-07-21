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
  //TrackSet trackCandidateSet(-1,1,_detectorGeometry);
  build2X1SASHitTrackCandidates(trackSet,trackHitCandidates,hitSet);




  // !!!!! This will eventually pass a tracking algorithm
  //findTracks(trackCandidateSet,hitSet);
  
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

	      // !!!!! need to check if intersection is at reasonable place.

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
    std::vector<int> trackHitCandidate = *trackHitCandidateIter;
    Track trackCandidate(hitSet,trackHitCandidate,primaryVertex,_detectorGeometry,_debugLevel);

    if (_debugLevel ==2) {
      std::cout << "Track after fit" << std::endl;
      trackCandidate.print();
    }

   trackCandidateSet.insertTrack(trackCandidate);


//    for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer){
//      double uncer = expectedMeasurementUncertianty1D(trackCandidate.getHelix(), trackCandidate.getCovMatrix(), ii_layer, _detectorGeometry);
//      std::cout << "Uncertainty on layer " << ii_layer << ": " << uncer << std::endl;

//   }


  }

}


