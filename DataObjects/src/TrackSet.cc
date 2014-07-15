#include "TrackSet.hh"

// !!!!! Do we need this one
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


void fc::TrackSet::insertTrack(Track track) {
  _trackSet.push_back(track);
}



void fc::TrackSet::print(void) const{

  trackSet::size_type numberTracks =_trackSet.size();

  std::cout << "Number Tracks: " << numberTracks << std::endl; 

  int trackNumber = 0;

  for (trackSet::const_iterator trackIter =  _trackSet.begin(); trackIter != _trackSet.end(); ++trackIter,++trackNumber){
 
    std::cout << "Track number: " << trackNumber << std::endl;
    trackIter->print(); 

  } // end track loop

}
