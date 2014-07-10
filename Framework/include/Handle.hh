#ifndef Handle_hh
#define Handle_hh

namespace fc {
  class Event;

  template<typename T>
  class Handle {
    const T* _product;

    friend class Event;
    explicit Handle( const T* iProduct): _product(iProduct) {}

    Handle() = delete;

  public:
    const T* operator->() const {
      return _product;
    }

    const T& operator*() const {
      return *_product;
    }

    const T* get() const {
      return _product;
    }

  };
}

#endif //Handle_hh
