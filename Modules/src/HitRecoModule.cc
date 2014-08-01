#include<iostream>
#include "Geometry/include/StripHitFunctions.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Hit.hh"
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
}

void fc::HitRecoModule::processEvent(fc::Event& event)
{
  Handle<StripSet> genStripSet = event.get<StripSet>(_inStripsLabel);
  std::unique_ptr<HitSet> recoHitSet( new HitSet );

  recoHits(*recoHitSet, *genStripSet);

  if (_debugLevel >= 2) recoHitSet->print(std::cout);

  event.put(_outHitsLabel, std::move(recoHitSet));
}

void fc::HitRecoModule::recoHits(HitSet & hitSet, const StripSet& stripSet) const{

  for (int ii_layer =  0; ii_layer < _detectorGeometry.getNSensors(); ++ii_layer){
 
    if (_debugLevel >= 5) std::cout << "HitReco layer: " << ii_layer << std::endl;
    recoHitsLayer(hitSet, stripSet,ii_layer);

  } // end layer loop


}


void fc::HitRecoModule::recoHitsLayer(HitSet& hitSet, const StripSet & stripSet, int layer) const{


  std::vector<int> stripAdcs;
  int initialStrip;

  const layerStripMap layerStripMap = stripSet.getLayerStripMap(layer);
  layerStripMap::const_iterator  layerStripMapIter = layerStripMap.begin();
  layerStripMap::const_iterator  layerStripMapIterEnd = layerStripMap.end();

  while (layerStripMapIter != layerStripMapIterEnd) {

    findCluster(initialStrip,layer,stripAdcs,layerStripMapIter,layerStripMapIterEnd,stripSet);

    hitSet.insertHit(buildHit(layer, initialStrip,stripAdcs));

  }

}


void fc::HitRecoModule::findCluster(int & initialStrip,int layer, std::vector<int> & stripAdcs,
				    layerStripMap::const_iterator & layerStripMapIter,
				    layerStripMap::const_iterator & layerStripMapIterEnd,const StripSet & stripSet) const{


  stripAdcs.clear();

  if (_debugLevel >= 5) std::cout << "findCluster " << std::endl;

  while (layerStripMapIter != layerStripMapIterEnd && stripSet.getStripAdc(layerStripMapIter) < _detectorGeometry.getSensor(layer)._threshold){
    ++layerStripMapIter;
  }

  if (layerStripMapIter == layerStripMapIterEnd) return;

  initialStrip = stripSet.getStripNumber(layerStripMapIter);

  if (_debugLevel >= 5) std::cout << "Initial strip: " << initialStrip << std::endl;

  int intermediateStrip = initialStrip;

  stripAdcs.push_back(stripSet.getStripAdc(layerStripMapIter));

  ++layerStripMapIter;


  while ( layerStripMapIter != layerStripMapIterEnd && 
	  (stripSet.getStripNumber(layerStripMapIter) == (intermediateStrip + 1)) && 
	  (stripSet.getStripAdc(layerStripMapIter) >= _detectorGeometry.getSensor(layer)._threshold  )) {

    intermediateStrip = stripSet.getStripNumber(layerStripMapIter);
    stripAdcs.push_back(stripSet.getStripAdc(layerStripMapIter));
    ++layerStripMapIter;
    if (_debugLevel >= 5) std::cout << "Intermediate strip: " << intermediateStrip  << std::endl;
    
  }

}

fc::Hit fc::HitRecoModule::buildHit(int layer, int initialStrip,const std::vector<int> & stripAdcs) const{

  double stripHitPosition = fcf::calculateStripHitPositionFromCluster(initialStrip,stripAdcs);
  double localHitPosition = fcf::calculateLoalFromStripPosition(stripHitPosition,layer,_detectorGeometry);
  TVector3 hitPosition = fcf::calculateGlobalFromLocalPosition(localHitPosition,layer,_detectorGeometry);

  int numberStrips = stripAdcs.size();
  int charge = 0;
  for (auto const& adc :   stripAdcs){
    charge += adc;
  }
    
  bool goodHit = true;
  if (numberStrips>2 || charge > _detectorGeometry.getMIP()) goodHit = false;

  double resolution;
  if (goodHit) {
    resolution = _detectorGeometry.getSensor(layer)._hitResolution;
  }else {
    resolution = _detectorGeometry.getSensor(layer)._badHitResolution;
  }
 

  return Hit(hitPosition,layer,numberStrips,charge,goodHit,resolution);

}



