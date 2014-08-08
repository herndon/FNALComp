#ifndef Modules_TrackCompareWithPerfectModule_hh
#define Modules_TrackCompareWithPerfectModule_hh
//============================================================================
// TrackCompareWithPerfectModule.hh
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
/// Class TrackCompareWithPerfectModule  Module for comparing reconstructed track parameters to generated track parameters
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-07-15
///

class TrackCompareWithPerfectModule : public Module {

public:

    TrackCompareWithPerfectModule(int debugLevel,
                       const std::string& inputGenTracksLabel,
                       const std::string& inputRecTracksLabel,
                       const DetectorGeometry & detectorGeometry );

    void processEvent(fc::Event&) override;

    void endJob() override;


private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    const std::string _perfectTracksLabel;
    const std::string _recTracksLabel;

    // Comparison of hits

    void compareTracks(const TrackSet &, const TrackSet &);
  const Track & matchTrack(const Track &,const TrackSet &, bool& goodMatch) const;
    double deltaTracks(const Track &, const Track &) const;
  bool goodMatch(const Track & perfectTrack,const Track& recoTrack) const;
    TVectorD deltaHP(const Track &, const Track &) const;
    void fillHistograms(const TVectorD &,const Track&) const;

    // Histograms
    // ROOT will delete these when the TFile to which they are attached is deleted

    TH1F * dR;
    TH1F * phi0;
    TH1F * kappa;
    TH1F * dZ;
    TH1F * tanL;

    TH1F * sigmaDr;
    TH1F * sigmaPhi0;
    TH1F * sigmaKappa;
    TH1F * sigmaDz;
    TH1F * sigmaTanL;


    TH1F * pT;
    TH1F * chi2;
    TH1F * nDof;
    TH1F * prob;

    TH1F * deltaDr;
    TH1F * deltaPhi0;
    TH1F * deltaKappa;
    TH1F * deltaDz;
    TH1F * deltaTanL;

  // counter for efficiency measurments
  int _perfectTracks;
  int _recoTracks;
  int _matchedRecoTracks;
  
    void initializeHistograms();

};
} // end namespace fc

#endif // Modules_TrackCompareWithPerfectModule_hh
