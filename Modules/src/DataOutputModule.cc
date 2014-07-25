#include "DataObjects/include/GenTrackSet.hh"
#include "Algorithms/include/GenTrackSetIO.hh"
#include "DataObjects/include/HitSet.hh"
#include "Algorithms/include/HitSetIO.hh"
#include "DataObjects/include/StripSet.hh"
#include "Algorithms/include/StripSetIO.hh"
#include "Modules/include/DataOutputModule.hh"


fc::DataOutputModule::DataOutputModule(int debugLevel, std::ofstream& outputeventdatafile,
				       const std::string& iInputTracksLabel,
				       const std::string& iInputHitsLabel,
				       const std::string& iInputStripsLabel,
				       const DetectorGeometry & detectorGeometry):
  _debugLevel(debugLevel),
  _inTracksLabel(iInputTracksLabel),
  _inHitsLabel(iInputHitsLabel),
  _inStripsLabel(iInputStripsLabel),
  _detectorGeometry(detectorGeometry),
  _outputeventdatafile(outputeventdatafile){

  _outputeventdatafile << detectorGeometry.getDetectorGeometryVersion() << std::endl;

}

void fc::DataOutputModule::processEvent(Event& event) {

  //Handle<GenTrackSet> genTrackSet = event.get<GenTrackSet>(_inTracksLabel);
  auto genTrackSet = event.get<fc::GenTrackSet>(_inTracksLabel);
  auto hitSet = event.get<fc::HitSet>("hits");
  auto stripSet = event.get<fc::StripSet>("strips");

  if (_debugLevel >=2) std::cout << "Event: " << event.eventNumber() << std::endl;
  if (_debugLevel >=2) genTrackSet->print();
  if (_debugLevel >=2) hitSet->print();
  if (_debugLevel >=2) stripSet->print();

  _outputeventdatafile << event.eventNumber() << std::endl;



  GenTrackSetIO genTrackSetIO;
  genTrackSetIO.writeEvent(*genTrackSet,_outputeventdatafile);

  HitSetIO hitSetIO;
  hitSetIO.writeEvent(*hitSet,_outputeventdatafile);

  StripSetIO stripSetIO(_detectorGeometry);
  stripSetIO.writeEvent(*stripSet,_outputeventdatafile);

}



