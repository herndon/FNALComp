#ifndef Framework_ProductID_hh
#define Framework_ProductID_hh

#include <string>

namespace fc {
  class ProductID {
    std::type_info const& _type;
    std::string _label;

    ProductID(std::type_info const& iType, std::string const& iLabel):
      _type(iType),
      _label(iLabel) {}

    ProductID() = delete;

  public:
    bool operator<( ProductID const &rhs) const {
      if(_type == rhs._type) {
	return _label < rhs._label;
      }
      return _type.before(rhs._type);
    }
    
    const char* typeName() const {
      return _type.name();
    }

    std::string const& label() const {
      return _label;
    }

    template<typename T>
    static ProductID make( std::string const& iLabel) {
      return ProductID(typeid(T), iLabel);
    }

  };
}

#endif //Framework_ProductID_hh
