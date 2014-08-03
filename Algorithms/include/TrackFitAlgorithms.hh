#ifndef Algorithms_TrackFitAlgorithms_hh
#define Algorithms_TrackFitAlgorithms_hh
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

class TVector3;

namespace fc {

class Helix;

const TVector3 calcXAt(const fc::Helix&,double phi);
const TMatrixD calcDxDHC(const fc::Helix&, double phi);
const TMatrixD calcDxDphi(const fc::Helix&, double phi);

const TMatrixD calcD1DDHC(const TVector3& hitPosition,
                          const TMatrixD &dxphiadHC, const TMatrixD &);


//void MoveTo(const TVector & x0, TMatrixD & propMatrix, TMatrixD & covMatrix);

} // end namescape fc

#endif // Algorithms_TrackFitAlgorithms_hh

