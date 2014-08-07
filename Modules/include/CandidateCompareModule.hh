#ifndef Modules_CandidateCompareModule_hh
#define Modules_CandidateCompareModule_hh
//============================================================================
// CandidateCompareModule.hh
// Module for comparing reconstructed track parameters to generated track parameters
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-08-06
//============================================================================

#include "Framework/include/Module.hh"
#include "TVectorD.h"

class TH1D;


namespace fc {

class DetectorGeometry;
class GenTrack;
class GenTrackSet;
class Track;
class TrackSet;

///
/// Class CandidateCompareModule  Module for comparing reconstructed track parameters to generated track parameters
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-00-06
///

class CandidateCompareModule : public Module {

public:

    CandidateCompareModule(int debugLevel,
                       const std::string& inputPerfectTracksLabel,
                       const std::string& inputCandidateTracksLabel,
                       const DetectorGeometry & detectorGeometry );

    void processEvent(fc::Event&) override;

    void endJob() override;


private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    const std::string _perfectTracksLabel;
    const std::string _candidateTracksLabel;

    // Comparison of hits

    void compareCandidates(const TrackSet &, const TrackSet &) const;
    const std::vector<int> matchCandidates(const Track &,const TrackSet &) const;
    bool matchCandidate(const Track &, const Track&) const; 
    const TVectorD deltaHP(const Track &, const Track &) const;
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

    TH1D * _hMatchedCandidates = nullptr;

    void initializeHistograms();

};
} // end namespace fc

#endif // Modules_CandidateCompareModule_hh
