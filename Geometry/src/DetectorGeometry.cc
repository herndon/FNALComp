#include <iostream>
#include <cmath>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"

fc::DetectorGeometry::DetectorGeometry(int detectorGeometryVersion,
                                       int nXSensors, int nSASSensors, int nZSensors,
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
    _primaryVertexZ(primaryVertexZ) {
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

    int iiLayer = 0;
    for (auto const& sensor: getSensors()){
        out << std::endl;
        out << "Sensor layer " << iiLayer << std::endl;
        out << "Sensor type 0: X, 1, SAS, 2 Z " << sensor._type <<std::endl;
        out << "N strips    "  << sensor._nStrips    << std::endl;
        out << "Strip pitch "  << sensor._stripPitch << std::endl;
        out << "X position (m) "  << sensor._center.x()       << std::endl;
        out << "Y position (m) "  << sensor._center.y()       << std::endl;
        out << "Z position (m) "  << sensor._center.z()       << std::endl;
        out << "Measurement direction  "  << std::endl;
        sensor._measurementDirection.Print();
        out << "Normal  "  << std::endl;
        sensor._normal.Print();
        out << "Intrinsic Hit Resolution (m) "  <<
            sensor._intrinsicHitResolution << std::endl;
        out << "Hit Resolution           (m) "  << sensor._hitResolution <<
            std::endl;
        out << "Bad Hit Resolution       (m) "  << sensor._badHitResolution
            << std::endl;
        out << "Strip Threshold          (m) "  << sensor._threshold <<
            std::endl;
        out << "Sensor dimentions        (m)X(m)  "  <<
            sensor._stripPitch*sensor._nStrips
            << " X " << sensor._perpSize  << std::endl;
	iiLayer++;
    }
    out << "PV position " << std::endl;
    out << "X position    (m) "  << _primaryVertexX._center.x()       << std::endl;
    out << "Y position    (m) "  << _primaryVertexX._center.y()       << std::endl;
    out << "Z position    (m) "  << _primaryVertexX._center.z()       << std::endl;
    out << "PV Resolution (m) "  << _primaryVertexX._hitResolution    << std::endl;



}

const fc::SensorDescriptor& fc::DetectorGeometry::getSensor(int layer) const {

    if (layer >= 0 && layer < getNSensors()) return _sensors[layer];
    if (layer==-2) return _primaryVertexX;
    if (layer==-1) return _primaryVertexZ;
    throw Exception(" DetectorGeometry::getSensor: Out of bounds layer");

}
