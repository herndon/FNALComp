#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include "DetectorGeometry.hh"
#include "TrackSet.hh"
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

void fc::HitStripGenModule::processEvent(TrackSet & myTrackSet, HitSet & myHitSet, StripSet& myStripSet)
{


  TrackMap & myTrackMap = myTrackSet.getTrackMap();
  std::map<int,Track>::size_type numberTracks =myTrackMap.size();
  int hitNumber = 0;

    for (TrackMap::iterator trackMapIter =  myTrackMap.begin(); trackMapIter != myTrackMap.end(); ++trackMapIter){
 
    makeHitsStrips(myHitSet, myStripSet,myTrackSet.getTrack(trackMapIter),myTrackSet.getTrackNumber(trackMapIter),hitNumber);

   } // end track loop

}


void fc::HitStripGenModule::makeHitsStrips(HitSet& myHitSet, StripSet & myStripSet, Track & track,int trackNumber, int & hitNumber){

  // make a copy of the track because we don't want to change the state of the helix in the original version
  Track trackCopy(track); 

  double hitPosition[3];


  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {

    calculateTrackSensorIntersection(trackCopy,ii_layer, hitPosition);

    storeHitInfo(myHitSet,track,trackNumber,hitNumber,hitPosition,ii_layer);

    storeStripInfo(myStripSet,hitPosition,ii_layer);
   

  } // end layer loop

}

void fc::HitStripGenModule::calculateTrackSensorIntersection(Track & track,int layer, double * hitPosition){

  // point and normal to sensor
  Double_t point[3];
  Double_t norm[3];

  point[0] = _myDetectorGeometry.getSensor(layer)._center[0];
  point[1] = _myDetectorGeometry.getSensor(layer)._center[1];
  point[2] = _myDetectorGeometry.getSensor(layer)._center[2];

  // normal vector to sensor
  norm[0] = _myDetectorGeometry.getSensor(layer)._norm[0];
  norm[1] = _myDetectorGeometry.getSensor(layer)._norm[1];
  norm[2] = _myDetectorGeometry.getSensor(layer)._norm[2];
  
  // !!!!! is this the best way to return the result?
  track.getGeoHelix().StepToPlane(point,norm);
  hitPosition[0] = track.getGeoHelix().GetCurrentPoint()[0];
  hitPosition[1] = track.getGeoHelix().GetCurrentPoint()[1];
  hitPosition[2] = track.getGeoHelix().GetCurrentPoint()[2];

}

void fc::HitStripGenModule::storeHitInfo(HitSet & myHitSet,Track & track,int trackNumber,int & hitNumber,double * hitPosition,int layer){

  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Hit y " << hitPosition[0] << std::endl;
    std::cout << "Layer " << layer << " Hit x " << hitPosition[1] << std::endl;
  }

  Hit hit(hitPosition,trackNumber);

  myHitSet.insertHit(hitNumber,hit,layer);

  track.insertHit(hitNumber,layer);

  ++hitNumber;

  hitPosition[0] = hitPosition[0] + _myRandom.getNormalDouble(0.0,_myDetectorGeometry.getSensor(layer)._resolution);
 
  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Resolution smeared Hit x " << hitPosition[0] << std::endl;
  }

}

 


void fc::HitStripGenModule::storeStripInfo(StripSet & myStripSet,const double * hitPosition,int layer){

  double stripHitPosition = calculateStripHitPosition(hitPosition,layer);

  int initialStrip;
  std::vector <int>stripAdcVector;
  generateCluster(stripHitPosition,initialStrip,stripAdcVector);

  storeCluster(myStripSet,layer,initialStrip,stripAdcVector);

}




double fc::HitStripGenModule::calculateStripHitPosition(const double * hitPosition, int layer) const{

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
