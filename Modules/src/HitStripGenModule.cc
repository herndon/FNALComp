#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include "Services/include/Random.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "DataObjects/include/Helix.hh"
#include "Algorithms/include/HelixIntersections.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/StripHitFunctions.hh"
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
}

void fc::HitStripGenModule::processEvent(fc::Event & event)
{

  Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_inTracksLabel);
  
  std::unique_ptr<HitSet> genHitSet{ new HitSet };
  std::unique_ptr<StripSet> genStripSet{ new StripSet(_detectorGeometry)};

  int trackNumber = 0;
  int hitNumber = 0;

  for (genTrackSet::const_iterator genTrackIter =  genTrackSet->getGenTracks().begin(); genTrackIter != genTrackSet->getGenTracks().end(); ++genTrackIter,++trackNumber){
 
    makeHitsStrips(*genHitSet, *genStripSet,*genTrackIter,trackNumber,hitNumber);

   } // end track loop

  event.put(_outHitsLabel, std::move(genHitSet));
  event.put(_outStripsLabel,std::move(genStripSet));
}


void fc::HitStripGenModule::makeHitsStrips(HitSet& hitSet, StripSet & stripSet, const GenTrack & genTrack,int trackNumber, int & hitNumber) const{


  TVector3 hitPosition;


  for (int ii_layer = 0; ii_layer < _detectorGeometry.getNSensors(); ++ii_layer) {

    bool intersectedLayer = intersectWithLayer(genTrack.makeHelix(_detectorGeometry.getCurvatureCInField(_detectorGeometry.getBField())),
					       hitPosition,ii_layer,_detectorGeometry);

    if (intersectedLayer){
    storeHitInfo(hitSet,trackNumber,hitNumber,hitPosition,ii_layer);

    storeStripInfo(stripSet,hitPosition,ii_layer);
    } else {
      std::cout << "missed intersection" << std::endl;
    }

  } // end layer loop


}


// !!!!! remove Gen track from here
void fc::HitStripGenModule::storeHitInfo(HitSet & hitSet,int trackNumber,int & hitNumber,TVector3 & hitPosition,int layer) const{

  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Hit y " << hitPosition[0] << std::endl;
    std::cout << "Layer " << layer << " Hit x " << hitPosition[1] << std::endl;
  }

  // Pure gen hit, numberStrip = -1
  Hit hit(hitPosition,layer,-1,trackNumber);

  bool isValidHit = fcf::isValidHit(layer,hitPosition,_detectorGeometry);

  if (isValidHit) {

    hitSet.insertHit(hit);

    ++hitNumber;

  }
  

  hitPosition = hitPosition + _random.getNormalDouble(0.0,_detectorGeometry.getSensor(layer)._hitResolution)*_detectorGeometry.getSensor(layer)._measurementDirection;
 
  if (_debugLevel >=5 ) {
    std::cout << "Layer " << layer << " Resolution smeared Hit x " << hitPosition[0] << std::endl;
  }

}


void fc::HitStripGenModule::storeStripInfo(StripSet & stripSet,const TVector3 & hitPosition,int layer) const{

  bool isValidHit = fcf::isValidHit(layer,hitPosition,_detectorGeometry);

  if (isValidHit) {

    double localHitPosition = fcf::calclateLocalFromGlobalPostion(hitPosition, layer,_detectorGeometry);
    double stripHitPosition = fcf::calculateStripFromLocalPosition(localHitPosition,layer,_detectorGeometry);

    int initialStrip;
    std::vector <int>stripAdcVector;
    generateClusterFromStripHitPosition(stripHitPosition,initialStrip,stripAdcVector);

    storeCluster(stripSet,layer,initialStrip,stripAdcVector);

  }
}




void fc::HitStripGenModule::generateClusterFromStripHitPosition(double stripHitPosition, int & initialStrip, std::vector<int> & stripAdcVector) const{

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


void fc::HitStripGenModule::storeCluster(StripSet & stripSet, int layer, int initialStrip, const std::vector<int> & stripAdcVector) const{

  int ii_strip = initialStrip;
  for (std::vector<int>::const_iterator stripAdcIter = stripAdcVector.begin(); stripAdcIter != stripAdcVector.end(); ++stripAdcIter){
    if (fcf::isValidStrip(layer,ii_strip,_detectorGeometry)) stripSet.insertStrip(layer,ii_strip,*stripAdcIter);
    ++ii_strip;
  }

}
