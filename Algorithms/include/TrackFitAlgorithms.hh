#ifndef TrackFitAlgorithms_hh
#define TrackFitAlgorithms_hh
//============================================================================
// TrackFitAlgorithms.hh
// header with function definitions of the TrackFitAlgorithms for TrackFits
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-14
//============================================================================


#include "TMatrixD.h"
#include "Helix.hh"

class TVector3;

namespace fcf {


  TVector3 calcXAt(const fc::Helix&,double phi);
  TMatrixD calcDxDHC(const fc::Helix&, double phi);
  TMatrixD calcDxDphi(const fc::Helix&, double phi);
  TMatrixD calcDXZDHC(const TVector3& hitPosition, const TMatrixD &dxphiadHC);

  TMatrixD calcDXDHC(const TVector3& hitPosition, const TMatrixD &dxphiadHC, const TMatrixD &);


  //void MoveTo(const TVector & x0, TMatrixD & propMatrix, TMatrixD & covMatrix); 

} // end namescape fcf

#endif // TrackFitAlgorithms_hh

