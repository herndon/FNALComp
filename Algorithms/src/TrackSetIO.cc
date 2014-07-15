#include <string>
#include <array>
#include "Exception.hh"
#include "TrackSet.hh"
#include "TrackSetIO.hh"
#include <limits>

// !!!!! Do we need this one
fc::TrackSetIO::TrackSetIO(const DetectorGeometry & myDetectorGeometry):
  _version(1),
  _myDetectorGeometry(myDetectorGeometry) {

  // Intialize commonly used DetectorGeometry data
  _curvatureC = _myDetectorGeometry.getCurvatureC();

}


void fc::TrackSetIO::writeEvent(const TrackSet & trackSet, std::ofstream & trackdata) const{

  if (_version != trackSet.getVersion()) {
    std::string wrongStreamerVersion = "TrackSetIO::writeEvent: TrackSet expected streamer version " + std::to_string(trackSet.getVersion()) + " using streamer version " + std::to_string(_version);
    throw Exception(wrongStreamerVersion);  
  }


  // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
  trackdata.precision(std::numeric_limits<double>::digits10 + 2);

  trackdata << "Tracks" << std::endl;
  trackdata << trackSet.getVersion() << std::endl;
  trackdata << trackSet.getGenTracks() << std::endl;

  std::vector<Track>::size_type numberTracks = trackSet.getConstTrackVector().size();
  trackdata << numberTracks << std::endl;

  int trackNumber;

  for (std::vector<Track>::const_iterator trackIter =  trackSet.getConstTrackVector().begin(); trackIter != trackSet.getConstTrackVector().end(); ++trackIter, ++trackNumber){

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

void fc::TrackSetIO::readEvent(TrackSet & trackSet, std::ifstream & trackdata) {

  std::string eventDataObject;
  int version;
  bool genTracks;
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

  trackdata >> genTracks;
  trackSet.setGenTracks(genTracks);

  trackdata >> numberTracks;

  for (int ii_track = 0; ii_track < numberTracks; ++ii_track) {
 
    trackdata >> trackNumber;

    trackdata >> charge;

    trackdata >> p4[0];
    trackdata >> p4[1];
    trackdata >> p4[2];
    trackdata >> p4[3];

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
 
    trackSet.insertTrack(track);

  } // end track loop 

}