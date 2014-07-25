#ifndef GenTrackSet_hh
#define GenTrackSet_hh
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
private:


  genTrackSet _genTrackSet;
 
public:

  GenTrackSet() {};
  ~GenTrackSet() {};

  const genTrackSet & getGenTracks(void) const { return _genTrackSet;};
 
  void insertTrack(const GenTrack&);

  void print(void) const;


};
} // end namespace fc

#endif // GenTrackSet_hh

