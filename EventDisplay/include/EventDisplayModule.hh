#ifndef EventDisplay_EventDisplayModule_hh
#define EventDisplay_EventDisplayModule_hh
//============================================================================
// EventDisplayModule.hh
// header with class definition for an root EVE event display
//
// Author Matt Herndon, University of Wisconsin,
//
// 2014-08-13
//============================================================================
#include <vector>
#include "TVector3.h"

namespace fc {


///
/// Class EventDisplayModule
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-08-13
///

class EventDisplayModule {


public:


    EventDisplayModule();

  void displayGeometry();

private:

    bool            _drawGenTracks;
    bool            _drawGenHits;
    bool            _drawRecoTracks;
    bool            _drawSeedRecoTracks;
    bool            _drawRecoHits;


};
} // end namespace fc

#endif // EventDisplay_EventDisplayModule_hh
