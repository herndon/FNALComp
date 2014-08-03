#ifndef Modules_Day0HistogrammingModule_hh
#define Modules_Day0HistogrammingModule_hh
//============================================================================
// Day0HistogrammingModule.hh
// Module used to fill histograms from Event data
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================

#include <string>
#include "Framework/include/Module.hh"

class TH1D;

///
/// Class Day0HistogrammingModule: Module for filling histograms from Event data
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

namespace fc {

class Day0HistogrammingModule : public Module {

public:

    Day0HistogrammingModule(const std::string&
                            inputGenTrackSetLabel, //label used to GenTrackSet
                            const std::string& inputHitSetLabel, //label used to get HitSet
                            const std::string& inputStripSetLabel //label used to get StripSet
                           );

    void processEvent(Event& ) override;


private:

    const std::string _tracksLabel;
    const std::string _hitSetLabel;
    const std::string _stripSetLabel;

    //ROOT will delete this when the TFile to which it is attached is deleted
    TH1D* _hPt;

};

} // end namespace fc

#endif // Modules_Day0HistogrammingModule_hh
