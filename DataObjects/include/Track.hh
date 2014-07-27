#ifndef Track_hh
#define Track_hh
//============================================================================
// Track.hh
// header with class definition of the Track
// Used for both generator level and reconstructed tracks
// New version to support track fittering
// See <A HREF="doc/notes/Track.pdf">Track.pdf</A> for more information
//
// Track Helix parameter definition
//
// Units are in m, GeV  and Tesla such that alpha = 1.0e9/(_bField[2]*2.99792458e8)
//
//      x = x0 + drho * cos(phi0) + rho * (cos(phi0) - cos(phi0 + phi))
//      y = y0 + drho * sin(phi0) + rho * (sin(phi0) - cos(sin0 + phi))
//      z = z0 + dz               - rho * tan(lambda) * phi
//
// Such that phi0 is the angle to the point of closest approach and the radius of curvature rho = alpha/kappa
//
// Standard track parameter diffinition
//          pT = 1/abs(kappa)
//          cot(theata) = 1/tan(lambda)
// standard phi0 = helix phi0 + PI/2
//          d0 = drho aka dr 
//          z0 = dz
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-18
//============================================================================

#include "TVector3.h"
#include "TMatrixD.h"
#include "TLorentzVector.h"
#include "DataObjects/include/Helix.hh"
#include<cmath>
#include <map>

class DetectorGeometry;

namespace fc {

typedef std::vector<int> trackHitSet; //!< Vector of hit indicies

///
/// Class Track: TLorentzVector to track physics properties and THGeoHelix to track tracking and helix properties
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class Track {
private:

  Helix _helix;

  TMatrixD _covMatrix;

  double _chi2;
  int _nDof;


  trackHitSet _trackHitSet;
  
  //const DetectorGeometry & _detectorGeometry;


public:

// Constructors

  Track();
  Track(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr, int d0sign, const DetectorGeometry & detectorGeometry);
  Track(const Helix& helix,const TMatrixD& covMatrix,double chi2,int nDof,const trackHitSet& trackHitCandidate);

  Track& operator=(Track track);
  
  // Destructor
  ~Track() {};

  // Get objects

  const Helix & getHelix() const {return _helix;};
  const TLorentzVector  getLorentzVector() const;

  int getCharge() const {return -getHelix().getKappa()/std::abs(getHelix().getKappa());};

  const TMatrixD & getCovMatrix() const {return _covMatrix;}
  double getSigmaD0() const {return std::sqrt(_covMatrix(0,0));}
  double getSigmaPhi0() const {return std::sqrt(_covMatrix(1,1));}
  double getSigmaKappa() const {return std::sqrt(_covMatrix(2,2));}
  double getSigmaZ0() const {return std::sqrt(_covMatrix(3,3));}
  double getSigmaTanL() const {return std::sqrt(_covMatrix(4,4));}

  double getChi2() const {return _chi2;}
  int getNDof() const {return _nDof;}

  // Hit/measurement point information

  const trackHitSet& getHits() const {return _trackHitSet;};
  int numberHits() const {return _trackHitSet.size();}


  void insertHit(int);

  void print() const;

};
} // end namescape fc

#endif // Track_hh

