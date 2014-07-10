#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "HitSet.hh"
#include "DetectorGeometry.hh"
#include "TrackFit.hh"
#include "Helix.hh"


// Helix initialization

fc::TrackFit::TrackFit(const Helix & helix,const DetectorGeometry & detectorGeometry):
   _helix(helix),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/_detectorGeometry.getCurvatureC()){

}




// // Helix parameter initialization
// fc::TrackFit::TrackFit(double kappa, double dr, double dz, double phi0, double tanL, const DetectorGeometry & detectorGeometry):
//   _helix(dr,phi0+M_PI/2.0,-1.0*kappa,dz,tanL),
//   _covMatrix(NULL),
//   _detectorGeometry(detectorGeometry),
//   _alpha(1.0/_detectorGeometry.getCurvatureC()),
//   _debugLevel(0) {



// }



fc::TrackFit::TrackFit(const TVector3 & x1, const TVector3 & x2, const TVector3 & x3, const DetectorGeometry & detectorGeometry, int debugLevel):
  _covMatrix(NULL),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/_detectorGeometry.getCurvatureC()),
  _debugLevel(debugLevel){


  // !!!!! change this to just take a hit map and HitSet for input
  // !!!!! Move calculation to function that can be called my either 3 hit or more hit function

  // Use three Cartesion points to determine a helix parameters in x,y
  // Pivot point is set to x1.  Have to be set to 0,0,0 or primary vertex if we want to dr and dz to distances to that point
  // Calculation is in 2D 

   
  // Calculate vector between the points in xy to determing rphi coordinates
  TVector3 x12 = x2 - x1;
  TVector3 x13 = x3 - x1;
  TVector3 x23 = x3 - x2;
  TVector3 z(0., 0., 1.);

  x12.SetZ(0.);
  x13.SetZ(0.);
  x23.SetZ(0.);

  // Magnitudes and unit vectors to simplify angle calculates
  double x12Mag = x12.Mag();
  x12 = x12.Unit();
  double x13Mag = x13.Mag();
  x13 = x13.Unit();
  double x23Mag = x23.Mag();
  x23 = x23.Unit();

   
  // Find radius of curvature
  double sinPhi23 = x12.Cross(x13).Z();
  double cosPhi23 = 0.5 * (x13Mag/x12Mag + (1. - x23Mag/x12Mag)*(x12Mag + x23Mag)/x13Mag);
  double phi23 = std::atan2(sinPhi23, cosPhi23);
  double radiusCurvature  = -0.5 * x23Mag / sinPhi23;

  // Find center of curvature
  TVector3 centerCurvature = 0.5 * (x2 + x3) + radiusCurvature * cosPhi23 * x23.Cross(z);

  TVectorD helix(5);
  

  helix(0) = 0.0;
  helix(1) = std::atan2(radiusCurvature * (centerCurvature.Y() - x1.Y()), radiusCurvature * (centerCurvature.X() - x1.X()));
  helix(2) = _alpha / radiusCurvature;
  helix(3) = 0.0;
  helix(4) =  (x2.Z() - x3.Z()) / (radiusCurvature * 2 * phi23);

  _helix.setHelix(helix);

  // Set Initial covariance matrix
  if (_covMatrix) { delete _covMatrix;}
  _covMatrix = new TMatrixD(Helix::_sDim,Helix::_sDim);

  _covMatrix->Zero();
  for (int ii = 0; ii < Helix::_sDim; ++ii){
    (*_covMatrix)(ii,ii) = 1.0e4;
  }


}


void fc::TrackFit::insertHit(int hitNumber, int layer){
   _trackHitMap.insert(trackHitMap::value_type(hitNumber,layer));
}


