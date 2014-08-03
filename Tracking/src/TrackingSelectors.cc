#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Tracking/include/TrackingSelectors.hh"

bool fcf::goodCandidateHelix(const fc::Helix & helix,
                             const fc::DetectorGeometry& detectorGeometry,
                             const TrackingSelector& trackSelector) {

    return (helix.getPT() >= trackSelector._minPTCut);

}

bool fcf::goodCandidateTrack(const fc::Track& track,
                             const fc::DetectorGeometry& detectorGeometry,
                             const TrackingSelector& trackSelector) {

    return  track.getHelix().getPT() > trackSelector._minPTCut &&
            (track.getNDof()<=0
             || track.getChi2()/track.getNDof()< trackSelector._maxChi2NDofCut) &&
            track.getHits().size() >= trackSelector._nHitCut &&
            (!trackSelector._useFiducialDRCut || std::abs(track.getHelix().getDr())
             < detectorGeometry.getSensor(detectorGeometry.getNSensors()-1)._perpSize) &&
            (!trackSelector._useFiducialDRCut || std::abs(track.getHelix().getDz())
             < detectorGeometry.getSensor(detectorGeometry.getNSensors()-1)._nStrips*
             detectorGeometry.getSensor(detectorGeometry.getNSensors()-1)._stripPitch/2.0);

}
