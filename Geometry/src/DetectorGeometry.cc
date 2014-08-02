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

  for (int iiLayer = 0; iiLayer < getNSensors(); ++iiLayer){
    out << std::endl;
    out << "Sensor layer " << iiLayer << std::endl;
    out << "Sensor type 0: X, 1, SAS, 2 Z " << _sensors[iiLayer]._type <<std::endl;
    out << "N strips    "  << _sensors[iiLayer]._nStrips    << std::endl;
    out << "Strip pitch "  << _sensors[iiLayer]._stripPitch << std::endl;
    out << "X position (m) "  << _sensors[iiLayer]._center.x()       << std::endl;
    out << "Y position (m) "  << _sensors[iiLayer]._center.y()       << std::endl;
    out << "Z position (m) "  << _sensors[iiLayer]._center.z()       << std::endl;
    out << "Measurement direction  "  << std::endl;
    _sensors[iiLayer]._measurementDirection.Print();
    out << "Normal  "  << std::endl;
    _sensors[iiLayer]._normal.Print();
    out << "Intrinsic Hit Resolution (m) "  << _sensors[iiLayer]._intrinsicHitResolution << std::endl;
    out << "Hit Resolution           (m) "  << _sensors[iiLayer]._hitResolution << std::endl;
    out << "Bad Hit Resolution       (m) "  << _sensors[iiLayer]._badHitResolution << std::endl;
    out << "Strip Threshold          (m) "  << _sensors[iiLayer]._threshold << std::endl;
    out << "Sensor dimentions        (m)X(m)  "  << _sensors[iiLayer]._stripPitch*_sensors[iiLayer]._nStrips 
	      << " X " << _sensors[iiLayer]._perpSize  << std::endl;
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
