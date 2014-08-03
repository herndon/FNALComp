#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Algorithms/include/BuildTrack.hh"
#include "Tracking/include/TrackingSelectors.hh"
#include "Tracking/include/TrackingFilters.hh"
#include "Tracking/include/LayerTrackFinder.hh"


fc::LayerTrackFinder::LayerTrackFinder(int debugLevel,
                                       const DetectorGeometry& detectorGeometry,int layer,int nExpHits,double minPTCut,
                                       double maxChi2NDofCut):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _layer(layer),
    _nExpHits(nExpHits),
    _minPTCut(minPTCut),
    _maxChi2NDofCut(maxChi2NDofCut) {
}

void fc::LayerTrackFinder::findCandidateTracks(const HitSet& recoHitSet,
        unsigned int expNHit,TrackSetContainer& trackCandidateSet) const {

    TrackSetContainer allNewTracks;

    for (auto& track :  trackCandidateSet) {

        findSingleCandidateTracks(track,recoHitSet,allNewTracks);

    }

    trackCandidateSet.reserve(trackCandidateSet.size()+allNewTracks.size());
    for (auto& track: allNewTracks) {
        trackCandidateSet.push_back(std::move(track));
    }

    fcf::TrackingSelector trackSelector;
    trackSelector._nHitCut = _nExpHits;

    fcf::simpleTrackSetFilter(_detectorGeometry,trackSelector,trackCandidateSet);
    //layerTrackFilter(trackCandidateSet,expNHit);

}

void fc::LayerTrackFinder::findSingleCandidateTracks(const Track& track,
        const HitSet& recoHitSet, TrackSetContainer& allNewTracks) const {



    std::vector<int> hits = findHits(track,recoHitSet);
    TrackSetContainer newTracks = buildTrackCandidates(track, hits, recoHitSet);
    std::vector<int> bestTracks = bestTrackCandidates(newTracks);
    //We may want to decide whether to remove the seed track
    //removeSeedTrack(trackCandidateSet,trackSet);

    for (auto trackNumber: bestTracks) {
        allNewTracks.push_back(std::move(newTracks[trackNumber]));
    }

}

std::vector<int>  fc::LayerTrackFinder::findHits(const Track & track ,
        const HitSet & recoHitSet) const {

    std::vector<int> hits;

    bool hasLayerHit = false;
    for (auto hitIndex : track.getHits()) {
        if (recoHitSet.getHits()[hitIndex].getLayer() == _layer) {
            hasLayerHit = true;
        }
    }
    if (hasLayerHit) return hits;

    double maxResidual = 5.0*expectedMeasurementUncertianty1D(track.getHelix(),
                         track.getCovMatrix(), _layer, _detectorGeometry);
    int hitNumber = 0;

    for (auto hit : recoHitSet.getHits()) {

        if (hit.getLayer()==_layer) {

            if (std::abs(expectedMeasurementVector1D(track.getHelix(),_layer,
                         _detectorGeometry)(0,0)
                         - measurementVector1D(hit.getHitPosition(),_layer,_detectorGeometry)(0,0))
                    < maxResidual) {

                hits.push_back(hitNumber);

            }

        }

        ++hitNumber;
    } // End loop over hits

    return hits;

}

std::vector<int> fc::LayerTrackFinder::bestTrackCandidates(
    const TrackSetContainer & tracks) const {

    std::vector<int> trackList;

    // keep the two best and all before the fit is constrined
    double bestChi2 = 999.0;
    double secondBestChi2 = 999.0;
    int bestTrack = -1;
    int secondBestTrack = -1;
    int trackNumber = 0;


    for (auto const& track : tracks) {
        if (track.getNDof() > 0 && track.getChi2() < bestChi2) {
            secondBestChi2 = bestChi2;
            secondBestTrack = bestTrack;
            bestChi2 = track.getChi2();
            bestTrack = trackNumber;
        } else if (track.getNDof() > 0 && track.getChi2() < secondBestChi2) {
            secondBestChi2 = track.getChi2();
            secondBestTrack = trackNumber;
        }
        ++trackNumber;
    }

    trackNumber = 0;

    // Keep all if not yet constrainted since we can't compare chi2/ndof
    for (auto const& track : tracks) {
        if (track.getNDof() <= 0) trackList.push_back(trackNumber);
        ++trackNumber;
    }


    if (bestTrack > -1)  trackList.push_back(bestTrack);
    if (secondBestTrack > -1)  trackList.push_back(secondBestTrack);

    // !!!!! could consider getting rid of the original track if the new ones are much better

    return trackList;

}



fc::TrackSetContainer fc::LayerTrackFinder::buildTrackCandidates(
    const Track & track, const std::vector<int> & hits,
    const HitSet & recoHitSet) const {

    fcf::TrackingSelector trackSelector;
    trackSelector._minPTCut = _minPTCut;
    trackSelector._maxChi2NDofCut = _maxChi2NDofCut;
    trackSelector._nHitCut = _nExpHits;
    trackSelector._useFiducialDRCut = true;
    trackSelector._useFiducialDZCut = true;


    TrackSetContainer newTracks;
    for (auto hitNumber : hits) {
        TrackHitContainer trackHitCandidate = track.getHits();
        trackHitCandidate.push_back(hitNumber);

        Track newTrack(buildTrack(recoHitSet,trackHitCandidate,_detectorGeometry,
                                  _debugLevel));

        if (fcf::goodCandidateTrack(newTrack,_detectorGeometry,
                                    trackSelector)) newTracks.push_back(std::move(newTrack));
    }
    return newTracks;

}



