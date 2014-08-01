#ifndef Modules_GenDataHistogrammingModule_hh
#define Modules_GenDataHistogrammingModule_hh
//============================================================================
// GenDataHistogrammingModule.hh
// Module used to fill histograms from generated Event data objects
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-08-01
//============================================================================

#include <string>
#include "Framework/include/Module.hh"

class TH1D;

///
/// Class GenDataHistogrammingModule: Module for filling histograms from generated Event data objects
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-08-01
///

namespace fc {

class GenDataHistogrammingModule : public Module {

public:

  GenDataHistogrammingModule(const std::string& inputGenTrackSetLabel, //label used to GenTrackSet
			  const std::string& inputHitSetLabel, //label used to get HitSet
			  const std::string& inputStripSetLabel //label used to get StripSet
			  );

  void processEvent(Event& ) override;


private:

  const std::string _tracksLabel;
  const std::string _hitSetLabel;
  const std::string _stripSetLabel;

  //ROOT will delete this when the TFile to which it is attached is deleted

  TH1D * _hDR;
  TH1D * _hPhi0;
  TH1D * _hKappa;
  TH1D * _hDZ;
  TH1D * _hTanL;
  TH1D * _hPT;
  TH1D * _hPZ;

  TH1D* _hNHits;

  TH1D* _hNStripsPerLayer;

  void initializeHistograms();


};

} // end namespace fc

#endif // Modules_GenDataHistogrammingModule_hh
