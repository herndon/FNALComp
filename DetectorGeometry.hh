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

struct sensorDescriptor {
  int _nStrips;
  double _stripPitch;
  double _yPos;
  double _resolution;
};

///
/// Class DetectorGeometry
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-05-01
/// For more information see <a href="../notes/detectorGeometry.pdf">detectorGeometry.pdf</a>
///

class DetectorGeometry {

  friend class StripSet;

public:

 DetectorGeometry();
 DetectorGeometry(std::ifstream&);
 ~DetectorGeometry() {};

  const sensorDescriptor& getSensor(int nsensor); //!< Returns struct describing sensor number nsensor
  int getNSensors(void);
  double getZBField(void);

  void _initDetectorGeometryFromFile(std::ifstream&);

  void printDetectorGeometry(void);
  void printSensorLimits(void);

private:

  // Numerology for declaring arrays
  static const int _nSensors=5;

  double _zBField;

  // Use default geometry or intialize geometry from a run time file
  bool _defaultGeometry;

  // limits on sensor specifications
  sensorDescriptor _sensorMinLimits;
  sensorDescriptor _sensorMaxLimits;

  sensorDescriptor _sensor[_nSensors];

  void _initDetectorGeometry(void);

};


inline int DetectorGeometry::getNSensors(void) {
  return _nSensors;
}

inline double DetectorGeometry::getZBField(void) {
  return _zBField;
}

#endif // DetectorGeometry_hh
