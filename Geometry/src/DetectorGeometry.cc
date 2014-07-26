#define _USE_MATH_DEFINES
#include<cmath>
#include<limits>
#include <fstream>
#include <iostream>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"

fc::DetectorGeometry::DetectorGeometry(){
  initDetectorGeometry();
}

fc::DetectorGeometry::DetectorGeometry(std::ifstream & detectorgeometryfile){
  initDetectorGeometry();
  initDetectorGeometryFromFile(detectorgeometryfile);
}



void fc::DetectorGeometry::initDetectorGeometry( void ) {

  _detectorGeometryVersion = 2;

  // Only Z oriented B field allowed
  _bField.SetXYZ(0.0,0.0,1.0);

  // Details of curvature calculation
  //using p = BqR, 
  //multiply by c to get energy in J  pc = BqRc, 
  //divide by e to get energy in eV pc = BqR3x108/1.6x10-19
  // q = Q1.6x10-19 where Q = 1, -1...
  //divide by 109 pc (in GeV) = BQR3x108/(1x10^9)
  // Also defines units  in meters
  _curvatureC = _bField.Z()*2.99792458e8/1.0e9; // ~ 0.3;  

  // MIP charge
  _MIP = 32.0;

  _maxNumberStrips = 2048;

}

void fc::DetectorGeometry::initDetectorGeometryFromFile(std::ifstream & detectorgeometryfile) {


  if (!detectorgeometryfile){
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: can't open sensorgeometry.txt file");
  }

  std::string detectorGeometryString;
  int detectorGeometryVersion;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "DetectorGeometryVersion"){
    detectorgeometryfile >> detectorGeometryVersion;
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }

   if ( detectorGeometryVersion != _detectorGeometryVersion) {
    std::string wrongDetectorGeometryVersion = "DetectorGeometry::_initDetectorGeometryFromFile: attempted to read version " + std::to_string(detectorGeometryVersion) + " when expecting " + std::to_string(_detectorGeometryVersion);
    throw Exception(wrongDetectorGeometryVersion);  
  }



  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "zBField"){
    // Only Z oriented B field allowed
    double zBField;
    _bField.SetX(0.0);
    _bField.SetY(0.0);
    detectorgeometryfile >> zBField;
    _bField.SetZ(zBField);
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }


  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "MIP"){
    // Only Z oriented B field allowed
    detectorgeometryfile >> _MIP;
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }

  _curvatureC = _bField[2]*2.99792458e8/1.0e9;  

  int numberSensors;
  int sensorNumber;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "NumberSensors"){
    detectorgeometryfile >> numberSensors;
  } else {
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  }


  detectorgeometryfile.precision(std::numeric_limits<double>::digits10 + 2);


  for (int ii_layer = 0; ii_layer < numberSensors; ++ii_layer){
    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString != "Sensor") throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");

    detectorgeometryfile >> sensorNumber;
    sensorDescriptor sensor;
    detectorgeometryfile >> sensor._nStrips;
    detectorgeometryfile >> sensor._stripPitch;
    detectorgeometryfile >> sensor._intrinsicHitResolution;
    detectorgeometryfile >> sensor._hitResolution;
    detectorgeometryfile >> sensor._badHitResolution;
    // !!!!! This is a good place for a test case using valgrind
    detectorgeometryfile >> sensor._threshold;
    detectorgeometryfile >> sensor._center[0];
    detectorgeometryfile >> sensor._center[1];
    detectorgeometryfile >> sensor._center[2];
    detectorgeometryfile >> sensor._normal[0];
    detectorgeometryfile >> sensor._normal[1];
    detectorgeometryfile >> sensor._normal[2];
    detectorgeometryfile >> sensor._measurementDirection[0];
    detectorgeometryfile >> sensor._measurementDirection[1];
    detectorgeometryfile >> sensor._measurementDirection[2];
    detectorgeometryfile >> sensor._perpSize;

    sensor._normal *= 1.0/sensor._normal.Mag();
    sensor._measurementDirection *= 1.0/sensor._measurementDirection.Mag();

    if ( sensor._nStrips > _maxNumberStrips)  
      throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Out of bounds sensor number specifications in sensorgeometry.txt, maximum number of strips is 2048");   

    _sensors.push_back(sensor);

  }

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString != "PVX") 
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");
  detectorgeometryfile >> _primaryVertexX._hitResolution;
  detectorgeometryfile >> _primaryVertexX._center[0];
  detectorgeometryfile >> _primaryVertexX._center[1];
  detectorgeometryfile >> _primaryVertexX._center[2];
  detectorgeometryfile >> _primaryVertexX._normal[0];
  detectorgeometryfile >> _primaryVertexX._normal[1];
  detectorgeometryfile >> _primaryVertexX._normal[2];
  detectorgeometryfile >> _primaryVertexX._measurementDirection[0];
  detectorgeometryfile >> _primaryVertexX._measurementDirection[1];
  detectorgeometryfile >> _primaryVertexX._measurementDirection[2];

  _primaryVertexX._normal *= 1.0/_primaryVertexX._normal.Mag();
  _primaryVertexX._measurementDirection *= 1.0/_primaryVertexX._measurementDirection.Mag();

  detectorgeometryfile >> detectorGeometryString;

  if (detectorGeometryString != "PVZ")
    throw Exception("DetectorGeometry::_initDetectorGeometryFromFile: Bad format in sensorgeometry.txt");

  detectorgeometryfile >> _primaryVertexZ._hitResolution;
  detectorgeometryfile >> _primaryVertexZ._center[0];
  detectorgeometryfile >> _primaryVertexZ._center[1];
  detectorgeometryfile >> _primaryVertexZ._center[2];
  detectorgeometryfile >> _primaryVertexZ._normal[0];
  detectorgeometryfile >> _primaryVertexZ._normal[1];
  detectorgeometryfile >> _primaryVertexZ._normal[2];
  detectorgeometryfile >> _primaryVertexZ._measurementDirection[0];
  detectorgeometryfile >> _primaryVertexZ._measurementDirection[1];
  detectorgeometryfile >> _primaryVertexZ._measurementDirection[2];

  _primaryVertexZ._normal *= 1.0/_primaryVertexZ._normal.Mag();
  _primaryVertexZ._measurementDirection *= 1.0/_primaryVertexZ._measurementDirection.Mag();

}

