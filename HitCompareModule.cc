#include<iostream>
#include<cmath>
#include<string>
#include "DetectorGeometry.hh"
#include "HitSet.hh"
#include "HitCompareModule.hh"
#include "TFile.h"
#include "TH1F.h"

fc::HitCompareModule::HitCompareModule(int debugLevel, const DetectorGeometry & myDetectorGeometry, TFile * outputrootfile):
  _debugLevel(debugLevel),
  _myDetectorGeometry(myDetectorGeometry),
  _outputrootfile(outputrootfile) {

  // Intialize commonly used DetectorGeometry data
  _nLayers = _myDetectorGeometry.getNSensors();

  _roothistogramlist = new TList();
  initializeHistograms();



}

void fc::HitCompareModule::processEvent(const HitSet & myGenHitSet, const HitSet& myRecoHitSet)
{



 compareHits(myGenHitSet,myRecoHitSet);

  // Function to histogram results

  //  if (_debugLevel >= 2) myHitSet.print();

}

void fc::HitCompareModule::compareHits(const HitSet & myGenHitSet, const HitSet& myRecoHitSet)
{


  for (int ii_layer =  0; ii_layer < _nLayers; ++ii_layer){
 
    const layerHitMap& myGenLayerHitMap  = myGenHitSet.getConstLayerHitMap(ii_layer);
    const layerHitMap& myRecoLayerHitMap  = myRecoHitSet.getConstLayerHitMap(ii_layer);
 
    compareHitsLayer(myGenLayerHitMap,myRecoLayerHitMap,myGenHitSet,myRecoHitSet,ii_layer);

  } // end layer loop


}


void fc::HitCompareModule::compareHitsLayer(const layerHitMap& myGenLayerHitMap, const layerHitMap& myRecoLayerHitMap,
					    const HitSet & myGenHitSet, const HitSet& myRecoHitSet, int layer){

  double deltaPosition;
  double tempDeltaPosition;

  for (layerHitMap::const_iterator genLayerHitMapIter = myGenLayerHitMap.begin(); genLayerHitMapIter != myGenLayerHitMap.end(); ++genLayerHitMapIter){

    deltaPosition = 999.0;

    const Hit genHit = myGenHitSet.getConstHit(genLayerHitMapIter);

    for (layerHitMap::const_iterator recoLayerHitMapIter = myRecoLayerHitMap.begin(); recoLayerHitMapIter != myRecoLayerHitMap.end(); ++recoLayerHitMapIter){

      const Hit recoHit = myRecoHitSet.getConstHit(recoLayerHitMapIter);
      tempDeltaPosition = compareHitPositions(genHit,recoHit);
      if (std::abs(tempDeltaPosition) < std::abs(deltaPosition)) deltaPosition = tempDeltaPosition;
    }

    deltaHitPositions[layer]->Fill(deltaPosition);

  }

}

double fc::HitCompareModule::compareHitPositions(const Hit & genHit, const Hit& recoHit)
{

  return recoHit.getHitPosition()[0] - genHit.getHitPosition()[0];

}

void fc::HitCompareModule::initializeHistograms(){

  //deltaHitPositions = new TH1F("deltaHitPositions", "Delta X Hit Positions","delta X (m)", "number of hits",100, -0.1, 0.1);

  deltaHitPositions[0] = new TH1F("deltaHitPositions L0", "Delta X Hit Positions",100, -0.01, 0.01);
  deltaHitPositions[1] = new TH1F("deltaHitPositions L1", "Delta X Hit Positions",100, -0.01, 0.01);
  deltaHitPositions[2] = new TH1F("deltaHitPositions L2", "Delta X Hit Positions",100, -0.01, 0.01);
  deltaHitPositions[3] = new TH1F("deltaHitPositions L3", "Delta X Hit Positions",100, -0.01, 0.01);
  deltaHitPositions[4] = new TH1F("deltaHitPositions L4", "Delta X Hit Positions",100, -0.01, 0.01);


  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {
    deltaHitPositions[ii_layer]->GetXaxis()->SetTitle("delta (m)");
    deltaHitPositions[ii_layer]->GetYaxis()->SetTitle("N Hits");
    _roothistogramlist->Add(deltaHitPositions[ii_layer]);
  }


}

void fc::HitCompareModule::endjob(){

  _roothistogramlist->Write();
  _outputrootfile->Close();

}
