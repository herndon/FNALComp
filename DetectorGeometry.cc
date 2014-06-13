#define _USE_MATH_DEFINES
#include<cmath>
#include <fstream>
#include <iostream>
#include "Exception.hh"
#include "DetectorGeometry.hh"

fc::DetectorGeometry::DetectorGeometry(){
  _initSensorLimits();
  _initDetectorGeometry();
}

fc::DetectorGeometry::DetectorGeometry(std::ifstream & detectorgeometryfile){
  _initSensorLimits();
  _initDetectorGeometryFromFile(detectorgeometryfile);
}

void fc::DetectorGeometry::_initSensorLimits( void ) {

  // Minimum and maximum sensor limits
  // Confirms that users and entering a reasonable geometry

  _sensorMinLimits._nStrips = 1;
  _sensorMinLimits._stripPitch = 0.0005;
  _sensorMinLimits._center[1] = 0.0;
  _sensorMinLimits._resolution = 0.0002;

  _sensorMaxLimits._nStrips = 10000;
  _sensorMaxLimits._stripPitch = 0.0200;
  _sensorMaxLimits._center[1] = 100;
  _sensorMaxLimits._resolution = 0.0100;
}


void fc::DetectorGeometry::_initDetectorGeometry( void ) {

  _detectorGeometryVersion = 1;

  // Only Z oriented B field allowed
  _bField[0] = 0.0;
  _bField[1] = 0.0;
  _bField[2] = 1.0;
  // Also defines units as
  _curvatureC = _bField[2]*2.99792458e8/1.0e9;  

  // Sensor postion and normal
  double center[3] {0.0,0.0,0.0};
  double norm[3] {0.0,1.0,0.0};

  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    for (int ii = 0; ii < 3; ++ii){
      _sensor[ii_layer]._center[ii] = center[ii];
      _sensor[ii_layer]._norm[ii] = norm[ii];
    }
  }

  _sensor[0]._nStrips = 2048;
  _sensor[0]._stripPitch = 0.0025;
  _sensor[0]._resolution = 0.0007; 
  _sensor[0]._center[1] = 2.0;


  _sensor[1]._nStrips = 2048;
  _sensor[1]._stripPitch = 0.0025;
  _sensor[1]._resolution = 0.0007;
  _sensor[1]._center[1] = 4.0;


  _sensor[2]._nStrips = 2048;
  _sensor[2]._stripPitch = 0.0050;
  _sensor[2]._resolution = 0.0012;
  _sensor[2]._center[1] = 6.0;


  _sensor[3]._nStrips = 2048;
  _sensor[3]._stripPitch = 0.0050;
  _sensor[3]._resolution = 0.0012;
  _sensor[3]._center[1] = 8.0;


  _sensor[4]._nStrips = 2048;
  _sensor[4]._stripPitch = 0.0050;
  _sensor[4]._resolution = 0.0012;
  _sensor[4]._center[1] = 10.0;

  _defaultGeometry = true;

}

