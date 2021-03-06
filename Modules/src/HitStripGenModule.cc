#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include<cmath>
#include "Services/include/Random.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "DataObjects/include/Helix.hh"
#include "Tracking/include/HelixIntersections.hh"
#include "Tracking/include/InitializeHelix.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/StripHitFunctions.hh"
#include "Modules/include/HitStripGenModule.hh"

fc::HitStripGenModule::HitStripGenModule(int debugLevel,std::ofstream & debugfile,
        const std::string& iInputTracksLabel,
        const std::string& iOutputHitsLabel,
        const std::string& iOutputStripsLabel,
        const DetectorGeometry & detectorGeometry, Random & random):
    _inTracksLabel(iInputTracksLabel),
    _outHitsLabel(iOutputHitsLabel),
    _outStripsLabel(iOutputStripsLabel),
    _debugLevel(debugLevel),
    _debugfile(debugfile), 
   _detectorGeometry(detectorGeometry),
    _random(random) {
}

void fc::HitStripGenModule::processEvent(fc::Event & event)
{

    Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_inTracksLabel);

    std::unique_ptr<GenHitSet> genHitSet { new GenHitSet };
    std::unique_ptr<StripSet> genStripSet { new StripSet(_detectorGeometry)};

    int trackNumber = 0;

    for (auto const& genTrack :  genTrackSet->genTracks()) {

        makeHitsStrips(genTrack,trackNumber,*genHitSet, *genStripSet);
        ++trackNumber;

    } // end track loop

    event.put(_outHitsLabel, std::move(genHitSet));
    event.put(_outStripsLabel,std::move(genStripSet));
}


void fc::HitStripGenModule::makeHitsStrips(const GenTrack & genTrack,
        int trackNumber,GenHitSet& genHitSet, StripSet & stripSet) const {


    TVector3 hitPosition;


    for (int iiLayer = 0; iiLayer < _detectorGeometry.nSensors(); ++iiLayer) {

        // 98% efficiency factor
        if (_random.uniformDouble(0.0,
				  1.0) > _detectorGeometry.sensor(iiLayer).hitEfficiency()) continue;
        bool intersectedLayer = intersectWithLayer(genTrack.makeHelix(
                                    _detectorGeometry.bField(),_detectorGeometry.curvatureC()),
                                iiLayer,_detectorGeometry,hitPosition);

        if (intersectedLayer) {
            storeHitInfo(trackNumber,iiLayer,hitPosition,genHitSet);

            storeStripInfo(hitPosition,iiLayer,stripSet);

        }
    } // end layer loop


}


// !!!!! remove Gen track from here
void fc::HitStripGenModule::storeHitInfo(int trackNumber,int layer,
        TVector3 & hitPosition,GenHitSet & genHitSet) const {


    // Pure gen hit, numberStrip = -1, charge -1, resolution 0.0, goodHit true
    GenHit hit(hitPosition,layer,trackNumber);

    bool isValidHit = fcf::isValidHit(layer,hitPosition,_detectorGeometry);

    if (isValidHit) {

        genHitSet.insertGenHit(std::move(hit));


    }


    hitPosition = hitPosition + _random.normalDouble(0.0,_detectorGeometry.sensor(layer).intrinsicHitResolution())*_detectorGeometry.sensor(layer).measurementDirection();


}


void fc::HitStripGenModule::storeStripInfo(const TVector3 & hitPosition,
        int layer,StripSet & stripSet) const {

    bool isValidHit = fcf::isValidHit(layer,hitPosition,_detectorGeometry);

    if (isValidHit) {

        double localHitPosition = fcf::calclateLocalFromGlobalPostion(hitPosition,
                                  layer,_detectorGeometry);
        double stripHitPosition = fcf::calculateStripFromLocalPosition(localHitPosition,
                                  layer,_detectorGeometry);

        int initialStrip;
        std::vector <int>stripAdcVector;
        generateClusterFromStripHitPosition(stripHitPosition,initialStrip,
                                            stripAdcVector);

        storeCluster(layer,initialStrip,stripAdcVector,stripSet);

    }
}




void fc::HitStripGenModule::generateClusterFromStripHitPosition(
    double stripHitPosition, int & initialStrip,
    std::vector<int> & stripAdcVector) const {

    int strip = stripHitPosition;
    double remainder = stripHitPosition - strip;


    if (remainder < 0.0) {
        initialStrip = strip - 1;
        stripAdcVector.push_back(-1.0*remainder*_detectorGeometry.MIP());
        stripAdcVector.push_back((1.0+remainder)*_detectorGeometry.MIP());
    } else {
        initialStrip = strip;
        stripAdcVector.push_back((1.0-remainder)*_detectorGeometry.MIP());
        stripAdcVector.push_back(remainder*_detectorGeometry.MIP());
    }

}


void fc::HitStripGenModule::storeCluster(int layer, int initialStrip,
        const std::vector<int> & stripAdcs,StripSet & stripSet) const {

    int iiStrip = initialStrip;
    for (auto adc : stripAdcs) {
      if (fcf::isValidStrip(layer,iiStrip,_detectorGeometry) && adc > 0 ) stripSet.insertStrip(layer,iiStrip,adc);
        ++iiStrip;
    }

}
