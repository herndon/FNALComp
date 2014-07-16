#include<iostream>
#include<cmath>
#include<string>
#include "DetectorGeometry.hh"
#include "Track.hh"
#include "TrackSet.hh"
#include "TrackCompareModule.hh"
#include "TFile.h"
#include "TH1F.h"

fc::TrackCompareModule::TrackCompareModule(int debugLevel, 
				       const std::string& inputGenTracksLabel,
				       const std::string& inputRecTracksLabel,
				       const DetectorGeometry & detectorGeometry, TFile * outputrootfile):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _genTracksLabel(inputGenTracksLabel),
  _recTracksLabel(inputRecTracksLabel),
  _outputrootfile(outputrootfile) {


  _outputrootfile = outputrootfile;

  _roothistogramlist = new TList();
  initializeHistograms();



}

void fc::TrackCompareModule::initializeHistograms(){


  deltaD0 = new TH1F("TrackDeltaD0", "Delta d0",100, -0.001, 0.001);
  deltaD0->GetXaxis()->SetTitle("delta d0 (m)");
  deltaD0->GetYaxis()->SetTitle("N");
  _roothistogramlist->Add(deltaD0);

  deltaPhi0 = new TH1F("TrackDeltaPhi0", "Delta phi0",100, -0.005, 0.005);
  deltaPhi0->GetXaxis()->SetTitle("delta phi0 (rad)");
  deltaPhi0->GetYaxis()->SetTitle("N");
  _roothistogramlist->Add(deltaPhi0);

  deltaKappa = new TH1F("TrackDeltaKappa", "Delta kappa",100, -0.01, 0.01);
  deltaKappa->GetXaxis()->SetTitle("delta kappa (1/GeV)");
  deltaKappa->GetYaxis()->SetTitle("N");
  _roothistogramlist->Add(deltaKappa);

  deltaZ0 = new TH1F("TrackDeltaZ0", "Delta z0",100, -0.1, 0.1);
  deltaZ0->GetXaxis()->SetTitle("delta z0 (m)");
  deltaZ0->GetYaxis()->SetTitle("N");
  _roothistogramlist->Add(deltaZ0);

  deltaTanL = new TH1F("TrackDeltaTanL", "Delta tanL",100, -0.1, 0.1);
  deltaTanL->GetXaxis()->SetTitle("delta tanL");
  deltaTanL->GetYaxis()->SetTitle("N");
  _roothistogramlist->Add(deltaTanL);



}



void fc::TrackCompareModule::processEvent(Event& event)
{
 Handle<TrackSet> genTrackSet = event.get<TrackSet>(_genTracksLabel);
 Handle<TrackSet> recoTrackSet = event.get<TrackSet>(_recTracksLabel);

 compareTracks(*genTrackSet,*recoTrackSet);

  // Function to histogram results

}

void fc::TrackCompareModule::compareTracks(const TrackSet & genTrackSet, const TrackSet& recoTrackSet)
{

  
  if (recoTrackSet.getTracks().begin() == recoTrackSet.getTracks().end()) return;

  for (trackSet::const_iterator genTrackIter =  genTrackSet.getTracks().begin(); genTrackIter !=  genTrackSet.getTracks().end(); ++genTrackIter){

    const Track& recoTrack = matchTrack(*genTrackIter,recoTrackSet);
    TVectorD bestDeltaHP = deltaHP(*genTrackIter,recoTrack);
    fillHistograms(bestDeltaHP);


  }
}


const fc::Track & fc::TrackCompareModule::matchTrack(const Track & genTrack, const TrackSet& recoTrackSet){

  double bestDeltaTracks = 1000.0;
  double tmpDeltaTracks;
  int trackNumber=0;
  int bestTrack=-1;

  std::cout << "In match Track" <<std::endl;
  for (trackSet::const_iterator recoTrackIter =  recoTrackSet.getTracks().begin(); recoTrackIter !=  recoTrackSet.getTracks().end(); ++recoTrackIter,++trackNumber){

    tmpDeltaTracks = deltaTracks(genTrack,*recoTrackIter);

    std::cout << "delta Track " << tmpDeltaTracks << std::endl;
    if (tmpDeltaTracks < bestDeltaTracks) {
      bestDeltaTracks = tmpDeltaTracks;
      bestTrack = trackNumber;
    }
  }

  return recoTrackSet.getTracks()[bestTrack];

}

double fc::TrackCompareModule::deltaTracks(const Track & genTrack, const Track& recoTrack){

  return std::sqrt((genTrack.getHelix().getKappa()-recoTrack.getHelix().getKappa())*(genTrack.getHelix().getKappa()-recoTrack.getHelix().getKappa())+
		   (genTrack.getHelix().getPhi0()-recoTrack.getHelix().getPhi0())*(genTrack.getHelix().getPhi0()-recoTrack.getHelix().getPhi0()));

}

TVectorD fc::TrackCompareModule::deltaHP(const Track & genTrack, const Track& recoTrack){

  std::cout << "gen Track" << std::endl;
  genTrack.print();
  std::cout << "reco Track" << std::endl;
  recoTrack.print();

  return recoTrack.getHelix().getHelix() - genTrack.getHelix().getHelix();

}

void fc::TrackCompareModule::fillHistograms(const TVectorD & deltaHP){

  deltaD0->Fill(deltaHP(0));
  deltaPhi0->Fill(deltaHP(1));
  deltaKappa->Fill(deltaHP(2));
  deltaZ0->Fill(deltaHP(3));
  deltaTanL->Fill(deltaHP(4));

}

void fc::TrackCompareModule::endJob(){

  _roothistogramlist->Write();
  _outputrootfile->Close();

}
