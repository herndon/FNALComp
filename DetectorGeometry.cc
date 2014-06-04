#define _USE_MATH_DEFINES
#include "DetectorGeometry.hh"
#include <fstream>
#include <iostream>
#include<cmath>
#include "Exception.hh"

DetectorGeometry::DetectorGeometry(){
  _initDetectorGeometry();
}

DetectorGeometry::DetectorGeometry(std::ifstream & detectorgeometryfile){
  _initDetectorGeometry();
  _initDetectorGeometryFromFile(detectorgeometryfile);
}


void DetectorGeometry::_initDetectorGeometry( void ) {

  // Minimum and maximum sensor limits
  // Confirms that users and entering a reasonable geometry

  _sensorMinLimits._nStrips = 1;
  _sensorMinLimits._stripPitch = 0.0005;
  _sensorMinLimits._yPos = 0.0;
  _sensorMinLimits._resolution = 0.0002;

  _sensorMaxLimits._nStrips = 10000;
  _sensorMaxLimits._stripPitch = 0.0200;
  _sensorMaxLimits._yPos = 100;
  _sensorMaxLimits._resolution = 0.0100;


  _zBField = 1.0;


  // Default geometry

  _sensor[0]._nStrips = 2048;
  _sensor[0]._stripPitch = 0.0025;
  _sensor[0]._yPos = 2.0;
  _sensor[0]._resolution = 0.0007;

  _sensor[1]._nStrips = 2048;
  _sensor[1]._stripPitch = 0.0025;
  _sensor[1]._yPos = 4.0;
  _sensor[1]._resolution = 0.0007;

  _sensor[2]._nStrips = 2048;
  _sensor[2]._stripPitch = 0.0050;
  _sensor[2]._yPos = 6.0;
  _sensor[2]._resolution = 0.0012;

  _sensor[3]._nStrips = 2048;
  _sensor[3]._stripPitch = 0.0050;
  _sensor[3]._yPos = 8.0;
  _sensor[3]._resolution = 0.0012;

  _sensor[4]._nStrips = 2048;
  _sensor[4]._stripPitch = 0.0050;
  _sensor[4]._yPos = 10.0;
  _sensor[4]._resolution = 0.0012;

  _defaultGeometry = true;

}

void DetectorGeometry::_initDetectorGeometryFromFile(std::ifstream & detectorgeometryfile) {

  // Sensor geometry file format is ridgid to make mistakes less likley

 
  bool badGeometry = false;

  if (!detectorgeometryfile){
    std::cerr << "can't open sensorgeometry.txt file: using default geometry" << std::endl;
    badGeometry = true;
  }

  std::string detectorGeometryString;
  int sensorNumber;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "zBField"){
    detectorgeometryfile >> _zBField;
    if (std::fabs(_zBField) > 10.0) {
      std::cerr << "BField is " << _zBField << ", and should not be greater than 10.0 Tesla" << std::endl;
      std::cerr << "using default geometry" << std::endl;
      badGeometry = true;
    }
  } else {
    std::cerr << "Bad format sensorgeometry.txt" << std::endl;
    std::cerr << "Reverting to default geometry" << std::endl;
    badGeometry = true;
  }



  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString == "Sensor"){
      detectorgeometryfile >> sensorNumber;
      if (sensorNumber == ii_layer){
	detectorgeometryfile >> _sensor[ii_layer]._nStrips;
	detectorgeometryfile >> _sensor[ii_layer]._stripPitch;
	detectorgeometryfile >> _sensor[ii_layer]._yPos;
	detectorgeometryfile >> _sensor[ii_layer]._resolution;
	if ( _sensor[ii_layer]._nStrips < _sensorMinLimits._nStrips|| _sensor[ii_layer]._nStrips > _sensorMaxLimits._nStrips ) badGeometry = true;
	if ( _sensor[ii_layer]._stripPitch < _sensorMinLimits._stripPitch|| _sensor[ii_layer]._stripPitch > _sensorMaxLimits._stripPitch ) badGeometry = true;
	if ( _sensor[ii_layer]._yPos < _sensorMinLimits._yPos|| _sensor[ii_layer]._yPos > _sensorMaxLimits._yPos ) badGeometry = true;
	if ( _sensor[ii_layer]._resolution < _sensorMinLimits._resolution|| _sensor[ii_layer]._resolution > _sensorMaxLimits._resolution ) badGeometry = true;
	if (badGeometry){
	  std::cerr << "Bad sensor data in sensorgeometry.txt" << std::endl;
	  std::cerr << "Reverting to default geometry" << std::endl;
	  printSensorLimits();
	}
      } else {
	std::cerr << "Bad layer number in sensorgeometry.txt" << std::endl;
	std::cerr << "Reverting to default geometry" << std::endl;
	badGeometry = true;
      }
    } else {
      std::cerr << "Bad format sensorgeometry.txt" << std::endl;
      std::cerr << "Reverting to default geometry" << std::endl;
      badGeometry = true;
    }
  }

  if (badGeometry) _initDetectorGeometry();

  if (!badGeometry) _defaultGeometry = false;


}

void DetectorGeometry::printDetectorGeometry( void ) {

  std::cout << "Detector Geometry information" << std::endl;
  std::cout << "Magnetic field " << _zBField << " Tesla" << std::endl;
  if (_defaultGeometry)  std::cout << "Using default geometry" << std::endl;
  if (!_defaultGeometry) std::cout << "Using custum geometry" << std::endl;
  std::cout << "Number sensor layers " << _nSensors << std::endl;

  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    std::cout << std::endl;
    std::cout << "Sensor layer " << ii_layer << std::endl;
    std::cout << "N strips    "  << _sensor[ii_layer]._nStrips    << std::endl;
    std::cout << "Strip pitch "  << _sensor[ii_layer]._stripPitch << std::endl;
    std::cout << "Y position  "  << _sensor[ii_layer]._yPos       << std::endl;
    std::cout << "Resolution  "  << _sensor[ii_layer]._resolution << std::endl;
  }
}

void DetectorGeometry::printSensorLimits( void ) {

   std::cout << "Detector Sencor Geometry limit information" << std::endl;
   std::cout << "Custom Geometry specified sensor information may be outside of allowed limits" << std::endl;
   std::cout << "This may be intentional in which case allowed limits in _initDetectorGeometry should be reprogramed" << std::endl;

   std::cout << "N strips    min - max "  << _sensorMinLimits._nStrips    << " - " <<  _sensorMaxLimits._nStrips << std::endl;
   std::cout << "Strip pitch min - max "  << _sensorMinLimits._stripPitch << " - " <<  _sensorMaxLimits._stripPitch << std::endl;
   std::cout << "Y position  min - max"  << _sensorMinLimits._yPos       << " - " <<  _sensorMaxLimits._yPos  << std::endl;
   std::cout << "Resolution  min - max"  << _sensorMinLimits._resolution << " - " <<  _sensorMaxLimits._resolution  << std::endl;
 }


const sensorDescriptor& DetectorGeometry::getSensor(int layer) {

  if (layer >= 0 && layer < _nSensors) return _sensor[layer];
  throw Exception(" DetectorGeometry::getSensor: Out of bounds layer");  

}
