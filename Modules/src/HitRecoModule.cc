#include<iostream>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/HitRecoModule.hh"

fc::HitRecoModule::HitRecoModule(int debugLevel,
				 const std::string& iInputStripsLabel, const std::string& iOutputHitsLabel,
				 const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inStripsLabel(iInputStripsLabel),
  _outHitsLabel(iOutputHitsLabel),
  _detectorGeometry(detectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _detectorGeometry.getNSensors();

}

void fc::HitRecoModule::processEvent(fc::Event& event)
{
  Handle<StripSet> genStripSet = event.get<StripSet>(_inStripsLabel);
  std::unique_ptr<HitSet> recoHitSet( new HitSet );

  recoHits(*recoHitSet, *genStripSet);

  if (_debugLevel >= 2) recoHitSet->print();

  event.put(_outHitsLabel, std::move(recoHitSet));
}

void fc::HitRecoModule::recoHits(HitSet & hitSet, const StripSet& stripSet)
{

  int hitNumber = 0;

  for (int ii_layer =  0; ii_layer < _nLayers; ++ii_layer){
 
    if (_debugLevel >= 5) std::cout << "HitReco layer: " << ii_layer << std::endl;
    recoHitsLayer(hitSet, stripSet,ii_layer,hitNumber);

  } // end layer loop


}


void fc::HitRecoModule::recoHitsLayer(HitSet& hitSet, const StripSet & stripSet, int layer, int & hitNumber){


  std::vector<int> stripAdcs;
  int initialStrip;
  std::vector<int>::size_type numberStrips;
  double stripHitPosition;
  TVector3 hitPosition;

  const layerStripMap layerStripMap = stripSet.getLayerStripMap(layer);
  layerStripMap::const_iterator  layerStripMapIter = layerStripMap.begin();
  layerStripMap::const_iterator  layerStripMapIterEnd = layerStripMap.end();

  while (layerStripMapIter != layerStripMapIterEnd) {

    findCluster(initialStrip,layer,stripAdcs,layerStripMapIter,layerStripMapIterEnd,stripSet);

    stripHitPosition = calculateStripHitPosition(initialStrip,stripAdcs);

    hitPosition = calculateHitPosition(layer,stripHitPosition);
  
    numberStrips = stripAdcs.size();

    Hit hit(hitPosition,layer,numberStrips);

    hitSet.insertHit(hit);

    // calculate hit postition function using vector of strips?
    // create cluster?
    // save number of strips. save gen track on strips?

    ++hitNumber;
    stripAdcs.clear();

  }

}


void fc::HitRecoModule::findCluster(int & initialStrip,int layer, std::vector<int> & stripAdcVector,
				    layerStripMap::const_iterator & layerStripMapIter,
				    layerStripMap::const_iterator & layerStripMapIterEnd,const StripSet & stripSet){


  if (_debugLevel >= 5) std::cout << "findCluster " << std::endl;

  while (layerStripMapIter != layerStripMapIterEnd && stripSet.getStripAdc(layerStripMapIter) < _detectorGeometry.getSensor(layer)._threshold){
    layerStripMapIter++;
  }

  if (layerStripMapIter == layerStripMapIterEnd) return;

  initialStrip = stripSet.getStripNumber(layerStripMapIter);

  if (_debugLevel >= 5) std::cout << "Initial strip: " << initialStrip << std::endl;

  int intermediateStrip = initialStrip;

  stripAdcVector.push_back(stripSet.getStripAdc(layerStripMapIter));

  ++layerStripMapIter;


  while ( layerStripMapIter != layerStripMapIterEnd && (stripSet.getStripNumber(layerStripMapIter) == (intermediateStrip + 1)) && (stripSet.getStripAdc(layerStripMapIter) >= _detectorGeometry.getSensor(layer)._threshold  )) {

    intermediateStrip = stripSet.getStripNumber(layerStripMapIter);
    stripAdcVector.push_back(stripSet.getStripAdc(layerStripMapIter));
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

TVector3  fc::HitRecoModule::calculateHitPosition(int layer, double stripHitPosition){

  TVector3 hitPosition;

  double localPosition = (stripHitPosition - (_detectorGeometry.getSensor(layer)._nStrips/2.0)) * _detectorGeometry.getSensor(layer)._stripPitch;



  return hitPosition = _detectorGeometry.getSensor(layer)._measurementDirection*localPosition + _detectorGeometry.getSensor(layer)._center;
 


}
