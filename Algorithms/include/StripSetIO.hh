#ifndef Algorithms_StripSetIO_hh
#define Algorithms_StripSetIO_hh
//============================================================================
// StripSetIO.hh
// header with class definition of the StripSetIO
// Input and output for the StripSet data object
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-04
//============================================================================
#include <fstream>
#include <iostream>

class StripSet;

namespace fc {

class DetectorGeometry;
class StripSet;

static const int bitmask1 = 0x00FF;
static const int bitmask2 = 0xFF00;
static const int adcBitmask = 0x001F;
static const int stripBitmask = 0xFFE0;


///
/// Class StripSetIO:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-07-04
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a>
///

class StripSetIO {
public:

    explicit StripSetIO(const DetectorGeometry &);

    void writeEvent(const StripSet &,
                    std::ofstream&)
    const; //!< Write all strip information for all sensors std::ofstream file for event _event
    void readEvent(StripSet &,
                   std::ifstream&);  //!< Read all strip information for all sensors std::ifstream file for event _event

  void printRawData(std::ifstream&, std::ofstream&) const;

private:


    const DetectorGeometry& _detectorGeometry;

    int _version;

};
} // end namespace

#endif // Algorithms_StripSetIO_hh

