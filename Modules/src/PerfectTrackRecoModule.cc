#include<iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Algorithms/include/BuildTrack.hh"
#include "Modules/include/PerfectTrackRecoModule.hh"

fc::PerfectTrackRecoModule::PerfectTrackRecoModule(int debugLevel,
        const std::string& inputHitsLabel, const std::string& inputGenHitsLabel,
        const std::string& outputTracksLabel,
        const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _inHitsLabel(inputHitsLabel),
    _inGenHitsLabel(inputGenHitsLabel),
    _outTracksLabel(outputTracksLabel),
    _detectorGeometry(detectorGeometry) {
}

void fc::PerfectTrackRecoModule::processEvent(Event& event)
{

    Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);
    Handle<GenHitSet> genHitSet = event.get<GenHitSet>(_inGenHitsLabel);

    std::unique_ptr<TrackSet> perfectRecoTrackSet { new TrackSet };

    recoTracks(*recoHitSet,*genHitSet,*perfectRecoTrackSet);

    if (_debugLevel>=2) {
        std::cout << "Perfect reconstructed tracks" << std::endl;
        perfectRecoTrackSet->print(std::cout);
    }

    event.put(_outTracksLabel,std::move(perfectRecoTrackSet) );
}

void fc::PerfectTrackRecoModule::recoTracks(const HitSet& recoHitSet,
        const GenHitSet& genHitSet,
        TrackSet& perfectRecoTrackSet) const {
    // !!!!! This will eventually pass a track hit candidates strategy
    // !!!!! Do I want to make a special object for the track hit candidates?
    std::vector<std::vector<int>> trackHitCandidates;


    findTrackPerfectCandidates(recoHitSet,genHitSet,trackHitCandidates);
    buildPerfectTrackCandidates(trackHitCandidates,recoHitSet,perfectRecoTrackSet);


}



void fc::PerfectTrackRecoModule::findTrackPerfectCandidates(
    const HitSet & recoHitSet,const GenHitSet & genHitSet,
    std::vector<std::vector<int>> & trackHitCandidates) const {

    int trackNumber = (genHitSet.getGenHits().empty() ? 0 :
                       genHitSet.getGenHits().front().getTrackNumber());

    std::vector<int> trackHitCandidate;

    // Form all hit candidates

    for (auto const& genHit : genHitSet.getGenHits()) {

        double deltaPosition = 999.0;
        int recoHitNumber = 0;
        int bestRecoHitNumber = -1;

        for (auto const& recoHit : recoHitSet.getHits()) {

            if (genHit.getLayer()==recoHit.getLayer()) {
                double tempDeltaPosition = compareHitPositions(genHit,recoHit);
                if (std::abs(tempDeltaPosition) < std::abs(deltaPosition)) {
                    deltaPosition = tempDeltaPosition;
                    bestRecoHitNumber = recoHitNumber;
                }
            }
            ++recoHitNumber;
        } // end reco Hit loop
        if (genHit.getTrackNumber() == trackNumber && bestRecoHitNumber>-1 && 
	    std::abs(deltaPosition)<(10.0*_detectorGeometry.getSensor(genHit.getLayer())._hitResolution)) {

            trackHitCandidate.push_back(bestRecoHitNumber);

        } else if (genHit.getTrackNumber() != trackNumber) {
            trackHitCandidates.push_back(trackHitCandidate);
            trackHitCandidate.clear();
            trackHitCandidate.push_back(bestRecoHitNumber);
            trackNumber = genHit.getTrackNumber();
        }
    } // end gen Hit loop
    trackHitCandidates.push_back(std::move(trackHitCandidate));


}


// !!!!! move to utility function
double fc::PerfectTrackRecoModule::compareHitPositions(const GenHit & genHit,
        const Hit& recoHit) const {

    return recoHit.getHitPosition()*_detectorGeometry.getSensor(
               recoHit.getLayer())._measurementDirection
           - genHit.getGenHitPosition()*_detectorGeometry.getSensor(
               genHit.getLayer())._measurementDirection;

}


void fc::PerfectTrackRecoModule::buildPerfectTrackCandidates(
    const std::vector<std::vector<int>> & trackHitCandidates,
    const HitSet & hitSet,TrackSet & trackCandidateSet) const {

    for (auto const& trackHitCandidate : trackHitCandidates) {
        Track trackCandidate(buildTrack(hitSet,trackHitCandidate,_detectorGeometry,
                                        _debugLevel));

        if (_debugLevel ==2) {
            std::cout << "Track after fit" << std::endl;
            trackCandidate.print(std::cout);

        }

        trackCandidateSet.insertTrack(std::move(trackCandidate));

    }

}
