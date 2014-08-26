#include <iostream>
#include <cmath>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"

fc::DetectorGeometry::DetectorGeometry(int detectorGeometryVersion,
                                       int nXSensors, int nSASSensors, int nZSensors,
                                       const TVector3& bField,double MIP,double curvatureC,int maxNumberStrips,
                                       const std::vector<Sensor>& sensors,
                                       const Sensor& primaryVertexX,const Sensor& primaryVertexZ):
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
    out << "Detector Geometry version: " << detectorGeometryVersion() << std::endl;
    out << "Magnetic field, Bz " << zBField() << " Tesla" << std::endl;
    out << "MIP charge ACD counts in silicon sensor " << MIP() << std::endl;
    out << "Number     sensor layers " << nSensors() << std::endl;
    out << "Number X   sensor layers " << nXSensors() << std::endl;
    out << "Number SAS sensor layers " << nSASSensors() << std::endl;
    out << "Number Z   sensor layers " << nZSensors() << std::endl;

    int iiLayer = 0;
    for (auto const& sensor: sensors()){
        out << std::endl;
        out << "Sensor layer " << iiLayer << std::endl;
        out << "Sensor type 0: X, 1, SAS, 2 Z " << sensor.type() <<std::endl;
        out << "N strips    "  << sensor.nStrips()    << std::endl;
        out << "Strip pitch "  << sensor.stripPitch() << std::endl;
        out << "X position (m) "  << sensor.center().x()       << std::endl;
        out << "Y position (m) "  << sensor.center().y()       << std::endl;
        out << "Z position (m) "  << sensor.center().z()       << std::endl;
        out << "Measurement direction  "  << std::endl;
        out << "X "  << sensor.measurementDirection().x()       << std::endl;
        out << "Y "  << sensor.measurementDirection().y()       << std::endl;
        out << "Z "  << sensor.measurementDirection().z()       << std::endl;
        out << "Normal  "  << std::endl;
       out << "X "  << sensor.normal().x()       << std::endl;
        out << "Y "  << sensor.normal().y()       << std::endl;
        out << "Z "  << sensor.normal().z()       << std::endl;
 
        out << "Intrinsic Hit Resolution (m) "  <<
	  sensor.intrinsicHitResolution() << std::endl;
        out << "Hit Resolution           (m) "  << sensor.hitResolution() <<
            std::endl;
        out << "Bad Hit Resolution       (m) "  << sensor.badHitResolution()
            << std::endl;
        out << "Strip Threshold          (m) "  << sensor.threshold() <<
            std::endl;
        out << "Sensor dimentions        (m)X(m)  "  <<
	  sensor.measurementSize()
            << " X " << sensor.perpSize()  << std::endl;
	iiLayer++;
    }
    out << "PV position " << std::endl;
    out << "X position    (m) "  << _primaryVertexX.center().x()       << std::endl;
    out << "Y position    (m) "  << _primaryVertexX.center().y()       << std::endl;
    out << "Z position    (m) "  << _primaryVertexX.center().z()       << std::endl;
    out << "PV Resolution (m) "  << _primaryVertexX.hitResolution()    << std::endl;



}

const fc::Sensor& fc::DetectorGeometry::sensor(int layer) const {

    if (layer >= 0 && layer < nSensors()) return _sensors[layer];
    if (layer==-2) return _primaryVertexX;
    if (layer==-1) return _primaryVertexZ;
    throw Exception(" DetectorGeometry::sensor: Out of bounds layer");

}
