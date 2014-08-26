#ifndef Geometry_DetectorGeometry_hh
#define Geometry_DetectorGeometry_hh
//============================================================================
// DetectorGeometry.hh
// header with class definition of a N layer planor silicon detector
// Includes global detector information
// and detailed sensor information SensorDescriptor structs
// For more information see <a href="doc/notes/detectorGeometry.pdf">detectorGeometry.pdf</a>
//
// Units: calculations are set up so that distance m, Energy GeV, magnetic field Teslaq
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-05-01
//============================================================================
#include <vector>
#include "TVector3.h"

namespace fc {

static const int _mDim = 1; //!< Measurement dimention of hits

  class Sensor{

  public:

    Sensor(){};

    Sensor(unsigned int type,
	   unsigned int nStrips,
	   double stripPitch,
	   double intrinsicHitResolution,
	   double hitResolution,
	   double badHitResolution,
	   double hitEfficiency,
	   double threshold,
	   TVector3 center,
	   TVector3 normal,
	   TVector3 measurementDirection,
	   double perpSize):
      _type(type),
      _nStrips(nStrips),
      _stripPitch(stripPitch),
      _intrinsicHitResolution(intrinsicHitResolution),
      _hitResolution(hitResolution),
      _badHitResolution(badHitResolution),
      _hitEfficiency(hitEfficiency),
      _threshold(threshold),
      _center(center),
      _normal(normal),
      _measurementDirection(measurementDirection),
      _perpSize(perpSize){
    };

    unsigned int type() const {return _type;}
    unsigned int nStrips() const {return _nStrips;} 
    double stripPitch() const {return _stripPitch;}
    double intrinsicHitResolution() const {return _intrinsicHitResolution;}
    double hitResolution() const {return _hitResolution;}
    double badHitResolution() const {return _badHitResolution;}
    double hitEfficiency() const {return _hitEfficiency;}
    double threshold() const {return _threshold;}
    const TVector3 & center() const {return _center;}
    const TVector3 & normal() const {return _normal;}
    const TVector3 & measurementDirection() const {return _measurementDirection;}
    const TVector3 perpDirection() const {return _normal.Cross(_measurementDirection);}
    double perpSize() const {return _perpSize;}
    double measurementSize() const {return _nStrips*_stripPitch;}

  private:

    unsigned int _type;// types 0: X, 1, SAS, 2, Z, 3 vertex
    unsigned int _nStrips;
    double _stripPitch;
    double _intrinsicHitResolution;
    double _hitResolution;
    double _badHitResolution;
    double _hitEfficiency;
    double _threshold;
    TVector3 _center;
    TVector3 _normal;
    TVector3 _measurementDirection;
    double _perpSize;


  };

typedef std::vector<Sensor> SensorContainer;



///
/// Class DetectorGeometry
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-05-01
/// For more information see <a href="../notes/detectorGeometry.pdf">detectorGeometry.pdf</a>
///

class DetectorGeometry {


public:


    DetectorGeometry(int detectorGeometryVersion,int nXSensors, int nSASSensors,
                     int nZSensors,
                     const TVector3& bField,double MIP,double curvatureC,int maxNumberStrips,
                     const std::vector<Sensor>& sensors,
                     const Sensor& primaryVertexX,const Sensor& primaryVertexZ);

  const std::vector<Sensor>& sensors() const {return _sensors;}
    const Sensor& sensor(int nsensor) const; //!< Returns struct describing sensor number nsensor
    int detectorGeometryVersion() const {
        return _detectorGeometryVersion;
    }
    int nSensors() const {
        return _sensors.size();
    }
    int nXSensors() const {
        return _nXSensors;
    }
    int nSASSensors() const {
        return _nSASSensors;
    }
    int nZSensors() const {
        return _nZSensors;
    }
    double zBField() const {
        return _bField.Z();
    }
    const TVector3 & bField() const {
        return _bField;
    }
    double MIP() const {
        return _MIP;
    }
    double curvatureCInField(TVector3 _bField) const {
        return _curvatureC*_bField.Z();
    }
    double curvatureC() const {
        return _curvatureC;
    }


    void printDetectorGeometry(ostream& out) const;





private:

    // Use default geometry or intialize geometry from a run time file and version
    int _detectorGeometryVersion;

    // Numerology
    int _nXSensors;
    int _nSASSensors;
    int _nZSensors;


    TVector3 _bField;

    // Useful constants
    double _MIP;
    double _curvatureC;
    int _maxNumberStrips;


    std::vector<Sensor> _sensors;

    // Parameters for the primary vetex if used in a fit

    Sensor _primaryVertexX;
    Sensor _primaryVertexZ;

};
} // end namespace fc

#endif // Geometry_DetectorGeometry_hh
