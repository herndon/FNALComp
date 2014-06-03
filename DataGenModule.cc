#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<cmath>
#include "Random.hh"
#include "DetectorGeometry.hh"
#include "StripSet.hh"
#include "DataGenModule.hh"
//#include "TGeoHelix.h"

void DataGenModule::run(void)
{


  // Physical constants, !! move to header/include file
  const double C = 2.99792458e8;

  // binary output
  std::ofstream stripdata;
  stripdata.open("stripdata.bin",std::ios::binary);

  // Debugging
  // Eventually this will come from the framework
  int debugLevel = 5;
  _debugLevel = debugLevel;
  // six levels of degug
  // 5 most verbose with all printouts
  // 1 basic information on progress
  // 0 none


  // Intialize DetectorGeomentry and StripSet
  // Eventually these will come from the framework
  DetectorGeometry myDetectorGeometry;
  setDetectorGeometry(myDetectorGeometry);
  _myDetectorGeometry->_initSensorGeometryFromFile();
  if (_debugLevel > 0) _myDetectorGeometry->printSensorGeometry();
  StripSet myStripSet;
  setStripSet(myStripSet);

  // Intialize commonly used DetectorGeometry data
  int nLayers = _myDetectorGeometry->getNSensors();
  _nLayers = nLayers;
  _curvatureC = _myDetectorGeometry->getZBField()*C/1.0e9;



  for (int ii_event = 0; ii_event < 128; ++ii_event) {

    
    for (int ii_track = 0; ii_track < 10; ++ii_track) {

      generateTrack();

      calculateHitPositions();

    } // end track loop

    _myStripSet->writeEvent(stripdata, ii_event);

    _myStripSet->clear();

  } // end event loop

}

void DataGenModule::setDetectorGeometry(DetectorGeometry & myDetectorGeometry){

  _myDetectorGeometry = & myDetectorGeometry;

}

void DataGenModule::setStripSet(StripSet & myStripSet){

  _myStripSet = & myStripSet;

}


void DataGenModule::generateTrack(){


  // Generate track data
    
  // Track pT and charge
  double trackPT = getUniformDouble()*10.0 + 10.0;
  int trackCharge = (getUniformDouble() > 0.5) ? 1 : -1;
  double trackPhi0 = getUniformDouble()*2.0*M_PI/6.0 - 1.0*M_PI/6.0 + M_PI/2.0;
 
  // Details of curvature calculation
  //using p = BqR, 
  //multiply by c to get energy in J  pc = BqRc, 
  //divide by e to get energy in eV pc = BqR3x108/1.6x10-19
  // q = Q1.6x10-19 where Q = 1, -1...
  //divide by 109 pc (in GeV) = BQR3x108/(1x10^9)

  double trackCurvature = (_curvatureC)/trackPT;


 if (_debugLevel >=5 ) {
    std::cout << "Track pT " << trackPT << std::endl;
    std::cout << "Track Charge " << trackCharge << std::endl;
    std::cout << "Track phi0 " << trackPhi0 << std::endl;
    std::cout << "Track radius curvature " << 1/trackCurvature << std::endl;
  }
 

  if (_helix) delete _helix;

  _helix = new TGeoHelix(std::fabs(trackCurvature),0.0,trackCharge);
  _helix->SetXYcurvature(std::fabs(trackCurvature));
  _helix->SetCharge(trackCharge);
  _helix->SetHelixStep(0.0);
  _helix->SetField(0.0,0.0,_myDetectorGeometry->getZBField());
  _helix->InitPoint(0.0,0.0,0.0);
  _helix->InitDirection(std::cos(trackPhi0),std::sin(trackPhi0),0.0,kTRUE);  

}


