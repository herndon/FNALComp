#ifndef DataObjects_Hit_hh
#define DataObjects_Hit_hh
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

#include "TVector3.h"

namespace fc {

///
/// Class Hit:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/Hit.pdf">Hit.pdf</a> 
///

class Hit {
public:

  Hit(const TVector3&, int, int); //!< TVector3 hitPosition, int layer, int trackNumber
  Hit(const TVector3&, int, int, int, bool, double);      //!< TVector3 hitPosition, int layer, int numberStrips, int charge, bool goodHit, double resolution 

  const TVector3 & getHitPosition() const {return _hitPosition; }
  int getLayer() const {return _layer;}
  int getNumberStrips() const {return _numberStrips;}
  int getCharge() const {return _charge;}
  bool isGoodHit() const {return _goodHit;}
  double getResolution() const {return _resolution;}
  int getTrackNumber() const {return _trackNumber;}

  void print(ostream& out) const;

private:

  TVector3 _hitPosition;
  int _layer;
  int _numberStrips;
  int _charge;
  bool _goodHit;
  double _resolution;
  int _trackNumber;  
};
} // end namespace fc
#endif // DataObjects_Hit_hh
