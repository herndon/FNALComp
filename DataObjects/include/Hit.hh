#ifndef DataObjects_Hit_hh
#define DataObjects_Hit_hh
//============================================================================
// Hit.hh
// header with class definition of the Hit
// Used for reconstructed Hits
// See <A HREF="doc/notes/Hit.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================

#include "TVector3.h"

namespace fc {

///
/// Class Hit:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/Hit.pdf">Hit.pdf</a>
///

class Hit {
public:

    Hit(const TVector3& position, int layer, int numberStrips, int charge,
        bool goodHit, double resolution);

    const TVector3 & position() const {
        return _hitPosition;
    }
    int layer() const {
        return _layer;
    }
    int numberStrips() const {
        return _numberStrips;
    }
    int charge() const {
        return _charge;
    }
    bool goodHit() const {
        return _goodHit;
    }
    double resolution() const {
        return _resolution;
    }

    void print(ostream& out) const;

private:

    TVector3 _hitPosition;
    int _layer;
    int _numberStrips;
    int _charge;
    bool _goodHit;
    double _resolution;
};
} // end namespace fc
#endif // DataObjects_Hit_hh
