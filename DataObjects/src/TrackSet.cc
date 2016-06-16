#include <iostream>
#include "DataObjects/include/TrackSet.hh"

// Using Default constructor


void fc::TrackSet::insertTrack(Track&& track) {
    _trackSet.push_back(std::move(track));
}

void fc::TrackSet::print(std::ostream& out) const {

    out << "TrackSet " << std::endl;

    TrackSetContainer::size_type numberTracks =tracks().size();

    out << "Number Tracks: " << numberTracks << std::endl;

    int trackNumber = 0;

    for (auto const& track: tracks()) {

        out << "Track number: " << trackNumber << std::endl;
        track.print(out);
        ++trackNumber;

    } // end track loop

}
