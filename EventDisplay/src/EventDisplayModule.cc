#define _USE_MATH_DEFINES
#include<cmath>

// ROOT includes
// ... libCore
#include <TApplication.h>
#include <TString.h>
#include <TSystem.h>
// ... libRIO
#include <TFile.h>
// ... libGui
#include <TGString.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGTextEntry.h>
#include <TGTextView.h>
#include <TGLayout.h>
#include <TGTab.h>
#include <TG3DLine.h>
// ... libGeom
#include <TGeoManager.h>
// ... libEG
#include <TParticle.h>
// ... libRGL
#include <TGLViewer.h>
// ... libEve
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveBrowser.h>
#include <TEveGeoNode.h>
#include <TEveViewer.h>
#include <TEveScene.h>
#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>
#include <TEvePointSet.h>
#include <TEveStraightLineSet.h>
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>

#include "TGeoMedium.h"

#include <TROOT.h>
#include <TCanvas.h>
#include <TApplication.h>

#include "DataObjects/include/Hit.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/GenHit.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Services/include/Config.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/TrackMatching.hh"
#include "EventDisplay/include/EventDisplayModule.hh"

#include <iostream>
#define X_DISPLAY_MISSING 0

fc::EventDisplayModule::EventDisplayModule(int debugLevel,const std::string& inputGenHitsLabel,const std::string& inputGenTracksLabel,
					   const std::string& inputRecoHitsLabel,const std::string& inputRecoTracksLabel,
					   const Config& config, const DetectorGeometry& detectorGeometry ):
  _debugLevel(debugLevel),
  _genHitsLabel(inputGenHitsLabel),
  _genTracksLabel(inputGenTracksLabel),
  _recoHitsLabel(inputRecoHitsLabel),
  _recoTracksLabel(inputRecoTracksLabel),
  _eventNumber(config.eventNumberForEventDisplay()),
  _config(config),
  _detectorGeometry(detectorGeometry),
  _drawGenTracks(true),
  _drawGenHits(true),
  _drawRecoTracks(false),
  _drawSeedRecoTracks(false),
  _drawRecoHits(false){


  }

void fc::EventDisplayModule::processEvent(Event& event) {

  Handle<GenHitSet> genHitSet = event.get<GenHitSet>(_genHitsLabel);
  Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_genTracksLabel);
  Handle<HitSet> recoHitSet = event.get<HitSet>(_recoHitsLabel);
  Handle<TrackSet> recoTrackSet = event.get<TrackSet>(_recoTracksLabel);


  if (event.eventNumber() == _eventNumber) {

    displayGeometry();

    TEveTrackList *genTrackList = new TEveTrackList();
    fillGenTrackList(*genTrackSet,*recoTrackSet,*genTrackList);
    gEve->AddElement(genTrackList);

    TEveTrackList *recoTrackList = new TEveTrackList();
    fillRecoTrackList(*recoTrackSet,*genTrackSet,*recoHitSet,*recoTrackList);
    gEve->AddElement(recoTrackList);

    TEveElementList* hitList = new TEveElementList("XHits"); 
    TEveElementList* sasZHitList = new TEveElementList("SASZHits"); 
    fillGenHitList(*genHitSet,*hitList,*sasZHitList);
    gEve->AddElement(hitList);
    gEve->AddElement(sasZHitList);
 
    gEve->Redraw3D(kTRUE);
    if (_debugLevel>=1) std::cout << "Exit Eve Veiwer to continue processing events." << std::endl;
    _tApp->Run(kTRUE);
  }

}

