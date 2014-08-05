#include<iostream>
#include<numeric>
#include "Geometry/include/StripHitFunctions.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Hit.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/HitRecoModule.hh"

// This natural typedef doesn't work because Root introduces a function
// at global scope with this name. This is why all code should be in a
// namespace!
//using Strip = std::map<int,int>::value_type;

using SiStrip = std::map<int, int>::value_type;

fc::HitRecoModule::HitRecoModule(int debugLevel,
                                 const std::string& iInputStripsLabel, const std::string& iOutputHitsLabel,
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

void fc::HitRecoModule::makeHit(int layer, int curr_start_strip,
                                std::vector<int> const& curr_cnts,
                                SensorDescriptor const& desc,
                                HitSet& hits) const {
  if (curr_cnts.size() < 2) { return; }
  double pos = fcf::calculateStripHitPositionFromCluster(curr_start_strip, curr_cnts);
  double local = fcf::calculateLocalFromStripPosition(pos, layer, _detectorGeometry);
  TVector3 hpos = fcf::calculateGlobalFromLocalPosition(local, layer, _detectorGeometry);
  int charge = std::accumulate(curr_cnts.begin(), curr_cnts.end(), 0);
  bool good_hit = curr_cnts.size() <= 2 && charge <= _detectorGeometry.getMIP();
  double res = good_hit ? desc._hitResolution : desc._badHitResolution;
  hits.insertHit(Hit(hpos, layer, curr_cnts.size(), charge, good_hit, res));

  std::cout << "closing:"
            << " num " << _num
            << " layer " << layer
            << " strip " << curr_start_strip
            << " hits " << curr_cnts.size()
            << " charge " << charge
            << " good " << good_hit
            << " pos " << hpos.x() << " " << hpos.y() << " " << hpos.z()
            << "\n";
}

namespace fc {

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

    void add(SiStrip const& p) { curr_strip = p.first, cnts.push_back(p.second); }
    void begin(int strip, int cnt) { start = strip; curr_strip = strip; cnts.push_back(cnt); }
    bool isAdjacent(int strip) const { return strip == curr_strip + 1; }
    bool makingCluster() const { return start >= 0; }

    // This member function defines what it means to be a strip that is a
    // candidate to go into a cluster.
    bool goodStrip(SiStrip const& s) const { return s.second > desc._threshold; }

    // This member function defines what it means for a strip to be
    // appropriate to add to the current cluster.
    bool inSameCluster(SiStrip const& s) const { return goodStrip(s) && isAdjacent(s.first); }

    void makeHit();
    void processStrip(SiStrip const& strip);

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
}

void fc::HitAccum::processStrip(SiStrip const& strip) {
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
      begin(strip.first, strip.second);
    }
  }
}

void fc::HitRecoModule::makeHits(int layer,
                                 LayerStripMap const& strips,
                                 HitSet& hits) const {
  HitAccum currentCluster(layer, _detectorGeometry, hits);

  for (auto const& strip : strips) {
    currentCluster.processStrip(strip);
  }
}

void fc::HitRecoModule::processLayers(const StripSet& stripSet,
                                      HitSet& hitSet) const {
  auto const& layers = stripSet.getStrips();
  for (size_t i = 0; i < layers.size(); ++i) {
    std::cout << "calling makeHits for layer " << i << "\n";
    makeHits(i, layers[i], hitSet);
  }

}







