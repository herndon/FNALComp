#ifndef Modules_TrackSeedCompareModule_hh
#define Modules_TrackSeedCompareModule_hh
//============================================================================
// TrackSeedCompareModule.hh
// Module for comparing reconstructed track parameters to generated track parameters
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-08-06
//============================================================================

#include "Framework/include/Module.hh"
#include "TVectorD.h"
#include <fstream>
#include <iostream>

class TH1D;


namespace fc {

class DetectorGeometry;
class GenTrack;
class GenTrackSet;
class Track;
class TrackSet;

///
/// Class TrackSeedCompareModule  Module for comparing reconstructed track parameters to generated track parameters
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-00-06
///

class TrackSeedCompareModule : public Module {

public:

  TrackSeedCompareModule(int debugLevel,  std::ofstream & debugfile,
                       const std::string& inputPerfectTracksLabel,
                       const std::string& inputSeedTracksLabel,
                       const DetectorGeometry & detectorGeometry );

    void processEvent(fc::Event&) override;

    void endJob() override;


private:

    int _debugLevel;
  std::ofstream & _debugfile;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    const std::string _perfectTracksLabel;
    const std::string _seedTracksLabel;

    // Comparison of hits

    void compareTrackSeeds(const TrackSet &, const TrackSet &) const;
    const std::vector<int> matchTrackSeeds(const Track &,const TrackSet &) const;
    bool matchTrackSeed(const Track &, const Track&) const; 
    void fillHistograms(const TVectorD &,const Track&) const;

    // Histograms
    // ROOT will delete these when the TFile to which they are attached is deleted

    TH1D * _hDR = nullptr;
    TH1D * _hPhi0 = nullptr;
    TH1D * _hKappa = nullptr;
    TH1D * _hDZ = nullptr;
    TH1D * _hTanL = nullptr;

    TH1D * _hSigmaDr = nullptr;
    TH1D * _hSigmaPhi0 = nullptr;
    TH1D * _hSigmaKappa = nullptr;
    TH1D * _hSigmaDz = nullptr;
    TH1D * _hSigmaTanL = nullptr;



    TH1D * _hDeltaDr = nullptr;
    TH1D * _hDeltaPhi0 = nullptr;
    TH1D * _hDeltaKappa = nullptr;
    TH1D * _hDeltaDz = nullptr;
    TH1D * _hDeltaTanL = nullptr;

    TH1D * _hDeltaDrPull = nullptr;
    TH1D * _hDeltaPhi0Pull = nullptr;
    TH1D * _hDeltaKappaPull = nullptr;
    TH1D * _hDeltaDzPull = nullptr;
    TH1D * _hDeltaTanLPull = nullptr;

    TH1D * _hMatchedTrackSeeds = nullptr;

    void initializeHistograms();

};
} // end namespace fc

#endif // Modules_TrackSeedCompareModule_hh
