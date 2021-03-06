#include <string>
#include <limits>
#include "Services/include/Exception.hh"
#include "DataObjects/include/HitSet.hh"
#include "Algorithms/include/HitSetIO.hh"


fc::HitSetIO::HitSetIO():
    _version(1) {
}

void fc::HitSetIO::writeEvent(const HitSet & hitSet,
                              std::ofstream & hitdata) const {


    // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
    hitdata.precision(std::numeric_limits<double>::digits10 + 2);

    int hitNumber = 0;

    hitdata << "Hits" << std::endl;
    hitdata << _version << std::endl;

    HitSetContainer::size_type numberHits = hitSet.hits().size();

    hitdata << numberHits << std::endl;

    for (auto const& hit : hitSet.hits()) {

        hitdata << hitNumber << std::endl;
        hitdata << hit.position().x() << std::endl;;
        hitdata << hit.position().y() << std::endl;;
        hitdata << hit.position().z() << std::endl;
        hitdata << hit.layer() << std::endl;
        hitdata << hit.numberStrips() << std::endl;
        hitdata << hit.charge() << std::endl;
        hitdata << hit.goodHit() << std::endl;
        hitdata << hit.resolution() << std::endl;

        ++hitNumber;

    } // end hit loop

}



void fc::HitSetIO::readEvent(HitSet& hitSet, std::ifstream & hitdata) {

    std::string eventDataObject;
    int version;
    int numberHits;
    int hitNumber;
    int layer;
    int numberStrips;
    int charge;
    bool goodHit;
    double resolution;
    TVector3 hitPosition;

    hitdata >> eventDataObject;

    if (eventDataObject != "Hits") {
        std::string wrongEventDataObject =
            "HitSetIO::readEvent: attempted to read wrong data object" + eventDataObject;
        throw Exception(wrongEventDataObject);
    }

    hitdata >> version;

    if (version !=  _version) {
        std::string wrongStreamerVersion =
            "HitSetIO::readEvent: attempted to read version " + std::to_string(
                version) + " using streamer version " + std::to_string(_version);
        throw Exception(wrongStreamerVersion);
    }

    hitdata >> numberHits;

    for (int iiHit = 0; iiHit < numberHits; ++iiHit) {

        hitdata >> hitNumber;
        hitdata >> hitPosition[0];
        hitdata >> hitPosition[1];
        hitdata >> hitPosition[2];
        hitdata >> layer;
        hitdata >> numberStrips;
        hitdata >> charge;
        hitdata >> goodHit;
        hitdata >> resolution;

        Hit hit(hitPosition,layer,numberStrips,charge,goodHit,resolution);

        hitSet.insertHit(std::move(hit));

    } // end hit loop

}
