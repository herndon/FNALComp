#include<iostream>
#include<cmath>
#include<string>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Tracking/include/TrackMatching.hh"
#include "Modules/include/TrackCompareWithPerfectModule.hh"
#include "TH1F.h"
#include "Services/include/UniqueRootDirectory.hh"

fc::TrackCompareWithPerfectModule::TrackCompareWithPerfectModule(int debugLevel,
        const std::string& inputPerfectTracksLabel,
        const std::string& inputRecTracksLabel,
        const DetectorGeometry & detectorGeometry ):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _perfectTracksLabel(inputPerfectTracksLabel),
    _recTracksLabel(inputRecTracksLabel),
    _perfectTracks(0),
    _recoTracks(0),
    _matchedRecoTracks(0) {

    initializeHistograms();
}

void fc::TrackCompareWithPerfectModule::initializeHistograms() {

    UniqueRootDirectory tdir("TrackComparePer");

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

 
}



void fc::TrackCompareWithPerfectModule::processEvent(Event& event)
{
    Handle<TrackSet> perfectTrackSet = event.get<TrackSet>(_perfectTracksLabel);
    Handle<TrackSet> recoTrackSet = event.get<TrackSet>(_recTracksLabel);

    compareTracks(*perfectTrackSet,*recoTrackSet);

    _perfectTracks += perfectTrackSet->getTracks().size();
    _recoTracks += recoTrackSet->getTracks().size();

    // Function to histogram results

}

void fc::TrackCompareWithPerfectModule::compareTracks(const TrackSet & perfectTrackSet,
        const TrackSet& recoTrackSet) {


    if (recoTrackSet.getTracks().begin() == recoTrackSet.getTracks().end()) return;

    for (auto const& perfectTrack : perfectTrackSet.getTracks()) {

      bool goodMatch = false;
      bool goodMatchXY = false;
      const Track& recoTrack = fcf::matchTrack(perfectTrack,recoTrackSet,goodMatch,goodMatchXY);
      if (goodMatch) { 
	++_matchedRecoTracks;
	TVectorD bestDeltaHP = fcf::deltaHP(perfectTrack,recoTrack);
        fillHistograms(bestDeltaHP,recoTrack);
      }

    }
}



void fc::TrackCompareWithPerfectModule::fillHistograms(const TVectorD & deltaHP,
        const Track& recoTrack) const {


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



}

void fc::TrackCompareWithPerfectModule::endJob() {

}
