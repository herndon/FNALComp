#include <string>
#include "HitSet.hh"
#include "Exception.hh"

fc::HitSet::HitSet():
  _version(1){
}


fc::HitSet::HitSet(int eventNumber,bool genHits):
  _version(1),
  _genHits(genHits),
  _eventNumber(eventNumber){
}

const fc::layerHitMap& fc::HitSet::getConstLayerHitMap(int layer) const {
    if (layer >= 0 && layer < DetectorGeometry::_nSensors) return _layerHitMapVector[layer];
     throw Exception("HitSet::getLayerHitMap: Out of bounds layer");  
}

fc::layerHitMap& fc::HitSet::getLayerHitMap(int layer) {
    if (layer >= 0 && layer < DetectorGeometry::_nSensors) return _layerHitMapVector[layer];
     throw Exception("HitSet::getLayerHitMap: Out of bounds layer");  
}

void fc::HitSet::insertHit(int hitNumber, Hit hit, int layer) {
  if (layer >= 0 && layer < DetectorGeometry::_nSensors) {
    _layerHitMapVector[layer].insert(layerHitMap::value_type(hitNumber,hit));
    return;
  } else {
    throw Exception("HitSet::insertHit: Out of bounds layer");
  }
  
}

void fc::HitSet::writeEvent(std::ofstream & hitdata) const{

  hitdata << "Hits" << std::endl;
  hitdata << _genHits << std::endl;
  hitdata << _version << std::endl;
  hitdata << _eventNumber << std::endl;

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer){
    std::map<int,int>::size_type numberHits =_layerHitMapVector[ii_layer].size();

    hitdata << ii_layer << std::endl;
    hitdata << numberHits << std::endl;

    for (layerHitMap::const_iterator layerHitMapIter =  _layerHitMapVector[ii_layer].begin(); layerHitMapIter != _layerHitMapVector[ii_layer].end(); ++layerHitMapIter){

      hitdata << getHitNumber(layerHitMapIter) << std::endl;
      hitdata << getConstHit(layerHitMapIter).getHitPosition()[0] << std::endl;;
      hitdata << getConstHit(layerHitMapIter).getHitPosition()[1] << std::endl;;
      hitdata << getConstHit(layerHitMapIter).getHitPosition()[2] << std::endl;
      hitdata << getConstHit(layerHitMapIter).getTrackNumber() << std::endl;

    } // end hit loop

  } // endl layer loop

}



void fc::HitSet::readEvent(std::ifstream & hitdata) {

  std::string eventDataObject;
  int version;
  int layer;
  int numberHits;
  int hitNumber;
  int trackNumber;
  double hitPosition[3];

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

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
 
    hitdata >> layer; 

    if (layer != ii_layer) throw Exception("HitSet::readEvent: bad hit data");

    hitdata >> numberHits;

    for (int ii_hit = 0; ii_hit < numberHits; ++ii_hit){

      hitdata >> hitNumber;
      hitdata >> hitPosition[0];
      hitdata >> hitPosition[1];
      hitdata >> hitPosition[2];
      hitdata >> trackNumber;

      Hit hit(hitPosition,trackNumber);

      insertHit(hitNumber,hit,ii_layer);

    } // end hit loop

  } // end layer loop 

}

void fc::HitSet::clear(void){

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
  _layerHitMapVector[ii_layer].clear();
  }

}

void fc::HitSet::print(void) const{

  std::cout << "Event: " << _eventNumber << std::endl;
  std::cout << "Gen Hits: " << _genHits << std::endl;

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
 
    std::map<int,int>::size_type numberHits =_layerHitMapVector[ii_layer].size();

    std::cout << "Layer: " << ii_layer << " number Hits: " << numberHits << std::endl; 

    for (layerHitMap::const_iterator layerHitMapIter =  _layerHitMapVector[ii_layer].begin(); layerHitMapIter != _layerHitMapVector[ii_layer].end(); ++layerHitMapIter){
 
      std::cout << "Hit: " << getHitNumber(layerHitMapIter) << " associated with track " << getConstHit(layerHitMapIter).getTrackNumber() << std::endl; 
      getConstHit(layerHitMapIter).print();
    } // end hit loop

  } // end layer loop

}
