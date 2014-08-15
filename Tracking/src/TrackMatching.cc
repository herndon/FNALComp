#include <cmath>
#include "DataObjects/include/Helix.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/TrackMatching.hh"
#include <iostream>


const fc::Track & fcf::matchTrack(const fc::GenTrack & genTrack,
				  const fc::TrackSet& trackSet, const fc::DetectorGeometry& detectorGeometry, 
				  bool& matchedTrack, bool& matchedTrackXY) {

    double bestDeltaTracks = 1000.0;
    double tmpDeltaTracks;
    int trackNumber=0;
    int bestTrack=-1;
 
    for (auto const& track : trackSet.getTracks()) {

      tmpDeltaTracks = fcf::deltaTrackHPs(genTrack,track,detectorGeometry);

        if (tmpDeltaTracks < bestDeltaTracks) {
            bestDeltaTracks = tmpDeltaTracks;
            bestTrack = trackNumber;
        }
        ++trackNumber;
    }
    matchedTrack = goodMatch(genTrack,trackSet.getTracks()[bestTrack],detectorGeometry);
    matchedTrackXY = goodMatchXY(genTrack,trackSet.getTracks()[bestTrack],detectorGeometry);

    return trackSet.getTracks()[bestTrack];

}

const fc::GenTrack & fcf::matchTrack(const fc::Track & track,
				  const fc::GenTrackSet& genTrackSet, const fc::DetectorGeometry& detectorGeometry, 
				  bool& matchedTrack, bool& matchedTrackXY) {

    double bestDeltaTracks = 1000.0;
    double tmpDeltaTracks;
    int trackNumber=0;
    int bestTrack=-1;
 
    for (auto const& genTrack : genTrackSet.getGenTracks()) {

      tmpDeltaTracks = fcf::deltaTrackHPs(genTrack,track,detectorGeometry);

        if (tmpDeltaTracks < bestDeltaTracks) {
            bestDeltaTracks = tmpDeltaTracks;
            bestTrack = trackNumber;
        }
        ++trackNumber;
    }
    matchedTrack = goodMatch(genTrackSet.getGenTracks()[bestTrack],track,detectorGeometry);
    matchedTrackXY = goodMatchXY(genTrackSet.getGenTracks()[bestTrack],track,detectorGeometry);

    return genTrackSet.getGenTracks()[bestTrack];

}


const fc::Track & fcf::matchTrack(const fc::Track & track,
				  const fc::TrackSet& trackSet, 
				  bool& matchedTrack, bool& matchedTrackXY) {

    double bestDeltaTracks = 1000.0;
    double tmpDeltaTracks;
    int trackNumber=0;
    int bestTrack=-1;
 
    for (auto const& track1 : trackSet.getTracks()) {

      tmpDeltaTracks = fcf::deltaTrackHPs(track,track1);

        if (tmpDeltaTracks < bestDeltaTracks) {
            bestDeltaTracks = tmpDeltaTracks;
            bestTrack = trackNumber;
        }
        ++trackNumber;
    }
    matchedTrack = goodMatch(trackSet.getTracks()[bestTrack],track);
    matchedTrackXY = goodMatchXY(trackSet.getTracks()[bestTrack],track);

    return trackSet.getTracks()[bestTrack];

}






bool fcf::goodMatch(const fc::Track & track,
		    const fc::Track& track1) {


  TVectorD dHP = fcf::deltaHP(track,track1);

    return (std::abs(dHP(0)/track1.getSigmaDr()) < 10.0 &&
	    std::abs(dHP(1)/track1.getSigmaPhi0()) < 10.0 &&
	    std::abs(dHP(2)/track1.getSigmaKappa()) < 10.0 &&
	    std::abs(dHP(3)/track1.getSigmaDz()) < 10.0 &&
	    std::abs(dHP(4)/track1.getSigmaTanL()) < 10.0);

}

bool fcf::goodMatchXY(const fc::Track & track,
		      const fc::Track& track1) {

  TVectorD dHP = fcf::deltaHP(track,track1);

    return (std::abs(dHP(0)/track1.getSigmaDr()) < 10.0 &&
	    std::abs(dHP(1)/track1.getSigmaPhi0()) < 10.0 &&
	    std::abs(dHP(2)/track1.getSigmaKappa()) < 10.0);

}

bool fcf::goodMatch(const fc::GenTrack & genTrack,
		    const fc::Track& track,const fc::DetectorGeometry& detectorGeometry) {



  TVectorD dHP = fcf::deltaHP(genTrack,track,detectorGeometry);

  if (std::abs(dHP(4)/track.getSigmaTanL()) > 10.0){
    //std::cout << "Reco Track " << track.getHelix().getTanL() << " " << track.getLorentzVector().Pz() << "/" << track.getLorentzVector().Pt() << std::endl;
    //std::cout << "Gen  Track " << genTrack.makeHelix(detectorGeometry.getBField(),detectorGeometry.getCurvatureC()).getTanL() << " " << genTrack.getLorentzVector().Pz() << "/" << genTrack.getLorentzVector().Pt() << std::endl;
  }

    return (std::abs(dHP(0)/track.getSigmaDr()) < 10.0 &&
	    std::abs(dHP(1)/track.getSigmaPhi0()) < 10.0 &&
	    std::abs(dHP(2)/track.getSigmaKappa()) < 10.0 &&
	    std::abs(dHP(3)/track.getSigmaDz()) < 10.0 &&
	    std::abs(dHP(4)/track.getSigmaTanL()) < 10.0);

}

bool fcf::goodMatchXY(const fc::GenTrack & genTrack,
		      const fc::Track& track,const fc::DetectorGeometry& detectorGeometry) {

  TVectorD dHP = fcf::deltaHP(genTrack,track,detectorGeometry);

    return (std::abs(dHP(0)/track.getSigmaDr()) < 10.0 &&
	    std::abs(dHP(1)/track.getSigmaPhi0()) < 10.0 &&
	    std::abs(dHP(2)/track.getSigmaKappa()) < 10.0);

}



TVectorD fcf::deltaHP(const fc::GenTrack & genTrack,
		      const fc::Track& track,const fc::DetectorGeometry& detectorGeometry) {


    return track.getHelix().getHelix() - genTrack.makeHelix(
               detectorGeometry.getBField(),detectorGeometry.getCurvatureC()).getHelix();

}

TVectorD fcf::deltaHP(const fc::Track & track,
		      const fc::Track& track1) {


  return track.getHelix().getHelix() - track1.getHelix().getHelix();

}


double fcf::deltaTrackHPs(const fc::GenTrack & genTrack,
			  const fc::Track& track,const fc::DetectorGeometry& detectorGeometry) {

  fc::Helix helix(genTrack.makeHelix(detectorGeometry.getBField(),
				 detectorGeometry.getCurvatureC()));

 
  return std::sqrt((helix.getKappa()-track.getHelix().getKappa())*
		   (helix.getKappa()-track.getHelix().getKappa())+
		   (helix.getPhi0()-track.getHelix().getPhi0())*
		   (helix.getPhi0()-track.getHelix().getPhi0()));


}

double fcf::deltaTrackHPs(const fc::Track & track0,
			  const fc::Track& track1) {


  return std::sqrt( (track0.getHelix().getKappa()-track1.getHelix().getKappa())*
		    (track0.getHelix().getKappa()-track1.getHelix().getKappa())+
		    (track0.getHelix().getPhi0()-track1.getHelix().getPhi0())*
		    (track0.getHelix().getPhi0()-track1.getHelix().getPhi0()));


}
