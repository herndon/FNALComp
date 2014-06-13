#include "TrackSet.hh"
#include "HitSet.hh"
#include "StripSet.hh"
#include "DataInputModule.hh"
#include <string>
#include "Exception.hh"

fc::DataInputModule::DataInputModule(int debugLevel, int detectorGeometryVersion,std::ifstream& inputeventdatafile):
  _debugLevel(debugLevel),
  _detectorGeometryVersion(detectorGeometryVersion),
  _inputeventdatafile(inputeventdatafile){

  int inputDetectorGeometryVersion;
  _inputeventdatafile >> inputDetectorGeometryVersion ;

  if (inputDetectorGeometryVersion != _detectorGeometryVersion){
    std::string wrongDetectorGeometryVersion = "DataInputModule constructor: wrong detector geometry version: Set up for " + std::to_string(_detectorGeometryVersion) + ", reading " + std::to_string(detectorGeometryVersion);
    throw Exception(wrongDetectorGeometryVersion); 
  }


}

void fc::DataInputModule::processEvent(TrackSet & myTrackSet, HitSet & myHitSet, StripSet & myStripSet) {


  //myStripSet->printRawData(_inputeventdatafile);
  // don't call rest of processEvent if you are going to do this  !!!!! could reset file pointer

  myTrackSet.readEvent(_inputeventdatafile);
  if (_debugLevel >=2) myTrackSet.print();


  myHitSet.readEvent(_inputeventdatafile);
  if (_debugLevel >=2) myHitSet.print();


  myStripSet.readEvent(_inputeventdatafile);

  if (_debugLevel >=2) myStripSet.print();

}






