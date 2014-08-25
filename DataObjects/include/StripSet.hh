#ifndef DataObjects_StripSet_hh
#define DataObjects_StripSet_hh
//============================================================================
// StripSet.hh
// header with class definition of the StripSet
// An array of map<int,int> layer_map's
// Map elements are int,int pairs of strip number (Key) and acd (value) information
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-05-02
//============================================================================
#include <map>
#include <vector>
#include <utility>

namespace fc {

class DetectorGeometry;

// Map used to allow keying by strip number
typedef std::map<unsigned int, int>
LayerStripMap; //!< Map of strip number and acd counts for one layer


///
/// Class StripSet:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-05-02
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a>
///

class StripSet {
public:

    explicit StripSet(const DetectorGeometry&);

    int numberLayers() const {
        return _layerStripMaps.size();
    }
    const std::vector<LayerStripMap>& strips() const {
        return _layerStripMaps;
    }
    const LayerStripMap& layerStrips(unsigned int layer) const;

    int stripNumber(LayerStripMap::const_iterator iter) const {
        return iter->first;
    };
    int stripAdc(LayerStripMap::const_iterator iter) const {
        return iter->second;
    };
    int stripNumber(const std::pair<int,int>& pair) const {
        return pair.first;
    };
    int stripAdc(const std::pair<int,int>& pair) const {
        return pair.second;
    };

    void insertStrip(unsigned int layer, int stripNumber, int adc);

    void print(std::ostream& out) const;

private:

    std::vector<LayerStripMap> _layerStripMaps;
};
} // end namespace

#endif // DataObjects_StripSet_hh

