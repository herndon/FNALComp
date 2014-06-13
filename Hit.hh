#ifndef Hit_hh
#define Hit_hh
//============================================================================
// Hit.hh
// header with class definition of the Hit
// Used for both generator level and reconstructed tracks
// See <A HREF="doc/notes/Hit.pdf">Hit.pdf</A> for more information
//
// Will be updated with convariance matrix and perhaps direct tracking information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================

namespace fc {

///
/// Class Hit:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/Hit.pdf">Hit.pdf</a> 
///

class Hit {
private:

  double _hitPosition[3];
  int _trackNumber;  

public:

  Hit(double *, int);
  Hit(double *);
  Hit(const Hit&);
  ~Hit() {};

  const double * getHitPosition(void) const {return _hitPosition; };
  int getTrackNumber(void) const {return _trackNumber;};

  void setTrackNumber(int trackNumber) { _trackNumber = trackNumber;};

  void print(void) const;

};
} // end namespace fc
#endif // Hit_hh
