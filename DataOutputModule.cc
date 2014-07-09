#include "TrackSet.hh"
#include "HitSet.hh"
#include "HitSetIO.hh"
#include "StripSet.hh"
#include "StripSetIO.hh"
#include "DataOutputModule.hh"


fc::DataOutputModule::DataOutputModule(int debugLevel,const DetectorGeometry & detectorGeometry,std::ofstream& outputeventdatafile):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _outputeventdatafile(outputeventdatafile){

  _outputeventdatafile << detectorGeometry.getDetectorGeometryVersion() << std::endl;

}

void fc::DataOutputModule::processEvent(int eventNumber,const TrackSet & trackSet, const HitSet & hitSet, const StripSet & stripSet) const{

  if (_debugLevel >=2) trackSet.print();
  if (_debugLevel >=2) hitSet.print();
  if (_debugLevel >=2) stripSet.print();

  _outputeventdatafile << eventNumber << std::endl;


  trackSet.writeEvent(_outputeventdatafile);


  HitSetIO hitSetIO;
  hitSetIO.writeEvent(hitSet,_outputeventdatafile);

  StripSetIO stripSetIO(_detectorGeometry);
  stripSetIO.writeEvent(stripSet,_outputeventdatafile);

}