void fc::DetectorGeometry::_initDetectorGeometryFromFile(std::ifstream & detectorgeometryfile) {

  // Sensor geometry file format is rigid to make mistakes less likley

  bool badGeometry = false;

  if (!detectorgeometryfile){
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: can't open sensorgeometry.txt file");
  }

  std::string detectorGeometryString;
  int sensorNumber;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "DetectorGeometryVersion"){
    detectorgeometryfile >> _detectorGeometryVersion;
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "zBField"){
  // Only Z oriented B field allowed
    _bField[0] = 0.0;
    _bField[1] = 0.0;
    detectorgeometryfile >> _bField[2];
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }

  _curvatureC = _bField[2]*2.99792458e8/1.0e9;  


  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString == "Sensor"){
      detectorgeometryfile >> sensorNumber;
      if (sensorNumber == ii_layer){
	detectorgeometryfile >> _sensor[ii_layer]._nStrips;
	detectorgeometryfile >> _sensor[ii_layer]._stripPitch;
	detectorgeometryfile >> _sensor[ii_layer]._resolution;
        detectorgeometryfile >> _sensor[ii_layer]._center[0];
        detectorgeometryfile >> _sensor[ii_layer]._center[1];
        detectorgeometryfile >> _sensor[ii_layer]._center[2];
        detectorgeometryfile >> _sensor[ii_layer]._norm[0];
        detectorgeometryfile >> _sensor[ii_layer]._norm[1];
        detectorgeometryfile >> _sensor[ii_layer]._norm[2];

	if ( _sensor[ii_layer]._nStrips < _sensorMinLimits._nStrips|| _sensor[ii_layer]._nStrips > _sensorMaxLimits._nStrips ) badGeometry = true;
	if ( _sensor[ii_layer]._stripPitch < _sensorMinLimits._stripPitch|| _sensor[ii_layer]._stripPitch > _sensorMaxLimits._stripPitch ) badGeometry = true;
	if ( _sensor[ii_layer]._center[1] < _sensorMinLimits._center[1]|| _sensor[ii_layer]._center[1] > _sensorMaxLimits._center[1] ) badGeometry = true;
	if ( _sensor[ii_layer]._resolution < _sensorMinLimits._resolution|| _sensor[ii_layer]._resolution > _sensorMaxLimits._resolution ) badGeometry = true;
	if (badGeometry){
	  printSensorLimits();
          throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Out of bounds sensor number specifications in sensorgeometry.txt");
	}
      } else {
	throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
     }
    } else {
	throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
    }
  }

  _defaultGeometry = false;


}

void fc::DetectorGeometry::printDetectorGeometry( void ) const {

  std::cout << "Detector Geometry information" << std::endl;
  std::cout << "Detector Geometry version: " << _detectorGeometryVersion << std::endl;
  std::cout << "Magnetic field " << _bField[2] << " Tesla" << std::endl;
  if (_defaultGeometry)  std::cout << "Using default geometry" << std::endl;
  if (!_defaultGeometry) std::cout << "Using custum geometry" << std::endl;
  std::cout << "Number sensor layers " << _nSensors << std::endl;

  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    std::cout << std::endl;
    std::cout << "Sensor layer " << ii_layer << std::endl;
    std::cout << "N strips    "  << _sensor[ii_layer]._nStrips    << std::endl;
    std::cout << "Strip pitch "  << _sensor[ii_layer]._stripPitch << std::endl;
    std::cout << "Y position  "  << _sensor[ii_layer]._center[1]       << std::endl;
    std::cout << "Resolution  "  << _sensor[ii_layer]._resolution << std::endl;
  }
}

void fc::DetectorGeometry::printSensorLimits( void ) const {

   std::cout << "Detector Sencor Geometry limit information" << std::endl;
   std::cout << "Custom Geometry specified sensor information may be outside of allowed limits" << std::endl;
   std::cout << "This may be intentional in which case allowed limits in _initDetectorGeometry should be reprogramed" << std::endl;

   std::cout << "N strips    min - max "  << _sensorMinLimits._nStrips    << " - " <<  _sensorMaxLimits._nStrips << std::endl;
   std::cout << "Strip pitch min - max "  << _sensorMinLimits._stripPitch << " - " <<  _sensorMaxLimits._stripPitch << std::endl;
   std::cout << "Y position  min - max"  << _sensorMinLimits._center[1]       << " - " <<  _sensorMaxLimits._center[1]  << std::endl;
   std::cout << "Resolution  min - max"  << _sensorMinLimits._resolution << " - " <<  _sensorMaxLimits._resolution  << std::endl;
 }


const fc::sensorDescriptor& fc::DetectorGeometry::getSensor(int layer) const{

  if (layer >= 0 && layer < _nSensors) return _sensor[layer];
  throw Exception(" DetectorGeometry::getSensor: Out of bounds layer");  

}
