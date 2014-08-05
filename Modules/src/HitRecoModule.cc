#include<iostream>
#include "Geometry/include/StripHitFunctions.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Hit.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/StripSet.hh"
#include "Modules/include/HitRecoModule.hh"

fc::HitRecoModule::HitRecoModule(int debugLevel,
                                 const std::string& iInputStripsLabel, const std::string& iOutputHitsLabel,
                                 const DetectorGeometry & detectorGeometry):
    _debugLevel(debugLevel),
    _inStripsLabel(iInputStripsLabel),
    _outHitsLabel(iOutputHitsLabel),
    _detectorGeometry(detectorGeometry) {
}

void fc::HitRecoModule::processEvent(fc::Event& event)
{
    Handle<StripSet> genStripSet = event.get<StripSet>(_inStripsLabel);
    std::unique_ptr<HitSet> recoHitSet( new HitSet );

    recoHits(*genStripSet,*recoHitSet);

    if (_debugLevel >= 2) recoHitSet->print(std::cout);

    event.put(_outHitsLabel, std::move(recoHitSet));
}

void fc::HitRecoModule::recoHits(const StripSet& stripSet,
                                 HitSet & hitSet) const {

    for (int iiLayer =  0; iiLayer < _detectorGeometry.getNSensors(); ++iiLayer) {

        if (_debugLevel >= 5) std::cout << "HitReco layer: " << iiLayer << std::endl;
        recoHitsLayer(stripSet,iiLayer,hitSet);

    } // end layer loop


}


void fc::HitRecoModule::recoHitsLayer(const StripSet & stripSet,int layer,
                                      HitSet& hitSet) const {


    std::vector<int> stripAdcs;

    const LayerStripMap& layerStrips = stripSet.getLayerStripMap(layer);
    unsigned int initialStrip = layerStrips.begin()->first;


    for (auto const & strip : layerStrips){

 
  
      if (strip.second > _detectorGeometry.getSensor(layer)._threshold && strip.first == (initialStrip+stripAdcs.size())){
	//Found an above threshold and an ajacent strip
	stripAdcs.push_back(strip.second);
      } else if (stripAdcs.size() > 0){
	// Was not adjacent and above threshold and a cluster is in stripAdc
	hitSet.insertHit(std::move(buildHit(layer, initialStrip,stripAdcs)));
	stripAdcs.clear();
	if (strip.second > _detectorGeometry.getSensor(layer)._threshold) {
	  // Was above threhold but not adjacent start new cluster
	  initialStrip = strip.first;
	  stripAdcs.push_back(strip.second);
	}
      } else if (strip.second > _detectorGeometry.getSensor(layer)._threshold){
	// Was above threhold but not adjacent start new cluster, no previous cluster to store
	initialStrip = strip.first;
	stripAdcs.push_back(strip.second);
      } else {

      }

    }

    // Was there a cluster in the stripAdc buffer
    if (stripAdcs.size()>0)	hitSet.insertHit(std::move(buildHit(layer, initialStrip,stripAdcs)));





}



fc::Hit fc::HitRecoModule::buildHit(int layer, int initialStrip,
                                    const std::vector<int> & stripAdcs) const {

    double stripHitPosition = fcf::calculateStripHitPositionFromCluster(
                                  initialStrip,stripAdcs);
    double localHitPosition = fcf::calculateLocalFromStripPosition(stripHitPosition,
                              layer,_detectorGeometry);
    TVector3 hitPosition = fcf::calculateGlobalFromLocalPosition(localHitPosition,
                           layer,_detectorGeometry);

    int numberStrips = stripAdcs.size();
    int charge = 0;
    for (auto const& adc :   stripAdcs) {
        charge += adc;
    }

    bool goodHit = true;
    if (numberStrips>2 || charge > _detectorGeometry.getMIP()) goodHit = false;

    double resolution;
    if (goodHit) {
        resolution = _detectorGeometry.getSensor(layer)._hitResolution;
    } else {
        resolution = _detectorGeometry.getSensor(layer)._badHitResolution;
    }


    return Hit(hitPosition,layer,numberStrips,charge,goodHit,resolution);

}



