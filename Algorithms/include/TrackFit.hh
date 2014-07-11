#ifndef TrackFit_hh
#define TrackFit_hh
//============================================================================
// TrackFit.hh
// header with class definition of the TrackFit for reconstructed tracks
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-08
//============================================================================

#include "TVector3.h"
#include "TMatrixD.h"
#include "TLorentzVector.h"
#include "Helix.hh"
#include <map>

class DetectorGeometry;
class HitSet;

namespace fc {

typedef std::map<int, int> trackHitMap; //!< Map of hit number and layer

///
/// Class TrackFit:  TrackFit for reconstructed tracks
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-07-08
/// For more information see <a href="../notes/Track.pdf">TrackFit.pdf</a> 
///

class TrackFit {
private:

 
  Helix _helix;

  TMatrixD * _covMatrix;

  trackHitMap _trackHitMap;
  
  const DetectorGeometry & _detectorGeometry;

  double _alpha; // 1/curvatureC

  int _debugLevel;


public:

// Constructors

  TrackFit(const Helix & helix,const DetectorGeometry & detectorGeometry);
  //TrackFit(const Helix & helix,const HitSet & hitSet, const std::vector<int> hitNumberVector, const DetectorGeometry & detectorGeometry,

  //TrackFit(double kappa, double dr, double dz, double phi0, double tanl, const DetectorGeometry & detectorGeometry);
  TrackFit(const TVector3 & x1, const TVector3 & x2, const TVector3 & x3, const DetectorGeometry & detectorGeometry,int debugLevel);

  
  // Destructor
  ~TrackFit() {};
 
  // Utility functions

  bool intersectWithLayer(TVector3 & hitPosition, int layer, const DetectorGeometry & detectorGeometry) const;
  bool intersectWithPlane(TVector3 & hitPosition, const TVector3 & center, const TVector3 & normal,double & phi) const;


  TVector3 calcXAt(double phi) const;
  TMatrixD calcDxDHC(double phi) const;
  TMatrixD calcDxDphi(double phi) const;
  TMatrixD calcDXZDHC(const TVector3   & hitPosition, const TMatrixD &dxphiadHC)  const;


  TMatrixD expectedMeasurementVectorXZ(int layer, const DetectorGeometry & detectorGeometry) const;
  TMatrixD expectedMeasurementDerivativedXZdHC(int layer, const DetectorGeometry & detectorGeometry) const;
  TMatrixD invMeasurementRes2XZ(int layer, const DetectorGeometry & detectorGeometry) const;
  TMatrixD measurementVectorXZ(const TVector3 & hitPosition) const;

  // Fit function
  void FitToHelix(const HitSet & hitSet, const DetectorGeometry & detectorGeometry,bool withPrimaryVertex);


  //void MoveTo(const TVector & x0, TMatrixD & propMatrix, TMatrixD & covMatrix); 

  // Get objects

  const Helix & getHelix() const {return _helix;};

  int getCharge(void) const {return -getHelix().getKappa()/std::abs(getHelix().getKappa());};


  // Hit/measurement point information

  const trackHitMap& getTrackFitHitMap(void) const {return _trackHitMap;};

  void insertHit(int,int);

  void print(void) const;

};
} // end namescape fc

#endif // TrackFit_hh

