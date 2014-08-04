#ifndef Algorithms_GenHitSetIO_hh
#define Algorithms_GenHitSetIO_hh
//============================================================================
// GenHitSetIO.hh
// header with class definition of the GenHitSetIO
// IO to disk for the GenHitSet
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-01
//============================================================================
#include <fstream>
#include <iostream>

class GenHitSet;

namespace fc {


///
/// Class GenHitSetIO:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-07-01
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a>
///

class GenHitSetIO {

public:
    GenHitSetIO();

    void writeEvent(const GenHitSet &,
                    std::ofstream&)
    const; //!< Write all hit information for all sensors std::ofstream file for event _event
    void readEvent(GenHitSet &,
                   std::ifstream&);  //!< Read all hit information for all sensors std::ifstream file for event _event

private:

    int _version;
};
} // end namespace fc
#endif // Algorithms_GenHitSetIO_hh

