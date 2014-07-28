#include "iostream"
#include "DataObjects/include/GenTrackSet.hh"

// Uses default constructor

void fc::GenTrackSet::insertTrack(const GenTrack& genTrack) {
  _genTrackSet.push_back(genTrack);
}

void fc::GenTrackSet::insertTrack(GenTrack&& genTrack) {
  _genTrackSet.push_back(std::move(genTrack));
}


void fc::GenTrackSet::print(ostream& out) const{

  out << "GenTrackSet" << std::endl;

  genTrackSet::size_type numberGenTracks =_genTrackSet.size();

  out << "Number Gen Tracks: " << numberGenTracks << std::endl; 

  int genTrackNumber = 0;

  for (genTrackSet::const_iterator genTrackIter =  _genTrackSet.begin(); genTrackIter != _genTrackSet.end(); ++genTrackIter,++genTrackNumber){
 
    out << "Gen Track number: " << genTrackNumber << std::endl;
    genTrackIter->print(out); 

  } // end gen track loop

}
