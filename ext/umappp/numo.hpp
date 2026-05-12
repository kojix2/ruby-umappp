/*!
 * Numo.hpp v0.3.1
 * https://github.com/ankane/numo.hpp
 * BSD-2-Clause License
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>

#include <rice/rice.hpp>

#include <numo/narray.h>

namespace numo {

class NArray : public Rice::Object {
public:
  NArray(VALUE v) : NArray(dtype(), v) { }

  NArray(Rice::Object o) : NArray(dtype(), o.value()) { }

  size_t ndim() const {
    return RNARRAY_NDIM(value());
  }

  size_t* shape() const {
    return RNARRAY_SHAPE(value());
  }

  size_t shape(size_t n) const {
    if (n >= ndim()) {
      throw std::out_of_range{"index out of range"};
    }
    return shape()[n];
  }

  size_t size() const {
    return RNARRAY_SIZE(value());
  }

  bool is_contiguous() const {
    return Rice::detail::protect(nary_check_contiguous, value()) == Qtrue;
  }

  const void* read_ptr() {
    return Rice::detail::protect([&]() {
      if (!nary_check_contiguous(value())) {
        set_value(nary_dup(value()));
      }
      return nary_get_pointer_for_read(value()) + nary_get_offset(value());
    });
  }

  void* write_ptr() {
    return Rice::detail::protect(nary_get_pointer_for_write, value());
  }

protected:
  NArray(VALUE dtype, VALUE v) : Rice::Object(Rice::detail::protect(rb_funcall, dtype, rb_intern("cast"), 1, v)) { }

  NArray(VALUE dtype, Rice::Object o) : NArray(dtype, o.value()) { }

  // rb_narray_new doesn't modify shape, but not marked as const
  NArray(VALUE dtype, std::initializer_list<size_t> shape) : Rice::Object(Rice::detail::protect(rb_narray_new, dtype, shape.size(), const_cast<size_t*>(shape.begin()))) { }

private:
  static VALUE dtype() {
    return numo_cNArray;
  }
};

class SFloat: public NArray {
public:
  SFloat(VALUE v) : NArray(dtype(), v) { }

  SFloat(Rice::Object o) : NArray(dtype(), o) { }

  SFloat(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const float* read_ptr() {
    return static_cast<const float*>(NArray::read_ptr());
  }

  float* write_ptr() {
    return static_cast<float*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cSFloat;
  }
};

class DFloat: public NArray {
public:
  DFloat(VALUE v) : NArray(dtype(), v) { }

  DFloat(Rice::Object o) : NArray(dtype(), o) { }

  DFloat(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const double* read_ptr() {
    return static_cast<const double*>(NArray::read_ptr());
  }

  double* write_ptr() {
    return static_cast<double*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cDFloat;
  }
};

class Int8: public NArray {
public:
  Int8(VALUE v) : NArray(dtype(), v) { }

  Int8(Rice::Object o) : NArray(dtype(), o) { }

  Int8(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const int8_t* read_ptr() {
    return static_cast<const int8_t*>(NArray::read_ptr());
  }

  int8_t* write_ptr() {
    return static_cast<int8_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cInt8;
  }
};

class Int16: public NArray {
public:
  Int16(VALUE v) : NArray(dtype(), v) { }

  Int16(Rice::Object o) : NArray(dtype(), o) { }

  Int16(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const int16_t* read_ptr() {
    return static_cast<const int16_t*>(NArray::read_ptr());
  }

  int16_t* write_ptr() {
    return static_cast<int16_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cInt16;
  }
};

class Int32: public NArray {
public:
  Int32(VALUE v) : NArray(dtype(), v) { }

  Int32(Rice::Object o) : NArray(dtype(), o) { }

  Int32(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const int32_t* read_ptr() {
    return static_cast<const int32_t*>(NArray::read_ptr());
  }

  int32_t* write_ptr() {
    return static_cast<int32_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cInt32;
  }
};

class Int64: public NArray {
public:
  Int64(VALUE v) : NArray(dtype(), v) { }

  Int64(Rice::Object o) : NArray(dtype(), o) { }

  Int64(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const int64_t* read_ptr() {
    return static_cast<const int64_t*>(NArray::read_ptr());
  }

  int64_t* write_ptr() {
    return static_cast<int64_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cInt64;
  }
};

class UInt8: public NArray {
public:
  UInt8(VALUE v) : NArray(dtype(), v) { }

  UInt8(Rice::Object o) : NArray(dtype(), o) { }

  UInt8(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const uint8_t* read_ptr() {
    return static_cast<const uint8_t*>(NArray::read_ptr());
  }

  uint8_t* write_ptr() {
    return static_cast<uint8_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cUInt8;
  }
};

class UInt16: public NArray {
public:
  UInt16(VALUE v) : NArray(dtype(), v) { }

  UInt16(Rice::Object o) : NArray(dtype(), o) { }

  UInt16(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const uint16_t* read_ptr() {
    return static_cast<const uint16_t*>(NArray::read_ptr());
  }

  uint16_t* write_ptr() {
    return static_cast<uint16_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cUInt16;
  }
};

class UInt32: public NArray {
public:
  UInt32(VALUE v) : NArray(dtype(), v) { }

  UInt32(Rice::Object o) : NArray(dtype(), o) { }

  UInt32(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const uint32_t* read_ptr() {
    return static_cast<const uint32_t*>(NArray::read_ptr());
  }

  uint32_t* write_ptr() {
    return static_cast<uint32_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cUInt32;
  }
};

class UInt64: public NArray {
public:
  UInt64(VALUE v) : NArray(dtype(), v) { }

  UInt64(Rice::Object o) : NArray(dtype(), o) { }

  UInt64(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const uint64_t* read_ptr() {
    return static_cast<const uint64_t*>(NArray::read_ptr());
  }

  uint64_t* write_ptr() {
    return static_cast<uint64_t*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cUInt64;
  }
};

class SComplex: public NArray {
public:
  SComplex(VALUE v) : NArray(dtype(), v) { }

  SComplex(Rice::Object o) : NArray(dtype(), o) { }

  SComplex(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

private:
  static VALUE dtype() {
    return numo_cSComplex;
  }
};

class DComplex: public NArray {
public:
  DComplex(VALUE v) : NArray(dtype(), v) { }

  DComplex(Rice::Object o) : NArray(dtype(), o) { }

  DComplex(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

private:
  static VALUE dtype() {
    return numo_cDComplex;
  }
};

class Bit: public NArray {
public:
  Bit(VALUE v) : NArray(dtype(), v) { }

  Bit(Rice::Object o) : NArray(dtype(), o) { }

  Bit(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

private:
  static VALUE dtype() {
    return numo_cBit;
  }
};

class RObject: public NArray {
public:
  RObject(VALUE v) : NArray(dtype(), v) { }

  RObject(Rice::Object o) : NArray(dtype(), o) { }

  RObject(std::initializer_list<size_t> shape) : NArray(dtype(), shape) { }

  const VALUE* read_ptr() {
    return static_cast<const VALUE*>(NArray::read_ptr());
  }

  VALUE* write_ptr() {
    return static_cast<VALUE*>(NArray::write_ptr());
  }

private:
  static VALUE dtype() {
    return numo_cRObject;
  }
};

} // namespace numo

namespace Rice::detail {

template<>
struct Type<numo::NArray> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::NArray> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::NArray>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::NArray convert(VALUE x) {
    return numo::NArray(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::NArray> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::NArray& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::SFloat> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::SFloat> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::SFloat>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::SFloat convert(VALUE x) {
    return numo::SFloat(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::SFloat> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::SFloat& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::DFloat> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::DFloat> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::DFloat>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::DFloat convert(VALUE x) {
    return numo::DFloat(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::DFloat> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::DFloat& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::Int8> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::Int8> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::Int8>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::Int8 convert(VALUE x) {
    return numo::Int8(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::Int8> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::Int8& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::Int16> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::Int16> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::Int16>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::Int16 convert(VALUE x) {
    return numo::Int16(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::Int16> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::Int16& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::Int32> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::Int32> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::Int32>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::Int32 convert(VALUE x) {
    return numo::Int32(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::Int32> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::Int32& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::Int64> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::Int64> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::Int64>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::Int64 convert(VALUE x) {
    return numo::Int64(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::Int64> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::Int64& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::UInt8> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::UInt8> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::UInt8>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::UInt8 convert(VALUE x) {
    return numo::UInt8(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::UInt8> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::UInt8& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::UInt16> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::UInt16> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::UInt16>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::UInt16 convert(VALUE x) {
    return numo::UInt16(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::UInt16> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::UInt16& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::UInt32> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::UInt32> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::UInt32>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::UInt32 convert(VALUE x) {
    return numo::UInt32(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::UInt32> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::UInt32& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::UInt64> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::UInt64> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::UInt64>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::UInt64 convert(VALUE x) {
    return numo::UInt64(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::UInt64> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::UInt64& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::SComplex> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::SComplex> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::SComplex>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::SComplex convert(VALUE x) {
    return numo::SComplex(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::SComplex> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::SComplex& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::DComplex> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::DComplex> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::DComplex>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::DComplex convert(VALUE x) {
    return numo::DComplex(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::DComplex> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::DComplex& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::Bit> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::Bit> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::Bit>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::Bit convert(VALUE x) {
    return numo::Bit(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::Bit> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::Bit& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

template<>
struct Type<numo::RObject> {
  static bool verify() { return true; }
};

template<>
class From_Ruby<numo::RObject> {
public:
  From_Ruby() = default;

  explicit From_Ruby(Arg* arg) : arg_(arg) { }

  double is_convertible(VALUE value) {
    switch (rb_type(value)) {
      case RUBY_T_DATA:
        return Data_Type<numo::RObject>::is_descendant(value) ? Convertible::Exact : Convertible::None;
      case RUBY_T_ARRAY:
        return Convertible::Exact;
      default:
        return Convertible::None;
    }
  }

  numo::RObject convert(VALUE x) {
    return numo::RObject(x);
  }

private:
  Arg* arg_ = nullptr;
};

template<>
class To_Ruby<numo::RObject> {
public:
  To_Ruby() = default;

  explicit To_Ruby(Arg* arg) : arg_(arg) { }

  VALUE convert(const numo::RObject& x) {
    return x.value();
  }

private:
  Arg* arg_ = nullptr;
};

} // namespace Rice::detail