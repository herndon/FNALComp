#include<iostream>
#include<cmath>
#include<string>
#include "TH1F.h"
#include "Geometry/include/DetectorGeometry.hh"
#include "Services/include/UniqueRootDirectory.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Tracking/include/TrackMatching.hh"
#include "Modules/include/TrackCompareWithGenModule.hh"

fc::TrackCompareWithGenModule::TrackCompareWithGenModule(int debugLevel,
        const std::string& inputGenTracksLabel,
        const std::string& inputRecTracksLabel,
        const DetectorGeometry & detectorGeometry ):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _genTracksLabel(inputGenTracksLabel),
    _recTracksLabel(inputRecTracksLabel),  
    _genTracks(0),
    _recoTracks(0),
    _matchedRecoTracks(0),
   _matchedRecoTracksXY(0) {

    initializeHistograms();
}

void fc::TrackCompareWithGenModule::initializeHistograms() {

  std::string tDir;


  if (_recTracksLabel == "perfectRecoTracks") {
    tDir = "TrackCompareGenWithPerfectRecoTracks";
  }else if (_recTracksLabel == "seedTracks") {
    tDir = "TrackCompareGenWithSeedTracks";
  }else if (_recTracksLabel == "recoTracks") {
    tDir = "TrackCompareGenWithRecoTracks";
  } else {
    tDir = "TrackCompareGenWithTracks";
  }
    UniqueRootDirectory tdir(tDir);


    dR    = new TH1F("TrackDr", "dr;dr(m);N",100, -0.02, 0.02);
    phi0  = new TH1F("TrackPhi0", "phi0;phi0(rad);N",100, 0.0, 2.0*M_PI);
    kappa = new TH1F("TrackKappa", "kappa(1/Gev); kappa (1/GeV);N",100, 0.0, 0.1);
    dZ    = new TH1F("TrackDz", "dz;dz (m); N",100, -0.02, 0.02);
    tanL  = new TH1F("TrackTanL", "tanL;tanL;N",100, 0.0, 1.0);

    sigmaDr    = new TH1F("TrackSigmaDr", "sigma dr;dr(m);N",1000, 0.0, 0.0005);
    sigmaPhi0  = new TH1F("TrackSigmaPhi0", "sigma phi0;phi0(rad);N",1000, 0.0,
                          0.0005);
    sigmaKappa = new TH1F("TrackSigmaKappa", "sigma kappa(1/Gev); kappa (1/GeV);N",
                          1000, 0.0, 0.005);
    sigmaDz    = new TH1F("TrackSigmaDz", "sigma dz;dz (m); N",1000, 0.0, 0.0005);
    sigmaTanL  = new TH1F("TrackSigmaTanL", "sigma tanL;tanL;N",1000, 0.0, 0.0005);



    pT = new TH1F("TrackPT", "pT); pT(GeV);N",100, 0.0, 100.0);
    chi2 = new TH1F("TrackChi2", "chi2;chi2;N",100, 0.0, 20.0);
    nDof = new TH1F("TrackNDof", "nDof;nDof;N",10, 0.0, 10.0);
    prob = new TH1F("TrackProb", "prob;prob;N",100, 0.0, 1.0);


    deltaDr    = new TH1F("TrackDeltaD0", "Delta dr;delta dr(m);N",100, -0.001,
                          0.001);
    deltaPhi0  = new TH1F("TrackDeltaPhi0", "Delta phi0;delta phi0(rad);N",100,
                          -0.005, 0.005);
    deltaKappa = new TH1F("TrackDeltaKappa",
                          "Delta kappa(1/GeV); delta kappa (1/GeV);N",100, -0.01, 0.01);
    deltaDz    = new TH1F("TrackDeltaZ0", "Delta dz;delta dz (m); N",100, -0.001,
                          0.001);
    deltaTanL  = new TH1F("TrackDeltaTanL", "Delta tanL;delta tanL;N",100, -0.001,
                          0.001);

    deltaDrPull    = new TH1F("TrackDeltaD0Pull", "Delta dr Pull;delta dr  Pull);N",
                              100, -4.0, 4.0);
    deltaPhi0Pull  = new TH1F("TrackDeltaPhi0Pull",
                              "Delta phi0 Pull;delta phi0 Pull);N",100, -4.0, 4.0);
    deltaKappaPull = new TH1F("TrackDeltaKappaPull",
                              "Delta kappa Pull; delta kappa  Pull;N",100, -4.0, 4.0);
    deltaDzPull    = new TH1F("TrackDeltaZ0Pull", "Delta dz Pull;delta dz  Pull; N",
                              100, -4.0, 4.0);
    deltaTanLPull  = new TH1F("TrackDeltaTanLPull","Delta tanL Pull;delta tanL PUll;N",100, -4.0, 4.0);

    badDR    = new TH1F("badTrackDr", "dr;dr(m);N",100, -0.02, 0.02);
    badPhi0  = new TH1F("badTrackPhi0", "phi0;phi0(rad);N",100, 0.0, 2.0*M_PI);
    badKappa = new TH1F("badTrackKappa", "kappa(1/Gev); kappa (1/GeV);N",100, 0.0, 0.1);
    badDZ    = new TH1F("badTrackDz", "dz;dz (m); N",100, -0.02, 0.02);
    badTanL  = new TH1F("badTrackTanL", "tanL;tanL;N",100, 0.0, 1.0);

    badSigmaDr    = new TH1F("badTrackSigmaDr", "sigma dr;dr(m);N",1000, 0.0, 0.0005);
    badSigmaPhi0  = new TH1F("badTrackSigmaPhi0", "sigma phi0;phi0(rad);N",1000, 0.0,0.0005);
    badSigmaKappa = new TH1F("badTrackSigmaKappa", "sigma kappa(1/Gev); kappa (1/GeV);N",1000, 0.0, 0.005);
    badSigmaDz    = new TH1F("badTrackSigmaDz", "sigma dz;dz (m); N",1000, 0.0, 0.0005);
    badSigmaTanL  = new TH1F("badTrackSigmaTanL", "sigma tanL;tanL;N",1000, 0.0, 0.0005);



    badPT = new TH1F("badTrackPT", "pT); pT(GeV);N",100, 0.0, 100.0);
    badChi2 = new TH1F("badTrackChi2", "chi2;chi2;N",100, 0.0, 20.0);
    badNDof = new TH1F("badTrackNDof", "nDof;nDof;N",10, 0.0, 10.0);
    badProb = new TH1F("badTrackProb", "prob;prob;N",100, 0.0, 1.0);

    badDeltaDr    = new TH1F("badTrackDeltaD0","Delta dr;delta dr(m);N",100, -0.001,0.001);
    badDeltaPhi0  = new TH1F("badTrackDeltaPhi0","Delta phi0;delta phi0(rad);N",100,-0.005, 0.005);
    badDeltaKappa = new TH1F("badTrackDeltaKappa","Delta kappa(1/GeV); delta kappa (1/GeV);N",100, -0.01, 0.01);
    badDeltaDz    = new TH1F("badTrackDeltaZ0", "Delta dz;delta dz (m); N",100, -0.001,0.001);
    badDeltaTanL  = new TH1F("badTrackDeltaTanL", "Delta tanL;delta tanL;N",100, -0.001,0.001);

    badDeltaDrPull    = new TH1F("badTrackDeltaD0Pull", "Delta dr Pull;delta dr  Pull);N",100, -10.0, 10.0);
    badDeltaPhi0Pull  = new TH1F("badTrackDeltaPhi0Pull","Delta phi0 Pull;delta phi0 Pull);N",100, -10.0, 10.0);
    badDeltaKappaPull = new TH1F("badTrackDeltaKappaPull","Delta kappa Pull; delta kappa  Pull;N",100, -10.0, 10.0);
    badDeltaDzPull    = new TH1F("badTrackDeltaZ0Pull", "Delta dz Pull;delta dz  Pull; N",100, -10.0, 10.0);
    badDeltaTanLPull  = new TH1F("badTrackDeltaTanLPull","Delta tanL Pull;delta tanL PUll;N",100, -1000.0, 1000.0);

}