void fc::DetectorGeometry::printDetectorGeometry( void ) const {

  std::cout << "Detector Geometry information" << std::endl;
  std::cout << "Detector Geometry version: " << _detectorGeometryVersion << std::endl;
  std::cout << "Magnetic field, Bz " << _bField[2] << " Tesla" << std::endl;
  std::cout << "MIP charge ACD counts in silicon sensor " << _MIP << std::endl;
  std::cout << "Number sensor layers " << _nSensors << std::endl;

  for (int ii_layer = 0; ii_layer < _nSensors; ++ii_layer){
    std::cout << std::endl;
    std::cout << "Sensor layer " << ii_layer << std::endl;
    std::cout << "N strips    "  << _sensors[ii_layer]._nStrips    << std::endl;
    std::cout << "Strip pitch "  << _sensors[ii_layer]._stripPitch << std::endl;
    std::cout << "X position (m) "  << _sensors[ii_layer]._center.x()       << std::endl;
    std::cout << "Y position (m) "  << _sensors[ii_layer]._center.y()       << std::endl;
    std::cout << "Z position (m) "  << _sensors[ii_layer]._center.z()       << std::endl;
    std::cout << "Measurement direction  "  << std::endl;
    _sensors[ii_layer]._measurementDirection.Print();
    std::cout << "Normal  "  << std::endl;
    _sensors[ii_layer]._normal.Print();
    std::cout << "Intrinsic Hit Resolution (m) "  << _sensors[ii_layer]._intrinsicHitResolution << std::endl;
    std::cout << "Hit Resolution           (m) "  << _sensors[ii_layer]._hitResolution << std::endl;
    std::cout << "Bad Hit Resolution       (m) "  << _sensors[ii_layer]._badHitResolution << std::endl;
    std::cout << "Strip Threshold          (m) "  << _sensors[ii_layer]._threshold << std::endl;
    std::cout << "Sensor dimentions        (m)X(m)  "  << _sensors[ii_layer]._stripPitch*_sensors[ii_layer]._nStrips 
	      << " X " << _sensors[ii_layer]._perpSize  << std::endl;
  }
  std::cout << "PV position " << std::endl;
    std::cout << "X position    (m) "  << _primaryVertexX._center.x()       << std::endl;
    std::cout << "Y position    (m) "  << _primaryVertexX._center.y()       << std::endl;
    std::cout << "Z position    (m) "  << _primaryVertexX._center.z()       << std::endl;
    std::cout << "PV Resolution (m) "  << _primaryVertexX._hitResolution    << std::endl;



}


const fc::sensorDescriptor& fc::DetectorGeometry::getSensor(int layer) const{

  if (layer >= 0 && layer < _nSensors) return _sensors[layer];
  if (layer==-2) return _primaryVertexX;
  if (layer==-1) return _primaryVertexZ;
  std::cout << "layer " << layer << std::endl;
  throw Exception(" DetectorGeometry::getSensor: Out of bounds layer");  

}
