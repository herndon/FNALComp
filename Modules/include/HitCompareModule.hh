#ifndef Modules_HitCompareModule_hh
#define Modules_HitCompareModule_hh
//============================================================================
// HitCompareModule.hh
// Module for comparing reconstructing hit position to generator hit positions
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-12
//============================================================================

#include "Framework/include/Module.hh"

class TH1D;


namespace fc {

class Hit;
class GenHit;
class HitSet;
class GenHitSet;
class DetectorGeometry;


///
/// Class HitCompareModule  Module for comparing reconstructing hit position to generator hit positions
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-12
///

class HitCompareModule : public Module {

public:

    HitCompareModule(int, const std::string& iInputGenHitsLabel,
                     const std::string& iInputRecHitsLabel,
                     const DetectorGeometry & );

    void processEvent(fc::Event&) override;

private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    const std::string _genHitsLabel;
    const std::string _recHitsLabel;

    // Comparison of hits

    void compareHits(const GenHitSet &, const HitSet &) const;

    double compareHitPositions(const GenHit &, const Hit &) const;

    // Histograms
    // ROOT will delete these when the TFile to which the are attached is deleted
    std::vector<TH1D *> _hDeltaHitPositions;
    std::vector<TH1D *> _hDeltaBadHitPositions;

    TH1D * _hHitResults;

    void initializeHistograms();


};
} // end namespace fc

#endif // Modules_HitCompareModule_hh
