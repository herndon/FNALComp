#ifndef Modules_TrackCompareModule_hh
#define Modules_TrackCompareModule_hh
//============================================================================
// TrackCompareModule.hh
// Module for comparing reconstructed track parameters to generated track parameters
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-15
//============================================================================

#include "Framework/include/Module.hh"
#include "TVectorD.h"

class TH1F;


namespace fc {

class DetectorGeometry;
class GenTrack;
class GenTrackSet;
class Track;
class TrackSet;

///
/// Class TrackCompareModule  Module for comparing reconstructed track parameters to generated track parameters
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-07-15
///

class TrackCompareModule : public Module {

public:

  TrackCompareModule(int debugLevel, 
                     const std::string& inputGenTracksLabel, 
                     const std::string& inputRecTracksLabel,
                     const DetectorGeometry & detectorGeometry );

  void processEvent(fc::Event&) override;

  void endJob() override;


private:

  int _debugLevel;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  const std::string _genTracksLabel;
  const std::string _recTracksLabel;

  // Comparison of hits

  void compareTracks(const GenTrackSet &, const TrackSet &) const;
  const Track & matchTrack(const GenTrack &,const TrackSet &) const;
  double deltaTracks(const GenTrack &, const Track &) const;
  TVectorD deltaHP(const GenTrack &, const Track &) const;
  void fillHistograms(const TVectorD &,const Track&) const;

  // Histograms
  // ROOT will delete these when the TFile to which they are attached is deleted

  TH1F * deltaD0;
  TH1F * deltaPhi0;
  TH1F * deltaKappa;
  TH1F * deltaZ0;
  TH1F * deltaTanL;

  TH1F * deltaD0Pull;
  TH1F * deltaPhi0Pull;
  TH1F * deltaKappaPull;
  TH1F * deltaZ0Pull;
  TH1F * deltaTanLPull;

  void initializeHistograms();

};
} // end namespace fc

#endif // Modules_TrackCompareModule_hh
