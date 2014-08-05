#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Tracking/include/TrackingSelectors.hh"

bool fcf::goodCandidateHelix(const fc::Helix & helix,
                             const fc::DetectorGeometry& detectorGeometry,
                             const TrackingSelector& trackSelector) {

  return helix.getPT() >= trackSelector._minPTCut &&
        std::abs(helix.getDr()) < 0.05 &&  std::abs(helix.getDr()) < 0.05 &&
    (!trackSelector._useFiducialDRCut || std::abs(helix.getDr())
     < detectorGeometry.getSensor(0)._perpSize) &&
    (!trackSelector._useFiducialDRCut || std::abs(helix.getDz())
     < detectorGeometry.getSensor(0)._nStrips*
     detectorGeometry.getSensor(0)._stripPitch/2.0);

}

bool fcf::goodCandidateTrack(const fc::Track& track,
                             const fc::DetectorGeometry& detectorGeometry,
                             const TrackingSelector& trackSelector) {

  return  track.getHelix().getPT() > trackSelector._minPTCut &&
    (track.getNDof()<=0
     || track.getChi2()/track.getNDof()< trackSelector._maxChi2NDofCut) &&
    track.getHits().size() >= trackSelector._nHitCut &&
    //     std::abs(track.getHelix().getDr()) < 0.05 &&  std::abs(track.getHelix().getDr()) < 0.05 &&
    (!trackSelector._useFiducialDRCut || std::abs(track.getHelix().getDr())
     < detectorGeometry.getSensor(0)._perpSize) &&
    (!trackSelector._useFiducialDRCut || std::abs(track.getHelix().getDz())
     < detectorGeometry.getSensor(0)._nStrips*
     detectorGeometry.getSensor(0)._stripPitch/2.0);

}
