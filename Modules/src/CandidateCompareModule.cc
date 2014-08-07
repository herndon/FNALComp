#include<iostream>
#include<cmath>
#include<string>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/CandidateCompareModule.hh"
#include "TH1F.h"
#include "Services/include/UniqueRootDirectory.hh"

fc::CandidateCompareModule::CandidateCompareModule(int debugLevel,
        const std::string& inputPerfectTracksLabel,
        const std::string& inputCandidateTracksLabel,
        const DetectorGeometry & detectorGeometry ):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _perfectTracksLabel(inputPerfectTracksLabel),
    _candidateTracksLabel(inputCandidateTracksLabel) {

    initializeHistograms();
}

void fc::CandidateCompareModule::initializeHistograms() {

    UniqueRootDirectory tdir("CandidateCompare");

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

    _hMatchedCandidates = new TH1D("MatchedCandidates", "Number matched candidates;number;N",5,-0.5,4.5);

}



void fc::CandidateCompareModule::processEvent(Event& event)
{
    Handle<TrackSet> perfectTrackSet = event.get<TrackSet>(_perfectTracksLabel);
    Handle<TrackSet> candidateTrackSet = event.get<TrackSet>(_candidateTracksLabel);

    compareCandidates(*perfectTrackSet,*candidateTrackSet);


}

void fc::CandidateCompareModule::compareCandidates(const TrackSet & perfectTrackSet,
						   const TrackSet& candidateTrackSet) const {


  for (auto const& track : perfectTrackSet.getTracks()) {

    // This is the the critical function.  You sould look for the up to four
    // candidates that match the perfect track and return the index of each one
    // from the candidatesTrackSet in the vector std::vector<int> candidates
    std::vector<int> candidates =  matchCandidates(track,candidateTrackSet);

    for (auto candidate: candidates) {
      TVectorD deltaPos = deltaHP(track,candidateTrackSet.getTracks()[candidate]);
      fillHistograms(deltaPos,candidateTrackSet.getTracks()[candidate]);
    }

  }

}



const std::vector<int> fc::CandidateCompareModule::matchCandidates(const Track & perfectTrack,
        const TrackSet& candidateTrackSet) const {

  std::vector<int> candidates;

  // loop over the candidates here and match them

    _hMatchedCandidates->Fill(candidates.size());

    return candidates;


}


// You might want to use algorithm abstraction and make a seperate function that compares one
// perfect track and one candidate track
bool fc::CandidateCompareModule::matchCandidate(const Track & perfectTrack,
        const Track& candidate) const {


  return false;
}


const TVectorD fc::CandidateCompareModule::deltaHP(const Track & track,
        const Track& candidate) const {


  return track.getHelix().getHelix() - candidate.getHelix().getHelix();
 
}

void fc::CandidateCompareModule::fillHistograms(const TVectorD & deltaHP,
        const Track& recoTrack) const {


    _hDR->Fill(recoTrack.getHelix().getDr());
    _hPhi0->Fill(recoTrack.getHelix().getPhi0());
    _hKappa->Fill(recoTrack.getHelix().getKappa());
    _hDZ->Fill(recoTrack.getHelix().getDz());
    _hTanL->Fill(recoTrack.getHelix().getTanL());

    _hSigmaDr->Fill(recoTrack.getSigmaDr());
    _hSigmaPhi0->Fill(recoTrack.getSigmaPhi0());
    _hSigmaKappa->Fill(recoTrack.getSigmaKappa());
    _hSigmaDz->Fill(recoTrack.getSigmaDz());
    _hSigmaTanL->Fill(recoTrack.getSigmaTanL());


 
    _hDeltaDr->Fill(deltaHP(0));
    _hDeltaPhi0->Fill(deltaHP(1));
    _hDeltaKappa->Fill(deltaHP(2));
    _hDeltaDz->Fill(deltaHP(3));
    _hDeltaTanL->Fill(deltaHP(4));


    _hDeltaDrPull->Fill(deltaHP(0)/recoTrack.getSigmaDr());
    _hDeltaPhi0Pull->Fill(deltaHP(1)/recoTrack.getSigmaPhi0());
    _hDeltaKappaPull->Fill(deltaHP(2)/recoTrack.getSigmaKappa());
    _hDeltaDzPull->Fill(deltaHP(3)/recoTrack.getSigmaDz());
    _hDeltaTanLPull->Fill(deltaHP(4)/recoTrack.getSigmaTanL());


}

void fc::CandidateCompareModule::endJob() {
}
