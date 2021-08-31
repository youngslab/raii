#pragma once

#include <memory>
#include <functional>

namespace raii {

template <typename T> class AutoDeletable {
private:
  std::shared_ptr<T> _storage;

public:
  AutoDeletable() : _storage(nullptr) {}

  AutoDeletable(AutoDeletable const &rhs) : _storage(rhs._storage) {}

  AutoDeletable(AutoDeletable const &&rhs)
      : _storage(std::move(rhs._storage)) {}

  AutoDeletable(T handle, std::function<void(T)> deleter)
      : _storage(std::shared_ptr<T>(new T(handle), [deleter](T *ptr) {
	  deleter(*ptr);
	  delete ptr;
	})) {}

  AutoDeletable<T> &operator=(const AutoDeletable<T> &rhs) {
    this->_storage = rhs._storage;
  }

  AutoDeletable<T> &operator=(const AutoDeletable<T> &&rhs) {
    this->_storage = rhs._storage;
    rhs._storage = nullptr;
  }

  operator T() & { return *_storage; }

  operator T() const & { return *_storage ; }

  // XXX: To prevent slicing, rvalue cant note be converted implicitly.
  operator T() && = delete;

  // XXX: To prevent slicing, rvalue cant note be converted implicitly.
  operator T() const && = delete;

  const T getVal() const { return *_storage ; }
};

template <typename T>
static auto MakeAutoDeletable(T handle, std::function<void(T)> deleter)
    -> AutoDeletable<T> {
  return AutoDeletable<T>(handle, deleter);
}
} // namespace raii