TMatrixD fc::TrackFit::expectedMeasurementVectorXZ(int layer, const DetectorGeometry & detectorGeometry) const{

  TVector3 hitPosition;

  double phi = 0.0;

  if (layer>0) intersectWithPlane(hitPosition,detectorGeometry.getSensor(layer)._center,detectorGeometry.getSensor(layer)._normal,phi);
  if (layer==-1) {
    TVector3 center(0.0,0.0,0.0);
    TVector3 normal(0.0,1.0,0.0); 
    intersectWithPlane(hitPosition,center,normal,phi);
  }

  TMatrixD expectedMeasurementVector(DetectorGeometry::_mDim,1);


  expectedMeasurementVector.Zero();

  expectedMeasurementVector(0,0) = hitPosition.Perp()*hitPosition.Phi();
  expectedMeasurementVector(1,0) = hitPosition.Z();
  
  return expectedMeasurementVector;

}

TMatrixD fc::TrackFit::expectedMeasurementDerivativedXZdHC(int layer, const DetectorGeometry & detectorGeometry) const{

  TVector3 hitPosition;

  double phi = 0.0;

  // advances phi to the phi at crossing!

  intersectWithPlane(hitPosition,detectorGeometry.getSensor(layer)._center,detectorGeometry.getSensor(layer)._normal,phi);



  // get normal
  // !!!!! could this be simpler and probably has to be in the correct coordinates. called dsdx
  TMatrixD detectorNormal(1,3);
  detectorNormal(0,0) = detectorGeometry.getSensor(layer)._normal.X();
  detectorNormal(0,1) = detectorGeometry.getSensor(layer)._normal.Y();
  detectorNormal(0,2) = detectorGeometry.getSensor(layer)._normal.Z();


  TMatrixD dxdHC(3,Helix::_sDim); 
  dxdHC = calcDxDHC(phi);
  TMatrixD dxdphi(3,1);
  dxdphi = calcDxDphi(phi); 

  TMatrixD dphidHC(1,Helix::_sDim);
  dphidHC = detectorNormal*dxdHC;
  TMatrix dsdphi(1,1);
  dsdphi = detectorNormal*dxdphi; 
  double denom = -dsdphi(0,0);
  dphidHC *= 1/denom;
  
  TMatrixD dxphidHC(3,Helix::_sDim);
  dxphidHC = dxdphi*dphidHC + dxdHC;

 
  TMatrixD expectedMeasurementDerivative(DetectorGeometry::_mDim,Helix::_sDim);
  expectedMeasurementDerivative.Zero();

  expectedMeasurementDerivative=  calcDXZDHC(hitPosition,dxphidHC);
  
  return expectedMeasurementDerivative;

}

TMatrixD fc::TrackFit::invMeasurementRes2XZ(int layer, const DetectorGeometry & detectorGeometry) const{

  TMatrixD invMeasurementRes2XZ(DetectorGeometry::_mDim,DetectorGeometry::_mDim);

  invMeasurementRes2XZ.Zero();

  invMeasurementRes2XZ(0,0) = detectorGeometry.getSensor(layer)._hitResolution*detectorGeometry.getSensor(layer)._hitResolution;
  invMeasurementRes2XZ(1,1) = 1.0;
  invMeasurementRes2XZ.Invert(); 

  return invMeasurementRes2XZ;

}

TMatrixD fc::TrackFit::measurementVectorXZ(const TVector3 & hitPosition) const{

  TMatrixD measurementVector(DetectorGeometry::_mDim,1);
  measurementVector(0,0) = hitPosition.Perp()*std::atan2( hitPosition.y(),hitPosition.x());
  measurementVector(1,0) = hitPosition.z();

  return measurementVector;

}


bool fc::TrackFit::intersectWithLayer(TVector3 & hitPosition, int layer, const DetectorGeometry & detectorGeometry) const{

  double phi = 0.0;

  return intersectWithPlane(hitPosition,detectorGeometry.getSensor(layer)._center,detectorGeometry.getSensor(layer)._normal,phi);
}


