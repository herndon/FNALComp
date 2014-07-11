#ifndef DataInputModule_hh
#define DataInputModule_hh
//============================================================================
// DataInputModule.hh
// Module for reading raw detector data from the
// sensors of a 5 layer planor silicon detector.
// See detectorGeometry.pdf for detector description
// Strip data is stored in bianary format
// See  dataFormat.pdf for data format information
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-03
//============================================================================

#include <fstream>
#include <iostream>
#include "DetectorGeometry.hh"
#include "Module.hh"

class TrackSet;
class HitSet;
class StripSet;

namespace fc {

///
/// Class DataInputModule
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-06
///

  class DataInputModule : public Module{

public:

  DataInputModule(int, const DetectorGeometry &, std::ifstream&);
  ~DataInputModule() {};

  void processEvent(Event&) override;


private:

  int _debugLevel;

  const DetectorGeometry & _detectorGeometry;
 
    // input event data file
    std::ifstream & _inputeventdatafile;

};
} // end namespace fc

#endif // DataInputModule_hh
