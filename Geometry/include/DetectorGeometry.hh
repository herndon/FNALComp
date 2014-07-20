#ifndef DetectorGeometry_hh
#define DetectorGeometry_hh
//============================================================================
// DetectorGeometry.hh
// header with class definition of a 5 layer planor silicon detector
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
#include <fstream>
#include <iostream>
#include "TVector3.h"

namespace fc {

struct sensorDescriptor {
  int _nStrips;
  double _stripPitch;
  double _intrinsicResolution;
  double _hitResolution;
  double _threshold;
  TVector3 _center;
  TVector3 _normal;
  TVector3 _measurementDirection;
};

///
/// Class DetectorGeometry
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-05-01
/// For more information see <a href="../notes/detectorGeometry.pdf">detectorGeometry.pdf</a>
///

class DetectorGeometry {

  friend class StripSet;
  friend class HitSet;
  friend class HitCompareModule;

public:

 DetectorGeometry();
 DetectorGeometry(std::ifstream&);
 ~DetectorGeometry() {};

  const sensorDescriptor& getSensor(int nsensor) const; //!< Returns struct describing sensor number nsensor
  int getDetectorGeometryVersion(void) const {return _detectorGeometryVersion;};
  int getNSensors(void) const {return _nSensors;};
  int getNXSensors(void) const {return _nXSensors;};
  int getNZSensors(void) const {return _nZSensors;};
  double getZBField(void) const {return _bField.Z();};
  const TVector3 & getBField(void) const {return _bField;};
  double getMIP(void) const {return _MIP;};
  double getCurvatureC(void) const {return _curvatureC;};

  void printDetectorGeometry(void) const;
  void printSensorLimits(void) const;

  // Numerology for declaring arrays
  static const int _nSensors=10;
  static const int _nXSensors=5;
  static const int _nZSensors=5;


  static const int _mDim = 1; //!< Measurement dimention of hits
 

private:

  // Use default geometry or intialize geometry from a run time file and version
  int _detectorGeometryVersion;


  sensorDescriptor _sensor[_nSensors];

  // limits on sensor specifications
  sensorDescriptor _sensorMinLimits;
  sensorDescriptor _sensorMaxLimits;

  TVector3 _bField;
  double _MIP;
  // Useful constants
  double _curvatureC;

  void initSensorLimits(void);
  void initDetectorGeometry(void);
  void initDetectorGeometryFromFile(std::ifstream&);

};
} // end namespace fc

#endif // DetectorGeometry_hh
