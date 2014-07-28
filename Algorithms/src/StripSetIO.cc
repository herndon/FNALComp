#include <string>
#include <algorithm>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/StripSet.hh"
#include "Algorithms/include/StripSetIO.hh"


fc::StripSetIO::StripSetIO(const DetectorGeometry&  detectorGeometry):
  _detectorGeometry(detectorGeometry),
  _version(1){
}


void fc::StripSetIO::writeEvent(const StripSet & stripSet, std::ofstream & stripdata) const{


  stripdata << "Strips" << std::endl;

  stripdata.write (reinterpret_cast<const char *>(&_version), 1);


  int binaryData;
  int binaryData1;
  int binaryData2;
 
  for (int ii_layer = 0; ii_layer < _detectorGeometry.getNSensors(); ++ii_layer){
    layerStripMap::size_type numberStrips =stripSet.getLayerStripMap(ii_layer).size();

    stripdata.write (reinterpret_cast<const char *>(&ii_layer), 1);
    stripdata.write (reinterpret_cast<const char *>(&numberStrips), 1);

    for (layerStripMap::const_iterator layerStripMapIter =  stripSet.getLayerStripMap(ii_layer).begin(); layerStripMapIter != stripSet.getLayerStripMap(ii_layer).end(); ++layerStripMapIter){

      binaryData = stripSet.getStripNumber(layerStripMapIter) * 32 + stripSet.getStripAdc(layerStripMapIter);
      binaryData1 = binaryData & bitmask1;
      binaryData2 = binaryData & bitmask2;
      binaryData2 = binaryData2 >> 8;

      stripdata.write (reinterpret_cast<const char *>(&binaryData2), 1);
      stripdata.write (reinterpret_cast<const char *>(&binaryData1), 1);


    } // end strip loop
  } // end layer loop
}

void fc::StripSetIO::readEvent(StripSet & stripSet, std::ifstream & stripdata) {


  std::string eventDataString;

  stripdata >> eventDataString;

  if (eventDataString != "Strips"){
    std::string wrongEventDataObject = "StripSetIO::readEvent: attempted to read wrong data object" + eventDataString;
    throw Exception(wrongEventDataObject);  
  }

  // Binary data in char byte format
  // !!!!! Candidate for run time bug 
  unsigned  char binaryData;

  stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
  stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
  int version = static_cast<int>(binaryData);

  if (version != _version) {
    std::string wrongStreamerVersion = "StripSetIO::readEvent: attempted to read version " + std::to_string(version) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }


  for (int ii_layer = 0; ii_layer < _detectorGeometry.getNSensors(); ++ii_layer) {
 
    stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
    int layer = static_cast<int>(binaryData);

    if (layer != ii_layer)    if (layer != ii_layer) throw Exception("StripSetIO::readEvent: bad strip data");

    stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
    int numberStrips = static_cast<int>(binaryData);

    for (int ii_strip = 0; ii_strip < numberStrips; ++ii_strip){

      stripdata.read (reinterpret_cast<char *>(&binaryData), 1);

      int stripData2 =  static_cast<int>(binaryData);
      stripData2 = stripData2 << 8;
 
      stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
      int adc =  static_cast<int>(binaryData);
      adc = adc&adcBitmask;
      int stripData1 =  static_cast<int>(binaryData);
      int stripData12 = stripData2 + stripData1;
      stripData12 = stripData12&stripBitmask;
      int strip = stripData12 >> 5;

      stripSet.insertStrip(ii_layer,strip,adc);

    } // end strip loop

  } // end layer loop 

}

void fc::StripSetIO::printRawData(std::ifstream & stripdata) const{

  int ii = 0;

  unsigned char binaryData;

  while (stripdata) {
 
    stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
    if (stripdata) {
      
      std::cout << "Byte " << ii << " " << static_cast<int>(binaryData) << "\n";
      ++ii;
    }
  }
}
