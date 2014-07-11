#include "TrackSet.hh"
#include "HitSet.hh"
#include "HitSetIO.hh"
#include "StripSet.hh"
#include "StripSetIO.hh"
#include "DataOutputModule.hh"


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

  auto trackSet = event.get<fc::TrackSet>("tracksWithHits");
  auto hitSet = event.get<fc::HitSet>("hits");
  auto stripSet = event.get<fc::StripSet>("strips");

  if (_debugLevel >=2) trackSet->print();
  if (_debugLevel >=2) hitSet->print();
  if (_debugLevel >=2) stripSet->print();

  _outputeventdatafile << event.eventNumber() << std::endl;


  trackSet->writeEvent(_outputeventdatafile);


  HitSetIO hitSetIO;
  hitSetIO.writeEvent(*hitSet,_outputeventdatafile);

  StripSetIO stripSetIO(_detectorGeometry);
  stripSetIO.writeEvent(*stripSet,_outputeventdatafile);

}



