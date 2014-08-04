#include<iostream>
#include "Geometry/include/StripHitFunctions.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Hit.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/HitRecoModule.hh"

fc::HitRecoModule::HitRecoModule(int debugLevel,
                                 const std::string& iInputStripsLabel, const std::string& iOutputHitsLabel,
                                 const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _inStripsLabel(iInputStripsLabel),
    _outHitsLabel(iOutputHitsLabel),
    _detectorGeometry(detectorGeometry) {
}

void fc::HitRecoModule::processEvent(fc::Event& event)
{
    Handle<StripSet> genStripSet = event.get<StripSet>(_inStripsLabel);
    std::unique_ptr<HitSet> recoHitSet( new HitSet );

    recoHits(*genStripSet,*recoHitSet);

    if (_debugLevel >= 2) recoHitSet->print(std::cout);

    event.put(_outHitsLabel, std::move(recoHitSet));
}

void fc::HitRecoModule::recoHits(const StripSet& stripSet,
                                 HitSet & hitSet) const {



}







