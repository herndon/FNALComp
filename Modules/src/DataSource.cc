#include "Geometry/include/DetectorGeometry.hh"
#include "Framework/include/Event.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Algorithms/include/GenTrackSetIO.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "Algorithms/include/GenHitSetIO.hh"
#include "DataObjects/include/StripSet.hh"
#include "Algorithms/include/StripSetIO.hh"
#include "Modules/include/DataSource.hh"
#include "Services/include/Exception.hh"
#include <string>

fc::DataSource::DataSource(int debugLevel,std::ifstream& inputeventdatafile,
                           bool genData,
                           const std::string& iOutputTracksLabel,
                           const std::string& iOutputHitsLabel,
                           const std::string& iOutputStripsLabel,
                           const DetectorGeometry& detectorGeometry):
    _outTracksLabel(iOutputTracksLabel),
    _outHitsLabel(iOutputHitsLabel),
    _outStripsLabel(iOutputStripsLabel),
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _inputeventdatafile(inputeventdatafile),
    _genData(genData) {

    int inputDetectorGeometryVersion;
    _inputeventdatafile >> inputDetectorGeometryVersion ;

    if (inputDetectorGeometryVersion !=
            _detectorGeometry.detectorGeometryVersion()) {
        std::string wrongDetectorGeometryVersion =
            "DataSource constructor: wrong detector geometry version: Set up for " +
            std::to_string(_detectorGeometry.detectorGeometryVersion()) + ", reading " +
            std::to_string(inputDetectorGeometryVersion);
        throw Exception(wrongDetectorGeometryVersion);
    }


}

std::unique_ptr<fc::Event> fc::DataSource::getNextEvent() {


    std::unique_ptr<GenTrackSet> genTrackSet { new GenTrackSet };

    std::unique_ptr<GenHitSet> genHitSet { new GenHitSet };
    std::unique_ptr<StripSet> stripSet { new StripSet(_detectorGeometry)};

    int eventNumber = 0;

    if( not (_inputeventdatafile >> eventNumber) ) {
        return std::unique_ptr<Event> {};
    }

    if ((_debugLevel >=2) || (_debugLevel>=1
                              && (eventNumber % 1000) == 0)) std::cout << "Event Number: " << eventNumber <<
                                          std::endl;


    GenTrackSetIO genTrackSetIO;
    genTrackSetIO.readEvent(*genTrackSet,_inputeventdatafile);

    GenHitSetIO genHitSetIO;
    genHitSetIO.readEvent(*genHitSet,_inputeventdatafile);


    // !!!!! chance to a construtor calling StripSetIO::readEvent and passing to an event object
    StripSetIO stripSetIO(_detectorGeometry);
    //stripSetIO.printRawData(_inputeventdatafile);
    // don't call rest of processEvent if you are going to do this  !!!!! could reset file pointer


    stripSetIO.readEvent(*stripSet,_inputeventdatafile);


    std::unique_ptr<fc::Event> event( new fc::Event {static_cast<unsigned int>(eventNumber)} );

    if (_debugLevel >=2) std::cout << "Event: " << event->eventNumber() <<
                                       std::endl;
    if (_debugLevel >=2) genTrackSet->print(std::cout);
    if (_debugLevel >=2) genHitSet->print(std::cout);
    if (_debugLevel >=2) stripSet->print(std::cout);

    event->put("genData", std::unique_ptr<bool>( new bool {_genData} ) );
    event->put(_outTracksLabel, std::move(genTrackSet) );
    event->put(_outHitsLabel, std::move(genHitSet));
    event->put(_outStripsLabel,std::move(stripSet));

    return std::move(event);
}






