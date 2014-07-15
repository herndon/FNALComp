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
#include "Helix.hh"
#include <map>

class DetectorGeometry;
class HitSet;

namespace fc {

typedef std::map<int, int> trackHitMap; //!< Map of hit number and layer

///
/// Class Track: TLorentzVector to track physics properties and THGeoHelix to track tracking and helix properties
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class Track {
private:

  Helix _helix;

  TMatrixD * _covMatrix;

  trackHitMap _trackHitMap;
  
  const DetectorGeometry & _detectorGeometry;

  double _alpha; // 1/curvatureC


public:

// Constructors

  Track();
  Track(double kappa, double dr, double dz, double phi0, double tanl, const DetectorGeometry & detectorGeometry);
  Track(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr, const DetectorGeometry & detectorGeometry);
  Track(const HitSet & hitSet, const std::vector<int> & trackHitCandidate, const TVector3 & primaryVertex, const DetectorGeometry & detectorGeometry, int debugLevel);

  Track(const Track&);
  
  // Destructor
  ~Track() {};

  // Get objects

  const Helix & getHelix() const {return _helix;};
  TLorentzVector  getLorentzVector(void) const;

  int getCharge(void) const {return -getHelix().getKappa()/std::abs(getHelix().getKappa());};


  // Hit/measurement point information

  const trackHitMap& getTrackHitMap(void) const {return _trackHitMap;};

  void insertHit(int,int);

  void print(void) const;

};
} // end namescape fc

#endif // Track_hh

