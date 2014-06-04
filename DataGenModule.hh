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

  DataGenModule(int,DetectorGeometry &,StripSet &,std::ofstream&);
  ~DataGenModule() {};

  void event(void);


private:

  int _debugLevel;

  // Generator Output event data file
  std::ofstream * _genoutputeventdatafile;

  // Detector information
  DetectorGeometry * _myDetectorGeometry;
  StripSet * _myStripSet;
  int _nLayers;
  double _curvatureC;

  // Events
  int _event;

  // Track information
  TGeoHelix* _helix = NULL;


  void generateTrack();
  void calculateHitPositions();


  void writeStripSetEvent(std::ofstream &, int); //!< Read int event from std::ofstream file

};

#endif // DataGenModule_hh
