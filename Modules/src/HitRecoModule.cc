#include<iostream>
#include<numeric>
#include "Geometry/include/StripHitFunctions.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Hit.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/HitRecoModule.hh"

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

  recoHits(*genStripSet, *recoHitSet);

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

struct HitData {
  HitData(): start(-1), curr_strip(-1), cnts() { }

  void clear() { start = -1; curr_strip = -1; cnts.clear(); }
  void add(int strip, int cnt) { curr_strip = strip; cnts.push_back(cnt); }
  void begin(int strip, int cnt) { start = strip; curr_strip = strip; cnts.push_back(cnt); }
  bool isAdjacent(int strip) { return strip == curr_strip + 1; }
  bool makingCluster() { return start >= 0; }

  int start;
  int curr_strip;
  std::vector<int> cnts;
};

void fc::HitRecoModule::makeHits(int layer,
                                 LayerStripMap const& strips,
                                 HitSet& hits) const {
  //int curr_start_strip = -1;
  //int curr_strip = -1;
  //std::vector<int> curr_cnts;

  HitData data;
  SensorDescriptor const& desc = _detectorGeometry.getSensor(layer);

  for (auto it = strips.cbegin(), end = strips.cend(); it != end; ++it) {
    //std::cout << "layer=" << layer << " strip=" << it->first << " " << it->second << "\n";

    if (data.makingCluster()) {
      // ongoing cluster ...
      if (it->second > desc._threshold && data.isAdjacent(it->first)) {
        data.add(it->first, it->second);
      }
      else {
        // make hit
        makeHit(layer, data.start, data.cnts, desc, hits);
        // clear holders
        data.clear();
      }
    }
    else {
      // no cluster ...
      if (it->second > desc._threshold) {
        data.begin(it->first, it->second);
      }
    }
  }
  if (data.makingCluster()) {
    // close last cluster
    std::cout << "got to close last cluster" << "\n";
    makeHit(layer, data.start, data.cnts, desc, hits);
  }
}

void fc::HitRecoModule::recoHits(const StripSet& stripSet,
                                 HitSet& hitSet) const {
  auto const& layers = stripSet.getStrips();
  for (size_t i = 0; i < layers.size(); ++i) {
    std::cout << "calling makeHits for layer " << i << "\n";
    makeHits(i, layers[i], hitSet);
  }

}







