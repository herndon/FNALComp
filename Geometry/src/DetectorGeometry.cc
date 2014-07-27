#include <iostream>
#include <cmath>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"

fc::DetectorGeometry::DetectorGeometry(int detectorGeometryVersion,int nXSensors, int nSASSensors, int nZSensors,
				       const TVector3& bField,double MIP,double curvatureC,int maxNumberStrips,
				       const std::vector<sensorDescriptor>& sensors,
				       const sensorDescriptor& primaryVertexX,const sensorDescriptor& primaryVertexZ):
  _detectorGeometryVersion(detectorGeometryVersion),
  _nXSensors(nXSensors),
  _nSASSensors(nSASSensors),
  _nZSensors(nZSensors),
  _bField(bField),
  _MIP(MIP),
  _curvatureC(curvatureC),
  _maxNumberStrips(maxNumberStrips),
  _sensors(sensors),
  _primaryVertexX(primaryVertexX),
  _primaryVertexZ(primaryVertexZ){
}

void fc::DetectorGeometry::printDetectorGeometry( void ) const {

  std::cout << "Detector Geometry information" << std::endl;
  std::cout << "Detector Geometry version: " << _detectorGeometryVersion << std::endl;
  std::cout << "Magnetic field, Bz " << _bField[2] << " Tesla" << std::endl;
  std::cout << "MIP charge ACD counts in silicon sensor " << _MIP << std::endl;
  std::cout << "Number     sensor layers " << getNSensors() << std::endl;
  std::cout << "Number X   sensor layers " << getNXSensors() << std::endl;
  std::cout << "Number SAS sensor layers " << getNSASSensors() << std::endl;
  std::cout << "Number Z   sensor layers " << getNZSensors() << std::endl;

  for (int ii_layer = 0; ii_layer < getNSensors(); ++ii_layer){
    std::cout << std::endl;
    std::cout << "Sensor layer " << ii_layer << std::endl;
    std::cout << "Sensor type 0: X, 1, SAS, 2 Z " << _sensors[ii_layer]._type <<std::endl;
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

  if (layer >= 0 && layer < getNSensors()) return _sensors[layer];
  if (layer==-2) return _primaryVertexX;
  if (layer==-1) return _primaryVertexZ;
  std::cout << "layer " << layer << std::endl;
  throw Exception(" DetectorGeometry::getSensor: Out of bounds layer");  

}
