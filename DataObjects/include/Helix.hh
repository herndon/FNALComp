#ifndef DataObjects_Helix_hh
#define DataObjects_Helix_hh
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
#include "TVectorD.h"
#include <memory>




namespace fc {


class Helix {

public:
  // Static members, matrix dimensions
  static const int _sDim = 5;     //!< helix dimension

// Constructors


  Helix();
  Helix(double kappa, double dr, double dz, double phi0, double tanl, double alpha);
  
  // Get track parameters

  const TVectorD& getHelix() const {return _helix;}

  double getDr() const {return _helix(0);}
  double getPhi0() const {return _helix(1);}
  double getKappa() const {return _helix(2);}
  double getDz() const {return _helix(3);}
  double getTanL() const {return _helix(4);}

  double getAlpha() const {return _alpha;}

  double getRadiusOfCurvature() const {return _alpha/_helix(2);}

  double getPT(const TVector3& bField) const {return std::abs(1.0/_helix(2));}
  double getPZ() const {return _helix(4)*std::abs(1.0/_helix(2));}
  double getCotTheta() const {return _helix(4);}

  // Set helix
  void setHelix(const TVectorD& helix) {_helix = helix;}

private:

  TVectorD _helix;
  double _alpha;
};
} // end namescape fc

#endif // DataObjects_Helix_hh

