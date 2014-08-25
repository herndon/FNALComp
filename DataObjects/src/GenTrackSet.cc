#include "iostream"
#include "DataObjects/include/GenTrackSet.hh"

// Uses default constructor


void fc::GenTrackSet::insertTrack(GenTrack&& genTrack) {
    _genTrackSet.push_back(std::move(genTrack));
}


void fc::GenTrackSet::print(ostream& out) const {

    out << "GenTrackSet" << std::endl;

    GenTrackSetContainer::size_type numberGenTracks = genTracks().size();

    out << "Number Gen Tracks: " << numberGenTracks << std::endl;

    int genTrackNumber = 0;

    for (auto const& genTrack: genTracks()) {

        out << "Gen Track number: " << genTrackNumber << std::endl;
        genTrack.print(out);
        ++genTrackNumber;
    } // end gen track loop

}
