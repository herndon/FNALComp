#ifndef DataSource_hh
#define DataSource_hh
//============================================================================
// DataSource.hh
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
#include "Framework/include/Source.hh"

namespace fc {

class TrackSet;
class HitSet;
class StripSet;
class Event;
class DetectorGeometry;

///
/// Class DataSource
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-06
///

class DataSource : public Source {

public:

  DataSource(int, std::ifstream&, bool genData,
	       const std::string& iOutputTracksLabel,
	       const std::string& iOutputHitsLabel,
	       const std::string& iOutputStripsLabel,
	       const DetectorGeometry&);
 
  ~DataSource() {};

  std::unique_ptr<Event> getNextEvent() override;


private:

  std::string const _outTracksLabel;
  std::string const _outHitsLabel;
  std::string const _outStripsLabel;
  int _debugLevel;

  const DetectorGeometry & _detectorGeometry;
 
    // input event data file
    std::ifstream & _inputeventdatafile;
    const bool _genData;

};
} // end namespace fc

#endif // DataSource_hh
