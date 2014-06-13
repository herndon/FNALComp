#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "Track.hh"


// Lorentz vector like initialization
fc::Track::Track(double * p4, int charge, double * v3, double * bField, double curvatureC){

  _lorentzVector.SetPxPyPzE(p4[0],p4[1],p4[2],p4[3]);

  _geoHelix.SetXYcurvature(std::fabs(curvatureC)/_lorentzVector.Pt());
  _geoHelix.SetCharge(charge);
  _geoHelix.SetHelixStep(2*M_PI*p4[3]/curvatureC);
  _geoHelix.SetField(bField[0],bField[1],bField[2]);
  _geoHelix.InitPoint(v3[0],v3[1],v3[2]);
  _geoHelix.InitDirection(p4[0],p4[1],p4[2],kFALSE);  
  _geoHelix.UpdateHelix();

}

// Track parameter like initialization
fc::Track::Track(double pT, double pZ, int charge, double phi0, double d0, double z0, double * vertex, double * bField, double curvatureC):
  _geoHelix(TGeoHelix(1.0,0.0,1.0)){

  // !!!!! Issue. need to test setting Z direction

  _geoHelix.SetXYcurvature(std::fabs(curvatureC)/pT);
  _geoHelix.SetCharge(charge);
  _geoHelix.SetHelixStep(2*M_PI*pZ/curvatureC);
  _geoHelix.SetField(bField[0],bField[1],bField[2]);
  _geoHelix.InitPoint(charge*d0*std::sin(phi0)+vertex[0],-charge*d0*std::cos(phi0)+vertex[1],z0+vertex[2]);
  _geoHelix.InitDirection(std::cos(phi0),std::sin(phi0),0.0,kTRUE);  
  _geoHelix.UpdateHelix();

  _lorentzVector.SetPxPyPzE(pT*std::cos(phi0),pT*std::sin(phi0),pZ,std::sqrt(pT*pT+pZ*pZ));

 }

// Copy constructor
fc::Track::Track(const Track & track):
  _geoHelix(TGeoHelix(1.0,0.0,1.0)){

  
  // GeoHelix does not have a copy constructor
  // If you don't reset the helix initial points will be current points.
  // You can recover by stepping to the initial point.

  // !!!!! Issue. need to test setting Z direction

  _geoHelix.SetXYcurvature(track._geoHelix.GetXYcurvature());
  _geoHelix.SetCharge(track._geoHelix.GetCharge());
  //  _geoHelix.SetHelixStep(2*M_PI*(1/track._geoHelix.GetXYcurvature())*(track._lorentzVector.Pz()/track._lorentzVector.Pt()));
  _geoHelix.SetHelixStep(track._geoHelix.GetZStep()); 
  _geoHelix.SetField(track._geoHelix.GetField()[0],track._geoHelix.GetField()[1],track._geoHelix.GetField()[2]);
  _geoHelix.InitPoint(track._geoHelix.GetInitPoint()[0],track._geoHelix.GetInitPoint()[1],track._geoHelix.GetInitPoint()[2]);
  _geoHelix.InitDirection(track._geoHelix.GetInitDirection()[0],track._geoHelix.GetInitDirection()[1],track._geoHelix.GetInitDirection()[2],kTRUE);  
  _geoHelix.UpdateHelix();

  _lorentzVector.SetPxPyPzE(track._lorentzVector.Px(),track._lorentzVector.Py(),track._lorentzVector.Pz(),track._lorentzVector.E());

  trackHitMap inputTrackHitMap = track.getTrackHitMap();

  for (trackHitMap::const_iterator trackHitMapIter = inputTrackHitMap.begin(); trackHitMapIter != inputTrackHitMap.end(); ++trackHitMapIter){
    insertHit(trackHitMapIter->first,trackHitMapIter->second);

  }

}



void fc::Track::insertHit(int hitNumber, int layer){

   _trackHitMap.insert(trackHitMap::value_type(hitNumber,layer));

}

void fc::Track::print(void) const{


  std::cout << "Charge " << _geoHelix.GetCharge() << std::endl;
  std::cout << "4 momentum " << _lorentzVector.Px() << " " <<  _lorentzVector.Py() << " " <<  _lorentzVector.Pz() << " " <<  _lorentzVector.E() << " " << std::endl;
  std::cout << "pT " <<  _lorentzVector.Pt() << " radius of curvature " << 1.0/_geoHelix.GetXYcurvature() << std::endl;
  std::cout << "d0 " << std::sqrt(_geoHelix.GetInitPoint()[0]*_geoHelix.GetInitPoint()[0] + _geoHelix.GetInitPoint()[1]*_geoHelix.GetInitPoint()[1]) <<
    " z0 " << _geoHelix.GetInitPoint()[2] << " phi0 " << std::atan(_geoHelix.GetInitDirection()[1]/_geoHelix.GetInitDirection()[0]) << std::endl;

  std::cout << "Initial Point " << _geoHelix.GetInitPoint()[0] << " " << _geoHelix.GetInitPoint()[1] << " " << _geoHelix.GetInitPoint()[2] << std::endl;
  std::cout << "Initial Dir   " << _geoHelix.GetInitDirection()[0] << " " << _geoHelix.GetInitDirection()[1] << " " << _geoHelix.GetInitDirection()[2] << std::endl;

  trackHitMap::size_type numberHits =_trackHitMap.size();

  std::cout << "Number of hits " << numberHits << std::endl;
  
  std::cout << "Hit layers, numbers: ";
  for (trackHitMap::const_iterator trackHitMapIter = _trackHitMap.begin(); trackHitMapIter != _trackHitMap.end(); ++trackHitMapIter){
    std::cout << trackHitMapIter->second << ", " << trackHitMapIter->first << ": ";
  }
  std::cout << std::endl;


  // !!!!! to be expanded to compare all helix and lorentz vector methods

}
