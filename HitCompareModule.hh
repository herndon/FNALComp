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

#include "StripSet.hh"
#include <vector>
#include <memory>
#include "DetectorGeometry.hh"

class DetectorGeometry;
class HitSet;
class TFile;
class TList;
class TH1F;


namespace fc {

///
/// Class HitCompareModule  Module for comparing reconstructing hit position to generator hit positions
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-12
///

class HitCompareModule {

public:

  HitCompareModule(int, const DetectorGeometry &, TFile *);
  ~HitCompareModule() {};

  void processEvent(const HitSet &,const HitSet &);

  void endjob(void);


private:

  int _debugLevel;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  int _nLayers;

  // Comparison of hits

  void compareHits(const HitSet &, const HitSet &);

  void compareHitsLayer(const layerHitMap&, const layerHitMap&,const HitSet &, const HitSet &,int);

  double compareHitPositions(const Hit &, const Hit &);

  // Histograms

  TFile * _outputrootfile;
  TList * _roothistogramlist;

  TH1F * deltaHitPositions[DetectorGeometry::_nSensors];


  void initializeHistograms(void);


};
} // end namespace fc

#endif // HitCompareModule_hh
