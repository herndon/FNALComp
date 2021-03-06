#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<cmath>
#include "Services/include/Random.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "Modules/include/TrackGenModule.hh"
#include "TVector3.h"
#include "TLorentzVector.h"


fc::TrackGenModule::TrackGenModule(int debugLevel, std::ofstream & debugfile, int numberOfTracks,
                                   const std::string& iTracksLabel, const DetectorGeometry & detectorGeometry,
                                   Random & random):
    _debugLevel(debugLevel),
    _debugfile(debugfile),
    _numberOfTracks(numberOfTracks),
    _tracksLabel(iTracksLabel),
    _detectorGeometry(detectorGeometry),
    _random(random)
{
}

void fc::TrackGenModule::processEvent(fc::Event& event)
{

    std::unique_ptr<GenTrackSet> genTrackSet { new GenTrackSet() };

    for (int iiTrack = 0; iiTrack < _numberOfTracks; ++iiTrack) {

        genTrackSet->insertTrack(std::move(generateTrack()));

    } // end track loop
    event.put(_tracksLabel,std::move(genTrackSet));
}



fc::GenTrack fc::TrackGenModule::generateTrack() const {

    // Generate track data

    // Track pT, phi0 and charge
    double trackPT = 1.0 + _random.exponentialDouble(1.0);
    int trackCharge = (_random.uniformDouble(0.0,1.0) > 0.5) ? 1 : -1;
    double trackPhi0 = _random.uniformDouble(-M_PI/16.0,M_PI/16.0) + M_PI/2.0;
    double trackTanL = _random.uniformDouble(-0.4,0.4);
    double trackD0 = _random.uniformDouble(-0.01,0.01);
    double trackZ0 = _random.uniformDouble(-0.01,0.01);


	// real phi to d0, reversed in d0 is negative

    double phi0ToD0 = trackPhi0-trackCharge*M_PI/2.0;

    TLorentzVector lorentzVector(trackPT*std::cos(trackPhi0),
                                 trackPT*std::sin(trackPhi0),trackTanL*trackPT,
                                 trackPT*std::sqrt(1+trackTanL*trackTanL));


    //    TVector3 position(trackD0*std::cos(phi0ToD0),trackD0*std::sin(phi0ToD0),trackZ0);
    TVector3 position(trackD0*std::cos(phi0ToD0),trackD0*std::sin(phi0ToD0),trackZ0);

    GenTrack genTrack(lorentzVector,trackCharge,position);

    return genTrack;

}




