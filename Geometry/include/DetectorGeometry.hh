#ifndef DetectorGeometry_hh
#define DetectorGeometry_hh
//============================================================================
// DetectorGeometry.hh
// header with class definition of a N layer planor silicon detector
// Includes global detector information 
// and detailed sensor information sensorDescriptor structs
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

struct sensorDescriptor {
  int _type;// types 0: X, 1, SAS, 2, Z
  int _nStrips;
  double _stripPitch;
  double _intrinsicHitResolution;
  double _hitResolution;
  double _badHitResolution;
  double _threshold;
  TVector3 _center;
  TVector3 _normal;
  TVector3 _measurementDirection;
  double _perpSize;
};

///
/// Class DetectorGeometry
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-05-01
/// For more information see <a href="../notes/detectorGeometry.pdf">detectorGeometry.pdf</a>
///

class DetectorGeometry {


public:


  DetectorGeometry(int detectorGeometryVersion,int nXSensors, int nSASSensors, int nZSensors, 
		   const TVector3& bField,double MIP,double curvatureC,int maxNumberStrips,
		   const std::vector<sensorDescriptor>& sensors,
		   const sensorDescriptor& primaryVertexX,const sensorDescriptor& primaryVertexZ);

 ~DetectorGeometry() {};

  const sensorDescriptor& getSensor(int nsensor) const; //!< Returns struct describing sensor number nsensor
  int getDetectorGeometryVersion(void) const {return _detectorGeometryVersion;}
  int getNSensors(void) const {return _sensors.size();}
  int getNXSensors(void) const {return _nXSensors;}
  int getNSASSensors(void) const {return _nSASSensors;}
  int getNZSensors(void) const {return _nZSensors;}
  double getZBField(void) const {return _bField.Z();}
  const TVector3 & getBField(void) const {return _bField;}
  double getMIP(void) const {return _MIP;}
  double getCurvatureC(void) const {return _curvatureC;}

  void printDetectorGeometry(void) const;



  static const int _mDim = 1; //!< Measurement dimention of hits
 

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


  std::vector<sensorDescriptor> _sensors;

  // Parameters for the primary vetex if used in a fit

  sensorDescriptor _primaryVertexX;
  sensorDescriptor _primaryVertexZ;

};
} // end namespace fc

#endif // DetectorGeometry_hh
