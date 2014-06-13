#ifndef Track_hh
#define Track_hh
//============================================================================
// Track.hh
// header with class definition of the Track
// Used for both generator level and reconstructed tracks
// See <A HREF="doc/notes/Track.pdf">Track.pdf</A> for more information
//
// Will be updated with convariance matrix and perhaps direct tracking information
// May need restructuring to support trackign operations
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-06
//============================================================================
#include "TNGeoHelix.hh"
#include "TLorentzVector.h"
#include <map>

namespace fc {

typedef std::map<int, int> trackHitMap; //!< Map of hit number and layer

///
/// Class Track: TLorentzVector to track physics properties and THGeoHelix to track tracking and helix properties
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class Track {
private:

  TLorentzVector _lorentzVector;
  TGeoHelix _geoHelix;

  trackHitMap _trackHitMap;

public:

  //Track(int,TLorentzVector,TGeoHelix); to be implemented
  Track(double * p4, int charge, double * v3, double * bField, double curvatureC);
  Track(double pT, double pZ, int charge, double phi0, double d0, double z0, double * vertex, double * bField, double curvatureC);
  //Track(double curvature, double zStep, int charge, double * dir, double * vectex, double * bField, double curvatureC); to be implemented
  Track(const Track&);
  ~Track() {};

  const TLorentzVector& getConstLorentzVector(void) const {return _lorentzVector; };
  TLorentzVector& getLorentzVector(void) {return _lorentzVector; };
  const TGeoHelix& getConstGeoHelix(void) const {return _geoHelix; };
  TGeoHelix& getGeoHelix(void) {return _geoHelix; };

  void insertHit(int,int);

  const trackHitMap& getTrackHitMap(void) const {return _trackHitMap;};

  void print(void) const;

};
} // end namescape fc

#endif // Track_hh