void fc::TrackCompareWithGenModule::processEvent(Event& event)
{
    Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_genTracksLabel);
    Handle<TrackSet> recoTrackSet = event.get<TrackSet>(_recTracksLabel);

    compareTracks(*genTrackSet,*recoTrackSet);

    _genTracks += genTrackSet->getGenTracks().size();
    _recoTracks += recoTrackSet->getTracks().size();


}

void fc::TrackCompareWithGenModule::compareTracks(const GenTrackSet & genTrackSet,
						  const TrackSet& recoTrackSet) {


    if (recoTrackSet.getTracks().begin() == recoTrackSet.getTracks().end()) return;

    for (auto const& genTrack : genTrackSet.getGenTracks()) {
 
      bool goodMatch = false;
      bool goodMatchXY = false;
 
      const Track& recoTrack = fcf::matchTrack(genTrack,recoTrackSet,_detectorGeometry,goodMatch,goodMatchXY);
      TVectorD bestDeltaHP = fcf::deltaHP(genTrack,recoTrack,_detectorGeometry);
      fillHistograms(bestDeltaHP,recoTrack,goodMatch);
      if (goodMatch)++_matchedRecoTracks;
     if (goodMatchXY)++_matchedRecoTracksXY;

    }
}



void fc::TrackCompareWithGenModule::fillHistograms(const TVectorD & deltaHP,
						   const Track& recoTrack, bool matched) const {


  if (matched){
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


    pT->Fill(recoTrack.getHelix().getPT());
    chi2->Fill(recoTrack.getChi2());
    nDof->Fill(recoTrack.getNDof());
    prob->Fill(recoTrack.getChi2Prob());

    deltaDr->Fill(deltaHP(0));
    deltaPhi0->Fill(deltaHP(1));
    deltaKappa->Fill(deltaHP(2));
    deltaDz->Fill(deltaHP(3));
    deltaTanL->Fill(deltaHP(4));


    deltaDrPull->Fill(deltaHP(0)/recoTrack.getSigmaDr());
    deltaPhi0Pull->Fill(deltaHP(1)/recoTrack.getSigmaPhi0());
    deltaKappaPull->Fill(deltaHP(2)/recoTrack.getSigmaKappa());
    deltaDzPull->Fill(deltaHP(3)/recoTrack.getSigmaDz());
    deltaTanLPull->Fill(deltaHP(4)/recoTrack.getSigmaTanL());
  } else {
    badDR->Fill(recoTrack.getHelix().getDr());
    badPhi0->Fill(recoTrack.getHelix().getPhi0());
    badKappa->Fill(recoTrack.getHelix().getKappa());
    badDZ->Fill(recoTrack.getHelix().getDz());
    badTanL->Fill(recoTrack.getHelix().getTanL());

    badSigmaDr->Fill(recoTrack.getSigmaDr());
    badSigmaPhi0->Fill(recoTrack.getSigmaPhi0());
    badSigmaKappa->Fill(recoTrack.getSigmaKappa());
    badSigmaDz->Fill(recoTrack.getSigmaDz());
    badSigmaTanL->Fill(recoTrack.getSigmaTanL());


    badPT->Fill(recoTrack.getHelix().getPT());
    badChi2->Fill(recoTrack.getChi2());
    badNDof->Fill(recoTrack.getNDof());
    badProb->Fill(recoTrack.getChi2Prob());

    badDeltaDr->Fill(deltaHP(0));
    badDeltaPhi0->Fill(deltaHP(1));
    badDeltaKappa->Fill(deltaHP(2));
    badDeltaDz->Fill(deltaHP(3));
    badDeltaTanL->Fill(deltaHP(4));


    badDeltaDrPull->Fill(deltaHP(0)/recoTrack.getSigmaDr());
    badDeltaPhi0Pull->Fill(deltaHP(1)/recoTrack.getSigmaPhi0());
    badDeltaKappaPull->Fill(deltaHP(2)/recoTrack.getSigmaKappa());
    badDeltaDzPull->Fill(deltaHP(3)/recoTrack.getSigmaDz());
    badDeltaTanLPull->Fill(deltaHP(4)/recoTrack.getSigmaTanL());

  }
}

void fc::TrackCompareWithGenModule::endJob() {
  if (_debugLevel >=1) {

    std::cout << "TrackCompareWithGenModule Results" << std::endl;
  std::cout << "Perfect Tracks:       " << _genTracks << std::endl;
  std::cout << "Reco eff:             " << static_cast<double>(_matchedRecoTracks)/static_cast<double>(_genTracks) << std::endl;     
  std::cout << "Ghost Tracks rate:    " << (static_cast<double>(_recoTracks) - static_cast<double>(_matchedRecoTracks))/static_cast<double>(_recoTracks) << std::endl;
  std::cout << "Reco eff XY:          " << static_cast<double>(_matchedRecoTracksXY)/static_cast<double>(_genTracks) << std::endl;     
  std::cout << "Ghost Tracks rate XY: " << (static_cast<double>(_recoTracks) - static_cast<double>(_matchedRecoTracksXY))/static_cast<double>(_recoTracks) << std::endl;
  }


}
