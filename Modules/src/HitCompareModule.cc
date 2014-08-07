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
    _recHitsLabel(inputRecHitsLabel),
    _hHitResults(nullptr) {

    initializeHistograms();


}

void fc::HitCompareModule::initializeHistograms() {

    UniqueRootDirectory tdir("HitCompare");


    for (int iiLayer = 0; iiLayer < _detectorGeometry.getNSensors(); ++iiLayer) {
        std::string histName = "deltaHitPositionsLayer" + std::to_string(iiLayer);
        const char * histNameC = histName.c_str();
        _hDeltaHitPositions.push_back(new TH1D(histNameC,
                                             "Delta X Hit Positions;delta X (m);number of hits",200, -0.0004, 0.0004));
    }
    for (int iiLayer = 0; iiLayer < _detectorGeometry.getNSensors(); ++iiLayer) {
        std::string histName = "deltaBadHitPositionsLayer" + std::to_string(iiLayer);
        const char * histNameC = histName.c_str();
        _hDeltaBadHitPositions.push_back(new TH1D(histNameC,
                                             "Delta X Bad Hit Positions;delta X (m);number of hits",200, -0.001, 0.001));
    }
    _hHitResults    = new TH1D("HitResults", "Hit results, gen, found, good, bad;N",4,-0.5,3.5);



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

    for (auto const& genHit :   genHitSet.getGenHits()) {

       deltaPosition = 999.0;


       int recoHitNumber = 0;
       int bestHit=-1;
       for (auto const& recoHit : recoHitSet.getHits()) {


            if (genHit.getLayer()==recoHit.getLayer()) {
                tempDeltaPosition = compareHitPositions(genHit,recoHit);
                if (std::abs(tempDeltaPosition) < std::abs(deltaPosition)) {
		  deltaPosition = tempDeltaPosition;;
		  bestHit = recoHitNumber;
		}
            }

	    ++recoHitNumber;

        }
       // HitResults fills 0 if there is a gen Hit
       //                  1 if any hit is found that is withing 10x the standard resolution, 5x gives an inefficiency for badHits
       //                  2 if a isGoodHit
       //                  3 if a bad hit
      _hHitResults->Fill(0);
	if (recoHitSet.getHits()[bestHit].isGoodHit()) {
	  if (std::abs(deltaPosition)<(10.0*_detectorGeometry.getSensor(genHit.getLayer())._hitResolution)){
	    _hHitResults->Fill(1);
	    _hHitResults->Fill(2);
	  }
	  _hDeltaHitPositions[genHit.getLayer()]->Fill(deltaPosition);

	} else {
	  if (std::abs(deltaPosition)<(10.0*_detectorGeometry.getSensor(genHit.getLayer())._hitResolution)){
	    _hHitResults->Fill(1);
	    _hHitResults->Fill(3);
	  }
	  _hDeltaBadHitPositions[genHit.getLayer()]->Fill(deltaPosition);

	}

    }
}


double fc::HitCompareModule::compareHitPositions(const GenHit & genHit,
        const Hit& recoHit) const {

    return recoHit.getHitPosition()*_detectorGeometry.getSensor(
               recoHit.getLayer())._measurementDirection
           - genHit.getGenHitPosition()*_detectorGeometry.getSensor(
               genHit.getLayer())._measurementDirection;

}
