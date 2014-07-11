#include "TrackSet.hh"
#include "HitSet.hh"
#include "HitSetIO.hh"
#include "StripSet.hh"
#include "StripSetIO.hh"
#include "DataInputModule.hh"
#include <string>
#include "Exception.hh"

fc::DataInputModule::DataInputModule(int debugLevel, const DetectorGeometry& detectorGeometry,std::ifstream& inputeventdatafile):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _inputeventdatafile(inputeventdatafile){

  int inputDetectorGeometryVersion;
  _inputeventdatafile >> inputDetectorGeometryVersion ;

  if (inputDetectorGeometryVersion != _detectorGeometry.getDetectorGeometryVersion()){
    std::string wrongDetectorGeometryVersion = "DataInputModule constructor: wrong detector geometry version: Set up for " + std::to_string(_detectorGeometry.getDetectorGeometryVersion()) + ", reading " + std::to_string(inputDetectorGeometryVersion);
    throw Exception(wrongDetectorGeometryVersion); 
  }


}

void fc::DataInputModule::processEvent(Event& event) {

  auto genData = event.get<bool>("genData");


std::unique_ptr<TrackSet> trackSet{ new TrackSet(_detectorGeometry) };


  std::unique_ptr<HitSet> hitSet{ new HitSet(*genData) };
  std::unique_ptr<StripSet> stripSet{ new StripSet(*genData) };

  int eventNumber;
  _inputeventdatafile >> eventNumber;


  trackSet->readEvent(_inputeventdatafile);

  HitSetIO hitSetIO;
  hitSetIO.readEvent(*hitSet,_inputeventdatafile);


  // !!!!! chance to a construtor calling StripSetIO::readEvent and passing to an event object
  StripSetIO stripSetIO(_detectorGeometry);
  //stripSetIO.printRawData(_inputeventdatafile);
  // don't call rest of processEvent if you are going to do this  !!!!! could reset file pointer


  stripSetIO.readEvent(*stripSet,_inputeventdatafile);

  if (_debugLevel >=2) trackSet->print();
  if (_debugLevel >=2) hitSet->print();
  if (_debugLevel >=2) stripSet->print();

  event.put("tracksWithHits", std::move(trackSet) );
  event.put("hits", std::move(hitSet));
  event.put("strips",std::move(stripSet));



}






