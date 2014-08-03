#define _USE_MATH_DEFINES
#include <string>

#include "TH1D.h"

#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/Day0HistogrammingModule.hh"
#include "Services/include/UniqueRootDirectory.hh"

fc::Day0HistogrammingModule::Day0HistogrammingModule(const std::string&
        inputGenTracksLabel,
        const std::string& inputHitSetLabel,
        const std::string& inputStripSetLabel):
    _tracksLabel(inputGenTracksLabel),
    _hitSetLabel(inputHitSetLabel),
    _stripSetLabel(inputStripSetLabel),
    _hPt(nullptr)
{
    //NOTE: The framework running the module already has a ROOT file open
    // all ROOT histograms made will be added to that file

    //Use a unique ROOT Directory to avoid possible clashes with other
    // modules making histograms which might use the same names or titles
    UniqueRootDirectory tDir( "Day0HistogrammingModule" );

    _hPt = new TH1D( "Pt", "Pt of Generated Track; GeV; N", 100, 0., 100.);

}

void fc::Day0HistogrammingModule::processEvent(fc::Event& event)
{
    //Get the fc::GenTrackSet from the Event using the label provided in the constructor
    // The label is used in case there are more than on GenTrackSet in the Event.
    fc::Handle<fc::GenTrackSet> genTracks = event.get<fc::GenTrackSet>
                                            (_tracksLabel);

    for(auto const& track : genTracks->getGenTracks()) {
        _hPt->Fill(track.getLorentzVector().Pt());
    }

    /* Use these to get the additional data items for the exercise
    fc::Handle<fc::HitSet> hitSet = event.get<fc::HitSet>(_hitSetLabel);
    fc::Handle<fc::StripSet> stripSet = event.get<fc::StripSet>(_stripSetLabel);
    */

}
