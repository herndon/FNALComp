#include <string>
#include <limits>
#include "Exception.hh"
#include "HitSet.hh"
#include "HitSetIO.hh"


fc::HitSetIO::HitSetIO():
  _version(1){
}

void fc::HitSetIO::writeEvent(const HitSet & hitSet, std::ofstream & hitdata) const{

  // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
  hitdata.precision(std::numeric_limits<double>::digits10 + 2);

  int hitNumber = 0;

  hitdata << "Hits" << std::endl;
  hitdata << hitSet.getVersion() << std::endl;
  hitdata << hitSet.getGenHits() << std::endl;

  std::vector<Hit>::size_type numberHits = hitSet.getHitVector().size();

  hitdata << numberHits << std::endl;

  for (std::vector<Hit>::const_iterator hitIter =   hitSet.getHitVector().begin(); hitIter !=  hitSet.getHitVector().end(); ++hitIter,++hitNumber){

    hitdata << hitNumber << std::endl;
    hitdata << hitIter->getHitPosition().x() << std::endl;;
    hitdata << hitIter->getHitPosition().y() << std::endl;;
    hitdata << hitIter->getHitPosition().z() << std::endl;
    hitdata << hitIter->getLayer() << std::endl;
    hitdata << hitIter->getNumberStrips() << std::endl;
    hitdata << hitIter->getTrackNumber() << std::endl;

  } // end hit loop

}



void fc::HitSetIO::readEvent(HitSet& hitSet, std::ifstream & hitdata) {

  std::string eventDataObject;
  int version;
  bool genHits;
  int numberHits;
  int hitNumber;
  int layer;
  int numberStrips;
  int trackNumber;
  TVector3 hitPosition;

  hitdata >> eventDataObject;

  if (eventDataObject != "Hits"){
    std::string wrongEventDataObject = "HitSetIO::readEvent: attempted to read wrong data object" + eventDataObject;
    throw Exception(wrongEventDataObject);  
  }

  hitdata >> version;

  if (version !=  hitSet.getVersion()) {
    std::string wrongStreamerVersion = "HitSetIO::readEvent: attempted to read version " + std::to_string(version) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }

  hitdata >>  genHits;
  hitSet.setGenHits(genHits);

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

    hitSet.insertHit(hit);

  } // end hit loop

}
