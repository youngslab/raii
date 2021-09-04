#pragma once

#include <memory>
#include <functional>
#include <any>
#include <vector>

#include <iostream>
#include <fmt/format.h>

namespace raii {

template <typename T> class AutoDeletable {
private:
  class Storage {
  public:
    T value;
    std::vector<std::any> deps;
    Storage(T handle) : value(handle) {}
  };

  std::shared_ptr<Storage> _storage;

public:
  using inner_type_t = T;

  AutoDeletable(T handle, std::function<void(T)> deleter)
      : _storage(new Storage(handle), [deleter](Storage *ptr) {
	  deleter(ptr->value);
	  ptr->deps.clear();
	  delete ptr;
	}) {}

  AutoDeletable() : _storage(nullptr) {}

  explicit AutoDeletable(AutoDeletable const &rhs) : _storage(rhs._storage) {}

  explicit AutoDeletable(AutoDeletable &&rhs)
      : _storage(std::move(rhs._storage)) {}

  AutoDeletable<T> &operator=(const AutoDeletable<T> &rhs) {
    this->_storage = rhs._storage;
    return *this;
  }

  AutoDeletable<T> &operator=(AutoDeletable<T> &&rhs) {
    this->_storage = std::move(rhs._storage);
    return *this;
  }

  ~AutoDeletable() {}

  operator T() & {
    if (_storage == nullptr) {
      throw std::runtime_error(
	  fmt::format("Raii Not Initilized(&) this={}", fmt::ptr(this)));
    }
    return (*_storage).value;
  }

  operator T() const & {
    if (_storage == nullptr) {
      throw std::runtime_error(
	  fmt::format("Raii Not Initilized(const&) this={}", fmt::ptr(this)));
    }
    return (*_storage).value;
  }

  // XXX: To prevent slicing, rvalue cant note be converted implicitly.
  operator T() && = delete;

  // XXX: To prevent slicing, rvalue cant note be converted implicitly.
  operator T() const && = delete;

  auto dependOn(std::any dependency) -> void {
    (*_storage).deps.push_back(dependency);
  }

  const T getVal() const { return (*_storage).value; }

  auto validate() -> bool { return _storage != nullptr; }
};

template <typename T>
static auto MakeAutoDeletable(T handle, std::function<void(T)> deleter)
    -> AutoDeletable<T> {
  return AutoDeletable<T>(handle, deleter);
}
} // namespace raii

