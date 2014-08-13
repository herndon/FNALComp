
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

#include "EventDisplay/include/EventDisplayModule.hh"

#include <iostream>
#define X_DISPLAY_MISSING 0

fc::EventDisplayModule::EventDisplayModule():
  _drawGenTracks(true),
  _drawGenHits(true),
  _drawRecoTracks(false),
  _drawSeedRecoTracks(false),
  _drawRecoHits(false){

}


void fc::EventDisplayModule::displayGeometry(){

  // Initialize global Eve application manager (return gEve)
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TApplication theApp("tapp",0,0);

  gROOT->SetBatch(kFALSE);
 std::cout << "batch " << gROOT->IsBatch() << " gClient " << gClient << std::endl;
 //TCanvas *c1 = new TCanvas("c1","Dynamic Filling Example",200,10,700,500);

   int temp;
   std::cin >> temp;


 std::cout << "batch " << gROOT->IsBatch() << " gClient " << gClient << std::endl;
 std::cout << "before create" << std::endl;
 std::cout << "batch " << gROOT->IsBatch() << " gClient " << gClient << std::endl;
  std::cout.flush();
   gSystem->IgnoreSignal(kSigSegmentationViolation, true);

  TEveManager::Create();

  std::cout << "before geo manager" << std::endl;
  std::cout.flush();

//--- Definition of a simple geometry
   gSystem->Load("libGeom");
   TGeoManager *geom = new TGeoManager("Assemblies",
      "Geometry using assemblies");
   Int_t i;
   //--- define some materials
   TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
   TGeoMaterial *matSi = new TGeoMaterial("Si", 26.085,14,9.37);
//   //--- define some media
   TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
   TGeoMedium *Si = new TGeoMedium("Silicon",2, matSi);

   //--- make the top container volume
   TGeoVolume *top = geom->MakeBox("TOP", Vacuum, 1.2, 1.2, 1.2);
   geom->SetTopVolume(top);

   TGeoVolume *plate0 = geom->MakeBox("SENSOR1", Si, 0.1024,0.000001,0.1024);
   plate0->SetLineColor(kBlue);
   top->AddNode(plate0,1,new TGeoTranslation(0.0,0.2,0.0));

   TGeoVolume *plate1 = geom->MakeBox("SENSOR2", Si, 0.2048,0.000001,0.2048);
   plate1->SetLineColor(kBlue);
   top->AddNode(plate1,2,new TGeoTranslation(0.0,0.4,0.0));

   TGeoVolume *plate2 = geom->MakeBox("SENSOR3", Si, 0.4096,0.000001,0.4096);
   plate2->SetLineColor(kBlue);
   top->AddNode(plate2,3,new TGeoTranslation(0.0,0.6,0.0));

   TGeoVolume *plate3 = geom->MakeBox("SENSOR3", Si, 0.4096,0.000001,0.4096);
   plate3->SetLineColor(kBlue);
   top->AddNode(plate3,4,new TGeoTranslation(0.0,0.8,0.0));

   TGeoVolume *plate4 = geom->MakeBox("SENSOR3", Si, 0.4096,0.000001,0.4096);
   plate4->SetLineColor(kBlue);
   top->AddNode(plate4,3,new TGeoTranslation(0.0,1.0,0.0));

   TGeoVolume *plate5 = geom->MakeBox("SENSOR1", Si, 0.1024,0.000001,0.1024);
   plate5->SetLineColor(kBlue);
   top->AddNode(plate5,6,new TGeoTranslation(0.0,0.2002,0.0));


   TGeoRotation *rot = new TGeoRotation();
   rot->RotateZ(1);


   TGeoVolume *plate6 = geom->MakeBox("SENSOR2", Si, 0.2048,0.000001,0.2048);
   plate6->SetLineColor(kBlue);
   top->AddNode(plate6,7,new TGeoTranslation(0.0,0.4002,0.0));

   TGeoVolume *plate7 = geom->MakeBox("SENSOR3", Si, 0.4096,0.000001,0.4096);
   plate7->SetLineColor(kBlue);
   top->AddNode(plate7,8,new TGeoTranslation(0.0,0.6002,0.0));

   TGeoVolume *plate8 = geom->MakeBox("SENSOR3", Si, 0.4096,0.000001,0.4096);
   plate8->SetLineColor(kBlue);
   top->AddNode(plate8,9,new TGeoCombiTrans(0.0,0.8002,0.0,rot));

   TGeoVolume *plate9 = geom->MakeBox("SENSOR3", Si, 0.4096,0.000001,0.4096);
   plate9->SetLineColor(kBlue);
   top->AddNode(plate9,10,new TGeoCombiTrans(0.0,1.0002,0.0,rot));



   //--- close the geometry
   geom->CloseGeometry();


   TGeoNode* node = gGeoManager->GetTopNode();
   TEveGeoTopNode* en = new TEveGeoTopNode(gGeoManager, node);
   en->SetVisLevel(4);
   en->GetNode()->GetVolume()->SetVisibility(kFALSE);


   //gEve->AddElement(list);
   //list->AddElement(track);

   //track->MakeTrack();

   //gEve->Redraw3D(kTRUE);

   gEve->AddGlobalElement(en);

  std::cout << "before draw" << std::endl;
  std::cout.flush();


  //gEve->Redraw3D(kTRUE);



  //    TEveViewer *ev = gEve->GetDefaultViewer();
  // TGLViewer  *gv = ev->GetGLViewer();
  // gv->SetGuideState(TGLUtil::kAxesOrigin, kTRUE, kFALSE, 0);
   std::cin >> temp;

    gEve->Redraw3D(kTRUE);
   std::cin >> temp;

//    gSystem->ProcessEvents();

//    gv->CurrentCamera().RotateRad(-0.5, 1.4);
//    gv->RequestDraw();




}
