#ifndef Tracking_TrackingFilters_hh
#define Tracking_TrackingFilters_hh
//============================================================================
// TrackingFilters.hh
// header with filter function definitions for Track, TrackCandididate finding strategies
// and LayerFinder
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-30
//============================================================================

#include "DataObjects/include/TrackSet.hh"
#include "Tracking/include/TrackingSelectors.hh"

class DetectorGeoemtry;

namespace fcf {


    // Track selection filters
  void simpleTrackSetFilter(fc::trackSet &,const fc::DetectorGeometry&,const TrackingSelector&);
  void contentionTrackSetFilter(fc::trackSet &);




} // end namescape fcf

#endif // Tracking_TrackingFilters_hh

