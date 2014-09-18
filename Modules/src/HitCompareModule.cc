#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/HitSet.hh"
#include "Modules/include/HitCompareModule.hh"
#include "TH1F.h"
#include "Services/include/UniqueRootDirectory.hh"

fc::HitCompareModule::HitCompareModule(int debugLevel,
                                       const std::string& inputGenHitsLabel,
                                       const std::string& inputRecHitsLabel,
                                       const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _genHitsLabel(inputGenHitsLabel),
    _recHitsLabel(inputRecHitsLabel){

    initializeHistograms();


}

void fc::HitCompareModule::initializeHistograms() {

    UniqueRootDirectory tdir("HitCompare");


    for (int iiLayer = 0; iiLayer < _detectorGeometry.nSensors(); ++iiLayer) {
        std::string histName = "deltaHitPositionsLayer" + std::to_string(iiLayer);
        const char * histNameC = histName.c_str();
        _hDeltaHitPositions.push_back(new TH1D(histNameC,
                                             "Delta X Hit Positions;delta X (m);number of hits",200, -0.0004, 0.0004));
    }
 


}


void fc::HitCompareModule::processEvent(Event& event) {
    Handle<GenHitSet> genHitSet = event.get<GenHitSet>(_genHitsLabel);
    Handle<HitSet> recoHitSet = event.get<HitSet>(_recHitsLabel);

    compareHits(*genHitSet,*recoHitSet);

    // Function to histogram results

}

void fc::HitCompareModule::compareHits(const GenHitSet & genHitSet,
                                       const HitSet& recoHitSet) const {



    double deltaPosition;
    double tempDeltaPosition;

    for (auto const& genHit :   genHitSet.genHits()) {

       deltaPosition = 999.0;


       int recoHitNumber = 0;
       int bestHit=-1;
       for (auto const& recoHit : recoHitSet.hits()) {


            if (genHit.layer()==recoHit.layer()) {
                tempDeltaPosition = compareHitPositions(genHit,recoHit);
                if (std::abs(tempDeltaPosition) < std::abs(deltaPosition)) {
		  deltaPosition = tempDeltaPosition;;
		  bestHit = recoHitNumber;
		}
            }

	    ++recoHitNumber;

        }
      _hHitResults->Fill(0);
	if (recoHitSet.hits()[bestHit].goodHit()||(!recoHitSet.hits()[bestHit].goodHit())) {
	  _hDeltaHitPositions[genHit.layer()]->Fill(deltaPosition);
	} 

    }
}


double fc::HitCompareModule::compareHitPositions(const GenHit & genHit,
        const Hit& recoHit) const {

    return recoHit.position()*_detectorGeometry.sensor(recoHit.layer()).measurementDirection()
           - genHit.position()*_detectorGeometry.sensor(genHit.layer()).measurementDirection();

}
