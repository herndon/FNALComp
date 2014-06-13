#ifndef DetectorGeometry_hh
#define DetectorGeometry_hh
//============================================================================
// DetectorGeometry.hh
// header with class definition of a 5 layer planor silicon detector
// Includes global detector information 
// and detailed sensor information sensorDescriptor structs
// For more information see <a href="doc/notes/detectorGeometry.pdf">detectorGeometry.pdf</a>
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-05-01
//============================================================================
#include <fstream>
#include <iostream>

namespace fc {

struct sensorDescriptor {
  int _nStrips;
  double _stripPitch;
  double _resolution;
  double _center[3];
  double _norm[3];
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
  double getZBField(void) const {return _bField[2];};
  const double * getBField(void) const {return _bField;};
  double getCurvatureC(void) const {return _curvatureC;};

  void _initDetectorGeometryFromFile(std::ifstream&);

  void printDetectorGeometry(void) const;
  void printSensorLimits(void) const;

private:

  // Use default geometry or intialize geometry from a run time file and version
  bool _defaultGeometry;
  int _detectorGeometryVersion;

  // Numerology for declaring arrays
  static const int _nSensors=5;

  sensorDescriptor _sensor[_nSensors];

  // limits on sensor specifications
  sensorDescriptor _sensorMinLimits;
  sensorDescriptor _sensorMaxLimits;

  // Useful constants
  double _bField[3];
  double _curvatureC;

  void _initSensorLimits(void);
  void _initDetectorGeometry(void);

};
} // end namespace fc

#endif // DetectorGeometry_hh
