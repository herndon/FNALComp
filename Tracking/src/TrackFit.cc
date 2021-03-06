#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "DataObjects/include/HitSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/TrackFitAlgorithms.hh"
#include "Tracking/include/TrackFitMeasurements.hh"
#include "DataObjects/include/Helix.hh"
#include "Tracking/include/TrackFit.hh"



const fc::Helix fc::fitToHelix(const Helix& initialHelix, const HitSet& hitSet,
                               const TrackHitContainer&  trackHits, const DetectorGeometry& detectorGeometry,
                               TMatrixD& finalCovMatrix, double& finalChi2, int& finalNDof, int _debugLevel) {


    // Define static constants...
  // The loop convergence pattern has been speeded up which with more realism would have to be revisited

    const double chi2Dummy = 1.e20;
    const double chi2Tol = 1.e-8;
    const double deltaIncr   = 100.;
    const double deltaDecr   = 0.001;
    const int    loopMax = 20;

    double delta  = 1.0;


    // for the  TrackFit  helix values set by generating or by hits
    //                          initial error matrix diagonal with 1.e4 on all elements
    //                          need to be set befreo FitToHelix


    TVectorD    helix(_sDim); // original helix paramters
    helix = initialHelix.helixParam();

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


    TMatrixD    dchi2dHCbest(1   ,
                             _sDim); // best based on chi2 _sDim contributions to chi2?
    TMatrixD    dchi2dHC    (1   , _sDim);
    TMatrixD    dchi2dHCT   (_sDim , 1  );
    TMatrixD    d2chi2dHCdHC (_sDim, _sDim);
    TMatrixD    d2chi2dHCdHCbest (_sDim, _sDim);
    TMatrixD    expectedMeasurementVector       (_mDim, 1   );
    TMatrixD    expectedMeasurementDerivative (_mDim, _sDim);
    TMatrixD    expectedMeasurementDerivativeT (_sDim, _mDim);
    TMatrixD    measurementVector(_mDim,1);
    TMatrixD    curHt      (_sDim, _mDim);
    TMatrixD    invMeasurementRes2    (_mDim, _mDim);
    TMatrixD    measurementResidualVector  (_mDim, 1   );
    TMatrixD    measurementResidualVectorT (1, _mDim);

    // Minimization loop starts here

    while (1) {
        if (nloops > loopMax) {

            helix = helixBest;
            d2chi2dHCdHC = d2chi2dHCdHCbest;
            chi2  = chi2Best;
            break;
        }
        ++nloops;

        d2chi2dHCdHC.Zero();
        dchi2dHC.Zero();
        chi2 = 0;

	int numberBadHits = 0;
        int numberHits = trackHits.size();
	bool useBadHits = true;
       for (auto const& hit : trackHits) {
	 if (hit>=0 && !hitSet.hits()[hit].goodHit()) ++numberBadHits;
       }

       if ((numberHits - numberBadHits) >6 ) useBadHits = false;
 

        for (auto const& hit : trackHits) {
 
            // Hit information
            int hitNumber = hit;
            TVector3 hitPosition;
            int layer;
            if (_debugLevel >= 5) {
	      if (hitNumber>=0 && !useBadHits && !hitSet.hits()[hit].goodHit() ) {
		continue;
	      }
	    }
           if (hitNumber>=0) {
                layer = hitSet.hits()[hit].layer();
                hitPosition = hitSet.hits()[hit].position();
            } else {
                layer = hit;
                hitPosition - detectorGeometry.sensor(layer).center();
            }



            // Find vector to measurement in local coordinate
            expectedMeasurementVector = expectedMeasurementVector1D(workingHelix,layer,
                                        detectorGeometry);



            // Find how the measurement expectation varies with each helix coordinate
            expectedMeasurementDerivative = expectedMeasurementDerivatived1DdHC(
                                                workingHelix,layer,detectorGeometry);

            expectedMeasurementDerivativeT.Transpose(expectedMeasurementDerivative);

 
            // Get the inverse resolutions squared
            if (layer < 0) {
                invMeasurementRes2(0,0) = detectorGeometry.sensor(layer).hitResolution()*detectorGeometry.sensor(layer).hitResolution();
            } else {
                invMeasurementRes2(0,0) = hitSet.hits()[hit].resolution()
                                          *hitSet.hits()[hit].resolution();
            }
            invMeasurementRes2.Invert();


            // 1D Hit position in local measurement coordinate
            measurementVector = measurementVector1D(hitPosition,layer,detectorGeometry);

            // Residuals in 1D
            measurementResidualVector = measurementVector - expectedMeasurementVector;
            measurementResidualVectorT.Transpose(measurementResidualVector);



            // Accumulate chi2
            double delchi2 = (measurementResidualVectorT * invMeasurementRes2 *
                              measurementResidualVector)(0,0);
            chi2 += delchi2;


            // Calculate derivatires of the chi2 with respect the helix parameters for each hit and accumulate
            dchi2dHC += (measurementResidualVectorT * invMeasurementRes2 *
                         expectedMeasurementDerivative);
            d2chi2dHCdHC += (expectedMeasurementDerivativeT * invMeasurementRes2 *
                             expectedMeasurementDerivative);

        }// end hit loop


        if (std::abs(chi2Best - chi2) < chi2Tol) {
            d2chi2dHCdHCbest = d2chi2dHCdHC;
            chi2Best = chi2;
            helixBest = helix;
            break;
        }

        if (chi2 < chi2Best) {
            // chi2 decreased. Save this step as the current best
           helixSave = helix;
            helixBest = helix;
            chi2Best    = chi2;
            dchi2dHCbest = dchi2dHC;
            d2chi2dHCdHCbest  = d2chi2dHCdHC;
            delta     *= deltaDecr;
        } else {
            // chi2 increased. Restore the current best
           helix = helixSave;
            helixBest = helixSave;
            dchi2dHC     = dchi2dHCbest;
            d2chi2dHCdHC  = d2chi2dHCdHCbest;
            delta     *= deltaIncr;
        }

        // Add delta to 2nd derivative and use to modify the helix

        for (int i=0; i<_sDim; ++i) {
            d2chi2dHCdHC(i, i) *= (1 + delta);
        }


        TMatrixD d2chi2dHCdHCinv = d2chi2dHCdHC;

        d2chi2dHCdHCinv.Invert();

        dchi2dHCT.Transpose(dchi2dHC);

        TMatrixD deltaM = d2chi2dHCdHCinv * dchi2dHCT;

        // modify the helix



        TVectorD deltaMVector(_sDim,deltaM.GetMatrixArray());
        helix += (deltaMVector); // propegate the helix parameters by derivatires times residual directions sqaured and normlized by uncerainties

 
        workingHelix.setHelix(helix);

    }




    TrackHitContainer::size_type nHits = trackHits.size();
    ndof = _mDim*nHits - _sDim;
    workingHelix.setHelix(helixBest);
    finalCovMatrix = d2chi2dHCdHCbest.Invert();
    finalChi2 = chi2Best;
    finalNDof = ndof;

    return workingHelix;

}

const fc::Helix fc::fitToHelixWithPV(const Helix& initialHelix,
                                     const HitSet& hitSet, const TrackHitContainer&  trackHitsNoPV,
                                     const DetectorGeometry& detectorGeometry, TMatrixD& finalCovMatrix,
                                     double& finalChi2, int& finalNDof, int fitType, int _debugLevel) {

    TrackHitContainer trackHitsWithPV;
    trackHitsWithPV.reserve(trackHitsNoPV.size()+1);

    for (auto hitNumber: trackHitsNoPV) {
        trackHitsWithPV.push_back(hitNumber);
    }

    // insert an local X  PV measurement point
    if (fitType ==1 || fitType ==3) trackHitsWithPV.push_back(-2);
    // insert a local Z PV measurement point
    if (fitType ==2 || fitType ==3) trackHitsWithPV.push_back(-1);
    return fitToHelix(initialHelix, hitSet, trackHitsWithPV, detectorGeometry,
                      finalCovMatrix, finalChi2, finalNDof, _debugLevel);
}
