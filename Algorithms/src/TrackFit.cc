#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "DataObjects/include/HitSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Algorithms/include/TrackFitAlgorithms.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "DataObjects/include/Helix.hh"
#include "Algorithms/include/TrackFit.hh"



const fc::Helix fc::fitToHelix(const Helix& initialHelix, const HitSet& hitSet, const trackHitSet&  trackHitSet, const DetectorGeometry& detectorGeometry, TMatrixD& finalCovMatrix, double& finalChi2, int& finalNDof, int _debugLevel){


  // Define static constants...
 
  static const double chi2Dummy = 1.e20;
  static const double chi2Tol = 1.e-8;
  static const double deltaIncr   = 10.;
  static const double deltaDecr   = 0.1;
  static const int    loopMax = 50;

  double delta  = 1.0;


  // for the  TrackFit  helix values set by generating or by hits
  //                          initial error matrix diagonal with 1.e4 on all elements
  //                          need to be set befreo FitToHelix


  TVectorD    helix(Helix::_sDim); // original helix paramters
  helix = initialHelix.getHelix();

  Helix workingHelix(initialHelix); 

  TVectorD helixBest(helix);
  TVectorD helixSave(helix);

  double   chi2Best = chi2Dummy;
  //double   deltabest   = delta;
  int      nloops   = 0;

  int ndof = 0;
  double chi2 = 0.;

  // Prepare some matrices
 
  // Note on parameters and derivatives
  // 1D (in X, SAS or Z) is the measurement direction of the sensors
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
      if (_debugLevel >= 5){
	std::cout << "TrackFit::FitToHelix >>>>>>>>>>>>>>"
		  << " Loop count limit reached. nloops = " << nloops << std::endl;
      }
 
      helix = helixBest;
      d2chi2dHCdHC = d2chi2dHCdHCbest;
      chi2  = chi2Best;
      break;
    }
    ++nloops;

    d2chi2dHCdHC.Zero();
    dchi2dHC.Zero();
    chi2 = 0;
 
 

    for (trackHitSet::const_iterator hitIter = trackHitSet.begin(); hitIter != trackHitSet.end(); ++hitIter){

      // Hit information
      int hitNumber = *hitIter; 
      TVector3 hitPosition;
      int layer; 
      if (hitNumber>=0) {
	layer = hitSet.getHits()[*hitIter].getLayer();
 	hitPosition = hitSet.getHits()[*hitIter].getHitPosition();
      }else {
	layer = *hitIter;
	hitPosition - detectorGeometry.getSensor(layer)._center;
      }

 
 
      // Find vector to measurement in local coordinate
      expectedMeasurementVector = expectedMeasurementVector1D(workingHelix,layer,detectorGeometry);


      if (_debugLevel >= 5){
	std::cout << "expectedVector 1D mDim:" << std::endl;
	expectedMeasurementVector.Print();
      }

      // Find how the measurement expectation varies with each helix coordinate
      expectedMeasurementDerivative = expectedMeasurementDerivatived1DdHC(workingHelix,layer,detectorGeometry);

      expectedMeasurementDerivativeT.Transpose(expectedMeasurementDerivative);
 
      if (_debugLevel >= 5){
	std::cout << "expectedDerivative d1DdHC  mDim x sDim:" << std::endl;
	expectedMeasurementDerivative.Print();
      }

      // Get the inverse resolutions squared
      if (layer < 0){
	invMeasurementRes2(0,0) = detectorGeometry.getSensor(layer)._hitResolution*detectorGeometry.getSensor(layer)._hitResolution;;
      } else {
	invMeasurementRes2(0,0) = hitSet.getHits()[*hitIter].getResolution()*hitSet.getHits()[*hitIter].getResolution();
      }
      invMeasurementRes2.Invert(); 
 
      if (_debugLevel >= 5){
	std::cout << " invMeasurementRes2 mDim x mDim: " << std::endl;
	invMeasurementRes2.Print();
      }

      // 1D Hit position in local measurement coordinate
      measurementVector = measurementVector1D(hitPosition,layer,detectorGeometry);

      // Residuals in 1D
      measurementResidualVector = measurementVector - expectedMeasurementVector;
      measurementResidualVectorT.Transpose(measurementResidualVector);


      if (_debugLevel >= 5){
	std::cout << "measurementResidualVector 1D " << std::endl;
	measurementResidualVector.Print();
      }

      // Accumulate chi2
      double delchi2 = (measurementResidualVectorT * invMeasurementRes2 * measurementResidualVector)(0,0);
      chi2 += delchi2; 
          

      if (_debugLevel >=3){
	std::cout.flush();
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


    if (std::abs(chi2Best - chi2) < chi2Tol) {
      if (_debugLevel >=3){
	std::cout << "Breaking loop, chi2 " << chi2 << " chi2Best " << chi2Best << " chi2Tol " << chi2Tol << std::endl; 
      }
      d2chi2dHCdHCbest = d2chi2dHCdHC;
      chi2Best = chi2;
      helixBest = helix;
      break;
    }

    if (chi2 < chi2Best) {
      // chi2 decreased. Save this step as the current best
      if (_debugLevel >=3){
	std::cout << "Chi2 decreased " << chi2 << " compared to " << chi2Best << std::endl;
      } 
      helixSave = helix;
      helixBest = helix;
      chi2Best    = chi2;
      dchi2dHCbest = dchi2dHC;
      d2chi2dHCdHCbest  = d2chi2dHCdHC;
      delta     *= deltaDecr;
    } else {
      // chi2 increased. Restore the current best
      if (_debugLevel >=3){
	std::cout << "Chi2 increased " << chi2 << " compared to " << chi2Best << std::endl;
      } 
      helix = helixSave;
      helixBest = helixSave; 
      dchi2dHC     = dchi2dHCbest;
      d2chi2dHCdHC  = d2chi2dHCdHCbest;
      delta     *= deltaIncr;
    }

    // Add delta to 2nd derivative and use to modify the helix

    for (int i=0; i<Helix::_sDim; ++i) {
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

    workingHelix.setHelix(helix);
       
  }

  if (_debugLevel >=3){
    std::cout.precision(std::numeric_limits<double>::digits10 + 2);
    std::cout << "Final Helix " << helix(0) << " " << helix(1)  << " " << helix(2)  << " " << helix(3)  << " " << helix(4) << std::endl;
  }
      


  trackHitSet::size_type nHits = trackHitSet.size();
  ndof = DetectorGeometry::_mDim*nHits - Helix::_sDim;
  workingHelix.setHelix(helixBest);
  finalCovMatrix = d2chi2dHCdHCbest.Invert();
  finalChi2 = chi2Best;
  finalNDof = ndof;

  return workingHelix;

}

const fc::Helix fc::fitToHelixWithPV(const Helix& initialHelix, const HitSet& hitSet, const trackHitSet&  trackHitSetNoPV, const DetectorGeometry& detectorGeometry, TMatrixD& finalCovMatrix, double& finalChi2, int& finalNDof, int fitType, int _debugLevel){

  trackHitSet trackHitSetWithPV;
  trackHitSetWithPV.reserve(trackHitSetNoPV.size()+1);

  for (auto hitNumber: trackHitSetNoPV) {
    trackHitSetWithPV.push_back(hitNumber);
  }

  // insert an local X  PV measurement point
  if (fitType ==1 || fitType ==3) trackHitSetWithPV.push_back(-2);
  // insert a local Z PV measurement point
  if (fitType ==2 || fitType ==3) trackHitSetWithPV.push_back(-1);
  return fitToHelix(initialHelix, hitSet, trackHitSetWithPV, detectorGeometry, finalCovMatrix, finalChi2, finalNDof, _debugLevel);
}