void fc::EventDisplayModule::displayGeometry(){


  // Instatiate TApplication, controls running root
  if (!_tApp) _tApp = new TApplication("tapp",0,0);

 
  // Initialize global Eve application manager (creates pointer gEve at global scope)
  TEveManager::Create();


  // Geometry Manager
  TGeoManager *geom = new TGeoManager("Assemblies","Geometry using assemblies");
  

  // define materials
  TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
  TGeoMaterial *matSi = new TGeoMaterial("Si", 26.085,14,9.37);
  // define media
  TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
  TGeoMedium *Si = new TGeoMedium("Silicon",2, matSi);

  // make the top container volume
  TGeoVolume *top = geom->MakeBox("TOP", Vacuum, 0.6, 1.2, 0.6);
  geom->SetTopVolume(top);


  // input sensor information from detectorGeometry
  int ii_layer=0;
  for (auto const& sensor: _detectorGeometry.sensors()){

    std::string geoSensorName = "SENSOR" + std::to_string(ii_layer);
    const char * geoSensorNameC = geoSensorName.c_str();
    TGeoVolume *geoSensor = geom->MakeBox(geoSensorNameC, Si, sensor.perpSize()/2.0,0.0,sensor.measurementSize()/2.0);
    geoSensor->SetLineColor(kBlue);
    geoSensor->	SetTransparency(40);
    TGeoRotation *rot = new TGeoRotation();
    rot->RotateY(-std::atan2(sensor.measurementDirection().Z(),sensor.measurementDirection().X())*180.0/M_PI);
    top->AddNode(geoSensor,ii_layer+1,new TGeoCombiTrans(sensor.center()[0],sensor.center()[1],sensor.center()[2],rot));
    ii_layer++;

  }

  geom->CloseGeometry();

  // Add geometry manager node to gEve
  TGeoNode* node = gGeoManager->GetTopNode();
  TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, node);
  en->SetVisLevel(4);
  en->GetNode()->GetVolume()->SetVisibility(kFALSE);
  gEve->AddGlobalElement(en);
 
  // Eve will run an event loop
  gEve->AddEvent(new TEveEventManager("Event", "FC Detector Event"));

  TEveViewer *ev = gEve->GetDefaultViewer();
  TGLViewer  *gv = ev->GetGLViewer();
  gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);
   
  _tApp->Connect( "TEveBrowser", "CloseWindow()", "TApplication", _tApp, "Terminate(=0)");
 
}

void fc::EventDisplayModule::fillGenTrackList(const fc::GenTrackSet& genTrackSet,const fc::TrackSet& recoTrackSet,TEveTrackList& trackList){
 

  TEveTrackPropagator* trackPropagator = trackList.GetPropagator();
  trackPropagator->SetMagFieldObj(new TEveMagFieldConst(_detectorGeometry.bField().X()*100.0, 
							_detectorGeometry.bField().Y()*100.0, 
							_detectorGeometry.bField().Z()*100.0));
  trackPropagator->SetMaxStep(0.01);
 
  trackPropagator->SetFitDaughters(kFALSE);
  trackPropagator->SetMaxR(1.2);
  trackPropagator->SetMaxZ(1.2);

  trackList.SetElementName(Form("%s, constB", trackList.GetElementName()));
  trackList.SetName("GenTrackList");

  bool matchedTrackLoose = false;
  bool matchedTrackXYLoose = false;
  bool matchedTrackTight = false;
  bool matchedTrackXYTight = false;

  int n = 0;
  for (auto const& genTrack : genTrackSet.genTracks()){
    TEveRecTrackD *eveRecoTrack = new TEveRecTrackD();

    double phi0ToD0 = std::atan2(genTrack.lorentzVector().Py(),genTrack.lorentzVector().Px())+genTrack.charge()*M_PI/2.0;
 
    eveRecoTrack->fV.Set(genTrack.charge()*genTrack.makeHelix(_detectorGeometry.bField(),_detectorGeometry.curvatureC()).dR()*std::cos(phi0ToD0),
			 genTrack.charge()*genTrack.makeHelix(_detectorGeometry.bField(),_detectorGeometry.curvatureC()).dR()*std::sin(phi0ToD0), 
			 genTrack.dR().Z());
    eveRecoTrack->fP.Set(genTrack.lorentzVector().Px(),genTrack.lorentzVector().Py(), genTrack.lorentzVector().Pz());
    eveRecoTrack->fSign = genTrack.charge();

    TEveTrack* eveTrack = new TEveTrack(eveRecoTrack, trackPropagator);
    eveTrack->SetIndex(n);
    eveTrack->SetStdTitle();
    eveTrack->SetLineWidth(2);
    Track track= fcf::matchTrack(genTrack,recoTrackSet,_detectorGeometry,matchedTrackLoose,matchedTrackXYLoose,matchedTrackTight,matchedTrackXYTight);
    if (matchedTrackLoose) {eveTrack->SetMainColor(210);} else {eveTrack->SetMainColor(88);}
    eveTrack->MakeTrack();
    trackList.AddElement(eveTrack);
    ++n;
 
  }
}


