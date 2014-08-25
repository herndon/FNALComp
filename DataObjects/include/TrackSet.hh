#ifndef DataObjects_TrackSet_hh
#define DataObjects_TrackSet_hh
//============================================================================
// TrackSet.hh
// header with class definition of the TrackSet
// A vector of Tracks
// Map elements are int,Track pairs of track number (Key) and Track (value) information
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-06
//============================================================================
#include <vector>
#include <list>
#include "DataObjects/include/Track.hh"

namespace fc {

typedef std::vector<Track> TrackSetContainer;
typedef std::list<Track> FastTrackSetContainer;

///
/// Class TrackSet:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a>
///

class TrackSet {
public:

    const TrackSetContainer & tracks() const {
        return _trackSet;
    };

    void insertTrack(Track &&);

    void print(ostream& out) const;

private:

    TrackSetContainer _trackSet;
};
} // end namespace fc

#endif // DataObjects_TrackSet_hh