bool fc::TrackFit::intersectWithPlane(TVector3 & hitPosition, const TVector3 & center, const TVector3 & normal,double & phi) const{


   static const int       maxcount   = 100;
   static const double    initlambda = 1.e-10;
   static const double    lambdaincr = 10.;
   static const double    lambdadecr = 0.1;
   
   hitPosition = calcXAt(phi);

 
   double  eps = 1.0e-8;
   double  lastphi =  phi;
   double  lasts   =  99999;
   double  lambda  =  initlambda;

   TVector3  lastHitPosition  =  hitPosition;
   int     count   =  0;

   double s;

   while (1) {
      if (count > maxcount) {
// 	s       = lasts;
// 	phi     = lastphi;
// 	hitPosition      = lastHitPosition;
// 	std::cerr << "TVSurface::CalcXingPointWith:"
// 		  << "   --- Loop count limit reached ---------- " << std::endl
// 		  << "   phi    : " << phi    << std::endl
// 		  << "   x      : " << hitPosition.X() << " "
// 		  << hitPosition.Y() << " "
// 		  << hitPosition.Z() << std::endl 
// 		  << "   s      : " << s      << std::endl
// 		  << "   lambda : " << lambda << std::endl;
         return 0;
      }
	 // put in debug
// 	 std::cerr << "TVSurface::CalcXingPointWith:"
// 		   << "   phi    : " << lastphi    << std::endl
//                    << "   x      : " << lastHitPosition.X() << " "
//                                 << lastHitPosition.Y() << " "
// 		   << lastHitPosition.Z() << std::endl 
// 		   << "   s      : " << lasts      << std::endl
// 		   << "   lambda : " << lambda << std::endl;
//       count++;
      s  = (hitPosition-center)*normal;
      if (std::abs(s) < eps) break;
      if (std::abs(s) < std::abs(lasts)) {
         lasts   = s;
         lastphi = phi;
         lastHitPosition  = hitPosition;
         lambda *= lambdadecr;
      } else {
         s       = lasts;
         phi     = lastphi;
         hitPosition      = lastHitPosition;
         lambda *= lambdaincr;
      }
      TMatrixD dsdx(1,3);
      dsdx(0,0) = normal.X();
      dsdx(0,1) = normal.Y();
      dsdx(0,2) = normal.Z();

      TMatrixD dxdphi = calcDxDphi(phi);
      TMatrixD dsdphi = dsdx * dxdphi;
      double denom = (1 + lambda) * dsdphi(0,0);
      phi -= s / denom;
      hitPosition   = calcXAt(phi);
   }
   return (hitPosition - center) * normal == 0. ? 1 : 0; 

}

TVector3 fc::TrackFit::calcXAt(double phi) const
{
  double csf0 = std::cos(getHelix().getPhi0());
   double snf0 = std::sin(getHelix().getPhi0());
   double snfd = std::sin(getHelix().getPhi0() + phi);
   double csfd = std::cos(getHelix().getPhi0() + phi);
   double rho  = _alpha/getHelix().getKappa();

   double x    = getHelix().getDr() * csf0 + rho * (csf0 - csfd);
   double y    = getHelix().getDr() * snf0 + rho * (snf0 - snfd);
   double z    = getHelix().getDz()          - rho * getHelix().getTanL() * phi;

   return TVector3(x,y,z);
}

TMatrixD fc::TrackFit::calcDxDphi(double phi) const
{
   double radiusCurvature    = _alpha/getHelix().getKappa();

   double snfd = std::sin(getHelix().getPhi0() + phi);
   double csfd = std::cos(getHelix().getPhi0() + phi);

   TMatrixD dxdphi(3,1);
   dxdphi(0,0) =  radiusCurvature * snfd;
   dxdphi(1,0) = -radiusCurvature * csfd;
   dxdphi(2,0) = -radiusCurvature * getHelix().getTanL();

   return dxdphi;
}



