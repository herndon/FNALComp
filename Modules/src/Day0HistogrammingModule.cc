#define _USE_MATH_DEFINES
#include <iostream>
#include <sstream>
#include <string>

#include "TH1D.h"
#include "TH2D.h"
#include "TDirectory.h"

#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "DataObjects/include/Helix.hh"
#include "DataObjects/include/GenHitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Modules/include/Day0HistogrammingModule.hh"
#include "Services/include/UniqueRootDirectory.hh"

fc::Day0HistogrammingModule::Day0HistogrammingModule(const std::string& inputGenTracksLabel,
						     const std::string& inputHitSetLabel,
						     const std::string& inputStripSetLabel,
						     const DetectorGeometry & detector ):
  _tracksLabel(inputGenTracksLabel),
  _hitSetLabel(inputHitSetLabel),
  _stripSetLabel(inputStripSetLabel),
  _detector(detector),
  _hStripsPerLayer(_detector.nSensors(),nullptr),
  _hOccupancyPerLayer(_detector.nSensors(),nullptr),
  _hHitxz(_detector.nSensors(),nullptr)
{
  //NOTE: The framework running the module already has a ROOT file open
  // all ROOT histograms made will be added to that file

  //Use a unique ROOT Directory to avoid possible clashes with other 
  // modules making histograms which might use the same names or titles
  UniqueRootDirectory tDir( "Day0HistogrammingModule" );

  bookGenTrackHistograms( tDir.directory(), _tracksLabel );
  bookStripHistograms   ( tDir.directory(), _stripSetLabel );
  bookHitHistograms     ( tDir.directory(), _hitSetLabel );
  
}

void fc::Day0HistogrammingModule::processEvent(fc::Event& event)
{
  fillGenTrackHistograms(event);
  fillStripHistograms(event);
  fillHitHistograms(event);
}

// Book histograms about GenTracks
void fc::Day0HistogrammingModule::bookGenTrackHistograms(TDirectory* dir,
							 std::string const& label){

  // Save original directory;
  TDirectory* savedDir(dir);

  // These histograms will appear in the new directory.
  TDirectory* newDir=dir->mkdir(TString(label));
  newDir->cd();

  // Make histograms in the new directory.
  _hNTracks = new TH1D( "NTracks", "Number of Tracks per Event;;N",       20,  0.,     20.      );
  _hPt      = new TH1D( "pt",     "Pt of Generated Track; GeV; N",       100,  0.,    100.      );
  _hP       = new TH1D( "p",      "P of Generated Track; GeV; N",        100,  0.,    100.      );
  _hKappa   = new TH1D( "kappa",  "Pt of Generated Track; 1/GeV; N",     100,  0.,      0.1     );
  _hphi0    = new TH1D( "phi0",   "Phi0 of Generated Track; radians; N", 100,  0.,      2.*M_PI );
  _htanl    = new TH1D( "tanl",   "Tan(lambda) of Generated Track; ; N", 100, -2.,      2.      );
  _hdr      = new TH1D( "dr",     "dr of Generated Track; m; N",         100,  0.010,   0.010   );
  _hdz      = new TH1D( "dz",     "dz of Generated Track; m; N",         100,  0.010,   0.010   );

  _hradius = new TH1D( "radius", "Radius of Curvature of Generated Track; m; N", 100,  0.,    200.      );

  // Restore working directory
  savedDir->cd();

}

// Fill histograms about GenTracks
void fc::Day0HistogrammingModule::fillGenTrackHistograms(fc::Event& event)
{
  // Constant used to convert between pt and radius of curvature
  double alpha = _detector.curvatureC();

  // Get the fc::GenTrackSet from the Event using the label provided in the constructor
  fc::Handle<fc::GenTrackSet> genTracks = event.get<fc::GenTrackSet>(_tracksLabel);

  _hNTracks->Fill( genTracks->genTracks().size() );

  for(auto const& track : genTracks->genTracks()) {
    _hPt->Fill(track.lorentzVector().Pt());
    _hP ->Fill(track.lorentzVector().P());

    // Get the helix representation of the track parameters.
    Helix helix = track.makeHelix(_detector.bField(),alpha);
    _hKappa ->Fill( helix.kappa() );
    _hphi0  ->Fill( helix.phi0()  );
    _htanl  ->Fill( helix.tanL()  );
    _hdr    ->Fill( helix.dR()    );
    _hdz    ->Fill( helix.dZ()    );
    _hradius->Fill( std::abs(helix.radiusOfCurvature(_detector.bField())) );
  }

}

