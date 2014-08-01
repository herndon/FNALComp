#include <string>
#include <limits>
#include "Services/include/Exception.hh"
#include "DataObjects/include/HitSet.hh"
#include "Algorithms/include/HitSetIO.hh"


fc::HitSetIO::HitSetIO():
  _version(1){
}

void fc::HitSetIO::writeEvent(const HitSet & hitSet, std::ofstream & hitdata) const{


  // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
  hitdata.precision(std::numeric_limits<double>::digits10 + 2);

  int hitNumber = 0;

  hitdata << "Hits" << std::endl;
  hitdata << _version << std::endl;

  std::vector<Hit>::size_type numberHits = hitSet.getHits().size();

  hitdata << numberHits << std::endl;

  for (hitSet::const_iterator hitIter =   hitSet.getHits().begin(); hitIter !=  hitSet.getHits().end(); ++hitIter,++hitNumber){

    hitdata << hitNumber << std::endl;
    hitdata << hitIter->getHitPosition().x() << std::endl;;
    hitdata << hitIter->getHitPosition().y() << std::endl;;
    hitdata << hitIter->getHitPosition().z() << std::endl;
    hitdata << hitIter->getLayer() << std::endl;
    hitdata << hitIter->getNumberStrips() << std::endl;
    hitdata << hitIter->getCharge() << std::endl;
    hitdata << hitIter->isGoodHit() << std::endl;
    hitdata << hitIter->getResolution() << std::endl;


  } // end hit loop

}



void fc::HitSetIO::readEvent(HitSet& hitSet, std::ifstream & hitdata) {

  std::string eventDataObject;
  int version;
  int numberHits;
  int hitNumber;
  int layer;
  int numberStrips;
  int charge;
  bool goodHit;
  double resolution;
  TVector3 hitPosition;

  hitdata >> eventDataObject;

  if (eventDataObject != "Hits"){
    std::string wrongEventDataObject = "HitSetIO::readEvent: attempted to read wrong data object" + eventDataObject;
    throw Exception(wrongEventDataObject);  
  }

  hitdata >> version;

  if (version !=  _version) {
    std::string wrongStreamerVersion = "HitSetIO::readEvent: attempted to read version " + std::to_string(version) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }

  hitdata >> numberHits;
 
  for (int ii_hit = 0; ii_hit < numberHits; ++ii_hit){

    hitdata >> hitNumber;
    hitdata >> hitPosition[0];
    hitdata >> hitPosition[1];
    hitdata >> hitPosition[2];
    hitdata >> layer; 
    hitdata >> numberStrips;
    hitdata >> charge;
    hitdata >> goodHit;
    hitdata >> resolution;

      Hit hit(hitPosition,layer,numberStrips,charge,goodHit,resolution);

      hitSet.insertHit(std::move(hit));

  } // end hit loop

}
