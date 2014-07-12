#ifndef HitCompareModule_hh
#define HitCompareModule_hh
//============================================================================
// HitCompareModule.hh
// Module for comparing reconstructing hit position to generator hit positions
// 
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-12
//============================================================================

#include "DetectorGeometry.hh"
#include "Module.hh"

class Hit;
class HitSet;
class TFile;
class TList;
class TH1F;


namespace fc {

///
/// Class HitCompareModule  Module for comparing reconstructing hit position to generator hit positions
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-12
///

  class HitCompareModule : public Module {

public:

  HitCompareModule(int, const std::string& iInputGenHitsLabel, const std::string& iInputRecHitsLabel,
		   const DetectorGeometry &, TFile *);
  ~HitCompareModule() {};

  void processEvent(fc::Event&) override;

  void endJob(void) override;


private:

  int _debugLevel;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  int _nLayers;

  const std::string _genHitsLabel;
  const std::string _recHitsLabel;

  // Comparison of hits

  void compareHits(const HitSet &, const HitSet &);


  double compareHitPositions(const Hit &, const Hit &);

  // Histograms

  TFile * _outputrootfile;
  TList * _roothistogramlist;

  TH1F * deltaHitPositions[DetectorGeometry::_nSensors];


  void initializeHistograms(void);


};
} // end namespace fc

#endif // HitCompareModule_hh
