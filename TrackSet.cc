#include <string>
#include "Exception.hh"
#include "TrackSet.hh"

fc::TrackSet::TrackSet(const DetectorGeometry & myDetectorGeometry):
  _version(1),
  _myDetectorGeometry(myDetectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _curvatureC = _myDetectorGeometry.getCurvatureC();

}



fc::TrackSet::TrackSet(int eventNumber, bool genTracks,const DetectorGeometry & myDetectorGeometry):
  _version(1),
  _genTracks(genTracks),
  _eventNumber(eventNumber),
  _myDetectorGeometry(myDetectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _curvatureC = _myDetectorGeometry.getCurvatureC();

}


void fc::TrackSet::insertTrack(int trackNumber, Track track) {
  _trackMap.insert(TrackMap::value_type(trackNumber,track));
}

void fc::TrackSet::writeEvent(std::ofstream & trackdata) const{

  //  trackdata.write (reinterpret_cast<const char *>(&_eventNumber), 1);

  trackdata << "Tracks" << std::endl;
  trackdata << _version << std::endl;
  trackdata << _genTracks << std::endl;
  trackdata << _eventNumber << std::endl;

  std::map<int,Track>::size_type numberTracks =_trackMap.size();
  trackdata << numberTracks << std::endl;

  for (TrackMap::const_iterator trackMapIter =  _trackMap.begin(); trackMapIter != _trackMap.end(); ++trackMapIter){

    // Extract a copy of the lorentzVector since functions in it are not const
    TLorentzVector lorentzVector = getConstLorentzVector(trackMapIter);

    trackdata << getTrackNumber(trackMapIter) << std::endl;

    trackdata << getConstGeoHelix(trackMapIter).GetCharge() << std::endl;

    trackdata << lorentzVector.Px() << std::endl;
    trackdata << lorentzVector.Py() << std::endl;
    trackdata << lorentzVector.Pz() << std::endl;
    trackdata << lorentzVector.E()  << std::endl;

    trackdata << getConstGeoHelix(trackMapIter).GetInitPoint()[0] <<std::endl;
    trackdata << getConstGeoHelix(trackMapIter).GetInitPoint()[1] <<std::endl;
    trackdata << getConstGeoHelix(trackMapIter).GetInitPoint()[2] <<std::endl;    

    trackHitMap::size_type numberHits = getConstTrack(trackMapIter).getTrackHitMap().size();

    trackdata << numberHits << std::endl;

    // !!!!! Nice place for that reference bug 
    for (trackHitMap::const_iterator trackHitMapIter = getConstTrack(trackMapIter).getTrackHitMap().begin(); trackHitMapIter != getConstTrack(trackMapIter).getTrackHitMap().end(); ++trackHitMapIter){
      trackdata << trackHitMapIter->first << std::endl;
      trackdata << trackHitMapIter->second << std::endl;
    } // end hit loop

  } // end track loop
}


void fc::TrackSet::readEvent(std::ifstream & trackdata) {

  std::string eventDataObject;
  int version;
  int numberTracks;
  int trackNumber;
  int charge;
  double p4[4];
  double v3[3];
  int numberHits;
  int hitNumber;
  int hitLayer;
  double bField[3]{_myDetectorGeometry.getBField()[0],_myDetectorGeometry.getBField()[1],_myDetectorGeometry.getBField()[2]};

  trackdata >> eventDataObject;

  if (eventDataObject != "Tracks"){
    std::string wrongEventDataObject = "TrackSet::readEvent: attempted to read wrong data object" + eventDataObject;
    throw Exception(wrongEventDataObject);  
  }

  trackdata >> version;

  if (version != _version) {
    std::string wrongStreamerVersion = "TrackSet::readEvent: attempted to read version " + std::to_string(version) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }

  trackdata >> _genTracks;

  trackdata >> _eventNumber;

  trackdata >> numberTracks;

  for (int ii_track = 0; ii_track < numberTracks; ++ii_track) {
 
    trackdata >> trackNumber;

    trackdata >> charge;

    trackdata >> p4[0];
    trackdata >> p4[1];
    trackdata >> p4[2];
    trackdata >> p4[3];

    trackdata >> v3[0];
    trackdata >> v3[1];
    trackdata >> v3[2];

    trackdata >> numberHits;

    Track track(p4,charge,v3,bField,_curvatureC);
    
    for (int ii_hit = 0; ii_hit < numberHits; ++ii_hit) {

      trackdata >> hitNumber;
      trackdata >> hitLayer;

      track.insertHit(hitNumber,hitLayer);

    } // end hit loop
 
    insertTrack(trackNumber,track);

  } // end track loop 

}

void fc::TrackSet::clear(void){

  _trackMap.clear();

}

void fc::TrackSet::print(void) const{

  std::cout << "Event: " << _eventNumber << std::endl;

  std::map<int,Track>::size_type numberTracks =_trackMap.size();

  std::cout << "Number Tracks: " << numberTracks << std::endl; 

  for (TrackMap::const_iterator trackMapIter =  _trackMap.begin(); trackMapIter != _trackMap.end(); ++trackMapIter){
 
    std::cout << "Track number: " << getTrackNumber(trackMapIter) << std::endl;
    getConstTrack(trackMapIter).print(); 

  } // end track loop

}
