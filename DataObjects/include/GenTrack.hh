#ifndef DataObjects_GenTrack_hh
#define DataObjects_GenTrack_hh
//============================================================================
// GenTrack.hh
// header with class definition of the GenTrack
// Used for both generator level tracks
// New version to support track fittering
// See <A HREF="doc/notes/dataFormat.pdf" dataFormat.pdf</A> for more information
//
// GenTrack Helix parameter definition
//
// Units are in GeV
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-24
//============================================================================

#include <cmath>
#include "TVector3.h"
#include "TLorentzVector.h"


namespace fc {

class Helix;


///
/// Class GenTrack: TLorentzVector to track momentum, and TVector3 initial position
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-07-24
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a>
///

class GenTrack {
public:

// Constructors

    GenTrack(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr);

    // Get objects

    const TLorentzVector&  lorentzVector() const {
        return _lorentzVector;
    }
    const TVector3& dR() const {
        return _dr;
    }
    int charge() const {
        return _charge;
    }

    // convert to helix
    const Helix makeHelix(const TVector3& bField,double curvatureC) const;

    // Print method

    void print(ostream& out) const;

private:

    TLorentzVector _lorentzVector;
    int _charge;
    TVector3 _dr; // point of clossest approach to 0,0,0cp

};
} // end namescape fc

#endif // DataObjects_GenTrack_hh

