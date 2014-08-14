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
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>

#include "TGeoMedium.h"

#include <TROOT.h>
#include <TCanvas.h>
#include <TApplication.h>

#include "DataObjects/include/GenHit.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Services/include/Config.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "EventDisplay/include/EventDisplayModule.hh"

#include <iostream>
#define X_DISPLAY_MISSING 0

fc::EventDisplayModule::EventDisplayModule(int debugLevel,const std::string& inputGenHitsLabel,const std::string& inputGenTracksLabel,const std::string& inputRecoTracksLabel,
					   unsigned int eventNumber,
					   const Config& config, const DetectorGeometry& detectorGeometry ):
  _debugLevel(debugLevel),
  _genHitsLabel(inputGenHitsLabel),
  _genTracksLabel(inputGenTracksLabel),
  _recoTracksLabel(inputRecoTracksLabel),
  _eventNumber(config.getEventNumberForEventDisplay()),
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
  Handle<TrackSet> recoTrackSet = event.get<TrackSet>(_recoTracksLabel);


  if (event.eventNumber() == _eventNumber) {
  displayGeometry();
   if (_debugLevel>=1) std::cout << "Exit Eve Veiwer to continue processing events." << std::endl;



   TEveTrackList *trackList = new TEveTrackList();
   TEveTrackPropagator* trackPropagator = trackList->GetPropagator();
   //trackPropagator->SetMagField(_detectorGeometry.getBField().Z());
   //trackPropagator->SetMagField(1.0);
   trackPropagator->SetMagFieldObj(new TEveMagFieldConst(0., 0., -1.0*100.0));
trackPropagator->SetMaxStep(0.01);
 
 trackList->SetElementName(Form("%s, constB", trackList->GetElementName()));


   trackPropagator->SetFitDaughters(kFALSE);
   trackPropagator->SetMaxR(1.2);
   trackPropagator->SetMaxZ(1.2);

   trackList->SetName("GenTrackList");
   gEve->AddElement(trackList);


   for (auto const& genTrack : genTrackSet->getGenTracks()){
     TEveRecTrackD *eveRecoTrack = new TEveRecTrackD();
     eveRecoTrack->fV.Set(genTrack.getPosition().X(),genTrack.getPosition().Y() , genTrack.getPosition().Z());
     eveRecoTrack->fP.Set(genTrack.getLorentzVector().Px(),genTrack.getLorentzVector().Py(), -genTrack.getLorentzVector().Pz());
     eveRecoTrack->fSign = genTrack.getCharge();

     TEveTrack* eveTrack = new TEveTrack(eveRecoTrack, trackPropagator);
     eveTrack->SetIndex(0);
     eveTrack->SetStdTitle();
     eveTrack->SetAttLineAttMarker(trackList);
     eveTrack->SetMainColor(kGreen);
     trackList->AddElement(eveTrack);
     eveTrack->MakeTrack();
 
  }

 
  TEveTrackList *recoTrackList = new TEveTrackList();
 
   TEveTrackPropagator* trackPropagator2 = recoTrackList->GetPropagator();
   trackPropagator2->SetMagFieldObj(new TEveMagFieldConst(0., 0., -1.0*100.0));
trackPropagator2->SetMaxStep(0.01);
 
 recoTrackList->SetElementName(Form("%s, constB", trackList->GetElementName()));


   trackPropagator2->SetFitDaughters(kFALSE);
   trackPropagator2->SetMaxR(1.2);
   trackPropagator2->SetMaxZ(1.2);

   recoTrackList->SetName("RecoTrackList");
   gEve->AddElement(recoTrackList);



   for (auto const& track : recoTrackSet->getTracks()){
     TEveRecTrackD *eveRecoTrack = new TEveRecTrackD();
     eveRecoTrack->fV.Set(0.0,0.0 ,0.0);
     eveRecoTrack->fP.Set(track.getLorentzVector().Px(),track.getLorentzVector().Py(), -track.getLorentzVector().Pz());
     eveRecoTrack->fSign = track.getCharge();

     TEveTrack* eveTrack = new TEveTrack(eveRecoTrack, trackPropagator);
     eveTrack->SetIndex(0);
     eveTrack->SetStdTitle();
     eveTrack->SetAttLineAttMarker(trackList);
     eveTrack->SetMainColor(kRed);
     recoTrackList->AddElement(eveTrack);
     eveTrack->MakeTrack();
 
  }



     TEveElementList* hitList = new TEveElementList("Hits"); 

      int n = 0;
   for (auto const& hit : genHitSet->getGenHits()){
    std::string hstr=" hit %d";
    std::string dstr=" hit# %d\nLayer: %d";
    std::string strlst=hstr;
    std::string strlab=dstr;

    TEvePointSet* h = new TEvePointSet(Form(strlst.c_str(),n));
    h->SetTitle(Form(strlab.c_str(),n));
    h->SetNextPoint(hit.getGenHitPosition().x(),hit.getGenHitPosition().y(),hit.getGenHitPosition().z());
    h->SetMarkerColor(kYellow);
    h->SetMarkerSize(2.0);
    hitList->AddElement(h);
    n++;

   }

   gEve->AddElement(hitList);
 




  gEve->Redraw3D(kTRUE);
  _tApp->Run(kTRUE);
  }

}

void fc::EventDisplayModule::displayGeometry(){


  std::cout << "int Eve init " << std::endl;
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
   for (auto const& sensor: _detectorGeometry.getSensors()){

     std::string geoSensorName = "SENSOR" + std::to_string(ii_layer);
     const char * geoSensorNameC = geoSensorName.c_str();
     TGeoVolume *geoSensor = geom->MakeBox(geoSensorNameC, Si, sensor._perpSize/2.0,0.0,sensor._stripPitch*sensor._nStrips/2.0);
     geoSensor->SetLineColor(kBlue);
     TGeoRotation *rot = new TGeoRotation();
     rot->RotateY(std::atan2(sensor._measurementDirection.Z(),sensor._measurementDirection.X())*180.0/M_PI);
     top->AddNode(geoSensor,ii_layer+1,new TGeoCombiTrans(sensor._center[0],sensor._center[1],sensor._center[2],rot));
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
