#include "TrackSet.hh"
#include "HitSet.hh"
#include "StripSet.hh"
#include "DataOutputModule.hh"


fc::DataOutputModule::DataOutputModule(int debugLevel,int detectorGeometryVersion,std::ofstream& outputeventdatafile):
  _debugLevel(debugLevel),
  _detectorGeometryVersion(detectorGeometryVersion),
  _outputeventdatafile(outputeventdatafile){

  _outputeventdatafile << _detectorGeometryVersion << std::endl;


}

void fc::DataOutputModule::processEvent(const TrackSet & myTrackSet, const HitSet & myHitSet, const StripSet & myStripSet) const{

  if (_debugLevel >=2) myTrackSet.print();
  if (_debugLevel >=2) myHitSet.print();
  if (_debugLevel >=2) myStripSet.print();


  myTrackSet.writeEvent(_outputeventdatafile);
  myHitSet.writeEvent(_outputeventdatafile);
  myStripSet.writeEvent(_outputeventdatafile);

}



