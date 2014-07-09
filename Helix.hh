#ifndef Helix_hh
#define Helix_hh
//============================================================================
// Helix.hh
// header with class definition of the Helix
// See <A HREF="doc/notes/Track.pdf">Track.pdf</A> for more information !!!!! not done yet
//
// Helix parameter definition
//
// dr, phi0, kappa, dz, tanL 
//
// Units are in m, GeV  and Tesla such that alpha = 1.0e9/(_bField[2]*2.99792458e8)
//
//      x = x0 + drho * cos(phi0) + rho * (cos(phi0) - cos(phi0 + phi))
//      y = y0 + drho * sin(phi0) + rho * (sin(phi0) - cos(sin0 + phi))
//      z = z0 + dz               - rho * tan(lambda) * phi
//
// Such that phi0 is the angle to the point of closest approach to the reference point 0, 0, 0
//       and the radius of curvature rho = alpha/kappa
//
// Standard track parameter diffinition6
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


namespace fc {


class Helix {

  friend class Track;
  friend class TrackFit;

public:
  // Static members, matrix dimentions
  static const int _sDim = 5;     //!< helix dimention


private:

  TMatrixD* _helix;

public:

// Constructors


  Helix();
  Helix(double kappa, double dr, double dz, double phi0, double tanl);
  //Helix(const TMatrixD& helix);

  // more complex constructors are handled by the TrackMake class

  Helix(const Helix&);
  
  // Destructor
  ~Helix();
 

  // Get track parameters

  TMatrixD getHelix() const {return *_helix;}

  double getDr() const {return (*_helix)(0,0);}
  double getPhi0() const {return (*_helix)(1,0);}
  double getKappa() const {return (*_helix)(2,0);}
  double getDz() const {return (*_helix)(3,0);}
  double getTanL() const {return (*_helix)(4,0);}

};
} // end namescape fc

#endif // Helix_hh

