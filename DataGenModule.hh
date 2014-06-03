#ifndef DataGenModule_hh
#define DataGenModule_hh
//============================================================================
// DataGenModule.hh
// Module for generating tracks and associated hit and strip data on the 
// sensors of a 5 layer planor silicon detector.
// See detectorGeometry.pdf for detector description
// Strip data is stored in bianary format
// See  dataFormat.pdf for data format information
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-04-17
//============================================================================

#include <fstream>
#include <iostream>
#include "TGeoHelix.h"

class DetectorGeometry;
class StripSet;

///
/// Class DataGenModule
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

class DataGenModule {

public:

  DataGenModule() {};
  ~DataGenModule() {};

  void run(void);

private:

  int _debugLevel;


  // Detector information
  DetectorGeometry * _myDetectorGeometry;
  StripSet * _myStripSet;
  int _nLayers;
  double _curvatureC;

  // Track information
  TGeoHelix* _helix;

  void setDetectorGeometry(DetectorGeometry &);
  void setStripSet(StripSet &);

  void generateTrack();
  void calculateHitPositions();


  void writeStripSetEvent(std::ofstream &, int);

};

#endif // DataGenModule_hh
