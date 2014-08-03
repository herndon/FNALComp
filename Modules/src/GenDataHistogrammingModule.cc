#define _USE_MATH_DEFINES
#include <string>

#include "TH1D.h"

#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Services/include/UniqueRootDirectory.hh"
#include "DataObjects/include/Helix.hh"
#include "Modules/include/GenDataHistogrammingModule.hh"

fc::GenDataHistogrammingModule::GenDataHistogrammingModule(
    const std::string& inputGenTracksLabel,
    const std::string& inputHitSetLabel,
    const std::string& inputStripSetLabel,
    const DetectorGeometry & detectorGeometry):
    _tracksLabel(inputGenTracksLabel),
    _hitSetLabel(inputHitSetLabel),
    _stripSetLabel(inputStripSetLabel),
    _detectorGeometry(detectorGeometry),
    _hDR(nullptr),_hPhi0(nullptr),_hKappa(nullptr),_hDZ(nullptr),_hTanL(nullptr),
    _hPT(nullptr),_hPZ(nullptr),_hRC(nullptr),
    _hNHits(nullptr),
    _hHitPositionX(nullptr), _hHitPositionY(nullptr),_hHitPositionZ(nullptr),
    _hNStripsPerLayer(nullptr) {

    initializeHistograms();

}


void fc::GenDataHistogrammingModule::initializeHistograms() {

    //NOTE: The framework running the module already has a ROOT file open
    // all ROOT histograms made will be added to that file

    //Use a unique ROOT Directory to avoid possible clashes with other
    // modules making histograms which might use the same names or titles

    UniqueRootDirectory tdir("GenDataHistogramming");

    _hDR    = new TH1D("GenTrackDr", "dr of Generated Track;dr(m);N",100, -0.02,
                       0.02);
    _hPhi0  = new TH1D("GenTrackPhi0", "phi0 of Generated Track;phi0(rad);N",100,
                       0.0, 2.0*M_PI);
    _hKappa = new TH1D("GenTrackKappa", "kappa of Generated Track;kappa (1/GeV);N",
                       100, 0.0, 0.1);
    _hDZ    = new TH1D("GehTrackDz", "dz of Generated Track;dz (m);N",100, -0.02,
                       0.02);
    _hTanL  = new TH1D("GenTrackTanL", "tanL of Generated Track;tanL;N",100, 0.0,
                       1.0);
    _hPT = new TH1D( "GenTrackpT", "pT of Generated Track;pT (GeV);N", 100, 0.,
                     100.);
    _hPZ = new TH1D( "GenTrackpz", "pz of Generated Track;pz (GeV);N", 100, 0.,
                     100.);
    _hRC = new TH1D( "GenTrackRC", "Radium of Curvature (m);N", 100, 0., 100.);

    _hHitPositionX = new TH1D( "GenHitPositionX", "Gen Hit X;X (m);N", 100, -0.5, 0.5);
    _hHitPositionY = new TH1D( "GenHitPositionY", "Gen Hit Y;Y (m);N", 100, 0.0, 1.0);
    _hHitPositionZ = new TH1D( "GenHitPositionZ", "Gen Hit Z;Z (m);N", 100, -0.5, 0.5);


    _hNHits = new TH1D( "GenTracNHits",
                        "Number Hits associated with the Generated Track;Number Hits;N", 11, -0.5,
                        10.5);

    _hNStripsPerLayer = new TH1D( "GenStripPerLayer",
                                  "Ave Number strips on each layer;Number Strips;N", 10, -0.5, 9.5);


}




void fc::GenDataHistogrammingModule::processEvent(fc::Event& event)
{
    //Get the fc::GenTrackSet from the Event using the label provided in the constructor
    // The label is used in case there are more than on GenTrackSet in the Event.
    fc::Handle<fc::GenTrackSet> genTracks = event.get<fc::GenTrackSet>
                                            (_tracksLabel);
    fc::Handle<fc::GenHitSet> genHits = event.get<fc::GenHitSet>(_hitSetLabel);
    fc::Handle<fc::StripSet> strips = event.get<fc::StripSet>(_stripSetLabel);

    int trackNumber = 0;
    for(auto const& track : genTracks->getGenTracks()) {
        Helix helix(track.makeHelix(_detectorGeometry.getBField(),
                                    _detectorGeometry.getCurvatureC()));
        _hDR->Fill(helix.getDr());
        _hPhi0->Fill(helix.getPhi0());
        _hKappa->Fill(helix.getKappa());
        _hDZ->Fill(helix.getDz());
        _hTanL->Fill(helix.getTanL());
        _hPT->Fill(track.getLorentzVector().Pt());
        _hPZ->Fill(track.getLorentzVector().Pz());
        _hRC->Fill(helix.getRadiusOfCurvature(_detectorGeometry.getBField()));

        int numberHits = 0;
        for(auto const& hit : genHits->getGenHits()) {

            if (hit.getTrackNumber() == trackNumber) numberHits++;
	    _hHitPositionX->Fill(hit.getGenHitPosition().X());
	    _hHitPositionY->Fill(hit.getGenHitPosition().Y());
	    _hHitPositionZ->Fill(hit.getGenHitPosition().Z());

        }
        _hNHits->Fill(numberHits);

        trackNumber++;

    }

    int layer =0;
    for (auto const& stripMap : strips->getStrips()) {
        _hNStripsPerLayer->Fill(layer,stripMap.size()*0.1);
        ++layer;
    }




}