TMatrixD fc::TrackFit::calcDxDHC(double phi) const {
   double radiusCurvature     = _alpha/getHelix().getKappa();
   double rcpar = radiusCurvature/getHelix().getKappa();

   double snf0 = std::sin(getHelix().getPhi0());
   double csf0 = std::cos(getHelix().getPhi0());
   double snfd = std::sin(getHelix().getPhi0() + phi);
   double csfd = std::cos(getHelix().getPhi0() + phi);

   TMatrixD dxDHC(3,5);
   // @x/@a
   dxDHC(0,0) =  csf0;
   dxDHC(0,1) = -getHelix().getDr() * snf0 - radiusCurvature * (snf0 - snfd);
   dxDHC(0,2) = -rcpar * (csf0 - csfd);
   dxDHC(0,3) =  0;
   dxDHC(0,4) =  0;

   // @y/@a
   dxDHC(1,0) =  snf0;
   dxDHC(1,1) =  getHelix().getDr() * csf0 + radiusCurvature *(csf0 - csfd);
   dxDHC(1,2) = -rcpar * (snf0 - snfd);
   dxDHC(1,3) =  0;
   dxDHC(1,4) =  0;

   // @z/@a
   dxDHC(2,0) = 0;
   dxDHC(2,1) = 0;
   dxDHC(2,2) = rcpar * phi * getHelix().getTanL();
   dxDHC(2,3) = 1;
   dxDHC(2,4) = - radiusCurvature * phi;

   return dxDHC;
}


TMatrixD fc::TrackFit::calcDXZDHC(const TVector3   & hitPosition,
			       const TMatrixD &dxphiadHC)  const  // projector matrix = @h/@a
{
  // Calculate
  //    dXZdHC = (@measVec/@a) = (r@phi/@HC, r@z/@HC)^t

  double r = hitPosition.Perp();


  // Set dX = (@h/@a) = (@d/@a, @z/@a)^t
  TMatrixD dXZdHC(DetectorGeometry::_mDim,Helix::_sDim);   

  for (Int_t i=0; i<Helix::_sDim; i++) {
    dXZdHC(0,i) = - (hitPosition.Y() / r) * dxphiadHC(0,i) 
      + (hitPosition.X() / r) * dxphiadHC(1,i);
    dXZdHC(0,i) *= r;
    dXZdHC(1,i) =  dxphiadHC(2,i);
  
  }
  return dXZdHC;
}

