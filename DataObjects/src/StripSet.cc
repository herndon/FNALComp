//#include <algorithm>
#include "DataObjects/include/StripSet.hh"
#include "Services/include/Exception.hh"


fc::StripSet::StripSet():
  _version(1){
}

fc::StripSet::StripSet(bool genStrips):
  _genStrips(genStrips),
  _version(1){
}

const fc::layerStripMap& fc::StripSet::getLayerStripMap(int layer) const {
    if (layer >= 0 && layer < DetectorGeometry::_nSensors) return _layerStripMapArray[layer];
     throw Exception("StripSet::getLayerStripMap: Out of bounds layer");  
}

void fc::StripSet::insertStrip(int layer, int strip, int adc) {
  if (layer >= 0 && layer < DetectorGeometry::_nSensors){
    _layerStripMapArray[layer].insert(layerStripMap::value_type(strip,adc));
    return;
  } else {
    throw Exception("StripSet::insertStrip: Out of bounds layer");  
  }
}

// Improved insertStrip
// void fc::StripSet::insertStrip(int layer, int strip, int adc) {
//   if (layer >= 0 && layer < DetectorGeometry::_nSensors){
    
//     layerStripMap::iterator stripIter =  _layerStripMapArray[layer].find(strip);
//     if (stripIter == _layerStripMapArray[layer].end()) {
//       _layerStripMapArray[layer].insert(layerStripMap::value_type(strip,adc));
//     } else {
//       // second is the strip acd value in the map of key strip number and value adc
//       stripIter->second = std::min(stripIter->second + adc, 32);
//     }
//     return;
//   } else {
//     throw Exception("StripSet::insertStrip: Out of bounds layer");  
//   }
// }




void fc::StripSet::print(void) const{

  std::cout << "Version: " << _version << std::endl;
  std::cout << "Generator strip data: " << _genStrips << std::endl;

  int strip;
  int adc;

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
 
    layerStripMap::size_type numberStrips =_layerStripMapArray[ii_layer].size();

    std::cout << "Layer: " << ii_layer << " number Strips: " << numberStrips << std::endl; 

    for (layerStripMap::const_iterator layerStripMapIter =  _layerStripMapArray[ii_layer].begin(); layerStripMapIter != _layerStripMapArray[ii_layer].end(); ++layerStripMapIter){
      strip = getStripNumber(layerStripMapIter);
      adc = getStripAdc(layerStripMapIter); 
 
      std::cout << "Strip: " << strip << " " << " ADC " << adc << std::endl; 
    } // end strip loop

  } // end layer loop

}

