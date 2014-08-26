#define _USE_MATH_DEFINES
#include<cmath>
#include<limits>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/BuildDetectorGeometry.hh"

const fc::DetectorGeometry fc::buildDetectorGeometry(std::ifstream &
        detectorgeometryfile) {

    unsigned int maxNumberStrips = 4096;

    if (!detectorgeometryfile) {
        throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: can't open sensorgeometry.txt file");
    }

    std::string detectorGeometryString;
    int detectorGeometryVersion;

    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString == "DetectorGeometryVersion") {
        detectorgeometryfile >> detectorGeometryVersion;
    } else {
        throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, version");
    }


    // Only Z oriented B field allowed
    TVector3 bField;

    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString == "zBField") {
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
    if (detectorGeometryString == "MIP") {
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
    if (detectorGeometryString == "NumberSensors") {
        detectorgeometryfile >> numberSensors;
    } else {
        throw Exception("BuildDetectorGeometry::initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, NumberSensors");
    }


    detectorgeometryfile.precision(std::numeric_limits<double>::digits10 + 2);

    // Read numberSensors sensors
    std::vector<Sensor> sensors;
    sensors.reserve(numberSensors);

    unsigned int type;// types 0: X, 1, SAS, 2, Z, 3 vertex
    unsigned int nStrips;
    double stripPitch;
    double intrinsicHitResolution;
    double hitResolution;
    double badHitResolution;
    double hitEfficiency;
    double threshold;
    TVector3 center;
    TVector3 normal;
    TVector3 measurementDirection;
    double perpSize;



    for (int iiLayer = 0; iiLayer < numberSensors; ++iiLayer) {
        detectorgeometryfile >> detectorGeometryString;
        if (detectorGeometryString != "Sensor") throw
            Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, Sensor");

        detectorgeometryfile >> sensorNumber;

        detectorgeometryfile >> detectorGeometryString;
        if (detectorGeometryString == "X") {
            type = 0;
            ++nXSensors;
        } else if (detectorGeometryString == "SAS") {
            type = 1;
            ++nSASSensors;
        } else if (detectorGeometryString == "Z") {
            type = 2;
            ++nZSensors;
        } else {
           throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Unrecognized sensor type, types are X, SAS, and Z");
 	}


        detectorgeometryfile >> nStrips;
        detectorgeometryfile >> stripPitch;
        detectorgeometryfile >> intrinsicHitResolution;
        detectorgeometryfile >> hitResolution;
        detectorgeometryfile >> badHitResolution;
        // !!!!! This is a good place for a test case using valgrind
        detectorgeometryfile >> threshold;
        detectorgeometryfile >> hitEfficiency;
        detectorgeometryfile >> center[0];
        detectorgeometryfile >> center[1];
        detectorgeometryfile >> center[2];
        detectorgeometryfile >> normal[0];
        detectorgeometryfile >> normal[1];
        detectorgeometryfile >> normal[2];
        detectorgeometryfile >> measurementDirection[0];
        detectorgeometryfile >> measurementDirection[1];
        detectorgeometryfile >> measurementDirection[2];
        detectorgeometryfile >> perpSize;

        normal *= 1.0/normal.Mag();
        measurementDirection *= 1.0/measurementDirection.Mag();

        if ( nStrips > maxNumberStrips)
            throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Out of bounds sensor number specifications in sensorgeometry.txt, maximum number of strips is 4096");
        Sensor sensor(type,
		      nStrips,
		      stripPitch,
		      intrinsicHitResolution,
		      hitResolution,
		      badHitResolution,
		      hitEfficiency,
		      threshold,
		      center,
		      normal,
		      measurementDirection,
		      perpSize);

        sensors.push_back(std::move(sensor));

    }

 
    detectorgeometryfile >> detectorGeometryString;
    if (detectorGeometryString != "PVX")
        throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, PVX");
    type = 3;
    nStrips = 0;
    stripPitch = 0.0;
    detectorgeometryfile >> intrinsicHitResolution;
    hitResolution = intrinsicHitResolution;
    badHitResolution = intrinsicHitResolution;
    hitEfficiency = 0.0;
    threshold = 0;
    detectorgeometryfile >> center[0];
    detectorgeometryfile >> center[1];
    detectorgeometryfile >> center[2];
    detectorgeometryfile >> normal[0];
    detectorgeometryfile >> normal[1];
    detectorgeometryfile >> normal[2];
    detectorgeometryfile >> measurementDirection[0];
    detectorgeometryfile >> measurementDirection[1];
    detectorgeometryfile >> measurementDirection[2];

    normal *= 1.0/normal.Mag();
    measurementDirection *= 1.0/measurementDirection.Mag();

    Sensor primaryVertexX(type,
			  nStrips,
			  stripPitch,
			  intrinsicHitResolution,
			  hitResolution,
			  badHitResolution,
			  hitEfficiency,
			  threshold,
			  center,
			  normal,
			  measurementDirection,
			  perpSize);



    detectorgeometryfile >> detectorGeometryString;

    if (detectorGeometryString != "PVZ")
        throw Exception("BuildDetectorGeometry::_initBuildDetectorGeometryFromFile: Bad format in sensorgeometry.txt, PVY");
    type = 3;
    nStrips = 0;
    stripPitch = 0.0;
    detectorgeometryfile >> intrinsicHitResolution;
    hitResolution = intrinsicHitResolution;
    badHitResolution = intrinsicHitResolution;
    hitEfficiency = 0.0;
    threshold = 0;
    detectorgeometryfile >> center[0];
    detectorgeometryfile >> center[1];
    detectorgeometryfile >> center[2];
    detectorgeometryfile >> normal[0];
    detectorgeometryfile >> normal[1];
    detectorgeometryfile >> normal[2];
    detectorgeometryfile >> measurementDirection[0];
    detectorgeometryfile >> measurementDirection[1];
    detectorgeometryfile >> measurementDirection[2];

    normal *= 1.0/normal.Mag();
    measurementDirection *= 1.0/measurementDirection.Mag();

    Sensor primaryVertexZ(type,
			  nStrips,
			  stripPitch,
			  intrinsicHitResolution,
			  hitResolution,
			  badHitResolution,
			  hitEfficiency,
			  threshold,
			  center,
			  normal,
			  measurementDirection,
			  perpSize);


 
    DetectorGeometry detectorGeometry(detectorGeometryVersion,nXSensors,nSASSensors,
                                      nZSensors,
                                      bField,MIP,curvatureC,maxNumberStrips,
                                      sensors,primaryVertexX,primaryVertexZ);

    return detectorGeometry;

}


