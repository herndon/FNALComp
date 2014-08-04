#ifndef Modules_TrackCandidateModule_hh
#define Modules_TrackCandidateModule_hh
//============================================================================
// TrackCandidateModule.hh
// Module for finding candidate track to pursue in reconstruction
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-30
//============================================================================

#include "Framework/include/Module.hh"
#include <vector>


namespace fc {

class Config;
class DetectorGeometry;
class HitSet;
class TrackSet;


///
/// Class TrackCandidateModule  Module for reconstructing hits from strip infomation
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-11
///

class TrackCandidateModule : public Module {

public:

    TrackCandidateModule(int, const std::string& inputHitsLabel,
                         const std::string& outputTracksLabel,
                         const Config&, const DetectorGeometry &);

    void processEvent(Event&) override;


private:

    int _debugLevel;

    const std::string _inHitsLabel;
    const std::string _outTracksLabel;

    // Configuration object for tracking parameters
    const Config& _config;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    // Tracking parameters



    void findTrackCandidates(const HitSet& hitSet,TrackSet & trackSet) const;

};
} // end namespace fc

#endif // Modules_TrackCandidateModule_hh
