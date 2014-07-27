#include <string>
#include <array>
#include "Services/include/Exception.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Algorithms/include/TrackSetIO.hh"
#include <limits>

// !!!!! Do we need this one
fc::TrackSetIO::TrackSetIO(const DetectorGeometry & detectorGeometry):
  _version(1),
  _detectorGeometry(detectorGeometry) {

}


void fc::TrackSetIO::writeEvent(const TrackSet & trackSet, std::ofstream & trackdata) const{

  // set precision output to precision of a doulbe + 2 digits to avoid rounding problems
  trackdata.precision(std::numeric_limits<double>::digits10 + 2);

  trackdata << "Tracks" << std::endl;
  trackdata << _version << std::endl;

  trackSet::size_type numberTracks = trackSet.getTracks().size();
  trackdata << numberTracks << std::endl;

  int trackNumber;

  for (trackSet::const_iterator trackIter =  trackSet.getTracks().begin(); trackIter != trackSet.getTracks().end(); ++trackIter, ++trackNumber){

    // Extract a copy of the lorentzVector since functions in it are not const
    TLorentzVector lorentzVector = trackIter->getLorentzVector();

    trackdata << trackNumber << std::endl;

    trackdata << trackIter->getCharge() << std::endl;

    trackdata << lorentzVector.Px() << std::endl;
    trackdata << lorentzVector.Py() << std::endl;
    trackdata << lorentzVector.Pz() << std::endl;
    trackdata << lorentzVector.E()  << std::endl;

    // Point of clossest approach to the the reference point 0 0 0 and sign

    trackdata << std::abs(trackIter->getHelix().getDr())*std::cos(trackIter->getHelix().getPhi0()) <<std::endl;
    trackdata << std::abs(trackIter->getHelix().getDr())*std::sin(trackIter->getHelix().getPhi0()) <<std::endl;
    trackdata << trackIter->getHelix().getDz() <<std::endl;

    int d0Sign;
    if (std::signbit(trackIter->getHelix().getDr())) {d0Sign = -1;} else {d0Sign = 1;}
    trackdata << d0Sign << std::endl;


    trackHitSet::size_type numberHits = trackIter->getHits().size();

    trackdata << numberHits << std::endl;

    // !!!!! Nice place for that reference bug 
    for (trackHitSet::const_iterator hitIter = trackIter->getHits().begin(); hitIter != trackIter->getHits().end(); ++hitIter){
      trackdata << *hitIter << std::endl;
    } // end hit loop

  } // end track loop
}

void fc::TrackSetIO::readEvent(TrackSet & trackSet, std::ifstream & trackdata) {

  std::string eventDataObject;
  int version;
  int numberTracks;
  int trackNumber;
  int charge;
  std::array<double,4> p4;
  std::array<double,3> x3;
  int d0Sign;
  int numberHits;
  int hitNumber;

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
 
    trackdata >> d0Sign;

    Track track(p,charge,dr,d0Sign,_detectorGeometry);
    
    trackdata >> numberHits;
 

   for (int ii_hit = 0; ii_hit < numberHits; ++ii_hit) {

      trackdata >> hitNumber;

      track.insertHit(hitNumber);

    } // end hit loop
 
    trackSet.insertTrack(track);

  } // end track loop 

}