void fc::EventDisplayModule::fillRecoTrackList(const fc::TrackSet& recoTrackSet,const fc::GenTrackSet& genTrackSet,const fc::HitSet& recoHitSet,TEveTrackList& recoTrackList){



  TEveTrackPropagator* trackPropagator = recoTrackList.GetPropagator();
  trackPropagator->SetMagFieldObj(new TEveMagFieldConst(_detectorGeometry.bField().X()*100.0, 
							_detectorGeometry.bField().Y()*100.0, 
							_detectorGeometry.bField().Z()*100.0));
  trackPropagator->SetMaxStep(0.01);
 
  trackPropagator->SetFitDaughters(kFALSE);
  trackPropagator->SetMaxR(1.2);
  trackPropagator->SetMaxZ(1.2);

 
  recoTrackList.SetName("RecoTrackList");

  TEveStraightLineSet* lineSet = new TEveStraightLineSet();
  lineSet->SetLineWidth(2);

  bool matchedTrackLoose = false;
  bool matchedTrackXYLoose = false;
  bool matchedTrackTight = false;
  bool matchedTrackXYTight = false;

  int n = 0;
  for (auto const& track : recoTrackSet.tracks()){
    TEveRecTrackD *eveRecoTrack = new TEveRecTrackD();

    double phi0ToD0 = std::atan2(track.lorentzVector().Py(),track.lorentzVector().Px())+track.charge()*M_PI/2.0;
 
    eveRecoTrack->fV.Set(track.charge()*track.dR()*std::cos(phi0ToD0),track.charge()*track.dR()*std::sin(phi0ToD0) ,track.dZ());
    eveRecoTrack->fP.Set(track.lorentzVector().Px(),track.lorentzVector().Py(), track.lorentzVector().Pz());
    eveRecoTrack->fSign = track.charge();

    TEveTrack* eveTrack = new TEveTrack(eveRecoTrack, trackPropagator);
    eveTrack->SetIndex(n);
    eveTrack->SetStdTitle();
    eveTrack->SetLineWidth(2);
    GenTrack genTrack= fcf::matchTrack(track,genTrackSet,_detectorGeometry,matchedTrackLoose,matchedTrackXYLoose,matchedTrackTight,matchedTrackXYTight);
    if (matchedTrackLoose) {eveTrack->SetMainColor(kRed);} else {eveTrack->SetMainColor(kOrange-3);}
    if (!matchedTrackLoose){
      //hit lines here
      for (auto const hitNumber : track.trackHits()){
	const Hit& hit = recoHitSet.hits()[hitNumber];
	TVector3 hitPosition = hit.position();
	TVector3 stripDir = _detectorGeometry.sensor(hit.layer()).perpDirection();
	stripDir *= _detectorGeometry.sensor(hit.layer()).perpSize()/2.0;
	lineSet->AddLine(hitPosition.X()-stripDir.X(),hitPosition.Y()-stripDir.Y()+0.001,hitPosition.Z()-stripDir.Z(),
			 hitPosition.X()+stripDir.X(),hitPosition.Y()+stripDir.Y()+0.001,hitPosition.Z()+stripDir.Z());
      }
      ++n;
    }

    eveTrack->MakeTrack();
    recoTrackList.AddElement(eveTrack);
 
  }

  lineSet->SetLineColor(6);
  gEve->AddElement(lineSet);

}

void fc::EventDisplayModule::fillGenHitList(const fc::GenHitSet& genHitSet,TEveElementList& hitList,TEveElementList& sasZHitList){

  int n = 0;
  for (auto const& hit : genHitSet.genHits()){
    std::string hstr=" hit %d";
    std::string dstr=" hit# %d\nLayer: %d";
    std::string strlst=hstr;
    std::string strlab=dstr;

    TEvePointSet* h = new TEvePointSet(Form(strlst.c_str(),n));
    h->SetTitle(Form(strlab.c_str(),n));
    double yPos = hit.position().y();
    if (hit.layer() < _detectorGeometry.nXSensors()) yPos -= 0.001;
    if (hit.layer() >= _detectorGeometry.nXSensors()) yPos += 0.001;

    h->SetNextPoint(hit.position().x(),yPos,hit.position().z());
    if (hit.layer() < _detectorGeometry.nXSensors()) h->SetMarkerColor(kYellow);
    if (hit.layer() >= _detectorGeometry.nXSensors()) h->SetMarkerColor(kYellow);
    h->SetMarkerSize(1.5);
     if (hit.layer() < _detectorGeometry.nXSensors())  hitList.AddElement(h);
     if (hit.layer() >= _detectorGeometry.nXSensors())  sasZHitList.AddElement(h);
    n++;

  }

}
