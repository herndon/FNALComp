#include<iostream>
#include "DetectorGeometry.hh"
#include "HitSet.hh"
#include "StripSet.hh"
#include "HitRecoModule.hh"

fc::HitRecoModule::HitRecoModule(int debugLevel, const DetectorGeometry & myDetectorGeometry):
  _debugLevel(debugLevel),
  _myDetectorGeometry(myDetectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _myDetectorGeometry.getNSensors();

}

void fc::HitRecoModule::processEvent(HitSet & myHitSet, const StripSet& myStripSet)
{

  recoHits(myHitSet,myStripSet);

  if (_debugLevel >= 2) myHitSet.print();

}

void fc::HitRecoModule::recoHits(HitSet & myHitSet, const StripSet& myStripSet)
{

  int hitNumber = 0;

  for (int ii_layer =  0; ii_layer < _nLayers; ++ii_layer){
 
    if (_debugLevel >= 5) std::cout << "HitReco layer: " << ii_layer << std::endl;
    recoHitsLayer(myHitSet, myStripSet,ii_layer,hitNumber);

  } // end layer loop


}


void fc::HitRecoModule::recoHitsLayer(HitSet& myHitSet, const StripSet & myStripSet, int layer, int & hitNumber){


  std::vector<int> stripAdcs;
  int initialStrip;
  double stripHitPosition;
  double hitPosition[3];

  const layerStripMap myLayerStripMap = myStripSet.getLayerStripMap(layer);
  layerStripMap::const_iterator  layerStripMapIter = myLayerStripMap.begin();
  layerStripMap::const_iterator  layerStripMapIterEnd = myLayerStripMap.end();

  while (layerStripMapIter != layerStripMapIterEnd) {

    findCluster(initialStrip,layer,stripAdcs,layerStripMapIter,layerStripMapIterEnd,myStripSet);

    stripHitPosition = calculateStripHitPosition(initialStrip,stripAdcs);

    calculateHitPosition(layer,stripHitPosition,hitPosition);
  
    Hit hit(hitPosition);

    myHitSet.insertHit(hitNumber,hit,layer);

    // calculate hit postition function using vector of strips?
    // create cluster?
    // save number of strips. save gen track on strips?

    ++hitNumber;
    stripAdcs.clear();

  }

}


void fc::HitRecoModule::findCluster(int & initialStrip,int layer, std::vector<int> & stripAdcVector,
				    layerStripMap::const_iterator & layerStripMapIter,
				    layerStripMap::const_iterator & layerStripMapIterEnd,const StripSet & myStripSet){


  if (_debugLevel >= 5) std::cout << "findCluster " << std::endl;
  initialStrip = myStripSet.getStripNumber(layerStripMapIter);

  if (_debugLevel >= 5) std::cout << "Initial strip: " << initialStrip << std::endl;

  int intermediateStrip = initialStrip;

  stripAdcVector.push_back(myStripSet.getStripAdc(layerStripMapIter));

  ++layerStripMapIter;


  while ( layerStripMapIter != layerStripMapIterEnd && (myStripSet.getStripNumber(layerStripMapIter) == (intermediateStrip + 1))) {

    intermediateStrip = myStripSet.getStripNumber(layerStripMapIter);
    stripAdcVector.push_back(myStripSet.getStripAdc(layerStripMapIter));
    ++layerStripMapIter;
    if (_debugLevel >= 5) std::cout << "Intermediate strip: " << intermediateStrip  << std::endl;
    
  }

}


double fc::HitRecoModule::calculateStripHitPosition(int initialStrip,const std::vector<int> & stripAdcVector){

  double stripHitPosition = 0.0;
  double stripPosition = 0.0;
  double adcSum = 0.0;

  for (std::vector<int>::const_iterator stripAdcIter = stripAdcVector.begin(); stripAdcIter != stripAdcVector.end(); ++stripAdcIter){

    stripHitPosition = stripHitPosition + stripPosition*(*stripAdcIter);
    adcSum = adcSum + (*stripAdcIter);
    stripPosition =  stripPosition + 1.0;
    
  } // end strip loop

  stripHitPosition = stripHitPosition/adcSum;

  stripHitPosition = initialStrip + stripHitPosition;

  return stripHitPosition;

}

void  fc::HitRecoModule::calculateHitPosition(int layer, double stripHitPosition, double * hitPosition){

  hitPosition[0] =  (stripHitPosition - (_myDetectorGeometry.getSensor(layer)._nStrips/2.0))*_myDetectorGeometry.getSensor(layer)._stripPitch 
    + _myDetectorGeometry.getSensor(layer)._center[0];
  hitPosition[1] = _myDetectorGeometry.getSensor(layer)._center[1];
  hitPosition[2] = _myDetectorGeometry.getSensor(layer)._center[2];

}
