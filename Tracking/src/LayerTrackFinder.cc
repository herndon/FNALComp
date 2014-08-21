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
                                       const DetectorGeometry& detectorGeometry,int layer,int unsigned nExpHits,
				       double minPTCut,double dRCut, double dZCut,
                                       double maxChi2NDofCut):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _layer(layer),
    _nExpHits(nExpHits),
    _minPTCut(minPTCut),
    _dRCut(dRCut),
    _dZCut(dZCut),
    _maxChi2NDofCut(maxChi2NDofCut) {
}

void fc::LayerTrackFinder::findTracks(const HitSet& recoHitSet,
        unsigned int expNHit,FastTrackSetContainer& trackSet) const {

    FastTrackSetContainer allNewTracks;

    for (auto& track :  trackSet) {

        findTrack(track,recoHitSet,allNewTracks);

    }

    //trackSet.reserve(trackSet.size()+allNewTracks.size());
    for (auto& track: allNewTracks) {
        trackSet.push_back(std::move(track));
    }

    fcf::TrackingSelector trackSelector = {_minPTCut,_dRCut,_dZCut,_nExpHits,1000000.0,true,true};
    fcf::simpleTrackSetFilter(_detectorGeometry,trackSelector,trackSet);
    if (_layer==1||_layer==8)fcf::duplicateTrackSetFilter(trackSet);
 

}

void fc::LayerTrackFinder::findTrack(const Track& track,
        const HitSet& recoHitSet, FastTrackSetContainer& allNewTracks) const {



    std::vector<int> hits = findHits(track,recoHitSet);
    FastTrackSetContainer newTracks = buildTracks(track, hits, recoHitSet);
    std::vector<int> tracks = bestTracks(newTracks);
    //We may want to decide whether to remove the seed track
    //removeSeedTrack(trackSet,trackSet);

    int ii_track=0; 
    for (auto & track: newTracks){

      for (auto trackNumber: tracks) {
	if (trackNumber == ii_track) allNewTracks.push_back(std::move(track));
      }

      ++ii_track;
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

    double maxResidual = 4.0*expectedMeasurementUncertianty1D(track.getHelix(),
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

std::vector<int> fc::LayerTrackFinder::bestTracks(
    const FastTrackSetContainer & tracks) const {

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

    // Tested keeping all or just the best track.  Keeping top 2 works best
    if (bestTrack > -1)  trackList.push_back(bestTrack);
    if (secondBestTrack > -1)  trackList.push_back(secondBestTrack);





    return trackList;

}



fc::FastTrackSetContainer fc::LayerTrackFinder::buildTracks(
    const Track & track, const std::vector<int> & hits,
    const HitSet & recoHitSet) const {

  fcf::TrackingSelector trackSelector = {_minPTCut,_dRCut,_dZCut,_nExpHits,_maxChi2NDofCut,true,true};
    FastTrackSetContainer newTracks;
    for (auto hitNumber : hits) {
      //TrackHitContainer trackHits = track.getHits();
      TrackHitContainer trackHits;
        trackHits.push_back(hitNumber);

        Track newTrack(buildTrack(track,recoHitSet,trackHits,_detectorGeometry,
                                  _debugLevel));

        if (fcf::goodTrack(newTrack,_detectorGeometry,
                                    trackSelector)) newTracks.push_back(std::move(newTrack));
    }
    return newTracks;

}



