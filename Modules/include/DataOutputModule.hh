#ifndef DataOutputModule_hh
#define DataOutputModule_hh
//============================================================================
// DataOutputModule.hh
// Module for writing raw detector data and reconstructed objet from the
// sensors of a 5 layer planor silicon detector.
// Raw strip data is stored in bianary format
// See  dataFormat.pdf for data format information
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-03
//============================================================================

#include <fstream>
#include <iostream>
#include "Module.hh"

class TrackSet;
class HitSet;
class StripSet;

namespace fc {

///
/// Class DataOutputModule
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-06
///

class DataOutputModule : public Module {

public:

  DataOutputModule(int,const DetectorGeometry&,std::ofstream&);
  ~DataOutputModule() {};

  void processEvent(Event&) override;

private:

  int _debugLevel;

  const DetectorGeometry & _detectorGeometry;

  // Input event data file
  std::ofstream & _outputeventdatafile;

};
} // end namespace fc


#endif // DataOutputModule_hh
