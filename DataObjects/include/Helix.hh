#ifndef DataObjects_Helix_hh
#define DataObjects_Helix_hh
//============================================================================
// Helix.hh
// header with class definition of the Helix
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
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

static const int _sDim = 5;     //!< helix dimension


class Helix {

public:
    // Static members, matrix dimensions

// Constructors


    Helix();
    Helix(double dr, double phi0, double kappa, double dz, double tanl,
          double alpha,double curvatureC);

    // Get track parameters

    const TVectorD& helixParam() const {
        return _helix;
    }

    double dR() const {
        return _helix(0);
    }
    double phi0() const {
        return _helix(1);
    }
    double kappa() const {
        return _helix(2);
    }
    double dZ() const {
        return _helix(3);
    }
    double tanL() const {
        return _helix(4);
    }

    double alpha() const {
        return _alpha;
    }

    double radiusOfCurvatureAtOrigin() const {
        return _alpha/_helix(2);
    }
    // alpha is 1/curvatureCInField  curvatureCInField = curvatureC*bFieldZ, 1/curvatureC*bFieldZ
    // scale Radius of curvature by the ration of the magentic field at the origen (1/_alpha*_curvatureC) divided by the local field bField.Mag()
    double radiusOfCurvature(const TVector3& bField) const {
        return _alpha*((1/(_alpha*_curvatureC))/bField.Mag())/_helix(2);
    }



    double pT() const {
        return std::abs(1.0/_helix(2));
    }
    double pZ() const {
        return _helix(4)*std::abs(1.0/_helix(2));
    }
    double cotTheta() const {
        return _helix(4);
    }
    double cosTheta() const {
        return _helix(4)/std::sqrt(_helix(4)*_helix(4)+1);
    }
    double sinTheta() const {
        return 1.0/std::sqrt(_helix(4)*_helix(4)+1);
    }


    // Set helix
    void setHelix(const TVectorD& helix) {
        _helix = helix;
    }

private:

    TVectorD _helix;
    double _alpha;
    double _curvatureC;
};
} // end namescape fc

#endif // DataObjects_Helix_hh

