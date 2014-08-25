#include <string>
#include <algorithm>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/StripSet.hh"
#include "Algorithms/include/StripSetIO.hh"


fc::StripSetIO::StripSetIO(const DetectorGeometry&  detectorGeometry):
    _detectorGeometry(detectorGeometry),
    _version(1) {
}


void fc::StripSetIO::writeEvent(const StripSet & stripSet,
                                std::ofstream & stripdata) const {


    stripdata << "Strips" << std::endl;

    stripdata.write (reinterpret_cast<const char *>(&_version), 1);


    int binaryData;
    int binaryData1;
    int binaryData2;

    int iiLayer = 0;

    for (auto const& stripMap: stripSet.strips()) {

        LayerStripMap::size_type numberStrips = stripMap.size();

        stripdata.write (reinterpret_cast<const char *>(&iiLayer), 1);
        stripdata.write (reinterpret_cast<const char *>(&numberStrips), 1);

        for (auto const& strip : stripMap) {

	  binaryData = stripSet.stripNumber(strip);
	  binaryData1 = binaryData & bitmask1;
	  binaryData2 = binaryData & bitmask2;
	  binaryData2 = binaryData2 >> 8;
	  stripdata.write (reinterpret_cast<const char *>(&binaryData2), 1);
	  stripdata.write (reinterpret_cast<const char *>(&binaryData1), 1);

          binaryData =  stripSet.stripAdc(strip);
	  stripdata.write (reinterpret_cast<const char *>(&binaryData), 1);

        } // end strip loop

        iiLayer++;

    } // end layer loop

}

void fc::StripSetIO::readEvent(StripSet & stripSet, std::ifstream & stripdata) {


    std::string eventDataString;

    stripdata >> eventDataString;

    if (eventDataString != "Strips") {
        std::string wrongEventDataObject =
            "StripSetIO::readEvent: attempted to read wrong data object" + eventDataString;
        throw Exception(wrongEventDataObject);
    }

    // Binary data in char byte format
    // !!!!! Candidate for run time bug
    unsigned  char binaryData;

    stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
    stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
    int version = static_cast<int>(binaryData);

    if (version != _version) {
        std::string wrongStreamerVersion =
            "StripSetIO::readEvent: attempted to read version " + std::to_string(
                version) + " using streamer version " + std::to_string(_version);
        throw Exception(wrongStreamerVersion);
    }


    for (int iiLayer = 0; iiLayer < _detectorGeometry.nSensors(); ++iiLayer) {

        stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
        int layer = static_cast<int>(binaryData);

        if (layer != iiLayer)    if (layer != iiLayer) throw
                Exception("StripSetIO::readEvent: bad strip data");

        stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
        int numberStrips = static_cast<int>(binaryData);

        for (int iiStrip = 0; iiStrip < numberStrips; ++iiStrip) {

            stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
            int stripData2 =  static_cast<int>(binaryData);
            stripData2 = stripData2 << 8;

            stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
            int stripData1 =  static_cast<int>(binaryData);
            int stripData12 = stripData2 + stripData1;
            int strip = stripData12;

            stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
            int adc =  static_cast<int>(binaryData);
  
           stripSet.insertStrip(iiLayer,strip,adc);

        } // end strip loop

    } // end layer loop

}

void fc::StripSetIO::printRawData(std::ifstream & stripdata) const {

    int ii = 0;

    unsigned char binaryData;

    while (stripdata) {

        stripdata.read (reinterpret_cast<char *>(&binaryData), 1);
        if (stripdata) {

            std::cout << "Byte " << ii << " " << static_cast<int>(binaryData) << "\n";
            ++ii;
        }
    }
}
