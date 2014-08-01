#include <string>
#include <limits>
#include "Services/include/Exception.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "Algorithms/include/GenHitSetIO.hh"


fc::GenHitSetIO::GenHitSetIO():
  _version(1){
}

void fc::GenHitSetIO::writeEvent(const GenHitSet & genHitSet, std::ofstream & hitdata) const{


  // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
  hitdata.precision(std::numeric_limits<double>::digits10 + 2);

  int hitNumber = 0;

  hitdata << "GenHits" << std::endl;
  hitdata << _version << std::endl;

  genHitSet::size_type numberHits = genHitSet.getGenHits().size();

  hitdata << numberHits << std::endl;

  for (genHitSet::const_iterator hitIter =   genHitSet.getGenHits().begin(); hitIter !=  genHitSet.getGenHits().end(); ++hitIter,++hitNumber){

    hitdata << hitNumber << std::endl;
    hitdata << hitIter->getGenHitPosition().x() << std::endl;;
    hitdata << hitIter->getGenHitPosition().y() << std::endl;;
    hitdata << hitIter->getGenHitPosition().z() << std::endl;
    hitdata << hitIter->getLayer() << std::endl;
    hitdata << hitIter->getTrackNumber() << std::endl;

  } // end hit loop

}



void fc::GenHitSetIO::readEvent(GenHitSet& genHitSet, std::ifstream & hitdata) {

  std::string eventDataObject;
  int version;
  int numberHits;
  int hitNumber;
  int layer;
  int trackNumber;
  TVector3 hitPosition;

  hitdata >> eventDataObject;

  if (eventDataObject != "GenHits"){
    std::string wrongEventDataObject = "GenHitSetIO::readEvent: attempted to read wrong data object" + eventDataObject;
    throw Exception(wrongEventDataObject);  
  }

  hitdata >> version;

  if (version !=  _version) {
    std::string wrongStreamerVersion = "GenHitSetIO::readEvent: attempted to read version " + std::to_string(version) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }

  hitdata >> numberHits;
 
  for (int ii_hit = 0; ii_hit < numberHits; ++ii_hit){

    hitdata >> hitNumber;
    hitdata >> hitPosition[0];
    hitdata >> hitPosition[1];
    hitdata >> hitPosition[2];
    hitdata >> layer; 
    hitdata >> trackNumber;


    GenHit hit(hitPosition,layer,trackNumber);

    genHitSet.insertGenHit(hit);

  } // end hit loop

}