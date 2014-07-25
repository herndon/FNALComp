#include "iostream"
#include "DataObjects/include/GenTrackSet.hh"

// Uses default constructro

void fc::GenTrackSet::insertTrack(const GenTrack& genTrack) {
  _genTrackSet.push_back(genTrack);
}


void fc::GenTrackSet::print(void) const{

  genTrackSet::size_type numberGenTracks =_genTrackSet.size();

  std::cout << "Number Gen Tracks: " << numberGenTracks << std::endl; 

  int genTrackNumber = 0;

  for (genTrackSet::const_iterator genTrackIter =  _genTrackSet.begin(); genTrackIter != _genTrackSet.end(); ++genTrackIter,++genTrackNumber){
 
    std::cout << "Gen Track number: " << genTrackNumber << std::endl;
    genTrackIter->print(); 

  } // end gen track loop

}
