#ifndef Modules_HitAccum_hh
#define Modules_HitAccum_hh

#include <vector>
#include "DataObjects/include/HitSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Modules/include/Strip.hh"

namespace fc {
  // This structure is a helper that is used to accumulate hits within
  // one layer.

  struct HitAccum {

    // Mutable data, representing the current state of the accumulation
    // algorithm.
    int start;
    int curr_strip;
    std::vector<int> cnts;
    HitSet&          hits;

    // Immutable data, representing the context in which this HitAccum
    // object is working.
    int const               layer;
    DetectorGeometry const& geom;
    SensorDescriptor const& desc;

    // C'tor establishes our working context. This accumulator will put
    // hits into 'results'.
    HitAccum(int lyr, DetectorGeometry const& dg, HitSet& results);

    // D'tor makes sure that the last hit is completed.
    ~HitAccum();

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

  };
}

#endif
