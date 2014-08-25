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
				  bool& matchedTrackLoose, bool& matchedTrackXYLoose,bool& matchedTrackTight, bool& matchedTrackXYTight) {
    double bestDeltaTracks = 1000.0;
    double tmpDeltaTracks;
    int trackNumber=0;
    int bestTrack=-1;
 
    for (auto const& track : trackSet.tracks()) {

      tmpDeltaTracks = fcf::deltaTrackHPs(genTrack,track,detectorGeometry);

        if (tmpDeltaTracks < bestDeltaTracks) {
            bestDeltaTracks = tmpDeltaTracks;
            bestTrack = trackNumber;
        }
        ++trackNumber;
    }
    matchedTrackLoose = goodMatch(genTrack,trackSet.tracks()[bestTrack],50.0,detectorGeometry);
    matchedTrackXYLoose = goodMatchXY(genTrack,trackSet.tracks()[bestTrack],50.0,detectorGeometry);
    matchedTrackTight = goodMatch(genTrack,trackSet.tracks()[bestTrack],5.0,detectorGeometry);
    matchedTrackXYTight = goodMatchXY(genTrack,trackSet.tracks()[bestTrack],5.0,detectorGeometry);

    return trackSet.tracks()[bestTrack];

}

const fc::GenTrack & fcf::matchTrack(const fc::Track & track,
				  const fc::GenTrackSet& genTrackSet, const fc::DetectorGeometry& detectorGeometry, 
				     bool& matchedTrackLoose, bool& matchedTrackXYLoose,bool& matchedTrackTight, bool& matchedTrackXYTight) {

    double bestDeltaTracks = 1000.0;
    double tmpDeltaTracks;
    int trackNumber=0;
    int bestTrack=-1;
 
    for (auto const& genTrack : genTrackSet.genTracks()) {

      tmpDeltaTracks = fcf::deltaTrackHPs(genTrack,track,detectorGeometry);

        if (tmpDeltaTracks < bestDeltaTracks) {
            bestDeltaTracks = tmpDeltaTracks;
            bestTrack = trackNumber;
        }
        ++trackNumber;
    }
    matchedTrackLoose = goodMatch(genTrackSet.genTracks()[bestTrack],track,50.0,detectorGeometry);
    matchedTrackXYLoose = goodMatchXY(genTrackSet.genTracks()[bestTrack],track,50.0,detectorGeometry);
    matchedTrackTight = goodMatch(genTrackSet.genTracks()[bestTrack],track,5.0,detectorGeometry);
    matchedTrackXYTight = goodMatchXY(genTrackSet.genTracks()[bestTrack],track,5.0,detectorGeometry);


    return genTrackSet.genTracks()[bestTrack];

}


const fc::Track & fcf::matchTrack(const fc::Track & track,
				  const fc::TrackSet& trackSet, 
				  bool& matchedTrack, bool& matchedTrackXY) {

    double bestDeltaTracks = 1000.0;
    double tmpDeltaTracks;
    int trackNumber=0;
    int bestTrack=-1;
 
    for (auto const& track1 : trackSet.tracks()) {

      tmpDeltaTracks = fcf::deltaTrackHPs(track,track1);

        if (tmpDeltaTracks < bestDeltaTracks) {
            bestDeltaTracks = tmpDeltaTracks;
            bestTrack = trackNumber;
        }
        ++trackNumber;
    }
    matchedTrack = goodMatch(trackSet.tracks()[bestTrack],track);
    matchedTrackXY = goodMatchXY(trackSet.tracks()[bestTrack],track);

    return trackSet.tracks()[bestTrack];

}






bool fcf::goodMatch(const fc::Track & track,
		    const fc::Track& track1) {


  TVectorD dHP = fcf::deltaHP(track,track1);

    return (std::abs(dHP(0)/track1.sigmaDr()) < 5.0 &&
	    std::abs(dHP(1)/track1.sigmaPhi0()) < 5.0 &&
	    std::abs(dHP(2)/track1.sigmaKappa()) < 5.0 &&
	    std::abs(dHP(3)/track1.sigmaDz()) < 5.0 &&
	    std::abs(dHP(4)/track1.sigmaTanL()) < 5.0);

}

bool fcf::goodMatchXY(const fc::Track & track,
		      const fc::Track& track1) {

  TVectorD dHP = fcf::deltaHP(track,track1);

    return (std::abs(dHP(0)/track1.sigmaDr()) < 5.0 &&
	    std::abs(dHP(1)/track1.sigmaPhi0()) < 5.0 &&
	    std::abs(dHP(2)/track1.sigmaKappa()) < 5.0);

}

bool fcf::goodMatch(const fc::GenTrack & genTrack,
		    const fc::Track& track,double sigma, const fc::DetectorGeometry& detectorGeometry) {



  TVectorD dHP = fcf::deltaHP(genTrack,track,detectorGeometry);


    return (std::abs(dHP(0)/track.sigmaDr()) < sigma &&
	    std::abs(dHP(1)/track.sigmaPhi0()) < sigma &&
	    std::abs(dHP(2)/track.sigmaKappa()) < sigma &&
	    std::abs(dHP(3)/track.sigmaDz()) < sigma &&
	    std::abs(dHP(4)/track.sigmaTanL()) < sigma);

}

bool fcf::goodMatchXY(const fc::GenTrack & genTrack,
		      const fc::Track& track,double sigma, const fc::DetectorGeometry& detectorGeometry) {

  TVectorD dHP = fcf::deltaHP(genTrack,track,detectorGeometry);

    return (std::abs(dHP(0)/track.sigmaDr()) < sigma &&
	    std::abs(dHP(1)/track.sigmaPhi0()) < sigma &&
	    std::abs(dHP(2)/track.sigmaKappa()) < sigma);

}



TVectorD fcf::deltaHP(const fc::GenTrack & genTrack,
		      const fc::Track& track,const fc::DetectorGeometry& detectorGeometry) {


    return track.helixParam() - genTrack.makeHelix(
               detectorGeometry.bField(),detectorGeometry.curvatureC()).helixParam();

}

TVectorD fcf::deltaHP(const fc::Track & track,
		      const fc::Track& track1) {


  return track.helixParam() - track1.helixParam();

}


double fcf::deltaTrackHPs(const fc::GenTrack & genTrack,
			  const fc::Track& track,const fc::DetectorGeometry& detectorGeometry) {

  fc::Helix helix(genTrack.makeHelix(detectorGeometry.bField(),
				 detectorGeometry.curvatureC()));

 
  return std::sqrt((helix.kappa()-track.kappa())*
		   (helix.kappa()-track.kappa())+
		   (helix.phi0()-track.phi0())*
		   (helix.phi0()-track.phi0()));


}

double fcf::deltaTrackHPs(const fc::Track & track0,
			  const fc::Track& track1) {


  return std::sqrt( (track0.kappa()-track1.kappa())*
		    (track0.kappa()-track1.kappa())+
		    (track0.phi0()-track1.phi0())*
		    (track0.phi0()-track1.phi0()));


}
