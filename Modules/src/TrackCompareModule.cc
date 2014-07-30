#include<iostream>
#include<cmath>
#include<string>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Modules/include/TrackCompareModule.hh"
#include "TH1F.h"
#include "Services/include/UniqueRootDirectory.hh"

fc::TrackCompareModule::TrackCompareModule(int debugLevel, 
				       const std::string& inputGenTracksLabel,
				       const std::string& inputRecTracksLabel,
				       const DetectorGeometry & detectorGeometry ):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _genTracksLabel(inputGenTracksLabel),
  _recTracksLabel(inputRecTracksLabel){

  initializeHistograms();
}

void fc::TrackCompareModule::initializeHistograms(){

  UniqueRootDirectory tdir("TrackCompare");

  dR    = new TH1F("TrackDr", "dr;dr(m);N",100, -0.02, 0.02);
  phi0  = new TH1F("TrackPhi0", "phi0;phi0(rad);N",100, 0.0, 2.0*M_PI);
  kappa = new TH1F("TrackKappa", "kappa(1/Gev); kappa (1/GeV);N",100, 0.0, 0.1);
  dZ    = new TH1F("TrackDz", "dz;dz (m); N",100, -0.02, 0.02);
  tanL  = new TH1F("TrackTanL", "tanL;tanL;N",100, 0.0, 1.0);

  sigmaDr    = new TH1F("TrackSigmaDr", "sigma dr;dr(m);N",1000, 0.0, 0.0005);
  sigmaPhi0  = new TH1F("TrackSigmaPhi0", "sigma phi0;phi0(rad);N",1000, 0.0, 0.0005);
  sigmaKappa = new TH1F("TrackSigmaKappa", "sigma kappa(1/Gev); kappa (1/GeV);N",1000, 0.0, 0.005);
  sigmaDz    = new TH1F("TrackSigmaDz", "sigma dz;dz (m); N",1000, 0.0, 0.0005);
  sigmaTanL  = new TH1F("TrackSigmaTanL", "sigma tanL;tanL;N",1000, 0.0, 0.0005);



  pT = new TH1F("TrackPT", "pT); pT(GeV);N",100, 0.0, 100.0);
  chi2 = new TH1F("TrackChi2", "chi2;chi2;N",100, 0.0, 20.0);
  nDof = new TH1F("TrackNDof", "nDof;nDof;N",10, 0.0, 10.0);
  prob = new TH1F("TrackProb", "prob;prob;N",100, 0.0, 1.0);


  deltaDr    = new TH1F("TrackDeltaD0", "Delta dr;delta dr(m);N",100, -0.001, 0.001);
  deltaPhi0  = new TH1F("TrackDeltaPhi0", "Delta phi0;delta phi0(rad);N",100, -0.005, 0.005);
  deltaKappa = new TH1F("TrackDeltaKappa", "Delta kappa(1/GeV); delta kappa (1/GeV);N",100, -0.01, 0.01);
  deltaDz    = new TH1F("TrackDeltaZ0", "Delta dz;delta dz (m); N",100, -0.001, 0.001);
  deltaTanL  = new TH1F("TrackDeltaTanL", "Delta tanL;delta tanL;N",100, -0.001, 0.001);

  deltaDrPull    = new TH1F("TrackDeltaD0Pull", "Delta dr Pull;delta dr  Pull);N",100, -4.0, 4.0);
  deltaPhi0Pull  = new TH1F("TrackDeltaPhi0Pull", "Delta phi0 Pull;delta phi0 Pull);N",100, -4.0, 4.0);
  deltaKappaPull = new TH1F("TrackDeltaKappaPull", "Delta kappa Pull; delta kappa  Pull;N",100, -4.0, 4.0);
  deltaDzPull    = new TH1F("TrackDeltaZ0Pull", "Delta dz Pull;delta dz  Pull; N",100, -4.0, 4.0);
  deltaTanLPull  = new TH1F("TrackDeltaTanLPull", "Delta tanL Pull;delta tanL PUll;N",100, -4.0, 4.0);

}



void fc::TrackCompareModule::processEvent(Event& event)
{
 Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_genTracksLabel);
 Handle<TrackSet> recoTrackSet = event.get<TrackSet>(_recTracksLabel);

 compareTracks(*genTrackSet,*recoTrackSet);

  // Function to histogram results

}

