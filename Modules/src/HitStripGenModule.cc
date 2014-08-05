#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include "Services/include/Random.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "DataObjects/include/Helix.hh"
#include "Algorithms/include/HelixIntersections.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/StripHitFunctions.hh"
#include "Modules/include/HitStripGenModule.hh"

fc::HitStripGenModule::HitStripGenModule(int debugLevel,
        const std::string& iInputTracksLabel,
        const std::string& iOutputHitsLabel,
        const std::string& iOutputStripsLabel,
        const DetectorGeometry & detectorGeometry, Random & random):
    _inTracksLabel(iInputTracksLabel),
    _outHitsLabel(iOutputHitsLabel),
    _outStripsLabel(iOutputStripsLabel),
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _random(random) {
}

void fc::HitStripGenModule::processEvent(fc::Event & event)
{

    Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_inTracksLabel);

    std::unique_ptr<GenHitSet> genHitSet { new GenHitSet };
    std::unique_ptr<StripSet> genStripSet { new StripSet(_detectorGeometry)};

    int trackNumber = 0;

    for (auto const& genTrack :  genTrackSet->getGenTracks()) {

        makeHitsStrips(genTrack,trackNumber,*genHitSet, *genStripSet);
        ++trackNumber;

    } // end track loop

    event.put(_outHitsLabel, std::move(genHitSet));
    event.put(_outStripsLabel,std::move(genStripSet));
}


void fc::HitStripGenModule::makeHitsStrips(const GenTrack & genTrack,
        int trackNumber,GenHitSet& genHitSet, StripSet & stripSet) const {


    TVector3 hitPosition;


    for (int iiLayer = 0; iiLayer < _detectorGeometry.getNSensors(); ++iiLayer) {

        // 98% efficiency factor
        if (_random.getUniformDouble(0.0,
                                     1.0) > _detectorGeometry.getSensor(iiLayer)._hitEfficiency) continue;
        bool intersectedLayer = intersectWithLayer(genTrack.makeHelix(
                                    _detectorGeometry.getBField(),_detectorGeometry.getCurvatureC()),
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

    if (_debugLevel >=5 ) {
        std::cout << "Layer " << layer << " Hit y " << hitPosition[0] << std::endl;
        std::cout << "Layer " << layer << " Hit x " << hitPosition[1] << std::endl;
    }

    // Pure gen hit, numberStrip = -1, charge -1, resolution 0.0, goodHit true
    GenHit hit(hitPosition,layer,trackNumber);

    bool isValidHit = fcf::isValidHit(layer,hitPosition,_detectorGeometry);

    if (isValidHit) {

        genHitSet.insertGenHit(std::move(hit));


    }


    hitPosition = hitPosition + _random.getNormalDouble(0.0,
                  _detectorGeometry.getSensor(layer)._hitResolution)*_detectorGeometry.getSensor(
                      layer)._measurementDirection;

    if (_debugLevel >=5 ) {
        std::cout << "Layer " << layer << " Resolution smeared Hit x " << hitPosition[0]
                  << std::endl;
    }

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

    if (_debugLevel >=5) std::cout << "stripHitPosition " << stripHitPosition
                                       <<std::endl;
    if (_debugLevel >=5) std::cout << "strip " << strip << std::endl;
    if (_debugLevel >=5) std::cout << "remainder " << remainder << std::endl;



    if (remainder < 0.0) {
        initialStrip = strip - 1;
        stripAdcVector.push_back(-1.0*remainder*_detectorGeometry.getMIP());
        stripAdcVector.push_back((1.0+remainder)*_detectorGeometry.getMIP());
        if (_debugLevel >=5) std::cout << "initialStrip " << initialStrip << std::endl;
        if (_debugLevel >=5) std::cout << -1.0*remainder*_detectorGeometry.getMIP() <<
                                           " " << (1.0+remainder)*_detectorGeometry.getMIP() << std::endl;
    } else {
        initialStrip = strip;
        stripAdcVector.push_back((1.0-remainder)*_detectorGeometry.getMIP());
        stripAdcVector.push_back(remainder*_detectorGeometry.getMIP());
        if (_debugLevel >=5) std::cout << "initialStrip " << initialStrip << std::endl;
        if (_debugLevel >=5) std::cout << (1.0-remainder)*_detectorGeometry.getMIP() <<
                                           " " << remainder*_detectorGeometry.getMIP() << std::endl;
    }

}


void fc::HitStripGenModule::storeCluster(int layer, int initialStrip,
        const std::vector<int> & stripAdcs,StripSet & stripSet) const {

    int iiStrip = initialStrip;
    for (auto adc : stripAdcs) {
        if (fcf::isValidStrip(layer,iiStrip,
                              _detectorGeometry) && adc > 0 ) stripSet.insertStrip(layer,iiStrip,adc);
        ++iiStrip;
    }

}
