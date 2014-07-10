#ifndef Event_hh
#define Event_hh

#include <memory>
#include <map>
#include <string>

#include "Handle.hh"
#include "ProductID.hh"

namespace fc {

  class Event {
    std::map<ProductID, std::shared_ptr<const void>> _dataProducts;
    unsigned int _eventNumber;

    void throwNotFoundException(ProductID const&) const;
  public:
    explicit Event( unsigned int iNumber): _eventNumber{iNumber} 
    {}

    unsigned int eventNumber() const { return _eventNumber;}

    template<typename T>
    void put(const std::string& iLabel, std::unique_ptr<T> iProduct);

    template<typename T>
    Handle<T> get(const std::string& iLabel) const;
  };

  template<typename T>
  void Event::put(const std::string& iLabel, std::unique_ptr<T> iProduct) {
    std::shared_ptr<const T> holder{ iProduct.release() };
    _dataProducts[ProductID::make<T>(iLabel)] = holder;
  }
  
  template<typename T>
  Handle<T> Event::get(const std::string& iLabel) const {
    auto productID = ProductID::make<T>(iLabel);
    auto found = _dataProducts.find(productID);
    if(found == _dataProducts.end()) {
      throwNotFoundException(productID);
    }
    //We know this is a T so safe to cast
    return Handle<T>(reinterpret_cast<const T*>(found->second.get()));
  }
}
#endif //Event_hh
