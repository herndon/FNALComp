#ifndef DataObjects_GenTrackSet_hh
#define DataObjects_GenTrackSet_hh
//============================================================================
// GenTrackSet.hh
// header with class definition of the GenTrackSet
// A vector of GenTracks
// See <A HREF="doc/notes/Track.pdf">Track.pdf</A> for more information
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-25
//============================================================================
#include <vector>
#include "DataObjects/include/GenTrack.hh"

namespace fc {

  typedef std::vector<GenTrack> genTrackSet;

///
/// Class GenTrackSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class GenTrackSet {
public:

  const genTrackSet & getGenTracks() const { return _genTrackSet;};
 
  void insertTrack(const GenTrack&);

  void print(ostream& out) const;

private:

  genTrackSet _genTrackSet;
};
} // end namespace fc

#endif // DataObjects_GenTrackSet_hh

