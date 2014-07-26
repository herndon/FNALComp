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
#include "DataObjects/include/Helix.hh"

class TVector3;

namespace fc {


  TVector3 calcXAt(const fc::Helix&,double phi);
  TMatrixD calcDxDHC(const fc::Helix&, double phi);
  TMatrixD calcDxDphi(const fc::Helix&, double phi);

  TMatrixD calcD1DDHC(const TVector3& hitPosition, const TMatrixD &dxphiadHC, const TMatrixD &);


  //void MoveTo(const TVector & x0, TMatrixD & propMatrix, TMatrixD & covMatrix); 

} // end namescape fc

#endif // TrackFitAlgorithms_hh