// Book histograms about hit strips.
void fc::Day0HistogrammingModule::bookStripHistograms(TDirectory* dir,
						      std::string const& label)
{

  // Save original directory;
  TDirectory* savedDir(dir);

  // Make a new ROOT directory and cd to it.
  TDirectory* newDir=dir->mkdir(TString(label));
  newDir->cd();

  // These histograms will appear in the new directory.
  _hTotalStrips  = new TH1D( "totalStrips",  "Total Hit Strips per Event",   100, 150., 250. );
  _hADCAllStrips = new TH1D( "ADCAllStrips", "ADC Spectrum for all strips",  100,  0., 100. );

  for ( int lay=0; lay<_detector.nSensors(); ++lay ){
    std::ostringstream name, title;
    name << "hNStrips_" << lay;
    title << "Number of hit strips in layer " << lay << ";;N";
    _hStripsPerLayer.at(lay) = new TH1D( TString(name.str()), TString(title.str()), 40, 0., 40. );
  }

  for ( int lay=0; lay<_detector.nSensors(); ++lay ){
    std::ostringstream name, title;
    name << "hOccupancy_" << lay;
    title << "Strip cccupancy map for layer " << lay << ";;N";
    auto const& sensor = _detector.sensor(lay);
    _hOccupancyPerLayer.at(lay) = new TH1D( TString(name.str()), TString(title.str()), 128, 0., sensor._nStrips );
  }


  // Restore working directory
  savedDir->cd();

}

// Fill histograms about hit strips.
void fc::Day0HistogrammingModule::fillStripHistograms(fc::Event& event)
{
  fc::Handle<fc::StripSet> strips = event.get<fc::StripSet>(_stripSetLabel);

  // Total number of hit strips in this event.
  size_t sum(0);

  // Loop over layers
  for ( int lay=0; lay<strips->numberLayers(); ++lay ){
    auto const& layerStrips = strips->layerStrips(lay);
    sum += layerStrips.size();
    _hStripsPerLayer.at(lay)->Fill(layerStrips.size() );

    // Loop over hit strips within a layer.
    for ( auto const& hitStrip : layerStrips ){
      int stripNumber = hitStrip.first;
      int adc         = hitStrip.second;
      _hADCAllStrips->Fill(adc);
      _hOccupancyPerLayer.at(lay)->Fill(stripNumber);
    }
    
  }
  _hTotalStrips->Fill(sum);
}

// Book histograms about Hits
void fc::Day0HistogrammingModule::bookHitHistograms(TDirectory* dir,
						    std::string const& label){

  // Save original directory;
  TDirectory* savedDir(dir);

  // Make a new ROOT directory and cd to it.
  TDirectory* newDir=dir->mkdir(TString(label));
  newDir->cd();
  
  int nLayers = _detector.nSensors();

  // These histograms will appear in the new directory.
  _hNHits       = new TH1D( "NHits",       "Number of Hits per Event;;N",      50, 75.,    125.  );
  _hLayerHit    = new TH1D( "LayerHit",    "Hit Layers;;N",               nLayers,  0, nLayers   );
  _hTrackNumber = new TH1D( "TrackNumber", "Track Number that made Hit;;N",    20,  0.,     20.  );
  _hAllHity     = new TH1D( "AllHity",     "Hit y for all Hits;m;m",          110,  0.,      1.1 );

  // This is a good number for the histogram limits.
  double xzmax=0.2;

  _hAllHitxz = new TH2D( "AllHitxz", "z vs x for all Hits;m;m", 100, -xzmax, xzmax, 100, -xzmax, xzmax);

  for ( int lay=0; lay<_detector.nSensors(); ++lay ){
    std::ostringstream name, title;
    name  << "hHitxz_" << lay;
    title << "Hit Position, z vs x, in layer " << lay << ";m;m";
    _hHitxz.at(lay) = new TH2D( TString(name.str()), TString(title.str()), 100, -xzmax, xzmax, 100, -xzmax, xzmax);
  }

  // Restore working directory
  savedDir->cd();

}

// Fill histograms about Hits
void fc::Day0HistogrammingModule::fillHitHistograms(fc::Event& event )
{
  fc::Handle<fc::GenHitSet> hitSet = event.get<fc::GenHitSet>(_hitSetLabel);

  _hNHits->Fill( hitSet->genHits().size() );

  static int nn(0);
  ++nn;

  for ( auto const& hit : hitSet->genHits() ){
    auto const& pos = hit.position();

    _hLayerHit   ->Fill( hit.layer()        );
    _hTrackNumber->Fill( hit.trackNumber()  );
    _hAllHity    ->Fill( pos.y()               );
    _hAllHitxz   ->Fill( pos.x(), pos.z()      );
    
    _hHitxz.at(hit.layer())->Fill( pos.x(), pos.z() );
  }
}
