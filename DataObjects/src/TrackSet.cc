#include <string>
#include <array>
#include "Exception.hh"
#include "TrackSet.hh"
#include <limits>

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
  _trackVector.push_back(track);
}

void fc::TrackSet::writeEvent(std::ofstream & trackdata) const{

  //  trackdata.write (reinterpret_cast<const char *>(&_eventNumber), 1);

  // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
  trackdata.precision(std::numeric_limits<double>::digits10 + 2);

  trackdata << "Tracks" << std::endl;
  trackdata << _version << std::endl;
  trackdata << _genTracks << std::endl;
  trackdata << _eventNumber << std::endl;

  std::vector<Track>::size_type numberTracks =_trackVector.size();
  trackdata << numberTracks << std::endl;

  int trackNumber;

  for (std::vector<Track>::const_iterator trackIter =  _trackVector.begin(); trackIter != _trackVector.end(); ++trackIter, ++trackNumber){

    // Extract a copy of the lorentzVector since functions in it are not const
    TLorentzVector lorentzVector = trackIter->getLorentzVector();

    trackdata << trackNumber << std::endl;

    trackdata << trackIter->getCharge() << std::endl;

    trackdata << lorentzVector.Px() << std::endl;
    trackdata << lorentzVector.Py() << std::endl;
    trackdata << lorentzVector.Pz() << std::endl;
    trackdata << lorentzVector.E()  << std::endl;

    // Point of clossest approach to the the reference point 0 0 0

    trackdata << trackIter->getHelix().getDr()*std::cos(trackIter->getHelix().getPhi0()) <<std::endl;
    trackdata << trackIter->getHelix().getDr()*std::sin(trackIter->getHelix().getPhi0()) <<std::endl;
    trackdata << trackIter->getHelix().getDz() <<std::endl;

    trackHitMap::size_type numberHits = trackIter->getTrackHitMap().size();

    trackdata << numberHits << std::endl;

    // !!!!! Nice place for that reference bug 
    for (trackHitMap::const_iterator trackHitMapIter = trackIter->getTrackHitMap().begin(); trackHitMapIter != trackIter->getTrackHitMap().end(); ++trackHitMapIter){
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
  std::array<double,4> p4;
  std::array<double,3> x3;
  int numberHits;
  int hitNumber;
  int hitLayer;

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

    // !!!!! could this be done better
    TLorentzVector p(p4[0],p4[1],p4[2],p4[3]);
 
    trackdata >> x3[0];
    trackdata >> x3[1];
    trackdata >> x3[2];

    TVector3 dr(x3[0],x3[1],x3[2]);

 
    Track track(p,charge,dr,_myDetectorGeometry);
    
   trackdata >> numberHits;
 

   for (int ii_hit = 0; ii_hit < numberHits; ++ii_hit) {

      trackdata >> hitNumber;
      trackdata >> hitLayer;

      track.insertHit(hitNumber,hitLayer);

    } // end hit loop
 
    insertTrack(track);

  } // end track loop 

}


void fc::TrackSet::print(void) const{

  std::cout << "Event: " << _eventNumber << std::endl;

  std::vector<Track>::size_type numberTracks =_trackVector.size();

  std::cout << "Number Tracks: " << numberTracks << std::endl; 

  int trackNumber = 0;

  for (std::vector<Track>::const_iterator trackIter =  _trackVector.begin(); trackIter != _trackVector.end(); ++trackIter,++trackNumber){
 
    std::cout << "Track number: " << trackNumber << std::endl;
    trackIter->print(); 

  } // end track loop

}
