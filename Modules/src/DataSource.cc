#include "DataObjects/include/GenTrackSet.hh"
#include "Algorithms/include/GenTrackSetIO.hh"
#include "DataObjects/include/HitSet.hh"
#include "Algorithms/include/HitSetIO.hh"
#include "DataObjects/include/StripSet.hh"
#include "Algorithms/include/StripSetIO.hh"
#include "Modules/include/DataSource.hh"
#include "Services/include/Exception.hh"
#include <string>

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


  std::unique_ptr<GenTrackSet> genTrackSet{ new GenTrackSet() };


  std::unique_ptr<HitSet> hitSet{ new HitSet(_genData) };
  std::unique_ptr<StripSet> stripSet{ new StripSet(_genData) };

  int eventNumber = 0;
  if( not (_inputeventdatafile >> eventNumber) ) {
    return std::unique_ptr<Event>{};
  }

  GenTrackSetIO genTrackSetIO;
  genTrackSetIO.readEvent(*genTrackSet,_inputeventdatafile);

  HitSetIO hitSetIO;
  hitSetIO.readEvent(*hitSet,_inputeventdatafile);


  // !!!!! chance to a construtor calling StripSetIO::readEvent and passing to an event object
  StripSetIO stripSetIO(_detectorGeometry);
  //stripSetIO.printRawData(_inputeventdatafile);
  // don't call rest of processEvent if you are going to do this  !!!!! could reset file pointer


  stripSetIO.readEvent(*stripSet,_inputeventdatafile);

 
  std::unique_ptr<fc::Event> event( new fc::Event{static_cast<unsigned int>(eventNumber)} );

  if (_debugLevel >=2) std::cout << "Event: " << event->eventNumber() << std::endl;
  if (_debugLevel >=2) genTrackSet->print();
  if (_debugLevel >=2) hitSet->print();
  if (_debugLevel >=2) stripSet->print();

  event->put("genData", std::unique_ptr<bool>( new bool{_genData} ) );
  event->put(_outTracksLabel, std::move(genTrackSet) );
  event->put(_outHitsLabel, std::move(hitSet));
  event->put(_outStripsLabel,std::move(stripSet));

  return std::move(event);
}






