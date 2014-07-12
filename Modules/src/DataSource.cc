#include "TrackSet.hh"
#include "HitSet.hh"
#include "HitSetIO.hh"
#include "StripSet.hh"
#include "StripSetIO.hh"
#include "DataSource.hh"
#include <string>
#include "Exception.hh"

fc::DataSource::DataSource(int debugLevel,std::ifstream& inputeventdatafile, bool genData, 
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

  if (inputDetectorGeometryVersion != _detectorGeometry.getDetectorGeometryVersion()){
    std::string wrongDetectorGeometryVersion = "DataSource constructor: wrong detector geometry version: Set up for " + std::to_string(_detectorGeometry.getDetectorGeometryVersion()) + ", reading " + std::to_string(inputDetectorGeometryVersion);
    throw Exception(wrongDetectorGeometryVersion); 
  }


}

std::unique_ptr<fc::Event> fc::DataSource::getNextEvent() {

    //auto genData = event.get<bool>("genData");


  std::unique_ptr<TrackSet> trackSet{ new TrackSet(_detectorGeometry) };


  std::unique_ptr<HitSet> hitSet{ new HitSet(_genData) };
  std::unique_ptr<StripSet> stripSet{ new StripSet(_genData) };

  int eventNumber = 0;
  if( not (_inputeventdatafile >> eventNumber) ) {
    return std::unique_ptr<Event>{};
  }


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

  std::unique_ptr<fc::Event> event( new fc::Event{static_cast<unsigned int>(eventNumber)} );

  event->put("genData", std::unique_ptr<bool>( new bool{_genData} ) );
  event->put(_outTracksLabel, std::move(trackSet) );
  event->put(_outHitsLabel, std::move(hitSet));
  event->put(_outStripsLabel,std::move(stripSet));

  return std::move(event);
}