void fc::TrackFit::FitToHelix(const HitSet & hitSet,const DetectorGeometry & detectorGeometry,bool withPrimaryVertex)
{


  // Define static constants...
 
  static const double chi2Dummy = 1.e20;
  static const double chi2Tol = 1.e-8;
  static const double deltaIncr   = 10.;
  static const double deltaDecr   = 0.1;
  static const int    loopMax = 1000;

  double delta  = 1.0;


  // for the  TrackFit  helix values set by generating or by hits
  //                          initial error matrix diagonal with 1.e4 on all elements
  //                          need to be set befreo FitToHelix


  TVectorD    helix(Helix::_sDim); // original helix paramters

  helix = getHelix().getHelix();

  TVector3 newReferencePoint(0.0,0.0,0.0);


  Helix newHelix(_helix);


  TVectorD testMatrix(Helix::_sDim);
  testMatrix = newHelix.getHelix();


  //std::cout << "Test helix " << getHelix().getTanL() << " " << testHelix.getKappa() << " " << testMatrix(2) << std::endl;


    std::cout << "Test helix " << getHelix().getKappa() << " " << newHelix.getKappa() << " " << testMatrix(2) << " " << getHelix().getKappa() << " " << (getHelix().getHelix())(2) << std::endl;

  if (_debugLevel >= 3){
    std::cout << "In FitToHelix" << std::endl;
    std::cout << "Initial Helix: "  << std::endl;
    helix.Print();
    std::cout.precision(std::numeric_limits<double>::digits10 + 2);
    std::cout << "Initial Helix " << helix(0) << " " << helix(1)  << " " << helix(2)  << " " << helix(3)  << " " << helix(4) << std::endl;
  }

  helix(2) = helix(2) + 0.01;
  _helix.setHelix(helix);

 
  if (_debugLevel >= 3){
    std::cout << "Initial Helix Modified: "  << std::endl;
    helix.Print();
    std::cout.precision(std::numeric_limits<double>::digits10 + 2);
    std::cout << "Initial Helix modified: " << helix(0) << " " << helix(1)  << " " << helix(2)  << " " << helix(3)  << " " << helix(4) << std::endl;
  }

  TVectorD helixBest(helix);
  TVectorD helixSave(helix);

  double   chi2best = chi2Dummy;
  //double   deltabest   = delta;
  int      nloops   = 0;

  //int ndf = 0;
  double   chi2     = 0.;

  // Prepare some matrices
 
  // Note on parameters and derivatives
  // XZ is the measurement plane of the sensors.  This allows the possibility of z measurements
  // XYZ is cartesian coordinates
  // HC is helix coordinates
  // positions on the helix are determined by a single paramter phi


  TMatrixD    dchi2dHCbest(1   , Helix::_sDim); // best based on chi2 Helix::_sDim contributions to chi2?
  TMatrixD    dchi2dHC    (1   , Helix::_sDim);
  TMatrixD    dchi2dHCT   (Helix::_sDim , 1  );
  TMatrixD    d2chi2dHCdHC (Helix::_sDim, Helix::_sDim);
  TMatrixD    d2chi2dHCdHCbest (Helix::_sDim, Helix::_sDim);
  TMatrixD    expectedMeasurementVector       (DetectorGeometry::_mDim, 1   );
  TMatrixD    expectedMeasurementDerivative (DetectorGeometry::_mDim, Helix::_sDim);
  TMatrixD    expectedMeasurementDerivativeT (Helix::_sDim, DetectorGeometry::_mDim);
  TMatrixD    measurementVector(DetectorGeometry::_mDim,1);
  TMatrixD    curHt      (Helix::_sDim, DetectorGeometry::_mDim);
  TMatrixD    invMeasurementRes2    (DetectorGeometry::_mDim, DetectorGeometry::_mDim);
  TMatrixD    measurementResidualVector  (DetectorGeometry::_mDim, 1   );
  TMatrixD    measurementResidualVectorT (1, DetectorGeometry::_mDim);

  // Minimization loop starts here

  while (1) {
    if (_debugLevel >= 5){
      std::cout << "FitTo Helix minimization loop, nloop: " << nloops << std::endl;
    }
    if (nloops > loopMax) {
      // Replace with Exception call 
      if (_debugLevel >= 5){
	std::cout << "TrackFit::FitToHelix >>>>>>>>>>>>>>"
		  << " Loop count limit reached. nloops = " << nloops << std::endl;
	}
 
      _helix.setHelix(helixBest);

      chi2  = chi2best;
      break;
    }
    nloops++;

    d2chi2dHCdHC.Zero();
    dchi2dHC.Zero();
    chi2 = 0;

 
 

    for (trackHitMap::const_iterator trackHitMapIter = _trackHitMap.begin(); trackHitMapIter != _trackHitMap.end(); ++trackHitMapIter){

      // Hit information
      int layer = trackHitMapIter->second;
      TVector3 hitPosition = hitSet.getHitVector()[trackHitMapIter->first].getHitPosition();

 
      // Find XZ vector to measurement
      expectedMeasurementVector = expectedMeasurementVectorXZ(layer,detectorGeometry); 

      if (_debugLevel >= 5){
	std::cout << "expectedVector XZ mDim:" << std::endl;
	expectedMeasurementVector.Print();
      }

      // Find how the measurement expectation varies with each helix coordinate
      expectedMeasurementDerivative = expectedMeasurementDerivativedXZdHC(layer,detectorGeometry);
      expectedMeasurementDerivativeT.Transpose(expectedMeasurementDerivative);
 
      if (_debugLevel >= 5){
	std::cout << "expectedDerivative dXZdHC  mDim x sDim:" << std::endl;
	expectedMeasurementDerivative.Print();
      }

      // Get the resolutions squared for each measruement direction,

      //dchi2/da
      // This could be done better
 
      invMeasurementRes2 = invMeasurementRes2XZ(layer,detectorGeometry);
 
      if (_debugLevel >= 5){
	std::cout << " invMeasurementRes2 XZ mDim x mDim: " << std::endl;
	invMeasurementRes2.Print();
      }

      // Hit position in XZ

      measurementVector = measurementVectorXZ(hitPosition);

      // Residuals in XZ

      measurementResidualVector = measurementVector - expectedMeasurementVector; // just subtracts helix position and measurement point to get residual
      measurementResidualVectorT.Transpose(measurementResidualVector);

      if (_debugLevel >= 5){
	std::cout << "measurementVector XZ " << measurementVector(0,0) << " " << measurementVector(1,0) << std::endl;
	std::cout << "expectedMeasurementVector XZ " << expectedMeasurementVector(0,0) << " " << expectedMeasurementVector(1,0) << std::endl;
	std::cout << "measurementResidualVector XZ " << measurementResidualVector(0,0) << " " << measurementResidualVector(1,0) << std::endl;
      }


      // Accumulate chi2
      double delchi2 = (measurementResidualVectorT * invMeasurementRes2 * measurementResidualVector)(0,0);
      chi2 += delchi2; 
          
      if (_debugLevel >=3){
	std::cout << "Chi2 contribution: " << delchi2 << " total chi2: " << chi2 << std::endl;
      }         


      // Calculate derivatires of the chi2 with respect the helix parameters for each hit and accumulate
      dchi2dHC += (measurementResidualVectorT * invMeasurementRes2 * expectedMeasurementDerivative); 
      d2chi2dHCdHC += (expectedMeasurementDerivativeT * invMeasurementRes2 * expectedMeasurementDerivative);

      if (_debugLevel >=5){
	std::cout << " dchi2dHC total:" << std::endl;
	dchi2dHC.Print();
	std::cout << "d2chi2dHCdHC total:" << std::endl;
	d2chi2dHCdHC.Print();
      }

    }// end hit loop

    //
    // if (chi2best - chi2) < chi2Tol, break while loop.
    //

    if (TMath::Abs(chi2best - chi2) < chi2Tol) {
      if (_debugLevel >=3){
	std::cout << "Breaking loop, chi2 " << chi2 << " chi2best " << chi2best << " chi2Tol " << chi2Tol << std::endl; 
      }
      d2chi2dHCdHCbest = d2chi2dHCdHC;
      break;
    }

    // !!!!! need to make sure track is saved and restored 
    if (chi2 < chi2best) {
      // chi2 decreased. Save this step as the current best
      if (_debugLevel >=3){
	std::cout << "Chi2 decreased " << chi2 << " compared to " << chi2best << std::endl;
      } 
      helixSave = helix;
      helixBest = helix;
      chi2best    = chi2;
      dchi2dHCbest = dchi2dHC;
      d2chi2dHCdHCbest  = d2chi2dHCdHC;
      delta     *= deltaDecr;
    } else {
      // chi2 increased. Restore the current best
      if (_debugLevel >=3){
	std::cout << "Chi2 increased " << chi2 << " compared to " << chi2best << std::endl;
      } 
      helix = helixSave;
      helixBest = helixSave; 
      dchi2dHC     = dchi2dHCbest;
      d2chi2dHCdHC  = d2chi2dHCdHCbest;
      delta     *= deltaIncr;
    }

    // Add delta to 2nd derivative and use to modify the helix

    for (int i=0; i<Helix::_sDim; i++) {
      d2chi2dHCdHC(i, i) *= (1 + delta);
    }
      
       
    TMatrixD d2chi2dHCdHCinv = d2chi2dHCdHC;
    d2chi2dHCdHCinv.Invert();

    dchi2dHCT.Transpose(dchi2dHC);

    TMatrixD deltaM = d2chi2dHCdHCinv * dchi2dHCT;

    if (_debugLevel >=5){
      std::cout << "d2chi2dadainv " << std::endl;
      d2chi2dHCdHCinv.Print();
      std::cout << "dchi2dHCT" << std::endl;
      dchi2dHCT.Print();
      std::cout << "deltaM:" << std::endl;
      deltaM.Print(); 
      std::cout << "Intermediate helix before += delta" << std::endl;
      helix.Print();
    }

    if (_debugLevel >=3){
      std::cout.precision(std::numeric_limits<double>::digits10 + 2);
      std::cout << "Intermediate Helix " << helix(0) << " " << helix(1)  << " " << helix(2)  << " " << helix(3)  << " " << helix(4) << std::endl;
      }


    // modify the helix

 

    TVectorD deltaMVector(Helix::_sDim,deltaM.GetMatrixArray());
    helix += (deltaMVector); // propegate the helix parameters by derivatires times residual directions sqaured and normlized by uncerainties
 
    if (_debugLevel >=5){
      std::cout << "Intermediate helix after += delta" << std::endl;
      helix.Print();
      }

   if (_debugLevel >=3){
     std::cout.precision(std::numeric_limits<double>::digits10 + 2);
     std::cout << "Intermediate Helix after += delta" << helix(0) << " " << helix(1)  << " " << helix(2)  << " " << helix(3)  << " " << helix(4) << std::endl;
   }

    _helix.setHelix(helix);




      
  }

  if (_debugLevel >=3){
    std::cout.precision(std::numeric_limits<double>::digits10 + 2);
    std::cout << "Final Helix " << helix(0) << " " << helix(1)  << " " << helix(2)  << " " << helix(3)  << " " << helix(4) << std::endl;
  }

  // !!!! calculate chi2, ndf, and set cov matrix


  //return chi2;
}





