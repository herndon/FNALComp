#include <string>
#include <algorithm>
#include "StripSet.hh"
#include "Exception.hh"

fc::StripSet::StripSet():
  _version(1){
}

fc::StripSet::StripSet(int eventNumber,bool genStrips):
  _eventNumber(eventNumber),
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


void fc::StripSet::writeEvent(std::ofstream & stripdata) const{

  stripdata << "Strips" << std::endl;

  stripdata.write (reinterpret_cast<const char *>(&_version), 1);

  stripdata.write (reinterpret_cast<const char *>(&_genStrips), 1);

  stripdata.write (reinterpret_cast<const char *>(&_eventNumber), 1);

  int binaryData;
  int binaryData1;
  int binaryData2;
 
  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer){
    std::map<int,int>::size_type numberStrips =_layerStripMapArray[ii_layer].size();

    stripdata.write (reinterpret_cast<const char *>(&ii_layer), 1);
    stripdata.write (reinterpret_cast<const char *>(&numberStrips), 1);
 
    layerStripMap::const_iterator layerStripMapIter;
    for (layerStripMapIter =  _layerStripMapArray[ii_layer].begin(); layerStripMapIter != _layerStripMapArray[ii_layer].end(); ++layerStripMapIter){

      binaryData = getStripNumber(layerStripMapIter) * 32 + getStripAdc(layerStripMapIter);
      binaryData1 = binaryData & bitmask1;
      binaryData2 = binaryData & bitmask2;
      binaryData2 = binaryData2 >> 8;

      stripdata.write (reinterpret_cast<const char *>(&binaryData2), 1);
      stripdata.write (reinterpret_cast<const char *>(&binaryData1), 1);


    } // end strip loop
  } // end layer loop
}

void fc::StripSet::readEvent(std::ifstream & stripdata) {


  // Binary data in char byte format
  // !!!!! Candidate for run time bug 
  unsigned  char * binaryData;
  binaryData = new unsigned  char[1];

  int version;
  int layer;
  int numberStrips;
  int stripData12;
  int stripData1;
  int stripData2;
  int strip;
  int adc;

  std::string eventDataObject;

  stripdata >> eventDataObject;

  if (eventDataObject != "Strips"){
    std::string wrongEventDataObject = "StripSet::readEvent: attempted to read wrong data object" + eventDataObject;
    throw Exception(wrongEventDataObject);  
  }

  stripdata.read (reinterpret_cast<char *>(binaryData), 1);
  stripdata.read (reinterpret_cast<char *>(binaryData), 1);
  version = static_cast<int>(*binaryData);

  if (version != _version) {
    std::string wrongStreamerVersion = "StripSet::readEvent: attempted to read version " + std::to_string(version) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }

  stripdata.read (reinterpret_cast<char *>(binaryData), 1);
  _genStrips = static_cast<int>(*binaryData);

  stripdata.read (reinterpret_cast<char *>(binaryData), 1);
  _eventNumber = static_cast<int>(*binaryData);
   
  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
 
    stripdata.read (reinterpret_cast<char *>(binaryData), 1);
    layer = static_cast<int>(*binaryData);

    if (layer != ii_layer)    if (layer != ii_layer) throw Exception("StripSet::readEvent: bad strip data");

    stripdata.read (reinterpret_cast<char *>(binaryData), 1);
    numberStrips = static_cast<int>(*binaryData);

    for (int ii_strip = 0; ii_strip < numberStrips; ++ii_strip){

      stripdata.read (reinterpret_cast<char *>(binaryData), 1);

      stripData2 =  static_cast<int>(*binaryData);
      stripData2 = stripData2 << 8;
 
      stripdata.read (reinterpret_cast<char *>(binaryData), 1);
      adc =  static_cast<int>(*binaryData);
      adc = adc&adcBitmask;
      stripData1 =  static_cast<int>(*binaryData);
      stripData12 = stripData2 + stripData1;
      stripData12 = stripData12&stripBitmask;
      strip = stripData12 >> 5;

      insertStrip(ii_layer,strip,adc);

    } // end strip loop

  } // end layer loop 

}

void fc::StripSet::clear(void){

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
  _layerStripMapArray[ii_layer].clear();
  }

}

void fc::StripSet::print(void) const{

  std::cout << "Event: " << _eventNumber << std::endl;

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

void fc::StripSet::printRawData(std::ifstream & stripdata) const{

  int ii = 0;

  unsigned char * binaryData;
  binaryData = new unsigned  char[1];

  while (stripdata) {
 
    stripdata.read (reinterpret_cast<char *>(binaryData), 1);
    if (stripdata) {
      
      std::cout << "Byte " << ii << " " << static_cast<int>(*binaryData) << "\n";
      ++ii;
    }
  }
}
