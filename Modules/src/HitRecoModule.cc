#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>

#include "Geometry/include/StripHitFunctions.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Hit.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/HitRecoModule.hh"

namespace fc {

  // An instance of Strip represents a strip with a signal. We use small
  // integers for data members because their range is small. Our c'tor
  // has to use what the surrounding code uses.
  struct Strip {
    Strip(int i, int adc) : id(i), cnt(adc) { }
    unsigned short id;
    unsigned short cnt;
  };

  struct Layer {
    explicit Layer(int lyr) : strips(), layer(lyr) { }
    std::vector<Strip> strips;
    unsigned short     layer;
  };

  // This structure is a helper that is used to accumulate hits within
  // one layer.

  struct HitAccum {
    HitAccum(int lyr, DetectorGeometry const& dg, HitSet& results) :
      start(-1),
      curr_strip(-1),
      cnts(),
      layer(lyr),
      geom(dg),
      desc(dg.getSensor(layer)),
      hits(results)
    { }

    ~HitAccum() {
      if (makingCluster()) { makeHit(); }
    }

    void clear() { start = -1; curr_strip = -1; cnts.clear(); }

    void add(Strip const& s) { curr_strip = s.id, cnts.push_back(s.cnt); }
    void startNewHit(int strip, int cnt) { start = strip; curr_strip = strip; cnts.push_back(cnt); }
    bool isAdjacent(int strip) const { return strip == curr_strip + 1; }
    bool makingCluster() const { return start >= 0; }

    // This member function defines what it means to be a strip that is a
    // candidate to go into a cluster.
    bool goodStrip(Strip const& s) const { return s.cnt > desc._threshold; }

    // This member function defines what it means for a strip to be
    // appropriate to add to the current cluster.
    bool inSameCluster(Strip const& s) const { return goodStrip(s) && isAdjacent(s.id); }

    void makeHit();
    void processStrip(Strip const& strip);

    // Accumulators for our current state.
    int start;
    int curr_strip;
    std::vector<int> cnts;

    // Fixed for the whole layer.
    int layer;
    DetectorGeometry const& geom;
    SensorDescriptor const& desc;
    HitSet&                 hits;
  };
}

void fc::HitAccum::makeHit() {
  if (cnts.size() < 2) { return; }
  double pos = fcf::calculateStripHitPositionFromCluster(start, cnts);
  double local = fcf::calculateLocalFromStripPosition(pos, layer, geom);
  TVector3 hpos = fcf::calculateGlobalFromLocalPosition(local, layer, geom);
  int charge = std::accumulate(cnts.begin(), cnts.end(), 0);
  bool good_hit = cnts.size() <= 2 && charge <= geom.getMIP();
  double res = good_hit ? desc._hitResolution : desc._badHitResolution;
  hits.insertHit(Hit(hpos, layer, cnts.size(), charge, good_hit, res));

  std::cout << "closing:"
            << " layer " << layer
            << " strip " << start
            << " hits " << cnts.size()
            << " charge " << charge
            << " good " << good_hit
            << " pos " << hpos.x() << " " << hpos.y() << " " << hpos.z()
            << "\n";
}

void fc::HitAccum::processStrip(Strip const& strip) {
  if (makingCluster()) {
    // ongoing cluster ...
    if (inSameCluster(strip)) {
      add(strip);
    }
    else {
      makeHit();
      clear();
    }
  }
  else {
    // no cluster ...
    if (goodStrip(strip)) {
      startNewHit(strip.id, strip.cnt);
    }
  }
}

// Now we have the implementation of HitRecoModule.
fc::HitRecoModule::HitRecoModule(int debugLevel,
                                 const std::string& iInputStripsLabel,
                                 const std::string& iOutputHitsLabel,
                                 const DetectorGeometry& detectorGeometry):
  _debugLevel(debugLevel),
  _inStripsLabel(iInputStripsLabel),
  _outHitsLabel(iOutputHitsLabel),
  _detectorGeometry(detectorGeometry),
  _num(0) {
}

void fc::HitRecoModule::processEvent(fc::Event& event) {
  Handle<StripSet> genStripSet = event.get<StripSet>(_inStripsLabel);
  std::unique_ptr<HitSet> recoHitSet(new HitSet);

  processLayers(*genStripSet, *recoHitSet);

  if (_debugLevel >= 2) { recoHitSet->print(std::cout); }

  event.put(_outHitsLabel, std::move(recoHitSet));
  ++_num;
}

void fc::HitRecoModule::processLayers(const StripSet& stripSet,
                                      HitSet& hitSet) const {
  auto const& layers = stripSet.getStrips();
  for (size_t i = 0; i < layers.size(); ++i) {
    std::cout << "calling makeHits for layer " << i << "\n";
    makeHits(i, layers[i], hitSet);
  }
}

void fc::HitRecoModule::makeHits(int layer,
                                 LayerStripMap const& strips,
                                 HitSet& hits) const {
  // LayerStripMap is not a convenient structure for our use, so we
  // transform it here: a vector of pairs. The vector is sorted, so we
  // need that.
  Layer currentLayer(layer);
  std::transform(begin(strips), end(strips),
                 std::back_inserter(currentLayer.strips),
  [](LayerStripMap::value_type p) { return Strip(p.first, p.second); });

  HitAccum currentCluster(layer, _detectorGeometry, hits);

  for (auto const& strip : currentLayer.strips) {
    currentCluster.processStrip(strip);
  }
}
