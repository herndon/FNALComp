#include <iostream>
#include<vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackSeedModule.hh"
#include "Tracking/include/TrackSeedStrategy2X1SAS.hh"
#include "Tracking/include/TrackSeedStrategy2X1SASML.hh"
#include "Services/include/Config.hh"
#include "Services/include/Exception.hh"

fc::TrackSeedModule::TrackSeedModule(int debugLevel,
	const std::string& inputHitsLabel, const std::string& outputTracksLabel, const std::string& trackSeedStrategy,
        const Config& config, const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _inHitsLabel(inputHitsLabel),
    _outTracksLabel(outputTracksLabel),
    _trackSeedStrategy(trackSeedStrategy),
    _config(config),
    _detectorGeometry(detectorGeometry) {
}

void fc::TrackSeedModule::processEvent(Event& event)
{

    Handle<HitSet> recoHitSet = event.get<HitSet>(_inHitsLabel);

    std::unique_ptr<TrackSet> recoTrackSeedSet { new TrackSet };

    findTrackSeeds(*recoHitSet,*recoTrackSeedSet);

    if (_debugLevel>=2) {
        std::cout << "Seed track set" << std::endl;
        recoTrackSeedSet->print(std::cout);
    }


    event.put(_outTracksLabel,std::move(recoTrackSeedSet) );
}

void fc::TrackSeedModule::findTrackSeeds(const HitSet& recoHitSet,
        TrackSet & recoTrackSeedSet) const {

   FastTrackSetContainer trackSeedSet;


  if (_trackSeedStrategy=="trackSeedStrategy2X1SAS"){

    TrackSeedStrategy2X1SAS seedStrategy(_debugLevel,_detectorGeometry,
            _config.getMinCandPTCut());
    seedStrategy.findTrackSeeds(recoHitSet,trackSeedSet);


  } else if  (_trackSeedStrategy=="trackSeedStrategy2X1SASML"){

    TrackSeedStrategy2X1SASML seedStrategy(_debugLevel,_detectorGeometry,
            _config.getMinCandPTCut());
    seedStrategy.findTrackSeeds(recoHitSet,trackSeedSet);

  } else {
    throw Exception("TrackSeedModule::findTrackSeeds strategy not recongnized");
  }


    for (auto& track : trackSeedSet) {
        recoTrackSeedSet.insertTrack(std::move(track));
    }


}
