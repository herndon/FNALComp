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

void fc::HitCompareModule::initializeHistograms(){

  UniqueRootDirectory tdir("HitCompare");


  for (int iiLayer = 0; iiLayer < _detectorGeometry.getNSensors(); ++iiLayer) {
    std::string histName = "deltaHitPositions  Layer " + std::to_string(iiLayer);
    const char * histNameC = histName.c_str();
    deltaHitPositions.push_back(new TH1F(histNameC, "Delta X Hit Positions;delta X (m);number of hits",100, -0.0001, 0.0001));
  }

}


void fc::HitCompareModule::processEvent(Event& event) {
 Handle<GenHitSet> genHitSet = event.get<GenHitSet>(_genHitsLabel);
 Handle<HitSet> recoHitSet = event.get<HitSet>(_recHitsLabel);

 compareHits(*genHitSet,*recoHitSet);

  // Function to histogram results

}

void fc::HitCompareModule::compareHits(const GenHitSet & genHitSet, const HitSet& recoHitSet) const{


 
  double deltaPosition;
  double tempDeltaPosition;

  for (auto const& genHit :   genHitSet.getGenHits()) {

    deltaPosition = 999.0;

    for (auto const& recoHit : recoHitSet.getHits()) {


      if (genHit.getLayer()==recoHit.getLayer()) {
	  tempDeltaPosition = compareHitPositions(genHit,recoHit);
	  if (std::abs(tempDeltaPosition) < std::abs(deltaPosition)) deltaPosition = tempDeltaPosition;
    }



    }

    deltaHitPositions[genHit.getLayer()]->Fill(deltaPosition);


  }
}


double fc::HitCompareModule::compareHitPositions(const GenHit & genHit, const Hit& recoHit) const{

  return recoHit.getHitPosition()*_detectorGeometry.getSensor(recoHit.getLayer())._measurementDirection
    - genHit.getGenHitPosition()*_detectorGeometry.getSensor(genHit.getLayer())._measurementDirection;

}
