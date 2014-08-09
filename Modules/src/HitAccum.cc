#include <algorithm>
#include <iostream>

#include "Modules/include/HitAccum.hh"
#include "Geometry/include/StripHitFunctions.hh"

fc::HitAccum::HitAccum(int lyr, DetectorGeometry const& dg, HitSet& results) :
  start(-1),
  curr_strip(-1),
  cnts(),
  hits(results),
  layer(lyr),
  geom(dg),
  desc(dg.getSensor(layer))
{ }

fc::HitAccum::~HitAccum() {
  if (makingCluster()) { makeHit(); }
}

void fc::HitAccum::makeHit() {
  if (cnts.size() < 2) { return; }
  double const pos = fcf::calculateStripHitPositionFromCluster(start, cnts);
  double const local = fcf::calculateLocalFromStripPosition(pos, layer, geom);
  TVector3 const hpos = fcf::calculateGlobalFromLocalPosition(local, layer, geom);
  int const charge = std::accumulate(cnts.begin(), cnts.end(), 0);
  bool const good_hit = cnts.size() <= 2 && charge <= geom.getMIP();
  double const res = good_hit ? desc._hitResolution : desc._badHitResolution;
  hits.insertHit(Hit(hpos, layer, cnts.size(), charge, good_hit, res));

  std::cout << "closing:"
            << " layer " << layer
            << " strip " << start
            << " hits " << cnts.size()
            << " charge " << charge
            << " good " << good_hit
            << " pos " << hpos.x() << " " << hpos.y() << " " << hpos.z()
            << "\n";
  clear();
}

void fc::HitAccum::processStrip(Strip const& strip) {
  if (makingCluster()) {
    // ongoing cluster ...
    if (inSameCluster(strip)) {
      add(strip);
    }
    else {
      makeHit();
      startNewHit(strip.id, strip.cnt);
    }
  }
  else {
    // no cluster ...
    if (goodStrip(strip)) {
      startNewHit(strip.id, strip.cnt);
    }
  }
}
