#ifndef Tracking_TrackingFunctions_hh
#define Tracking_TrackingFunctions_hh
//============================================================================
// TrackingFunctions.hh
// header with helper function definitions for Track and TrackCandididate finding strategies
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-30
//============================================================================

class Track;

namespace fcf {

// Helper functions for track contention filters
unsigned int numberMatchedHits(const fc::Track&, const fc::Track&);
bool betterOverlappingTrack(const fc::Track&, const fc::Track&);

} // end namescape fcf

#endif // Tracking_TrackingFunctions_hh

