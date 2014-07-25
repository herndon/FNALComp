#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Algorithms/include/HelixIntersections.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Modules/include/HitStripGenModule.hh"

fc::HitStripGenModule::HitStripGenModule(int debugLevel, 
					 const std::string& iInputTracksLabel,
					 const std::string& iOutputHitsLabel,
					 const std::string& iOutputStripsLabel,
					 const DetectorGeometry & detectorGeometry, Random & random):
  _inTracksLabel(iInputTracksLabel),
  _outHitsLabel(iOutputHitsLabel),
  _outStripsLabel(iOutputStripsLabel),
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _random(random) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _detectorGeometry.getNSensors();

}

void fc::HitStripGenModule::processEvent(fc::Event & event)
{

  Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_inTracksLabel);
  
  std::unique_ptr<HitSet> genHitSet{ new HitSet };
  std::unique_ptr<StripSet> genStripSet{ new StripSet };

  int trackNumber = 0;
  int hitNumber = 0;

  for (genTrackSet::const_iterator genTrackIter =  genTrackSet->getGenTracks().begin(); genTrackIter != genTrackSet->getGenTracks().end(); ++genTrackIter,++trackNumber){
 
    makeHitsStrips(*genHitSet, *genStripSet,*genTrackIter,trackNumber,hitNumber);

   } // end track loop

  event.put(_outHitsLabel, std::move(genHitSet));
  event.put(_outStripsLabel,std::move(genStripSet));
}


void fc::HitStripGenModule::makeHitsStrips(HitSet& hitSet, StripSet & stripSet, const GenTrack & genTrack,int trackNumber, int & hitNumber){


  TVector3 hitPosition;


  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {

    calculateTrackSensorIntersection(genTrack,ii_layer, hitPosition);

    storeHitInfo(hitSet,trackNumber,hitNumber,hitPosition,ii_layer);

    storeStripInfo(stripSet,hitPosition,ii_layer);
   

  } // end layer loop


}

void fc::HitStripGenModule::calculateTrackSensorIntersection(const GenTrack & genTrack,int layer, TVector3 & hitPosition){

  //TrackFit trackFit(track.getHelix(),_detectorGeometry);
  intersectWithLayer(genTrack.makeHelix(1/_detectorGeometry.getCurvatureC()),hitPosition,layer,_detectorGeometry);

}

// !!!!! remove Gen track from here
void fc::HitStripGenModule::storeHitInfo(HitSet & hitSet,int trackNumber,int & hitNumber,TVector3 & hitPosition,int layer){

  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Hit y " << hitPosition[0] << std::endl;
    std::cout << "Layer " << layer << " Hit x " << hitPosition[1] << std::endl;
  }

  // Pure gen hit, numberStrip = -1
  Hit hit(hitPosition,layer,-1,trackNumber);

  hitSet.insertHit(hit);


  ++hitNumber;

  

  hitPosition = hitPosition + _random.getNormalDouble(0.0,_detectorGeometry.getSensor(layer)._hitResolution)*_detectorGeometry.getSensor(layer)._measurementDirection;
 
  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Resolution smeared Hit x " << hitPosition[0] << std::endl;
  }

}


void fc::HitStripGenModule::storeStripInfo(StripSet & stripSet,const TVector3 & hitPosition,int layer){

  double stripHitPosition = calculateStripHitPosition(hitPosition,layer);

  int initialStrip;
  std::vector <int>stripAdcVector;
  generateCluster(stripHitPosition,initialStrip,stripAdcVector);

  storeCluster(stripSet,layer,initialStrip,stripAdcVector);

}




double fc::HitStripGenModule::calculateStripHitPosition(const TVector3 & hitPosition, int layer) const{

  return (hitPosition-_detectorGeometry.getSensor(layer)._center)*_detectorGeometry.getSensor(layer)._measurementDirection/
_detectorGeometry.getSensor(layer)._stripPitch + _detectorGeometry.getSensor(layer)._nStrips/2;
  
}

void fc::HitStripGenModule::generateCluster(double stripHitPosition, int & initialStrip, std::vector<int> & stripAdcVector){

  int strip = stripHitPosition;
  double remainder = stripHitPosition - strip;

  if (_debugLevel >=5) std::cout << "stripHitPosition " << stripHitPosition <<std::endl;
  if (_debugLevel >=5) std::cout << "strip " << strip << std::endl;
  if (_debugLevel >=5) std::cout << "remainder " << remainder << std::endl;
  
  

  if (remainder < 0.0) {
    initialStrip = strip - 1;
    stripAdcVector.push_back(-1.0*remainder*_detectorGeometry.getMIP());
    stripAdcVector.push_back((1.0+remainder)*_detectorGeometry.getMIP());
  if (_debugLevel >=5) std::cout << "initialStrip " << initialStrip << std::endl;
  if (_debugLevel >=5) std::cout << -1.0*remainder*_detectorGeometry.getMIP() << " " << (1.0+remainder)*_detectorGeometry.getMIP() << std::endl;
  } else {
    initialStrip = strip;
    stripAdcVector.push_back((1.0-remainder)*_detectorGeometry.getMIP());
    stripAdcVector.push_back(remainder*_detectorGeometry.getMIP());
  if (_debugLevel >=5) std::cout << "initialStrip " << initialStrip << std::endl;
  if (_debugLevel >=5) std::cout << (1.0-remainder)*_detectorGeometry.getMIP() << " " << remainder*_detectorGeometry.getMIP() << std::endl;
  }

}


void fc::HitStripGenModule::storeCluster(StripSet & stripSet, int layer, int initialStrip, const std::vector<int> & stripAdcVector){

  int ii_strip = initialStrip;
  for (std::vector<int>::const_iterator stripAdcIter = stripAdcVector.begin(); stripAdcIter != stripAdcVector.end(); ++stripAdcIter){
    if (ii_strip >=0 && ii_strip < _detectorGeometry.getSensor(layer)._nStrips) stripSet.insertStrip(layer,ii_strip,*stripAdcIter);
    ++ii_strip;
  }

}
