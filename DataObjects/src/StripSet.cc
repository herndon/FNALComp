#include <iostream>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/StripSet.hh"

fc::StripSet::StripSet(const DetectorGeometry & detectorGeometry){
  _layerStripMapArray.resize(detectorGeometry.getNSensors());
}

const fc::layerStripMap& fc::StripSet::getLayerStripMap(unsigned int layer) const {
  if (layer >= 0 && layer < _layerStripMapArray.size()) return _layerStripMapArray[layer];
     throw Exception("StripSet::getLayerStripMap: Out of bounds layer");  
}

void fc::StripSet::insertStrip(unsigned int layer, int strip, int adc) {
  if (layer >= 0 && layer < _layerStripMapArray.size()){
    _layerStripMapArray[layer].insert(layerStripMap::value_type(strip,adc));
    return;
  } else {
    throw Exception("StripSet::insertStrip: Out of bounds layer");  
  }
}

// Improved insertStrip
// void fc::StripSet::insertStrip(int layer, unsigned int strip, int adc) {
//   if (layer >= 0 && layer < _layerStripMapArray.size()){
    
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




void fc::StripSet::print() const{

  std::cout << "StripSet" << std::endl;

  int strip;
  int adc;

  for (unsigned int ii_layer = 0; ii_layer < _layerStripMapArray.size(); ++ii_layer) {
 
    layerStripMap::size_type numberStrips =_layerStripMapArray[ii_layer].size();

    std::cout << "Layer: " << ii_layer << " number Strips: " << numberStrips << std::endl; 

    for (layerStripMap::const_iterator layerStripMapIter =  _layerStripMapArray[ii_layer].begin(); layerStripMapIter != _layerStripMapArray[ii_layer].end(); ++layerStripMapIter){
      strip = getStripNumber(layerStripMapIter);
      adc = getStripAdc(layerStripMapIter); 
 
      std::cout << "Strip: " << strip << " " << " ADC " << adc << std::endl; 
    } // end strip loop

  } // end layer loop

}