void fc::TrackFit::print(void) const{

  TLorentzVector lorentzVector;
  double pT = std::abs(1.0/getHelix().getKappa());
  double pZ = getHelix().getTanL()*pT;
  lorentzVector.SetPxPyPzE(pT*std::cos(getHelix().getPhi0()-M_PI/2.0),pT*std::sin(getHelix().getPhi0()-M_PI/2.0),pZ,std::sqrt(pT*pT+pZ*pZ));



  std::cout << "Charge " << getCharge() << std::endl;
  std::cout << "4 momentum " << lorentzVector.Px() << " " <<  lorentzVector.Py() << " " <<  lorentzVector.Pz() << " " <<  lorentzVector.E() << " " << std::endl;
  std::cout << "Track parameters:  pT " <<  lorentzVector.Pt() << " cot(theta) " << 1/getHelix().getTanL() << " phi0 " << getHelix().getPhi0()-M_PI/2.0 << " d0 " << getHelix().getDr() << " z0 " << getHelix().getDz() << std::endl;
  std::cout << "Helix paramters: kappa " << getHelix().getKappa() << " tan(Lambda) " << getHelix().getTanL() << " phi0 to d0 " << getHelix().getPhi0() << std::endl;  
  std::cout << "Reference Point " << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;


  trackHitMap::size_type numberHits =_trackHitMap.size();

  std::cout << "Number of hits " << numberHits << std::endl;
  
  std::cout << "Hit layers, numbers: ";
  for (trackHitMap::const_iterator trackHitMapIter = _trackHitMap.begin(); trackHitMapIter != _trackHitMap.end(); ++trackHitMapIter){
    std::cout << trackHitMapIter->second << ", " << trackHitMapIter->first << ": ";
  }
  std::cout << std::endl;



}