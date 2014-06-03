#include "StripSet.hh"
#include "Exception.hh"

void StripSet::writeEvent(std::ofstream & stripData, int event) {


  stripData.write (reinterpret_cast<const char *>(&event), 1);

  int binaryData;
  int binaryData1;
  int binaryData2;
 
  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer){
    std::map<int,int>::size_type numberStrips =_layerMapVector[ii_layer].size();

      //if (debugLevel >= 5) {
      std::cout << "Writing layer " << ii_layer << " number Strips " << numberStrips << std::endl; 
      //}


    stripData.write (reinterpret_cast<const char *>(&ii_layer), 1);
    stripData.write (reinterpret_cast<const char *>(&numberStrips), 1);
 
    layerMap::iterator layerMapIter;
    for (layerMapIter =  _layerMapVector[ii_layer].begin(); layerMapIter != _layerMapVector[ii_layer].end(); ++layerMapIter){

      binaryData = layerMapIter->first * 32 + layerMapIter->second;
      binaryData1 = binaryData & bitmask1;
      binaryData2 = binaryData & bitmask2;
      binaryData2 = binaryData2 >> 8;

      std::cout << "Writing Strip " << layerMapIter->first << " ADC " << layerMapIter->second << std::endl;
      std::cout << "Writing binary data " << binaryData << " " << binaryData2 << " " << binaryData1 << std::endl;
      stripData.write (reinterpret_cast<const char *>(&binaryData2), 1);
      stripData.write (reinterpret_cast<const char *>(&binaryData1), 1);


    }
  }
}



const layerMap& StripSet::getLayerMap(int layer) {
    if (layer >= 0 && layer < DetectorGeometry::_nSensors) return _layerMapVector[layer];
     throw Exception("StripSet::genLayerMap: Out of bounds layer");  

}


void StripSet::insertStrip(int layer, int strip, int adc) {
  _layerMapVector[layer].insert(layerMap::value_type(strip,adc));
}

void StripSet::clear(void){

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
  _layerMapVector[ii_layer].clear();
  }

}
