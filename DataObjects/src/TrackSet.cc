#include <iostream>
#include "DataObjects/include/TrackSet.hh"

fc::TrackSet::TrackSet(const DetectorGeometry & detectorGeometry):
  _detectorGeometry(detectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _curvatureC = _detectorGeometry.getCurvatureC();

}

void fc::TrackSet::insertTrack(const Track& track) {
  _trackSet.push_back(track);
}




void fc::TrackSet::print(void) const{

  std::cout << "TrackSet " << std::endl;

  trackSet::size_type numberTracks =_trackSet.size();

  std::cout << "Number Tracks: " << numberTracks << std::endl; 

  int trackNumber = 0;

  for (trackSet::const_iterator trackIter =  _trackSet.begin(); trackIter != _trackSet.end(); ++trackIter,++trackNumber){
 
    std::cout << "Track number: " << trackNumber << std::endl;
    trackIter->print(); 

  } // end track loop

}
