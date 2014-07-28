#define _USE_MATH_DEFINES
#include<cmath>
#include<limits>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/BuildDetectorGeometry.hh"

const fc::DetectorGeometry fc::buildDetectorGeometry(std::ifstream & detectorgeometryfile) {

  int maxNumberStrips = 2048;

  if (!detectorgeometryfile){
    throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: can't open sensorgeometry.txt file");
  }

  std::string detectorGeometryString;
  int detectorGeometryVersion;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "DetectorGeometryVersion"){
    detectorgeometryfile >> detectorGeometryVersion;
  } else {
    throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, version");
  }


  // Only Z oriented B field allowed
  TVector3 bField;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "zBField"){
    // Only Z oriented B field allowed
    double zBField;
    bField.SetX(0.0);
    bField.SetY(0.0);
    detectorgeometryfile >> zBField;
    bField.SetZ(zBField);
  } else {
    throw Exception("BuildDetectorGeometry::initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, bField");
  }


  // Details of curvature calculation
  //using p = BqR, 
  //multiply by c to get energy in J  pc = BqRc, 
  //divide by e to get energy in eV pc = BqR3x108/1.6x10-19
  // q = Q1.6x10-19 where Q = 1, -1...
  //divide by 109 pc (in GeV) = BQR3x108/(1x10^9)
  // Also defines units  in meters
  double curvatureC = 2.99792458e8/1.0e9; // ~ 0.3;  


  // MIP charge
  double MIP;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "MIP"){
    detectorgeometryfile >> MIP;
  } else {
    throw Exception("BuildDetectorGeometry::initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, MIP");
  }

  int numberSensors;
  int sensorNumber;

  int nXSensors = 0;
  int nSASSensors = 0;
  int nZSensors = 0;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString == "NumberSensors"){
    detectorgeometryfile >> numberSensors;
  } else {
    throw Exception("BuildDetectorGeometry::initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, NumberSensors");
  }


  detectorgeometryfile.precision(std::numeric_limits<double>::digits10 + 2);

  // Read numberSensors sensors
  std::vector<SensorDescriptor> sensors;

  for (int ii_layer = 0; ii_layer < numberSensors; ++ii_layer){
    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString != "Sensor") throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, Sensor");

    detectorgeometryfile >> sensorNumber;

    SensorDescriptor sensor;

    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString != "X" && detectorGeometryString != "SAS" && detectorGeometryString != "Z")
      throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Unrecognized sensor type, types are X, SAS, and Z");
    if (detectorGeometryString == "X") {
      sensor._type = 0;
      ++nXSensors;
    }
    if (detectorGeometryString == "SAS") {
      sensor._type = 1;
      ++nSASSensors;
    }
    if (detectorGeometryString == "Z") {
      sensor._type = 2;
      ++nZSensors;
    }
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

    if ( sensor._nStrips > maxNumberStrips)  
      throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Out of bounds sensor number specifications in sensorgeometry.txt, maximum number of strips is 2048");   

    sensors.push_back(sensor);

  }

  SensorDescriptor primaryVertexX;

  detectorgeometryfile >> detectorGeometryString;
  if (detectorGeometryString != "PVX") 
    throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, PVX");
  detectorgeometryfile >> primaryVertexX._hitResolution;
  detectorgeometryfile >> primaryVertexX._center[0];
  detectorgeometryfile >> primaryVertexX._center[1];
  detectorgeometryfile >> primaryVertexX._center[2];
  detectorgeometryfile >> primaryVertexX._normal[0];
  detectorgeometryfile >> primaryVertexX._normal[1];
  detectorgeometryfile >> primaryVertexX._normal[2];
  detectorgeometryfile >> primaryVertexX._measurementDirection[0];
  detectorgeometryfile >> primaryVertexX._measurementDirection[1];
  detectorgeometryfile >> primaryVertexX._measurementDirection[2];

  primaryVertexX._normal *= 1.0/primaryVertexX._normal.Mag();
  primaryVertexX._measurementDirection *= 1.0/primaryVertexX._measurementDirection.Mag();

  detectorgeometryfile >> detectorGeometryString;

  SensorDescriptor primaryVertexZ;

  if (detectorGeometryString != "PVZ")
    throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, PVY");

  detectorgeometryfile >> primaryVertexZ._hitResolution;
  detectorgeometryfile >> primaryVertexZ._center[0];
  detectorgeometryfile >> primaryVertexZ._center[1];
  detectorgeometryfile >> primaryVertexZ._center[2];
  detectorgeometryfile >> primaryVertexZ._normal[0];
  detectorgeometryfile >> primaryVertexZ._normal[1];
  detectorgeometryfile >> primaryVertexZ._normal[2];
  detectorgeometryfile >> primaryVertexZ._measurementDirection[0];
  detectorgeometryfile >> primaryVertexZ._measurementDirection[1];
  detectorgeometryfile >> primaryVertexZ._measurementDirection[2];

  primaryVertexZ._normal *= 1.0/primaryVertexZ._normal.Mag();
  primaryVertexZ._measurementDirection *= 1.0/primaryVertexZ._measurementDirection.Mag();

  DetectorGeometry detectorGeometry(detectorGeometryVersion,nXSensors,nSASSensors,nZSensors,
				    bField,MIP,curvatureC,maxNumberStrips,
				    sensors,primaryVertexX,primaryVertexZ);

  return detectorGeometry;

}


