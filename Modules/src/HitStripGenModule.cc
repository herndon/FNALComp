#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include "Track.hh"
#include "TrackSet.hh"
#include "HelixIntersections.hh"
#include "HitSet.hh"
#include "StripSet.hh"
#include "DetectorGeometry.hh"
#include "HitStripGenModule.hh"

fc::HitStripGenModule::HitStripGenModule(int debugLevel, 
					 const std::string& iInputTracksLabel,
					 const std::string& iOutputTracksLabel,
					 const std::string& iOutputHitsLabel,
					 const std::string& iOutputStripsLabel,
					 const DetectorGeometry & detectorGeometry, Random & random):
  _inTracksLabel(iInputTracksLabel),
  _outTracksLabel(iOutputTracksLabel),
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
  auto inputTrackSet = event.get<fc::TrackSet>(_inTracksLabel);
  auto genData = event.get<bool>("genData");
  std::unique_ptr<TrackSet> genTrackSet{ new TrackSet(*inputTrackSet) };
  
  std::unique_ptr<HitSet> genHitSet{ new HitSet(*genData) };
  std::unique_ptr<StripSet> genStripSet{ new StripSet(*genData) };

  int trackNumber = 0;
  int hitNumber = 0;

  for (trackSet::iterator trackIter =  genTrackSet->getTracks().begin(); trackIter != genTrackSet->getTracks().end(); ++trackIter,++trackNumber){
 
    makeHitsStrips(*genHitSet, *genStripSet,*trackIter,trackNumber,hitNumber);

   } // end track loop

  event.put(_outTracksLabel, std::move(genTrackSet) );
  event.put(_outHitsLabel, std::move(genHitSet));
  event.put(_outStripsLabel,std::move(genStripSet));
}


void fc::HitStripGenModule::makeHitsStrips(HitSet& hitSet, StripSet & stripSet, Track & track,int trackNumber, int & hitNumber){

  // make a copy of the track because we don't want to change the state of the helix in the original version
  Track trackCopy(track); 

  TVector3 hitPosition;


  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {

    calculateTrackSensorIntersection(trackCopy,ii_layer, hitPosition);

    storeHitInfo(hitSet,track,trackNumber,hitNumber,hitPosition,ii_layer);

    storeStripInfo(stripSet,hitPosition,ii_layer);
   

  } // end layer loop

}

void fc::HitStripGenModule::calculateTrackSensorIntersection(const Track & track,int layer, TVector3 & hitPosition){

  //TrackFit trackFit(track.getHelix(),_detectorGeometry);
  fcf::intersectWithLayer(track.getHelix(),hitPosition,layer,_detectorGeometry);

}

void fc::HitStripGenModule::storeHitInfo(HitSet & hitSet,Track & track,int trackNumber,int & hitNumber,TVector3 & hitPosition,int layer){

  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Hit y " << hitPosition[0] << std::endl;
    std::cout << "Layer " << layer << " Hit x " << hitPosition[1] << std::endl;
  }

  // Pure gen hit, numberStrip = -1
  Hit hit(hitPosition,layer,-1,trackNumber);

  hitSet.insertHit(hit);

  track.insertHit(hitNumber,layer);

  ++hitNumber;

  hitPosition[0] = hitPosition[0] + _random.getNormalDouble(0.0,_detectorGeometry.getSensor(layer)._hitResolution);
 
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
