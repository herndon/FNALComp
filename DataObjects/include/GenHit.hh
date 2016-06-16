#ifndef DataObjects_GenHit_hh
#define DataObjects_GenHit_hh
//============================================================================
// GenHit.hh
// header with class definition of the GenHit
// Used for generator level Hits
// See <A HREF="doc/notes/GenHit.pdf">GenHit.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-01
//============================================================================

#include "TVector3.h"

namespace fc {

///
/// Class GenHit:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-07-01
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a>
///

class GenHit {
public:

    GenHit(const TVector3& position, int layer, int trackNumber);

    const TVector3 & position() const {
        return _position;
    }
    int layer() const {
        return _layer;
    }
    int trackNumber() const {
        return _trackNumber;
    }

    void print(std::ostream& out) const;

private:

    TVector3 _position;
    int _layer;
    int _trackNumber;
};
} // end namespace fc
#endif // DataObjects_GenHit_hh
