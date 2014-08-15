#ifndef Modules_TrackCompareWithGenModule_hh
#define Modules_TrackCompareWithGenModule_hh
//============================================================================
// TrackCompareWithGenModule.hh
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
/// Class TrackCompareWithGenModule  Module for comparing reconstructed track parameters to generated track parameters
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-07-15
///

class TrackCompareWithGenModule : public Module {

public:

    TrackCompareWithGenModule(int debugLevel,
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

  // counter for efficiency measurments

  int _genTracks;
  int _recoTracks;
  int _matchedRecoTracks;
  int _matchedRecoTracksXY;

    // Histograms
    // ROOT will delete these when the TFile to which they are attached is deleted

    TH1F * dR = nullptr;
    TH1F * phi0 = nullptr;
    TH1F * kappa = nullptr;
    TH1F * dZ = nullptr;
    TH1F * tanL = nullptr;

    TH1F * sigmaDr = nullptr;
    TH1F * sigmaPhi0 = nullptr;
    TH1F * sigmaKappa = nullptr;
    TH1F * sigmaDz = nullptr;
    TH1F * sigmaTanL = nullptr;

    TH1F * pT = nullptr;
    TH1F * chi2 = nullptr;
    TH1F * nDof = nullptr;
    TH1F * prob = nullptr;

    TH1F * deltaDr = nullptr;
    TH1F * deltaPhi0 = nullptr;
    TH1F * deltaKappa = nullptr;
    TH1F * deltaDz = nullptr;
    TH1F * deltaTanL = nullptr;

    TH1F * deltaDrPull = nullptr;
    TH1F * deltaPhi0Pull = nullptr;
    TH1F * deltaKappaPull = nullptr;
    TH1F * deltaDzPull = nullptr;
    TH1F * deltaTanLPull = nullptr;

    TH1F * badDR = nullptr;
    TH1F * badPhi0 = nullptr;
    TH1F * badKappa = nullptr;
    TH1F * badDZ = nullptr;
    TH1F * badTanL = nullptr;

    TH1F * badSigmaDr = nullptr;
    TH1F * badSigmaPhi0 = nullptr;
    TH1F * badSigmaKappa = nullptr;
    TH1F * badSigmaDz = nullptr;
    TH1F * badSigmaTanL = nullptr;

    TH1F * badPT = nullptr;
    TH1F * badChi2 = nullptr;
    TH1F * badNDof = nullptr;
    TH1F * badProb = nullptr;

    TH1F * badDeltaDr = nullptr;
    TH1F * badDeltaPhi0 = nullptr;
    TH1F * badDeltaKappa = nullptr;
    TH1F * badDeltaDz = nullptr;
    TH1F * badDeltaTanL = nullptr;

    TH1F * badDeltaDrPull = nullptr;
    TH1F * badDeltaPhi0Pull = nullptr;
    TH1F * badDeltaKappaPull = nullptr;
    TH1F * badDeltaDzPull = nullptr;
    TH1F * badDeltaTanLPull = nullptr;

    void initializeHistograms();

    void compareTracks(const GenTrackSet &, const TrackSet &);

  void fillHistograms(const TVectorD &,const Track&,bool matched) const;



};
} // end namespace fc

#endif // Modules_TrackCompareWithGenModule_hh
