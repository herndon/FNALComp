#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Algorithms/include/GenTrackSetIO.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "Algorithms/include/GenHitSetIO.hh"
#include "DataObjects/include/StripSet.hh"
#include "Algorithms/include/StripSetIO.hh"
#include "Modules/include/DataOutputModule.hh"


fc::DataOutputModule::DataOutputModule(int debugLevel,std::ofstream & debugfile,
                                       std::ofstream& outputeventdatafile,
                                       const std::string& iInputTracksLabel,
                                       const std::string& iInputHitsLabel,
                                       const std::string& iInputStripsLabel,
                                       const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _debugfile(debugfile),
    _inTracksLabel(iInputTracksLabel),
    _inHitsLabel(iInputHitsLabel),
    _inStripsLabel(iInputStripsLabel),
    _detectorGeometry(detectorGeometry),
    _outputeventdatafile(outputeventdatafile) {

    _outputeventdatafile << detectorGeometry.detectorGeometryVersion() <<
                         std::endl;

}

void fc::DataOutputModule::processEvent(Event& event) {

    auto genTrackSet = event.get<fc::GenTrackSet>(_inTracksLabel);
    auto genHitSet = event.get<fc::GenHitSet>(_inHitsLabel);
    auto stripSet = event.get<fc::StripSet>(_inStripsLabel);

    if (_debugLevel >=2) _debugfile << "Event: " << event.eventNumber() << std::endl;
    if (_debugLevel >=2) genTrackSet->print(_debugfile);
    if (_debugLevel >=2) genHitSet->print(_debugfile);
    if (_debugLevel >=2) stripSet->print(_debugfile);

    _outputeventdatafile << event.eventNumber() << std::endl;



    GenTrackSetIO genTrackSetIO;
    genTrackSetIO.writeEvent(*genTrackSet,_outputeventdatafile);

    GenHitSetIO genHitSetIO;
    genHitSetIO.writeEvent(*genHitSet,_outputeventdatafile);

    StripSetIO stripSetIO(_detectorGeometry);
    stripSetIO.writeEvent(*stripSet,_outputeventdatafile);

}



