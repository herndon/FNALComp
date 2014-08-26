#include<iostream>
#include<cmath>
#include<string>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackSeedCompareModule.hh"
#include "Tracking/include/TrackMatching.hh"
#include "TH1F.h"
#include "Services/include/UniqueRootDirectory.hh"

fc::TrackSeedCompareModule::TrackSeedCompareModule(int debugLevel,  std::ofstream & debugfile,
        const std::string& inputPerfectTracksLabel,
        const std::string& inputSeedTracksLabel,
        const DetectorGeometry & detectorGeometry ):
    _debugLevel(debugLevel),
    _debugfile(debugfile),
    _detectorGeometry(detectorGeometry),
    _perfectTracksLabel(inputPerfectTracksLabel),
    _seedTracksLabel(inputSeedTracksLabel) {

    initializeHistograms();
}

void fc::TrackSeedCompareModule::initializeHistograms() {

    UniqueRootDirectory tdir("TrackSeedCompare");

    _hDR    = new TH1D("TrackDr", "dr;dr(m);N",100, -0.02, 0.02);
    _hPhi0  = new TH1D("TrackPhi0", "phi0;phi0(rad);N",100, 0.0, 2.0*M_PI);
    _hKappa = new TH1D("TrackKappa", "kappa(1/Gev); kappa (1/GeV);N",100, 0.0, 0.1);
    _hDZ    = new TH1D("TrackDz", "dz;dz (m); N",100, -0.02, 0.02);
    _hTanL  = new TH1D("TrackTanL", "tanL;tanL;N",100, 0.0, 1.0);

    _hSigmaDr    = new TH1D("TrackSigmaDr", "sigma dr;dr(m);N",1000, 0.0, 0.0005);
    _hSigmaPhi0  = new TH1D("TrackSigmaPhi0", "sigma phi0;phi0(rad);N",1000, 0.0,
                          0.0005);
    _hSigmaKappa = new TH1D("TrackSigmaKappa", "sigma kappa(1/Gev); kappa (1/GeV);N",
                          1000, 0.0, 0.005);
    _hSigmaDz    = new TH1D("TrackSigmaDz", "sigma dz;dz (m); N",1000, 0.0, 0.0005);
    _hSigmaTanL  = new TH1D("TrackSigmaTanL", "sigma tanL;tanL;N",1000, 0.0, 0.0005);





    _hDeltaDr    = new TH1D("TrackDeltaD0", "Delta dr;delta dr(m);N",100, -0.001,
                          0.001);
    _hDeltaPhi0  = new TH1D("TrackDeltaPhi0", "Delta phi0;delta phi0(rad);N",100,
                          -0.005, 0.005);
    _hDeltaKappa = new TH1D("TrackDeltaKappa",
                          "Delta kappa(1/GeV); delta kappa (1/GeV);N",100, -0.01, 0.01);
    _hDeltaDz    = new TH1D("TrackDeltaZ0", "Delta dz;delta dz (m); N",100, -0.001,
                          0.001);
    _hDeltaTanL  = new TH1D("TrackDeltaTanL", "Delta tanL;delta tanL;N",100, -0.001,
                          0.001);

    _hDeltaDrPull    = new TH1D("TrackDeltaD0Pull", "Delta dr Pull;delta dr  Pull);N",
                              100, -4.0, 4.0);
    _hDeltaPhi0Pull  = new TH1D("TrackDeltaPhi0Pull",
                              "Delta phi0 Pull;delta phi0 Pull);N",100, -4.0, 4.0);
    _hDeltaKappaPull = new TH1D("TrackDeltaKappaPull",
                              "Delta kappa Pull; delta kappa  Pull;N",100, -4.0, 4.0);
    _hDeltaDzPull    = new TH1D("TrackDeltaZ0Pull", "Delta dz Pull;delta dz  Pull; N",
                              100, -4.0, 4.0);
    _hDeltaTanLPull  = new TH1D("TrackDeltaTanLPull",
                              "Delta tanL Pull;delta tanL PUll;N",100, -4.0, 4.0);

    _hMatchedTrackSeeds = new TH1D("MatchedTrackSeeds", "Number matched seeds;number;N",5,-0.5,4.5);

}



void fc::TrackSeedCompareModule::processEvent(Event& event)
{
    Handle<TrackSet> perfectTrackSet = event.get<TrackSet>(_perfectTracksLabel);
    Handle<TrackSet> seedTrackSet = event.get<TrackSet>(_seedTracksLabel);

    compareTrackSeeds(*perfectTrackSet,*seedTrackSet);


}

void fc::TrackSeedCompareModule::compareTrackSeeds(const TrackSet & perfectTrackSet,
						   const TrackSet& seedTrackSet) const {


  for (auto const& track : perfectTrackSet.tracks()) {

    std::vector<int> seeds =  matchTrackSeeds(track,seedTrackSet);

    for (auto seed: seeds) {
      TVectorD deltaPos = fcf::deltaHP(track,seedTrackSet.tracks()[seed]);
      fillHistograms(deltaPos,seedTrackSet.tracks()[seed]);
    }

  }

}



const std::vector<int> fc::TrackSeedCompareModule::matchTrackSeeds(const Track & track,
        const TrackSet& seedTrackSet) const {

  int seedNumber = 0;
  std::vector<int> seedNumbers;
    for (auto const& seed : seedTrackSet.tracks()) {

      if (matchTrackSeed(track,seed)) seedNumbers.push_back(seedNumber);
        ++seedNumber;
    }

    _hMatchedTrackSeeds->Fill(seedNumbers.size());

    return seedNumbers;

}

bool fc::TrackSeedCompareModule::matchTrackSeed(const Track & track,
        const Track& seed) const {

  unsigned int matchedHits = 0;
  for (auto const hitp : track.trackHits()) {

    for (auto const hits : seed.trackHits()){

      if (hitp == hits) matchedHits++;
    }
  }

  return (matchedHits == seed.trackHits().size());
}


void fc::TrackSeedCompareModule::fillHistograms(const TVectorD & deltaHP,
        const Track& recoTrack) const {


    _hDR->Fill(recoTrack.dR());
    _hPhi0->Fill(recoTrack.phi0());
    _hKappa->Fill(recoTrack.kappa());
    _hDZ->Fill(recoTrack.dZ());
    _hTanL->Fill(recoTrack.tanL());

    _hSigmaDr->Fill(recoTrack.sigmaDr());
    _hSigmaPhi0->Fill(recoTrack.sigmaPhi0());
    _hSigmaKappa->Fill(recoTrack.sigmaKappa());
    _hSigmaDz->Fill(recoTrack.sigmaDz());
    _hSigmaTanL->Fill(recoTrack.sigmaTanL());


 
    _hDeltaDr->Fill(deltaHP(0));
    _hDeltaPhi0->Fill(deltaHP(1));
    _hDeltaKappa->Fill(deltaHP(2));
    _hDeltaDz->Fill(deltaHP(3));
    _hDeltaTanL->Fill(deltaHP(4));


    _hDeltaDrPull->Fill(deltaHP(0)/recoTrack.sigmaDr());
    _hDeltaPhi0Pull->Fill(deltaHP(1)/recoTrack.sigmaPhi0());
    _hDeltaKappaPull->Fill(deltaHP(2)/recoTrack.sigmaKappa());
    _hDeltaDzPull->Fill(deltaHP(3)/recoTrack.sigmaDz());
    _hDeltaTanLPull->Fill(deltaHP(4)/recoTrack.sigmaTanL());


}

void fc::TrackSeedCompareModule::endJob() {
}
