#include <string>
#include <limits>
#include "DetectorGeometry.hh"
#include "Exception.hh"
#include "TVector3.h"
#include "HitSet.hh"


fc::HitSet::HitSet():
  _version(1){
}


fc::HitSet::HitSet(int eventNumber,bool genHits):
  _version(1),
  _genHits(genHits),
  _eventNumber(eventNumber){
}


void fc::HitSet::insertHit(Hit hit) {
  if (hit.getLayer() >= 0 && hit.getLayer() < DetectorGeometry::_nSensors) {
    _hitVector.push_back(hit);
    return;
  } else {
    throw Exception("HitSet::insertHit: Out of bounds layer");
  }
  
}

void fc::HitSet::writeEvent(std::ofstream & hitdata) const{

  // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
  hitdata.precision(std::numeric_limits<double>::digits10 + 2);

  int hitNumber = 0;

  hitdata << "Hits" << std::endl;
  hitdata << _version << std::endl;
  hitdata << _genHits << std::endl;
  hitdata << _eventNumber << std::endl;

  std::vector<Hit>::size_type numberHits =_hitVector.size();

  hitdata << numberHits << std::endl;

  for (std::vector<Hit>::const_iterator hitIter =  _hitVector.begin(); hitIter != _hitVector.end(); ++hitIter,++hitNumber){

    hitdata << hitNumber << std::endl;
    hitdata << hitIter->getHitPosition().x() << std::endl;;
    hitdata << hitIter->getHitPosition().y() << std::endl;;
    hitdata << hitIter->getHitPosition().z() << std::endl;
    hitdata << hitIter->getLayer() << std::endl;
    hitdata << hitIter->getNumberStrips() << std::endl;
    hitdata << hitIter->getTrackNumber() << std::endl;

  } // end hit loop

}



void fc::HitSet::readEvent(std::ifstream & hitdata) {

  std::string eventDataObject;
  int version;
  int numberHits;
  int hitNumber;
  int layer;
  int numberStrips;
  int trackNumber;
  TVector3 hitPosition;

  hitdata >> eventDataObject;

  if (eventDataObject != "Hits"){
    std::string wrongEventDataObject = "HitSet::readEvent: attempted to read wrong data object" + eventDataObject;
    throw Exception(wrongEventDataObject);  
  }

  hitdata >> version;

  if (version != _version) {
    std::string wrongStreamerVersion = "HitSet::readEvent: attempted to read version " + std::to_string(version) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }

  hitdata >> _genHits;
  hitdata >> _eventNumber;
  hitdata >> numberHits;
 
  for (int ii_hit = 0; ii_hit < numberHits; ++ii_hit){

    hitdata >> hitNumber;
    hitdata >> hitPosition[0];
    hitdata >> hitPosition[1];
    hitdata >> hitPosition[2];
    hitdata >> layer; 
    hitdata >> numberStrips;
    hitdata >> trackNumber;

    Hit hit(hitPosition,layer,numberStrips,trackNumber);

    insertHit(hit);

  } // end hit loop

}


void fc::HitSet::print(void) const{

  std::cout << "Event: " << _eventNumber << std::endl;
  std::cout << "Hit Set version: " << _version << std::endl;
  std::cout << "Gen Hits: " << _genHits << std::endl;

 
  std::vector<Hit>::size_type numberHits =_hitVector.size();

  int hitNumber = 0;

  std::cout << "Number Hits: " << numberHits << std::endl; 

  for (std::vector<Hit>::const_iterator hitIter =  _hitVector.begin(); hitIter != _hitVector.end(); ++hitIter,++hitNumber){
 
    std::cout << "Hit: " << hitNumber << " associated with track " << hitIter->getTrackNumber() << std::endl; 
    hitIter->print();
  } // end hit loop


}
