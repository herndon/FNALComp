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
#include "Modules/include/Strip.hh"
#include "Modules/include/Layer.hh"
#include "Modules/include/HitAccum.hh"

fc::HitRecoModule::HitRecoModule(int debugLevel,
                                 const std::string& iInputStripsLabel,
                                 const std::string& iOutputHitsLabel,
                                 const DetectorGeometry& detectorGeometry):
  _debugLevel(debugLevel),
  _inStripsLabel(iInputStripsLabel),
  _outHitsLabel(iOutputHitsLabel),
  _detectorGeometry(detectorGeometry) {
}

void fc::HitRecoModule::processEvent(fc::Event& event) {
  Handle<StripSet> const genStripSet = event.get<StripSet>(_inStripsLabel);
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
  // transform it here, to a vector of pairs. The vector is sorted,
  // which we rely upon.
  Layer currentLayer(layer);
  std::transform(begin(strips), end(strips),
                 std::back_inserter(currentLayer.strips),
  [](LayerStripMap::value_type p) { return Strip(p.first, p.second); });

  HitAccum currentCluster(layer, _detectorGeometry, hits);

  for (auto const& strip : currentLayer.strips) {
    currentCluster.processStrip(strip);
  }
}
