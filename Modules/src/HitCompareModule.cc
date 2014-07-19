#include<iostream>
#include<cmath>
#include<string>
#include "Geometry/include/DetectorGeometry.hh"
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

  // Intialize commonly used DetectorGeometry data
  _nLayers = _detectorGeometry.getNSensors();

  initializeHistograms();



}

void fc::HitCompareModule::initializeHistograms(){

  UniqueRootDirectory tdir("HitCompare");

  // Generalize to number of layers
  //deltaHitPositions = new TH1F("deltaHitPositions", "Delta X Hit Positions","delta X (m)", "number of hits",100, -0.1, 0.1);

  deltaHitPositions[0] = new TH1F("deltaHitPositions L0", "Delta X Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[1] = new TH1F("deltaHitPositions L1", "Delta X Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[2] = new TH1F("deltaHitPositions L2", "Delta X Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[3] = new TH1F("deltaHitPositions L3", "Delta X Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[4] = new TH1F("deltaHitPositions L4", "Delta X Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[5] = new TH1F("deltaHitPositions L5", "Delta Z Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[6] = new TH1F("deltaHitPositions L6", "Delta Z Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[7] = new TH1F("deltaHitPositions L7", "Delta Z Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[8] = new TH1F("deltaHitPositions L8", "Delta Z Hit Positions",100, -0.0001, 0.0001);
  deltaHitPositions[9] = new TH1F("deltaHitPositions L9", "Delta Z Hit Positions",100, -0.0001, 0.0001);


  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {
    deltaHitPositions[ii_layer]->GetXaxis()->SetTitle("delta (m)");
    deltaHitPositions[ii_layer]->GetYaxis()->SetTitle("N Hits");
  }


}


void fc::HitCompareModule::processEvent(Event& event)
{
 Handle<HitSet> genHitSet = event.get<HitSet>(_genHitsLabel);
 Handle<HitSet> recoHitSet = event.get<HitSet>(_recHitsLabel);

 compareHits(*genHitSet,*recoHitSet);

  // Function to histogram results

}

void fc::HitCompareModule::compareHits(const HitSet & genHitSet, const HitSet& recoHitSet)
{


 
  double deltaPosition;
  double tempDeltaPosition;

  for (hitSet::const_iterator genHitIter =  genHitSet.getHits().begin(); genHitIter !=  genHitSet.getHits().end(); ++genHitIter){

    deltaPosition = 999.0;

    for (hitSet::const_iterator recoHitIter = recoHitSet.getHits().begin(); recoHitIter != recoHitSet.getHits().end(); ++recoHitIter){


      if (genHitIter->getLayer()==recoHitIter->getLayer()) {
	  tempDeltaPosition = compareHitPositions(*genHitIter,*recoHitIter);
	  if (std::abs(tempDeltaPosition) < std::abs(deltaPosition)) deltaPosition = tempDeltaPosition;
    }



    }

    deltaHitPositions[genHitIter->getLayer()]->Fill(deltaPosition);


  }
}


double fc::HitCompareModule::compareHitPositions(const Hit & genHit, const Hit& recoHit)
{

  return recoHit.getHitPosition()*_detectorGeometry.getSensor(recoHit.getLayer())._measurementDirection
    - genHit.getHitPosition()*_detectorGeometry.getSensor(genHit.getLayer())._measurementDirection;

}


void fc::HitCompareModule::endJob(){
}
