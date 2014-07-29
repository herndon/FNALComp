#ifndef Modules_TrackGenModule_hh
#define Modules_TrackGenModule_hh
//============================================================================
// TrackGenModule.hh
// Module for generating tracks 
// Track data is stored in bianary format
// See  dataFormat.pdf for data format information
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================

#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include "Framework/include/Module.hh"

///
/// Class TrackGenModule: Module for generating tracks
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

namespace fc {

class GenTrack;
class GenTrackSet;
class Random;

class TrackGenModule : public Module {

public:

  TrackGenModule(int debugLevel, int numberOfTracks,
		 const std::string& iTracksLabel, //label used for the generated tracks
		 const DetectorGeometry &,Random &);

  void processEvent(Event& ) override;


private:

  int _debugLevel;
  int _numberOfTracks;

  const std::string _tracksLabel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  // Random numbers
  Random &  _random;

  GenTrack generateTrack() const;

};

} // end namespace fc

#endif // Modules_TrackGenModule_hh
