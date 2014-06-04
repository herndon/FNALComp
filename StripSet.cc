#include "StripSet.hh"
#include "Exception.hh"

const layerMap& StripSet::getLayerMap(int layer) {
    if (layer >= 0 && layer < DetectorGeometry::_nSensors) return _layerMapVector[layer];
     throw Exception("StripSet::genLayerMap: Out of bounds layer");  

}

void StripSet::insertStrip(int layer, int strip, int adc) {
  _layerMapVector[layer].insert(layerMap::value_type(strip,adc));
}

void StripSet::writeEvent(std::ofstream & stripData) {


  stripData.write (reinterpret_cast<const char *>(&_event), 1);

  int binaryData;
  int binaryData1;
  int binaryData2;
 
  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer){
    std::map<int,int>::size_type numberStrips =_layerMapVector[ii_layer].size();



    stripData.write (reinterpret_cast<const char *>(&ii_layer), 1);
    stripData.write (reinterpret_cast<const char *>(&numberStrips), 1);
 
    layerMap::iterator layerMapIter;
    for (layerMapIter =  _layerMapVector[ii_layer].begin(); layerMapIter != _layerMapVector[ii_layer].end(); ++layerMapIter){

      binaryData = layerMapIter->first * 32 + layerMapIter->second;
      binaryData1 = binaryData & bitmask1;
      binaryData2 = binaryData & bitmask2;
      binaryData2 = binaryData2 >> 8;

      stripData.write (reinterpret_cast<const char *>(&binaryData2), 1);
      stripData.write (reinterpret_cast<const char *>(&binaryData1), 1);


    }
  }
}



void StripSet::readEvent(std::ifstream & stripdata) {


  // Binary data in char byte format
  // !!!!! Candidate for run time bug 
 unsigned  char * binaryData;
  binaryData = new unsigned  char[1];

  int event;
  int layer;
  int numberStrips;
  int stripData12;
  int stripData1;
  int stripData2;
  int strip;
  int adc;

   stripdata.read (reinterpret_cast<char *>(binaryData), 1);
   event = static_cast<int>(*binaryData);
  
   _event = event;
  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
 
    stripdata.read (reinterpret_cast<char *>(binaryData), 1);
    layer = static_cast<int>(*binaryData);

    if (layer != ii_layer) std::cerr << "StripSet::readEvent corrupt layer data" << std::endl;

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


    }

  } // layer loop 
  //print();

}

void StripSet::clear(void){

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
  _layerMapVector[ii_layer].clear();
  }

}

void StripSet::print(void){

  std::cout << "Event: " << _event << std::endl;

  int strip;
  int adc;

  for (int ii_layer = 0; ii_layer < DetectorGeometry::_nSensors; ++ii_layer) {
 
    std::map<int,int>::size_type numberStrips =_layerMapVector[ii_layer].size();

    std::cout << "Layer: " << ii_layer << " number Strips: " << numberStrips << std::endl; 

    layerMap::iterator layerMapIter;
    for (layerMapIter =  _layerMapVector[ii_layer].begin(); layerMapIter != _layerMapVector[ii_layer].end(); ++layerMapIter){
      strip = layerMapIter->first;
      adc = layerMapIter->second; 
 
      std::cout << "Strip: " << strip << " " << " ADC " << adc << std::endl; 
    }

  }

}

void StripSet::printRawData(std::ifstream & stripdata){

  int ii = 0;

  unsigned char * binaryData;
  binaryData = new unsigned  char[1];

  while (stripdata) {
    //in.get(c);
 

    stripdata.read (reinterpret_cast<char *>(binaryData), 1);
    if (stripdata) {
      
      std::cout << "Byte " << ii << " " << static_cast<int>(*binaryData) << "\n";
      ii++;
    }
  }

}
