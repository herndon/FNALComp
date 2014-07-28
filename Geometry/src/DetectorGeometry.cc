#include <iostream>
#include <cmath>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"

fc::DetectorGeometry::DetectorGeometry(int detectorGeometryVersion,int nXSensors, int nSASSensors, int nZSensors,
				       const TVector3& bField,double MIP,double curvatureC,int maxNumberStrips,
				       const std::vector<SensorDescriptor>& sensors,
				       const SensorDescriptor& primaryVertexX,const SensorDescriptor& primaryVertexZ):
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

void fc::DetectorGeometry::printDetectorGeometry(ostream& out) const {

  out << "Detector Geometry information" << std::endl;
  out << "Detector Geometry version: " << _detectorGeometryVersion << std::endl;
  out << "Magnetic field, Bz " << _bField[2] << " Tesla" << std::endl;
  out << "MIP charge ACD counts in silicon sensor " << _MIP << std::endl;
  out << "Number     sensor layers " << getNSensors() << std::endl;
  out << "Number X   sensor layers " << getNXSensors() << std::endl;
  out << "Number SAS sensor layers " << getNSASSensors() << std::endl;
  out << "Number Z   sensor layers " << getNZSensors() << std::endl;

  for (int ii_layer = 0; ii_layer < getNSensors(); ++ii_layer){
    out << std::endl;
    out << "Sensor layer " << ii_layer << std::endl;
    out << "Sensor type 0: X, 1, SAS, 2 Z " << _sensors[ii_layer]._type <<std::endl;
    out << "N strips    "  << _sensors[ii_layer]._nStrips    << std::endl;
    out << "Strip pitch "  << _sensors[ii_layer]._stripPitch << std::endl;
    out << "X position (m) "  << _sensors[ii_layer]._center.x()       << std::endl;
    out << "Y position (m) "  << _sensors[ii_layer]._center.y()       << std::endl;
    out << "Z position (m) "  << _sensors[ii_layer]._center.z()       << std::endl;
    out << "Measurement direction  "  << std::endl;
    _sensors[ii_layer]._measurementDirection.Print();
    out << "Normal  "  << std::endl;
    _sensors[ii_layer]._normal.Print();
    out << "Intrinsic Hit Resolution (m) "  << _sensors[ii_layer]._intrinsicHitResolution << std::endl;
    out << "Hit Resolution           (m) "  << _sensors[ii_layer]._hitResolution << std::endl;
    out << "Bad Hit Resolution       (m) "  << _sensors[ii_layer]._badHitResolution << std::endl;
    out << "Strip Threshold          (m) "  << _sensors[ii_layer]._threshold << std::endl;
    out << "Sensor dimentions        (m)X(m)  "  << _sensors[ii_layer]._stripPitch*_sensors[ii_layer]._nStrips 
	      << " X " << _sensors[ii_layer]._perpSize  << std::endl;
  }
  out << "PV position " << std::endl;
    out << "X position    (m) "  << _primaryVertexX._center.x()       << std::endl;
    out << "Y position    (m) "  << _primaryVertexX._center.y()       << std::endl;
    out << "Z position    (m) "  << _primaryVertexX._center.z()       << std::endl;
    out << "PV Resolution (m) "  << _primaryVertexX._hitResolution    << std::endl;



}

const fc::SensorDescriptor& fc::DetectorGeometry::getSensor(int layer) const{

  if (layer >= 0 && layer < getNSensors()) return _sensors[layer];
  if (layer==-2) return _primaryVertexX;
  if (layer==-1) return _primaryVertexZ;
  throw Exception(" DetectorGeometry::getSensor: Out of bounds layer");  

}