void DataGenModule::calculateHitPositions(){

  // x coordiate of hits
  double xHitPosition[_nLayers];
  // y coordinate of hits.  Eqauls Y postion of sensor layers
  double yHitPosition[_nLayers];
  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {
    std::cout << "layer " << ii_layer << std::endl;
    yHitPosition[ii_layer] = _myDetectorGeometry->getSensor(ii_layer)._yPos;
  }

  // Resolution Smeared Hit positions
  double xHitPositionS[_nLayers];

  // Strip postions and ADC counts
  int xStripNumber[_nLayers][2];
  int xStripADC[_nLayers][2];

  Double_t point[3];
  Double_t norm[3];
  point[0] = 0.0; 
  point[2]=0.0;

  norm[0] = 0.0;
  norm[2] = 0.0;
  norm[1] = 1.0;
  
  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer) {

  point[1]=_myDetectorGeometry->getSensor(ii_layer)._yPos;

  _helix->StepToPlane(point,norm);

  xHitPosition[ii_layer] = _helix->GetCurrentPoint()[0];


    if (_debugLevel >=5 ) {
      std::cout << "Layer " << ii_layer << " Hit y " << yHitPosition[ii_layer] << std::endl;
      std::cout << "Layer " << ii_layer << " Hit x " << xHitPosition[ii_layer] << std::endl;
    }

    xHitPositionS[ii_layer] = xHitPosition[ii_layer] + getNormalDouble()*0.005;


    // Generate strip data by spliting charge across nearest strips
    double stripPitch;
    stripPitch = _myDetectorGeometry->getSensor(ii_layer)._stripPitch;

    int strip = (int) (xHitPositionS[ii_layer]/stripPitch);
    double remainder = (xHitPositionS[ii_layer]/stripPitch) - strip;

    if (remainder < 0.0) {
      xStripNumber[ii_layer][0] = strip - 1 + 1024;
      xStripNumber[ii_layer][1] = strip + 1024;
      xStripADC[ii_layer][0] = -1.0*remainder*32.0;
      xStripADC[ii_layer][1] = (1.0+remainder)*32.;


    } else {
      xStripNumber[ii_layer][0] = strip + 1024;
      xStripNumber[ii_layer][1] = strip+1 + 1024;
      xStripADC[ii_layer][0] = 1.0*remainder*32.0;
      xStripADC[ii_layer][1] = (1.0-remainder)*32.0;
 
    }

    // Store data in strip set.
    // !!!!! upgrade could be to move data binary output/input to StripSet
    _myStripSet->insertStrip(ii_layer,xStripNumber[ii_layer][0],xStripADC[ii_layer][0]);
    _myStripSet->insertStrip(ii_layer,xStripNumber[ii_layer][1],xStripADC[ii_layer][1]);



    if (_debugLevel >=5 ) {
      std::cout << "Layer " << ii_layer << " Strip 0 " << xStripNumber[ii_layer][0] << " ADC " << xStripADC[ii_layer][0] << std::endl;
      std::cout << "Layer " << ii_layer << " Strip 1 " << xStripNumber[ii_layer][1] << " ADC " << xStripADC[ii_layer][1] << std::endl;
    }


  } // end layer loop




}


void DataGenModule::writeStripSetEvent(std::ofstream & stripdata, int ii_event){

  const int bitmask1 = 0x00FF;
  const int bitmask2 = 0xFF00;


  int binaryData;
  int binaryData1;
  int binaryData2;

  stripdata.write (reinterpret_cast<const char *>(&ii_event), 1);

  // test difference between passing by ref and by value.
  //   for (int ii_fun = 0; ii_fun < 10000; ++ii_fun){

  // Works using pass by reference or value
  for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer){

    const layerMap& myLayerMap =  _myStripSet->getLayerMap(ii_layer);

    std::map<int,int>::size_type numberStrips = myLayerMap.size();
    if (_debugLevel >= 5) {
      std::cout << "Writing layer " << ii_layer << " number Strips " << numberStrips << std::endl; 
    }
 
    stripdata.write (reinterpret_cast<const char *>(&ii_layer), 1);
    stripdata.write (reinterpret_cast<const char *>(&numberStrips), 1);
            
    for (layerMap::const_iterator layerMapIter = myLayerMap.begin(); layerMapIter !=  myLayerMap.end(); ++layerMapIter){
      if (_debugLevel >= 5) {
	
	std::cout << "Writing Strip " << layerMapIter->first << " ADC " << layerMapIter->second << std::endl;
      }

      binaryData = layerMapIter->first * 32 + layerMapIter->second;
      binaryData1 = binaryData & bitmask1;
      binaryData2 = binaryData & bitmask2;
      binaryData2 = binaryData2 >> 8;

      stripdata.write (reinterpret_cast<const char *>(&binaryData2), 1);
      stripdata.write (reinterpret_cast<const char *>(&binaryData1), 1);
    }
  }

  // But don't do it wrong. Passing by value can be dangerous
  // Using pass by reference, will break for pass by value!
  //     for (int ii_layer = 0; ii_layer < _nLayers; ++ii_layer){


  //       std::map<int,int>::size_type numberStrips = _myStripSet->getLayerMap(ii_layer).size();
  //       if (_debugLevel >= 5) {
  //       std::cout << "Writing layer " << ii_layer << " number Strips " << numberStrips << std::endl; 
  //       }
 
  //       stripdata.write (reinterpret_cast<const char *>(&ii_layer), 1);
  //       stripdata.write (reinterpret_cast<const char *>(&numberStrips), 1);
            
  //       for (layerMap::iterator layerMapIter = _myStripSet->getLayerMap(ii_layer).begin(); layerMapIter !=  _myStripSet->getLayerMap(ii_layer).end(); ++layerMapIter){
  // 	//if (_debugLevel >= 5) {
  // 	std::cout << "Writing Strip " << layerMapIter->first << " ADC " << layerMapIter->second << std::endl;
  // 	//}


  // 	binaryData = layerMapIter->first * 32 + layerMapIter->second;
  // 	binaryData1 = binaryData & bitmask1;
  // 	binaryData2 = binaryData & bitmask2;
  // 	binaryData2 = binaryData2 >> 8;

  // 	stripdata.write (reinterpret_cast<const char *>(&binaryData2), 1);
  // 	stripdata.write (reinterpret_cast<const char *>(&binaryData1), 1);
  //       }
  //     }


}
