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
class TH2D;
class TDirectory;

///
/// Class Day0HistogrammingModule: Module for filling histograms from Event data
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-04-17
///

namespace fc {

class Day0HistogrammingModule : public Module {

public:

  Day0HistogrammingModule(const std::string& inputGenTrackSetLabel, // label used to GenTrackSet
			  const std::string& inputHitSetLabel,      // label used to get HitSet
			  const std::string& inputStripSetLabel,    // label used to get StripSet
			  const DetectorGeometry & detector
			  );

  void processEvent(Event& ) override;


private:

  const std::string _tracksLabel;
  const std::string _hitSetLabel;
  const std::string _stripSetLabel;
  const DetectorGeometry& _detector;

  //ROOT will delete the pointees when the TFile to which each TH1D* is attached is deleted

  // Histogram helper functions and the histograms that they manage.
  void bookGenTrackHistograms( TDirectory* dir, std::string const& label );
  void fillGenTrackHistograms(fc::Event& event);
  TH1D* _hNTracks = nullptr;
  TH1D* _hPt      = nullptr;
  TH1D* _hP       = nullptr;
  TH1D* _hKappa   = nullptr;
  TH1D* _hphi0    = nullptr;
  TH1D* _htanl    = nullptr;
  TH1D* _hdr      = nullptr;
  TH1D* _hdz      = nullptr;
  TH1D* _hradius  = nullptr;

  // Histogram helper functions and the histograms that they manage.
  void bookStripHistograms( TDirectory* dir, std::string const& label );
  void fillStripHistograms(fc::Event& event);
  TH1D* _hTotalStrips = nullptr;
  TH1D* _hADCAllStrips = nullptr;
  std::vector<TH1D*> _hStripsPerLayer;
  std::vector<TH1D*> _hOccupancyPerLayer;

  // Histogram helper functions and the histograms that they manage.
  void bookHitHistograms( TDirectory* dir, std::string const& label );
  void fillHitHistograms(fc::Event& event);
  TH1D* _hNHits       = nullptr;
  TH1D* _hClusterSize = nullptr;
  TH1D* _hLayerHit    = nullptr;
  TH1D* _hTrackNumber = nullptr;
  TH1D* _hAllHity     = nullptr;
  TH2D* _hAllHitxz    = nullptr;
  std::vector<TH2D*> _hHitxz;

};

} // end namespace fc

#endif // Modules_Day0HistogrammingModule_hh
