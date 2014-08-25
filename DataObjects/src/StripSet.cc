#include <iostream>
#include "Services/include/Exception.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/StripSet.hh"

fc::StripSet::StripSet(const DetectorGeometry & detectorGeometry) {
    _layerStripMaps.resize(detectorGeometry.nSensors());
}

const fc::LayerStripMap& fc::StripSet::layerStrips(unsigned int layer) const {
    if (layer >= 0 && layer < _layerStripMaps.size()) return _layerStripMaps[layer];
    throw Exception("StripSet::layerStripMap: Out of bounds layer");
}

// void fc::StripSet::insertStrip(unsigned int layer, int strip, int adc) {
//     if (layer >= 0 && layer < _layerStripMaps.size()) {
//         _layerStripMaps[layer].insert(LayerStripMap::value_type(strip,adc));
//         return;
//     } else {
//         throw Exception("StripSet::insertStrip: Out of bounds layer");
//     }
// }

//Improved insertStrip
void fc::StripSet::insertStrip(unsigned int layer, int strip, int adc) {
  if (layer >= 0 && layer < _layerStripMaps.size()){

    LayerStripMap::iterator stripIter =  _layerStripMaps[layer].find(strip);
    if (stripIter == _layerStripMaps[layer].end()) {
      _layerStripMaps[layer].insert(LayerStripMap::value_type(strip,adc));
    } else {
      // second is the strip acd value in the map of key strip number and value adc
      stripIter->second = std::min(stripIter->second + adc, 32);
    }
    return;
  } else {
    throw Exception("StripSet::insertStrip: Out of bounds layer");
  }
}




void fc::StripSet::print(ostream& out) const {

    out << "StripSet" << std::endl;


    int iiLayer = 0;

    for (auto const& stripMap: strips()) {

        LayerStripMap::size_type numberStrips = stripMap.size();
        out << "Layer: " << iiLayer << " number Strips: " << numberStrips << std::endl;

        for (auto const& strip : stripMap) {
            out << "Strip: " << stripNumber(strip) << " " << " ADC " << stripAdc(strip)  << std::endl;
        } // end strip loop

        ++iiLayer;

    } // end layer loop

}

