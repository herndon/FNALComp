#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Tracking/include/TrackingSelectors.hh"
#include <iostream>

bool fcf::goodSeedHelix(const fc::Helix & helix,
                             const fc::DetectorGeometry& detectorGeometry,
                             const TrackingSelector& trackSelector) {

  return helix.pT() >= trackSelector._minPTCut;

}

bool fcf::goodTrack(const fc::Track& track,
                             const fc::DetectorGeometry& detectorGeometry,
                             const TrackingSelector& trackSelector) {


  return  track.pT() > trackSelector._minPTCut &&
    std::abs(track.dR()) < trackSelector._dRCut &&
    std::abs(track.dZ()) < trackSelector._dZCut &&
    (track.nDof()<=0
     || track.chi2()/track.nDof()< trackSelector._maxChi2NDofCut) &&
    track.trackHits().size() >= trackSelector._nHitCut &&
    (!trackSelector._useFiducialDRCut || std::abs(track.dR())
     < detectorGeometry.sensor(0)._perpSize) &&
    (!trackSelector._useFiducialDRCut || std::abs(track.dZ())
     < detectorGeometry.sensor(0)._nStrips*
     detectorGeometry.sensor(0)._stripPitch/2.0);

}
