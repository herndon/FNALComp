#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include "DetectorGeometry.hh"
#include "Track.hh"
#include "TrackSet.hh"
#include "TrackFit.hh"
#include "HitSet.hh"
#include "StripSet.hh"
#include "HitStripGenModule.hh"

fc::HitStripGenModule::HitStripGenModule(int debugLevel, const DetectorGeometry & myDetectorGeometry, Random & myRandom):
  _debugLevel(debugLevel),
  _myDetectorGeometry(myDetectorGeometry),
  _myRandom(myRandom) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _myDetectorGeometry.getNSensors();

}

void fc::HitStripGenModule::processEvent(fc::Event & event)
{
  auto trackSet = event.get<fc::TrackSet>("genTracks");
  auto genData = event.get<bool>("genData");
  std::unique_ptr<TrackSet> myTrackSet{ new TrackSet(*trackSet) };
  
  std::unique_ptr<HitSet> myHitSet{ new HitSet(*genData) };
  std::unique_ptr<StripSet> myStripSet{ new StripSet(*genData) };

  std::vector<Track> & myTrackVector = myTrackSet->getTrackVector();
  int trackNumber = 0;
  int hitNumber = 0;

  for (std::vector<Track>::iterator trackIter =  myTrackVector.begin(); trackIter != myTrackVector.end(); ++trackIter,++trackNumber){
 
    makeHitsStrips(*myHitSet, *myStripSet,*trackIter,trackNumber,hitNumber);

   } // end track loop

  event.put("tracksWithHits", std::move(myTrackSet) );
  event.put("hits", std::move(myHitSet));
  event.put("strips",std::move(myStripSet));
}


void fc::HitStripGenModule::makeHitsStrips(HitSet& myHitSet, StripSet & myStripSet, Track & track,int trackNumber, int & hitNumber){

  // make a copy of the track because we don't want to change the state of the helix in the original version
  Track trackCopy(track); 

  TVector3 hitPosition;


  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {

    calculateTrackSensorIntersection(trackCopy,ii_layer, hitPosition);

    storeHitInfo(myHitSet,track,trackNumber,hitNumber,hitPosition,ii_layer);

    storeStripInfo(myStripSet,hitPosition,ii_layer);
   

  } // end layer loop

}

void fc::HitStripGenModule::calculateTrackSensorIntersection(const Track & track,int layer, TVector3 & hitPosition){

  TrackFit trackFit(track.getHelix(),_myDetectorGeometry);
  trackFit.intersectWithLayer(hitPosition,layer,_myDetectorGeometry);

}

void fc::HitStripGenModule::storeHitInfo(HitSet & myHitSet,Track & track,int trackNumber,int & hitNumber,TVector3 & hitPosition,int layer){

  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Hit y " << hitPosition[0] << std::endl;
    std::cout << "Layer " << layer << " Hit x " << hitPosition[1] << std::endl;
  }

  // Pure gen hit, numberStrip = -1
  Hit hit(hitPosition,layer,-1,trackNumber);

  myHitSet.insertHit(hit);

  track.insertHit(hitNumber,layer);

  ++hitNumber;

  hitPosition[0] = hitPosition[0] + _myRandom.getNormalDouble(0.0,_myDetectorGeometry.getSensor(layer)._hitResolution);
 
  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Resolution smeared Hit x " << hitPosition[0] << std::endl;
  }

}

 


void fc::HitStripGenModule::storeStripInfo(StripSet & myStripSet,const TVector3 & hitPosition,int layer){

  double stripHitPosition = calculateStripHitPosition(hitPosition,layer);

  int initialStrip;
  std::vector <int>stripAdcVector;
  generateCluster(stripHitPosition,initialStrip,stripAdcVector);

  storeCluster(myStripSet,layer,initialStrip,stripAdcVector);

}




double fc::HitStripGenModule::calculateStripHitPosition(const TVector3 & hitPosition, int layer) const{

  // This assumes the senstors are normal to the y axis.  Could be made more general
  return hitPosition[0]/_myDetectorGeometry.getSensor(layer)._stripPitch + _myDetectorGeometry.getSensor(layer)._nStrips/2   ;
  
}

void fc::HitStripGenModule::generateCluster(double stripHitPosition, int & initialStrip, std::vector<int> & stripAdcVector){

  int strip = stripHitPosition;
  double remainder = stripHitPosition - strip;

  if (_debugLevel >=5) std::cout << "stripHitPosition " << stripHitPosition <<std::endl;
  if (_debugLevel >=5) std::cout << "strip " << strip << std::endl;
  if (_debugLevel >=5) std::cout << "remainder " << remainder << std::endl;
  
  

  if (remainder < 0.0) {
    initialStrip = strip - 1;
    stripAdcVector.push_back(-1.0*remainder*_myDetectorGeometry.getMIP());
    stripAdcVector.push_back((1.0+remainder)*_myDetectorGeometry.getMIP());
  if (_debugLevel >=5) std::cout << "initialStrip " << initialStrip << std::endl;
  if (_debugLevel >=5) std::cout << -1.0*remainder*_myDetectorGeometry.getMIP() << " " << (1.0+remainder)*_myDetectorGeometry.getMIP() << std::endl;
  } else {
    initialStrip = strip;
    stripAdcVector.push_back((1.0-remainder)*_myDetectorGeometry.getMIP());
    stripAdcVector.push_back(remainder*_myDetectorGeometry.getMIP());
  if (_debugLevel >=5) std::cout << "initialStrip " << initialStrip << std::endl;
  if (_debugLevel >=5) std::cout << (1.0-remainder)*_myDetectorGeometry.getMIP() << " " << remainder*_myDetectorGeometry.getMIP() << std::endl;
  }

}


void fc::HitStripGenModule::storeCluster(StripSet & myStripSet, int layer, int initialStrip, const std::vector<int> & stripAdcVector){

  int ii_strip = initialStrip;
  for (std::vector<int>::const_iterator stripAdcIter = stripAdcVector.begin(); stripAdcIter != stripAdcVector.end(); ++stripAdcIter){
    if (ii_strip >=0 && ii_strip < _myDetectorGeometry.getSensor(layer)._nStrips) myStripSet.insertStrip(layer,ii_strip,*stripAdcIter);
    ++ii_strip;
  }

}