void fc::TrackCompareModule::compareTracks(const GenTrackSet & genTrackSet, const TrackSet& recoTrackSet) const{

  
  if (recoTrackSet.getTracks().begin() == recoTrackSet.getTracks().end()) return;

  for (genTrackSet::const_iterator genTrackIter =  genTrackSet.getGenTracks().begin(); genTrackIter !=  genTrackSet.getGenTracks().end(); ++genTrackIter){

    const Track& recoTrack = matchTrack(*genTrackIter,recoTrackSet);
    TVectorD bestDeltaHP = deltaHP(*genTrackIter,recoTrack);
    fillHistograms(bestDeltaHP,recoTrack);


  }
}


const fc::Track & fc::TrackCompareModule::matchTrack(const GenTrack & genTrack, const TrackSet& recoTrackSet) const{

  double bestDeltaTracks = 1000.0;
  double tmpDeltaTracks;
  int trackNumber=0;
  int bestTrack=-1;

  for (trackSet::const_iterator recoTrackIter =  recoTrackSet.getTracks().begin(); recoTrackIter !=  recoTrackSet.getTracks().end(); ++recoTrackIter,++trackNumber){

    tmpDeltaTracks = deltaTracks(genTrack,*recoTrackIter);

    if (tmpDeltaTracks < bestDeltaTracks) {
      bestDeltaTracks = tmpDeltaTracks;
      bestTrack = trackNumber;
    }
  }

  return recoTrackSet.getTracks()[bestTrack];

}

double fc::TrackCompareModule::deltaTracks(const GenTrack & genTrack, const Track& recoTrack) const{

  return std::sqrt((genTrack.makeHelix().getKappa()-recoTrack.getHelix().getKappa())*(genTrack.makeHelix().getKappa()-recoTrack.getHelix().getKappa())+
		   (genTrack.makeHelix().getPhi0()-recoTrack.getHelix().getPhi0())*(genTrack.makeHelix().getPhi0()-recoTrack.getHelix().getPhi0()));

}

TVectorD fc::TrackCompareModule::deltaHP(const GenTrack & genTrack, const Track& recoTrack) const{


  return recoTrack.getHelix().getHelix() - genTrack.makeHelix().getHelix();

}

void fc::TrackCompareModule::fillHistograms(const TVectorD & deltaHP, const Track& recoTrack) const{


  dR->Fill(recoTrack.getHelix().getDr());
  phi0->Fill(recoTrack.getHelix().getPhi0());
  kappa->Fill(recoTrack.getHelix().getKappa());
  dZ->Fill(recoTrack.getHelix().getDz());
  tanL->Fill(recoTrack.getHelix().getTanL());

  sigmaDr->Fill(recoTrack.getSigmaDr());
  sigmaPhi0->Fill(recoTrack.getSigmaPhi0());
  sigmaKappa->Fill(recoTrack.getSigmaKappa());
  sigmaDz->Fill(recoTrack.getSigmaDz());
  sigmaTanL->Fill(recoTrack.getSigmaTanL());


  pT->Fill(recoTrack.getHelix().getPT(_detectorGeometry.getBField()));
  chi2->Fill(recoTrack.getChi2());
  nDof->Fill(recoTrack.getNDof());
  prob->Fill(recoTrack.getChi2Prob());

  deltaDr->Fill(deltaHP(0));
  deltaPhi0->Fill(deltaHP(1));
  deltaKappa->Fill(deltaHP(2));
  deltaDz->Fill(deltaHP(3));
  deltaTanL->Fill(deltaHP(4));

  // adjust for 3 parameters

  deltaDrPull->Fill(deltaHP(0)/recoTrack.getSigmaDr());
  deltaPhi0Pull->Fill(deltaHP(1)/recoTrack.getSigmaPhi0());
  deltaKappaPull->Fill(deltaHP(2)/recoTrack.getSigmaKappa());
  deltaDzPull->Fill(deltaHP(3)/recoTrack.getSigmaDz());
  deltaTanLPull->Fill(deltaHP(4)/recoTrack.getSigmaTanL());


}

void fc::TrackCompareModule::endJob(){
}
