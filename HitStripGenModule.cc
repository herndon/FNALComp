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


void fc::HitStripGenModule::makeHitsStrips(HitSet& myHitSet, StripSet & _myStripSet, Track & track,int trackNumber, int & hitNumber){

  // make a copy of the track because we don't want to change the state of the helix in the original version
  Track trackCopy(track); 

  double hitPosition[3];

  // Strip postions and ADC counts
  int xStripNumber[2];
  int xStripADC[2];

  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {

    calculateTrackSensorIntersection(trackCopy,ii_layer, hitPosition);

    if (_debugLevel >=5 ) {
      std::cout << "Layer " << ii_layer << " Hit y " << hitPosition[0] << std::endl;
      std::cout << "Layer " << ii_layer << " Hit x " << hitPosition[1] << std::endl;
    }

    Hit hit(hitPosition,trackNumber);

    myHitSet.insertHit(hitNumber,hit,ii_layer);

    track.insertHit(hitNumber,ii_layer);

    hitNumber++;

    hitPosition[0] = hitPosition[0] + _myRandom.getNormalDouble(0.0,_myDetectorGeometry.getSensor(ii_layer)._resolution);
 
   if (_debugLevel >=5 ) {
      std::cout << "Layer " << ii_layer << " Resolution smeared Hit x " << hitPosition[0] << std::endl;
    }


    // Generate strip data by spliting charge across nearest strips
    double stripPitch = _myDetectorGeometry.getSensor(ii_layer)._stripPitch;

    int strip = (int) (hitPosition[0]/stripPitch);
    double remainder = (hitPosition[0]/stripPitch) - strip;

    if (remainder < 0.0) {
      xStripNumber[0] = strip - 1 + 1024;
      xStripNumber[1] = strip + 1024;
      xStripADC[0] = -1.0*remainder*32.0;
      xStripADC[1] = (1.0+remainder)*32.0;
    } else {
      xStripNumber[0] = strip + 1024;
      xStripNumber[1] = strip+1 + 1024;
      xStripADC[0] = 1.0*remainder*32.0;
      xStripADC[1] = (1.0-remainder)*32.0;
 
    }

    // Store data in strip set.
    if (xStripNumber[0] > 0 && xStripNumber[0] < 2048) _myStripSet.insertStrip(ii_layer,xStripNumber[0],xStripADC[0]);
    if (xStripNumber[1] > 0 && xStripNumber[1] < 2048) _myStripSet.insertStrip(ii_layer,xStripNumber[1],xStripADC[1]);


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

