/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <fmt/format.h>
#include <folly/Format.h>
#include <folly/Range.h>
#include <folly/String.h>
#include <folly/json.h>
#include <time.h>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <vector>
#include "folly/CPortability.h"
#include "velox/common/base/ClassName.h"
#include "velox/common/serialization/Serializable.h"
#include "velox/type/Date.h"
#include "velox/type/StringView.h"
#include "velox/type/Timestamp.h"
#include "velox/type/Tree.h"

namespace facebook::velox {

// Velox type system supports a small set of SQL-compatible composeable types:
// BOOLEAN, TINYINT, SMALLINT, INTEGER, BIGINT, REAL, DOUBLE, VARCHAR,
// VARBINARY, TIMESTAMP, DATE, ARRAY, MAP, ROW
//
// This file has multiple C++ type definitions for each of these logical types.
// These logical definitions each serve slightly different purposes.
// These type sets are:
// - TypeKind
// - Type (RowType, BigIntType, ect.)
// - Templated Types (Row<T...>, Map<K, V>, ...)
//     C++ templated classes. Never instantiated, used to pass limited type
//     information into template parameters.

// Simple enum with type category.
enum class TypeKind : int8_t {
  BOOLEAN = 0,
  TINYINT = 1,
  SMALLINT = 2,
  INTEGER = 3,
  BIGINT = 4,
  REAL = 5,
  DOUBLE = 6,
  VARCHAR = 7,
  VARBINARY = 8,
  TIMESTAMP = 9,
  DATE = 10,

  // Enum values for ComplexTypes start after 30 to leave
  // some values space to accommodate adding new scalar/native
  // types above.
  ARRAY = 30,
  MAP = 31,
  ROW = 32,
  UNKNOWN = 33,
  FUNCTION = 34,
  OPAQUE = 35,
  INVALID = 36
};

TypeKind mapNameToTypeKind(const std::string& name);
std::string mapTypeKindToName(const TypeKind& typeKind);

std::ostream& operator<<(std::ostream& os, const TypeKind& kind);

template <TypeKind KIND>
class ScalarType;

class ArrayType;
class MapType;
class RowType;
class FunctionType;
class OpaqueType;
class UnknownType;

struct UnknownValue {
  bool operator==(const UnknownValue& /* b */) const {
    return true;
  }

  bool operator!=(const UnknownValue& /* b */) const {
    return false;
  }

  bool operator<(const UnknownValue& /* b */) const {
    return false;
  }

  bool operator<=(const UnknownValue& /* b */) const {
    return true;
  }

  bool operator>(const UnknownValue& /* b */) const {
    return false;
  }

  bool operator>=(const UnknownValue& /* b */) const {
    return true;
  }
};

template <typename T>
void toAppend(
    const ::facebook::velox::UnknownValue& /* value */,
    T* /* result */) {
  // TODO Implement
}

template <TypeKind KIND>
struct TypeTraits {};

template <>
struct TypeTraits<TypeKind::BOOLEAN> {
  using ImplType = ScalarType<TypeKind::BOOLEAN>;
  using NativeType = bool;
  using DeepCopiedType = NativeType;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::BOOLEAN;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "BOOLEAN";
};

template <>
struct TypeTraits<TypeKind::TINYINT> {
  using ImplType = ScalarType<TypeKind::TINYINT>;
  using NativeType = int8_t;
  using DeepCopiedType = NativeType;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::TINYINT;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "TINYINT";
};

template <>
struct TypeTraits<TypeKind::SMALLINT> {
  using ImplType = ScalarType<TypeKind::SMALLINT>;
  using NativeType = int16_t;
  using DeepCopiedType = NativeType;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::SMALLINT;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "SMALLINT";
};

template <>
struct TypeTraits<TypeKind::INTEGER> {
  using ImplType = ScalarType<TypeKind::INTEGER>;
  using NativeType = int32_t;
  using DeepCopiedType = NativeType;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::INTEGER;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "INTEGER";
};

template <>
struct TypeTraits<TypeKind::BIGINT> {
  using ImplType = ScalarType<TypeKind::BIGINT>;
  using NativeType = int64_t;
  using DeepCopiedType = NativeType;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::BIGINT;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "BIGINT";
};

template <>
struct TypeTraits<TypeKind::REAL> {
  using ImplType = ScalarType<TypeKind::REAL>;
  using NativeType = float;
  using DeepCopiedType = NativeType;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::REAL;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "REAL";
};

template <>
struct TypeTraits<TypeKind::DOUBLE> {
  using ImplType = ScalarType<TypeKind::DOUBLE>;
  using NativeType = double;
  using DeepCopiedType = NativeType;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::DOUBLE;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "DOUBLE";
};

template <>
struct TypeTraits<TypeKind::VARCHAR> {
  using ImplType = ScalarType<TypeKind::VARCHAR>;
  using NativeType = velox::StringView;
  using DeepCopiedType = std::string;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::VARCHAR;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "VARCHAR";
};

template <>
struct TypeTraits<TypeKind::TIMESTAMP> {
  using ImplType = ScalarType<TypeKind::TIMESTAMP>;
  using NativeType = Timestamp;
  using DeepCopiedType = Timestamp;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::TIMESTAMP;
  // isPrimitiveType in the type traits indicate whether it is a leaf type.
  // So only types which have other sub types, should be set to false.
  // Timestamp does not contain other types, so it is set to true.
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "TIMESTAMP";
};

// Date is internally an int32_t that represents the days since the epoch
template <>
struct TypeTraits<TypeKind::DATE> {
  using ImplType = ScalarType<TypeKind::DATE>;
  using NativeType = Date;
  using DeepCopiedType = Date;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::DATE;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "DATE";
};

template <>
struct TypeTraits<TypeKind::VARBINARY> {
  using ImplType = ScalarType<TypeKind::VARBINARY>;
  using NativeType = velox::StringView;
  using DeepCopiedType = std::string;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::VARBINARY;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "VARBINARY";
};

template <>
struct TypeTraits<TypeKind::ARRAY> {
  using ImplType = ArrayType;
  using NativeType = void;
  using DeepCopiedType = void;
  static constexpr uint32_t minSubTypes = 1;
  static constexpr uint32_t maxSubTypes = 1;
  static constexpr TypeKind typeKind = TypeKind::ARRAY;
  static constexpr bool isPrimitiveType = false;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "ARRAY";
};

template <>
struct TypeTraits<TypeKind::MAP> {
  using ImplType = MapType;
  using NativeType = void;
  using DeepCopiedType = void;
  static constexpr uint32_t minSubTypes = 2;
  static constexpr uint32_t maxSubTypes = 2;
  static constexpr TypeKind typeKind = TypeKind::MAP;
  static constexpr bool isPrimitiveType = false;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "MAP";
};

template <>
struct TypeTraits<TypeKind::ROW> {
  using ImplType = RowType;
  using NativeType = void;
  using DeepCopiedType = void;
  static constexpr uint32_t minSubTypes = 1;
  static constexpr uint32_t maxSubTypes = std::numeric_limits<char16_t>::max();
  static constexpr TypeKind typeKind = TypeKind::ROW;
  static constexpr bool isPrimitiveType = false;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "ROW";
};

template <>
struct TypeTraits<TypeKind::UNKNOWN> {
  using ImplType = UnknownType;
  using NativeType = UnknownValue;
  using DeepCopiedType = UnknownValue;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::UNKNOWN;
  static constexpr bool isPrimitiveType = true;
  static constexpr bool isFixedWidth = true;
  static constexpr const char* name = "UNKNOWN";
};

template <>
struct TypeTraits<TypeKind::INVALID> {
  using ImplType = void;
  using NativeType = void;
  using DeepCopiedType = void;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::INVALID;
  static constexpr bool isPrimitiveType = false;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "INVALID";
};

template <>
struct TypeTraits<TypeKind::FUNCTION> {
  using ImplType = FunctionType;
  using NativeType = void;
  using DeepCopiedType = void;
  static constexpr uint32_t minSubTypes = 1;
  static constexpr uint32_t maxSubTypes = std::numeric_limits<char16_t>::max();
  static constexpr TypeKind typeKind = TypeKind::FUNCTION;
  static constexpr bool isPrimitiveType = false;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "FUNCTION";
};

template <>
struct TypeTraits<TypeKind::OPAQUE> {
  using ImplType = OpaqueType;
  using NativeType = std::shared_ptr<void>;
  using DeepCopiedType = std::shared_ptr<void>;
  static constexpr uint32_t minSubTypes = 0;
  static constexpr uint32_t maxSubTypes = 0;
  static constexpr TypeKind typeKind = TypeKind::OPAQUE;
  static constexpr bool isPrimitiveType = false;
  static constexpr bool isFixedWidth = false;
  static constexpr const char* name = "OPAQUE";
};

template <TypeKind KIND>
struct TypeFactory;

#define VELOX_FLUENT_CAST(NAME, KIND)                                     \
  const typename TypeTraits<TypeKind::KIND>::ImplType& as##NAME() const { \
    return this->as<TypeKind::KIND>();                                    \
  }                                                                       \
  bool is##NAME() const {                                                 \
    return this->kind() == TypeKind::KIND;                                \
  }

// Abstract class hierarchy. Instances of these classes carry full
// information about types, including for example field names.
// Can be instantiated by factory methods, like INTEGER()
// or MAP(INTEGER(), BIGINT()).
// Instances of these classes form a tree, and are immutable.
// For example, MAP<INTEGER, ARRAY<BIGINT>> will form a tree like:
//
//             MapType
//           /         \
//   IntegerType    ArrayType
//                     |
//                   BigintType
class Type : public Tree<const std::shared_ptr<const Type>>,
             public velox::ISerializable {
 public:
  explicit Type(TypeKind kind) : kind_{kind} {}

  TypeKind kind() const {
    return kind_;
  }

  virtual ~Type() = default;

  // this convenience method makes pattern matching easier. Rather than having
  // to know the implementation type up front, just use as<TypeKind::MAP> (for
  // example) to dynamically cast to the appropriate type.
  template <TypeKind KIND>
  const typename TypeTraits<KIND>::ImplType& as() const {
    return dynamic_cast<const typename TypeTraits<KIND>::ImplType&>(*this);
  }

  virtual bool isPrimitiveType() const = 0;

  virtual const char* kindName() const = 0;

  virtual std::string toString() const = 0;

  virtual bool operator==(const Type& other) const = 0;

  inline bool operator!=(const Type& other) const {
    return !(*this == other);
  }

  // todo(youknowjack): avoid expensive virtual function calls for these
  // simple functions
  virtual size_t cppSizeInBytes() const {
    throw std::invalid_argument{"Not a fixed width type: " + toString()};
  }

  virtual bool isFixedWidth() const = 0;

  // Used in FixedSizeArrayType to return the width constraint of the type.
  virtual size_type fixedElementsWidth() const {
    throw std::invalid_argument{"unimplemented"};
  }

  static std::shared_ptr<const Type> create(const folly::dynamic& obj);

  // recursive kind hashing (ignores names)
  size_t hashKind() const;

  // recursive kind match (ignores names)
  bool kindEquals(const std::shared_ptr<const Type>& other) const;

  template <TypeKind KIND, typename... CHILDREN>
  static std::shared_ptr<const typename TypeTraits<KIND>::ImplType> create(
      CHILDREN... children) {
    return TypeFactory<KIND>::create(std::forward(children)...);
  }

  VELOX_FLUENT_CAST(Boolean, BOOLEAN)
  VELOX_FLUENT_CAST(Tinyint, TINYINT)
  VELOX_FLUENT_CAST(Smallint, SMALLINT)
  VELOX_FLUENT_CAST(Integer, INTEGER)
  VELOX_FLUENT_CAST(Bigint, BIGINT)
  VELOX_FLUENT_CAST(Real, REAL)
  VELOX_FLUENT_CAST(Double, DOUBLE)
  VELOX_FLUENT_CAST(Varchar, VARCHAR)
  VELOX_FLUENT_CAST(Varbinary, VARBINARY)
  VELOX_FLUENT_CAST(Timestamp, TIMESTAMP)
  VELOX_FLUENT_CAST(Date, DATE)
  VELOX_FLUENT_CAST(Array, ARRAY)
  VELOX_FLUENT_CAST(Map, MAP)
  VELOX_FLUENT_CAST(Row, ROW)
  VELOX_FLUENT_CAST(Opaque, OPAQUE)

  bool containsUnknown() const;

 private:
  const TypeKind kind_;

  VELOX_DEFINE_CLASS_NAME(Type)
};

#undef VELOX_FLUENT_CAST

using TypePtr = std::shared_ptr<const Type>;

template <TypeKind KIND>
class TypeBase : public Type {
 public:
  using NativeType = TypeTraits<KIND>;

  TypeBase() : Type{KIND} {}

  bool isPrimitiveType() const override {
    return TypeTraits<KIND>::isPrimitiveType;
  }

  bool isFixedWidth() const override {
    return TypeTraits<KIND>::isFixedWidth;
  }

  const char* kindName() const override {
    return TypeTraits<KIND>::name;
  }
};

template <TypeKind KIND>
class ScalarType : public TypeBase<KIND> {
 public:
  uint32_t size() const override {
    return 0;
  }

  const std::shared_ptr<const Type>& childAt(uint32_t) const override {
    throw std::invalid_argument{"scalar type has no children"};
  }

  std::string toString() const override {
    return TypeTraits<KIND>::name;
  }

  size_t cppSizeInBytes() const override {
    if (TypeTraits<KIND>::isFixedWidth) {
      return sizeof(typename TypeTraits<KIND>::NativeType);
    }
    // TODO: velox throws here for non fixed width types.
    return Type::cppSizeInBytes();
  }

  FOLLY_NOINLINE static const std::shared_ptr<const ScalarType<KIND>> create();

  bool operator==(const Type& other) const override {
    return KIND == other.kind();
  }

  // TODO: velox implementation is in cpp
  folly::dynamic serialize() const override {
    folly::dynamic obj = folly::dynamic::object;
    obj["name"] = "Type";
    obj["type"] = TypeTraits<KIND>::name;
    return obj;
  }
};

template <TypeKind KIND>
const std::shared_ptr<const ScalarType<KIND>> ScalarType<KIND>::create() {
  static const auto instance = std::make_shared<const ScalarType<KIND>>();
  return instance;
}

class UnknownType : public TypeBase<TypeKind::UNKNOWN> {
 public:
  UnknownType() = default;

  uint32_t size() const override {
    return 0;
  }

  const std::shared_ptr<const Type>& childAt(uint32_t) const override {
    throw std::invalid_argument{"UnknownType type has no children"};
  }

  std::string toString() const override {
    return TypeTraits<TypeKind::UNKNOWN>::name;
  }

  size_t cppSizeInBytes() const override {
    return 0;
  }

  bool operator==(const Type& other) const override {
    return TypeKind::UNKNOWN == other.kind();
  }

  folly::dynamic serialize() const override {
    folly::dynamic obj = folly::dynamic::object;
    obj["name"] = "Type";
    obj["type"] = TypeTraits<TypeKind::UNKNOWN>::name;
    return obj;
  }
};

class ArrayType : public TypeBase<TypeKind::ARRAY> {
 public:
  explicit ArrayType(std::shared_ptr<const Type> child);

  const std::shared_ptr<const Type>& elementType() const {
    return child_;
  }

  uint32_t size() const override {
    return 1;
  }

  const std::shared_ptr<const Type>& childAt(uint32_t idx) const override;

  std::string toString() const override;

  bool operator==(const Type& other) const override;

  folly::dynamic serialize() const override;

 protected:
  std::shared_ptr<const Type> child_;
};

// FixedSizeArrayType implements an Array that is constrained to
// always be a fixed size (width). When passing this type on the wire,
// a FixedSizeArrayType may change into a general variable width array
// as Presto/Spark do not have a notion of fixed size array.
//
// Anywhere an ArrayType can be used, a FixedSizeArrayType can be
// used.
class FixedSizeArrayType : public ArrayType {
 public:
  explicit FixedSizeArrayType(size_type len, std::shared_ptr<const Type> child);

  bool isFixedWidth() const override {
    return true;
  }

  size_type fixedElementsWidth() const override {
    return len_;
  }

  const char* kindName() const override {
    return "FIXED_SIZE_ARRAY";
  }

  std::string toString() const override;

 private:
  size_type len_;
};

class MapType : public TypeBase<TypeKind::MAP> {
 public:
  MapType(
      std::shared_ptr<const Type> keyType,
      std::shared_ptr<const Type> valueType);

  const std::shared_ptr<const Type>& keyType() const {
    return keyType_;
  }

  const std::shared_ptr<const Type>& valueType() const {
    return valueType_;
  }

  uint32_t size() const override {
    return 2;
  }

  std::string toString() const override;

  const std::shared_ptr<const Type>& childAt(uint32_t idx) const override;

  bool operator==(const Type& other) const override;

  folly::dynamic serialize() const override;

 private:
  std::shared_ptr<const Type> keyType_;
  std::shared_ptr<const Type> valueType_;
};

class RowType : public TypeBase<TypeKind::ROW> {
 public:
  RowType(
      std::vector<std::string>&& names,
      std::vector<std::shared_ptr<const Type>>&& types);

  uint32_t size() const override;

  const std::shared_ptr<const Type>& childAt(uint32_t idx) const override;

  const std::vector<std::shared_ptr<const Type>>& children() const {
    return children_;
  }

  const std::shared_ptr<const Type>& findChild(folly::StringPiece name) const;

  // Note: Internally does a linear search on all child names, cost is O(N).
  bool containsChild(std::string_view name) const;

  uint32_t getChildIdx(const std::string& name) const;

  std::optional<uint32_t> getChildIdxIfExists(const std::string& name) const;

  const std::string& nameOf(uint32_t idx) const {
    return names_.at(idx);
  }

  bool operator==(const Type& other) const override;

  std::string toString() const override;

  std::shared_ptr<RowType> unionWith(std::shared_ptr<RowType>& rowType) const;

  folly::dynamic serialize() const override;

  const std::vector<std::string>& names() const {
    return names_;
  }

 private:
  const std::vector<std::string> names_;
  const std::vector<std::shared_ptr<const Type>> children_;
};

using RowTypePtr = std::shared_ptr<const RowType>;

// Represents a lambda function. The children are the argument types
// followed by the return value type.
class FunctionType : public TypeBase<TypeKind::FUNCTION> {
 public:
  FunctionType(
      std::vector<std::shared_ptr<const Type>>&& argumentTypes,
      std::shared_ptr<const Type> returnType)
      : children_(allChildren(std::move(argumentTypes), returnType)) {}

  uint32_t size() const override {
    return children_.size();
  }

  const std::shared_ptr<const Type>& childAt(uint32_t idx) const override {
    VELOX_CHECK_LT(idx, children_.size());
    return children_[idx];
  }

  const std::vector<std::shared_ptr<const Type>>& children() const {
    return children_;
  }

  bool operator==(const Type& other) const override;

  std::string toString() const override;

  folly::dynamic serialize() const override;

 private:
  static std::vector<std::shared_ptr<const Type>> allChildren(
      std::vector<std::shared_ptr<const Type>>&& argumentTypes,
      std::shared_ptr<const Type> returnType) {
    auto children = std::move(argumentTypes);
    children.push_back(returnType);
    return children;
  }
  // Argument types from left to right followed by return value type.
  const std::vector<std::shared_ptr<const Type>> children_;
};

class OpaqueType : public TypeBase<TypeKind::OPAQUE> {
 public:
  template <typename T>
  using SerializeFunc = std::function<std::string(const std::shared_ptr<T>&)>;
  template <typename T>
  using DeserializeFunc = std::function<std::shared_ptr<T>(const std::string&)>;

  explicit OpaqueType(const std::type_index& typeIndex);

  uint32_t size() const override {
    return 0;
  }

  const std::shared_ptr<const Type>& childAt(uint32_t) const override {
    throw std::invalid_argument{"OpaqueType type has no children"};
  }

  std::string toString() const override;

  bool operator==(const Type& other) const override;

  const std::type_index& typeIndex() const {
    return typeIndex_;
  }

  folly::dynamic serialize() const override;
  // In special cases specific OpaqueTypes might want to serialize additional
  // metadata. In those cases we need to deserialize it back. Since
  // OpaqueType::create<T>() returns canonical type for T without metadata, we
  // allow to create new instance here or return nullptr if the same one can be
  // used. Note that it's about deserialization of type itself, DeserializeFunc
  // above is about deserializing instances of the type. It's implemented as a
  // virtual member instead of a standalone registry just for convenience.
  virtual std::shared_ptr<const OpaqueType> deserializeExtra(
      const folly::dynamic& json) const;

  // Function for converting std::shared_ptr<T> into a string. Always returns
  // non-nullptr function or throws if not function has been registered.
  SerializeFunc<void> getSerializeFunc() const;
  DeserializeFunc<void> getDeserializeFunc() const;

  template <typename Class>
  FOLLY_NOINLINE static std::shared_ptr<const OpaqueType> create() {
    // static vars in templates are dangerous across DSOs, but it's just a
    // performance optimization. Comparison looks at type_index anyway.
    static const auto instance =
        std::make_shared<const OpaqueType>(std::type_index(typeid(Class)));
    return instance;
  }

  // This function currently doesn't do synchronization neither with reads
  // or writes, so it's caller's responsibility to not invoke it concurrently
  // with other Velox code. Usually it'd be invoked at static initialization
  // time. It can be changed in the future if it becomes a problem.
  template <typename T>
  FOLLY_NOINLINE static void registerSerialization(
      const std::string& persistentName,
      SerializeFunc<T> serialize = nullptr,
      DeserializeFunc<T> deserialize = nullptr) {
    SerializeFunc<void> serializeTypeErased;
    if (serialize) {
      serializeTypeErased =
          [serialize](const std::shared_ptr<void>& x) -> std::string {
        return serialize(std::static_pointer_cast<T>(x));
      };
    }
    DeserializeFunc<void> deserializeTypeErased;
    if (deserialize) {
      deserializeTypeErased =
          [deserialize](const std::string& s) -> std::shared_ptr<void> {
        return std::static_pointer_cast<void>(deserialize(s));
      };
    }
    registerSerializationTypeErased(
        OpaqueType::create<T>(),
        persistentName,
        serializeTypeErased,
        deserializeTypeErased);
  }

 private:
  const std::type_index typeIndex_;

  static void registerSerializationTypeErased(
      const std::shared_ptr<const OpaqueType>& type,
      const std::string& persistentName,
      SerializeFunc<void> serialize = nullptr,
      DeserializeFunc<void> deserialize = nullptr);
};

using IntegerType = ScalarType<TypeKind::INTEGER>;
using BooleanType = ScalarType<TypeKind::BOOLEAN>;
using TinyintType = ScalarType<TypeKind::TINYINT>;
using SmallintType = ScalarType<TypeKind::SMALLINT>;
using BigintType = ScalarType<TypeKind::BIGINT>;
using RealType = ScalarType<TypeKind::REAL>;
using DoubleType = ScalarType<TypeKind::DOUBLE>;
using TimestampType = ScalarType<TypeKind::TIMESTAMP>;
using VarcharType = ScalarType<TypeKind::VARCHAR>;
using VarbinaryType = ScalarType<TypeKind::VARBINARY>;
using DateType = ScalarType<TypeKind::DATE>;

// Used as T for SimpleVector subclasses that wrap another vector when
// the wrapped vector is of a complex type. Applies to
// DictionaryVector, SequenceVector and ConstantVector. This must have
// a size different from any of the scalar data type sizes to enable
// run time checking with 'elementSize_'.
struct ComplexType {
  TypePtr create() {
    VELOX_NYI();
  }

  operator folly::dynamic() const {
    return folly::dynamic("ComplexType");
  }
};

template <TypeKind KIND>
struct TypeFactory {
  // default factory
  static std::shared_ptr<const typename TypeTraits<KIND>::ImplType> create() {
    return TypeTraits<KIND>::ImplType::create();
  }
};

template <>
struct TypeFactory<TypeKind::UNKNOWN> {
  static std::shared_ptr<const UnknownType> create() {
    return std::make_shared<UnknownType>();
  }
};

template <>
struct TypeFactory<TypeKind::ARRAY> {
  static std::shared_ptr<const ArrayType> create(
      std::shared_ptr<const Type> elementType) {
    return std::make_shared<ArrayType>(std::move(elementType));
  }
};

template <>
struct TypeFactory<TypeKind::MAP> {
  static std::shared_ptr<const MapType> create(
      std::shared_ptr<const Type> keyType,
      std::shared_ptr<const Type> valType) {
    return std::make_shared<MapType>(keyType, valType);
  }
};

template <>
struct TypeFactory<TypeKind::ROW> {
  static std::shared_ptr<const RowType> create(
      std::vector<std::string>&& names,
      std::vector<std::shared_ptr<const Type>>&& types) {
    return std::make_shared<const RowType>(std::move(names), std::move(types));
  }
};

std::shared_ptr<const ArrayType> ARRAY(std::shared_ptr<const Type> elementType);
std::shared_ptr<const FixedSizeArrayType> FIXED_SIZE_ARRAY(
    FixedSizeArrayType::size_type size,
    std::shared_ptr<const Type> elementType);

std::shared_ptr<const RowType> ROW(
    std::vector<std::string>&& names,
    std::vector<std::shared_ptr<const Type>>&& types);

std::shared_ptr<const RowType> ROW(
    std::initializer_list<
        std::pair<const std::string, std::shared_ptr<const Type>>>&& pairs);

std::shared_ptr<const RowType> ROW(
    std::vector<std::shared_ptr<const Type>>&& pairs);

std::shared_ptr<const MapType> MAP(
    std::shared_ptr<const Type> keyType,
    std::shared_ptr<const Type> valType);

std::shared_ptr<const TimestampType> TIMESTAMP();

std::shared_ptr<const DateType> DATE();

template <typename Class>
std::shared_ptr<const OpaqueType> OPAQUE() {
  return OpaqueType::create<Class>();
}

#define VELOX_DYNAMIC_SCALAR_TYPE_DISPATCH(TEMPLATE_FUNC, typeKind, ...)      \
  [&]() {                                                                     \
    switch (typeKind) {                                                       \
      case ::facebook::velox::TypeKind::BOOLEAN: {                            \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::BOOLEAN>(           \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::INTEGER: {                            \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::INTEGER>(           \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::TINYINT: {                            \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::TINYINT>(           \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::SMALLINT: {                           \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::SMALLINT>(          \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::BIGINT: {                             \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::BIGINT>(            \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::REAL: {                               \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::REAL>(__VA_ARGS__); \
      }                                                                       \
      case ::facebook::velox::TypeKind::DOUBLE: {                             \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::DOUBLE>(            \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::VARCHAR: {                            \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::VARCHAR>(           \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::VARBINARY: {                          \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::VARBINARY>(         \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::TIMESTAMP: {                          \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::TIMESTAMP>(         \
            __VA_ARGS__);                                                     \
      }                                                                       \
      case ::facebook::velox::TypeKind::DATE: {                               \
        return TEMPLATE_FUNC<::facebook::velox::TypeKind::DATE>(__VA_ARGS__); \
      }                                                                       \
      default:                                                                \
        VELOX_FAIL(                                                           \
            "not a scalar type! kind: {}", mapTypeKindToName(typeKind));      \
    }                                                                         \
  }()

#define VELOX_DYNAMIC_SCALAR_TEMPLATE_TYPE_DISPATCH(                     \
    TEMPLATE_FUNC, T, typeKind, ...)                                     \
  [&]() {                                                                \
    switch (typeKind) {                                                  \
      case ::facebook::velox::TypeKind::BOOLEAN: {                       \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::BOOLEAN>(   \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::INTEGER: {                       \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::INTEGER>(   \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::TINYINT: {                       \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::TINYINT>(   \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::SMALLINT: {                      \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::SMALLINT>(  \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::BIGINT: {                        \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::BIGINT>(    \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::REAL: {                          \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::REAL>(      \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::DOUBLE: {                        \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::DOUBLE>(    \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::VARCHAR: {                       \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::VARCHAR>(   \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::VARBINARY: {                     \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::VARBINARY>( \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::TIMESTAMP: {                     \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::TIMESTAMP>( \
            __VA_ARGS__);                                                \
      }                                                                  \
      case ::facebook::velox::TypeKind::DATE: {                          \
        return TEMPLATE_FUNC<T, ::facebook::velox::TypeKind::DATE>(      \
            __VA_ARGS__);                                                \
      }                                                                  \
      default:                                                           \
        VELOX_FAIL(                                                      \
            "not a scalar type! kind: {}", mapTypeKindToName(typeKind)); \
    }                                                                    \
  }()

#define VELOX_DYNAMIC_SCALAR_TYPE_DISPATCH_ALL(TEMPLATE_FUNC, typeKind, ...)   \
  [&]() {                                                                      \
    if ((typeKind) == ::facebook::velox::TypeKind::UNKNOWN) {                  \
      return TEMPLATE_FUNC<::facebook::velox::TypeKind::UNKNOWN>(__VA_ARGS__); \
    } else if ((typeKind) == ::facebook::velox::TypeKind::OPAQUE) {            \
      return TEMPLATE_FUNC<::facebook::velox::TypeKind::OPAQUE>(__VA_ARGS__);  \
    } else {                                                                   \
      return VELOX_DYNAMIC_SCALAR_TYPE_DISPATCH(                               \
          TEMPLATE_FUNC, typeKind, __VA_ARGS__);                               \
    }                                                                          \
  }()

#define VELOX_DYNAMIC_TYPE_DISPATCH_IMPL(PREFIX, SUFFIX, typeKind, ...)        \
  [&]() {                                                                      \
    switch (typeKind) {                                                        \
      case ::facebook::velox::TypeKind::BOOLEAN: {                             \
        return PREFIX<::facebook::velox::TypeKind::BOOLEAN> SUFFIX(            \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::INTEGER: {                             \
        return PREFIX<::facebook::velox::TypeKind::INTEGER> SUFFIX(            \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::TINYINT: {                             \
        return PREFIX<::facebook::velox::TypeKind::TINYINT> SUFFIX(            \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::SMALLINT: {                            \
        return PREFIX<::facebook::velox::TypeKind::SMALLINT> SUFFIX(           \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::BIGINT: {                              \
        return PREFIX<::facebook::velox::TypeKind::BIGINT> SUFFIX(             \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::REAL: {                                \
        return PREFIX<::facebook::velox::TypeKind::REAL> SUFFIX(__VA_ARGS__);  \
      }                                                                        \
      case ::facebook::velox::TypeKind::DOUBLE: {                              \
        return PREFIX<::facebook::velox::TypeKind::DOUBLE> SUFFIX(             \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::VARCHAR: {                             \
        return PREFIX<::facebook::velox::TypeKind::VARCHAR> SUFFIX(            \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::VARBINARY: {                           \
        return PREFIX<::facebook::velox::TypeKind::VARBINARY> SUFFIX(          \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::TIMESTAMP: {                           \
        return PREFIX<::facebook::velox::TypeKind::TIMESTAMP> SUFFIX(          \
            __VA_ARGS__);                                                      \
      }                                                                        \
      case ::facebook::velox::TypeKind::DATE: {                                \
        return PREFIX<::facebook::velox::TypeKind::DATE> SUFFIX(__VA_ARGS__);  \
      }                                                                        \
      case ::facebook::velox::TypeKind::ARRAY: {                               \
        return PREFIX<::facebook::velox::TypeKind::ARRAY> SUFFIX(__VA_ARGS__); \
      }                                                                        \
      case ::facebook::velox::TypeKind::MAP: {                                 \
        return PREFIX<::facebook::velox::TypeKind::MAP> SUFFIX(__VA_ARGS__);   \
      }                                                                        \
      case ::facebook::velox::TypeKind::ROW: {                                 \
        return PREFIX<::facebook::velox::TypeKind::ROW> SUFFIX(__VA_ARGS__);   \
      }                                                                        \
      default:                                                                 \
        VELOX_FAIL("not a known type kind: {}", mapTypeKindToName(typeKind));  \
    }                                                                          \
  }()

#define VELOX_DYNAMIC_TYPE_DISPATCH(TEMPLATE_FUNC, typeKind, ...) \
  VELOX_DYNAMIC_TYPE_DISPATCH_IMPL(TEMPLATE_FUNC, , typeKind, __VA_ARGS__)

#define VELOX_DYNAMIC_TYPE_DISPATCH_ALL(TEMPLATE_FUNC, typeKind, ...)          \
  [&]() {                                                                      \
    if ((typeKind) == ::facebook::velox::TypeKind::UNKNOWN) {                  \
      return TEMPLATE_FUNC<::facebook::velox::TypeKind::UNKNOWN>(__VA_ARGS__); \
    } else if ((typeKind) == ::facebook::velox::TypeKind::OPAQUE) {            \
      return TEMPLATE_FUNC<::facebook::velox::TypeKind::OPAQUE>(__VA_ARGS__);  \
    } else {                                                                   \
      return VELOX_DYNAMIC_TYPE_DISPATCH_IMPL(                                 \
          TEMPLATE_FUNC, , typeKind, __VA_ARGS__);                             \
    }                                                                          \
  }()

#define VELOX_DYNAMIC_TYPE_DISPATCH_METHOD( \
    CLASS_NAME, METHOD_NAME, typeKind, ...) \
  VELOX_DYNAMIC_TYPE_DISPATCH_IMPL(         \
      CLASS_NAME, ::METHOD_NAME, typeKind, __VA_ARGS__)

#define VELOX_DYNAMIC_TYPE_DISPATCH_METHOD_ALL(                             \
    CLASS_NAME, METHOD_NAME, typeKind, ...)                                 \
  [&]() {                                                                   \
    if ((typeKind) == ::facebook::velox::TypeKind::UNKNOWN) {               \
      return CLASS_NAME<::facebook::velox::TypeKind::UNKNOWN>::METHOD_NAME( \
          __VA_ARGS__);                                                     \
    } else if ((typeKind) == ::facebook::velox::TypeKind::OPAQUE) {         \
      return CLASS_NAME<::facebook::velox::TypeKind::OPAQUE>::METHOD_NAME(  \
          __VA_ARGS__);                                                     \
    } else {                                                                \
      return VELOX_DYNAMIC_TYPE_DISPATCH_IMPL(                              \
          CLASS_NAME, ::METHOD_NAME, typeKind, __VA_ARGS__);                \
    }                                                                       \
  }()

#define VELOX_SCALAR_ACCESSOR(KIND) \
  std::shared_ptr<const ScalarType<TypeKind::KIND>> KIND()

#define VELOX_STATIC_FIELD_DYNAMIC_DISPATCH(CLASS, FIELD, typeKind)           \
  [&]() {                                                                     \
    switch (typeKind) {                                                       \
      case ::facebook::velox::TypeKind::BOOLEAN: {                            \
        return CLASS<::facebook::velox::TypeKind::BOOLEAN>::FIELD;            \
      }                                                                       \
      case ::facebook::velox::TypeKind::INTEGER: {                            \
        return CLASS<::facebook::velox::TypeKind::INTEGER>::FIELD;            \
      }                                                                       \
      case ::facebook::velox::TypeKind::TINYINT: {                            \
        return CLASS<::facebook::velox::TypeKind::TINYINT>::FIELD;            \
      }                                                                       \
      case ::facebook::velox::TypeKind::SMALLINT: {                           \
        return CLASS<::facebook::velox::TypeKind::SMALLINT>::FIELD;           \
      }                                                                       \
      case ::facebook::velox::TypeKind::BIGINT: {                             \
        return CLASS<::facebook::velox::TypeKind::BIGINT>::FIELD;             \
      }                                                                       \
      case ::facebook::velox::TypeKind::REAL: {                               \
        return CLASS<::facebook::velox::TypeKind::REAL>::FIELD;               \
      }                                                                       \
      case ::facebook::velox::TypeKind::DOUBLE: {                             \
        return CLASS<::facebook::velox::TypeKind::DOUBLE>::FIELD;             \
      }                                                                       \
      case ::facebook::velox::TypeKind::VARCHAR: {                            \
        return CLASS<::facebook::velox::TypeKind::VARCHAR>::FIELD;            \
      }                                                                       \
      case ::facebook::velox::TypeKind::VARBINARY: {                          \
        return CLASS<::facebook::velox::TypeKind::VARBINARY>::FIELD;          \
      }                                                                       \
      case ::facebook::velox::TypeKind::TIMESTAMP: {                          \
        return CLASS<::facebook::velox::TypeKind::TIMESTAMP>::FIELD;          \
      }                                                                       \
      case ::facebook::velox::TypeKind::DATE: {                               \
        return CLASS<::facebook::velox::TypeKind::TIMESTAMP>::FIELD;          \
      }                                                                       \
      case ::facebook::velox::TypeKind::ARRAY: {                              \
        return CLASS<::facebook::velox::TypeKind::ARRAY>::FIELD;              \
      }                                                                       \
      case ::facebook::velox::TypeKind::MAP: {                                \
        return CLASS<::facebook::velox::TypeKind::MAP>::FIELD;                \
      }                                                                       \
      case ::facebook::velox::TypeKind::ROW: {                                \
        return CLASS<::facebook::velox::TypeKind::ROW>::FIELD;                \
      }                                                                       \
      default:                                                                \
        VELOX_FAIL("not a known type kind: {}", mapTypeKindToName(typeKind)); \
    }                                                                         \
  }()

#define VELOX_STATIC_FIELD_DYNAMIC_DISPATCH_ALL(CLASS, FIELD, typeKind)   \
  [&]() {                                                                 \
    if ((typeKind) == ::facebook::velox::TypeKind::UNKNOWN) {             \
      return CLASS<::facebook::velox::TypeKind::UNKNOWN>::FIELD;          \
    } else if ((typeKind) == ::facebook::velox::TypeKind::OPAQUE) {       \
      return CLASS<::facebook::velox::TypeKind::OPAQUE>::FIELD;           \
    } else {                                                              \
      return VELOX_STATIC_FIELD_DYNAMIC_DISPATCH(CLASS, FIELD, typeKind); \
    }                                                                     \
  }()

// todo: union convenience creators

VELOX_SCALAR_ACCESSOR(INTEGER);
VELOX_SCALAR_ACCESSOR(BOOLEAN);
VELOX_SCALAR_ACCESSOR(TINYINT);
VELOX_SCALAR_ACCESSOR(SMALLINT);
VELOX_SCALAR_ACCESSOR(BIGINT);
VELOX_SCALAR_ACCESSOR(REAL);
VELOX_SCALAR_ACCESSOR(DOUBLE);
VELOX_SCALAR_ACCESSOR(TIMESTAMP);
VELOX_SCALAR_ACCESSOR(VARCHAR);
VELOX_SCALAR_ACCESSOR(VARBINARY);
VELOX_SCALAR_ACCESSOR(DATE);
VELOX_SCALAR_ACCESSOR(UNKNOWN);

template <TypeKind KIND>
std::shared_ptr<const Type> createScalarType() {
  return ScalarType<KIND>::create();
}

std::shared_ptr<const Type> createScalarType(TypeKind kind);

std::shared_ptr<const Type> createType(
    TypeKind kind,
    std::vector<std::shared_ptr<const Type>>&& children);

template <TypeKind KIND>
std::shared_ptr<const Type> createType(
    std::vector<std::shared_ptr<const Type>>&& children) {
  if (children.size() != 0) {
    throw std::invalid_argument{
        std::string(TypeTraits<KIND>::name) +
        " primitive type takes no childern"};
  }
  VELOX_USER_CHECK(TypeTraits<KIND>::isPrimitiveType);
  return ScalarType<KIND>::create();
}

template <>
std::shared_ptr<const Type> createType<TypeKind::ROW>(
    std::vector<std::shared_ptr<const Type>>&& /*children*/);
template <>
std::shared_ptr<const Type> createType<TypeKind::ARRAY>(
    std::vector<std::shared_ptr<const Type>>&& children);

template <>
std::shared_ptr<const Type> createType<TypeKind::MAP>(
    std::vector<std::shared_ptr<const Type>>&& children);
template <>
std::shared_ptr<const Type> createType<TypeKind::OPAQUE>(
    std::vector<std::shared_ptr<const Type>>&& children);

#undef VELOX_SCALAR_ACCESSOR

template <typename KEY, typename VALUE>
struct Map {
  using key_type = KEY;
  using value_type = VALUE;

 private:
  Map() {}
};

template <typename ELEMENT>
struct Array {
  using element_type = ELEMENT;

 private:
  Array() {}
};

template <typename... T>
struct Row {
  template <size_t idx>
  using type_at = typename std::tuple_element<idx, std::tuple<T...>>::type;

 private:
  Row() {}
};

struct DynamicRow {
 private:
  DynamicRow() {}
};

template <typename T>
struct CppToType {};

template <TypeKind KIND>
struct CppToTypeBase : public TypeTraits<KIND> {
  static auto create() {
    return TypeFactory<KIND>::create();
  }
};

struct Varbinary {
 private:
  Varbinary() {}
};
struct Varchar {
 private:
  Varchar() {}
};
template <>
struct CppToType<int64_t> : public CppToTypeBase<TypeKind::BIGINT> {};

template <>
struct CppToType<uint64_t> : public CppToTypeBase<TypeKind::BIGINT> {};

template <>
struct CppToType<int32_t> : public CppToTypeBase<TypeKind::INTEGER> {};

template <>
struct CppToType<uint32_t> : public CppToTypeBase<TypeKind::INTEGER> {};

template <>
struct CppToType<int16_t> : public CppToTypeBase<TypeKind::SMALLINT> {};

template <>
struct CppToType<uint16_t> : public CppToTypeBase<TypeKind::SMALLINT> {};

template <>
struct CppToType<int8_t> : public CppToTypeBase<TypeKind::TINYINT> {};

template <>
struct CppToType<uint8_t> : public CppToTypeBase<TypeKind::TINYINT> {};

template <>
struct CppToType<bool> : public CppToTypeBase<TypeKind::BOOLEAN> {};

template <>
struct CppToType<Varchar> : public CppToTypeBase<TypeKind::VARCHAR> {};

template <>
struct CppToType<folly::StringPiece> : public CppToTypeBase<TypeKind::VARCHAR> {
};

template <>
struct CppToType<velox::StringView> : public CppToTypeBase<TypeKind::VARCHAR> {
};

template <>
struct CppToType<std::string> : public CppToTypeBase<TypeKind::VARCHAR> {};

template <>
struct CppToType<const char*> : public CppToTypeBase<TypeKind::VARCHAR> {};

template <>
struct CppToType<Varbinary> : public CppToTypeBase<TypeKind::VARBINARY> {};

template <>
struct CppToType<folly::ByteRange> : public CppToTypeBase<TypeKind::VARBINARY> {
};

template <>
struct CppToType<float> : public CppToTypeBase<TypeKind::REAL> {};

template <>
struct CppToType<double> : public CppToTypeBase<TypeKind::DOUBLE> {};

template <>
struct CppToType<Timestamp> : public CppToTypeBase<TypeKind::TIMESTAMP> {};

template <>
struct CppToType<Date> : public CppToTypeBase<TypeKind::DATE> {};

// TODO: maybe do something smarter than just matching any shared_ptr, e.g. we
// can declare "registered" types explicitly
template <typename T>
struct CppToType<std::shared_ptr<T>> : public CppToTypeBase<TypeKind::OPAQUE> {
  // We override the type with the concrete specialization here!
  // using NativeType = std::shared_ptr<T>;
  static auto create() {
    return OpaqueType::create<T>();
  }
};

template <typename KEY, typename VAL>
struct CppToType<Map<KEY, VAL>> : public TypeTraits<TypeKind::MAP> {
  static auto create() {
    return MAP(CppToType<KEY>::create(), CppToType<VAL>::create());
  }
};

template <typename ELEMENT>
struct CppToType<Array<ELEMENT>> : public TypeTraits<TypeKind::ARRAY> {
  static auto create() {
    return ARRAY(CppToType<ELEMENT>::create());
  }
};

template <typename... T>
struct CppToType<Row<T...>> : public TypeTraits<TypeKind::ROW> {
  static auto create() {
    return ROW({CppToType<T>::create()...});
  }
};

template <>
struct CppToType<DynamicRow> : public TypeTraits<TypeKind::ROW> {
  static std::shared_ptr<const Type> create() {
    throw std::logic_error{"can't determine exact type for DynamicRow"};
  }
};

template <>
struct CppToType<UnknownValue> : public CppToTypeBase<TypeKind::UNKNOWN> {};

// todo: remaining cpp2type

template <TypeKind KIND>
static inline int32_t sizeOfTypeKindHelper() {
  return sizeof(typename TypeTraits<KIND>::NativeType);
}

static inline int32_t sizeOfTypeKind(TypeKind kind) {
  if (kind == TypeKind::BOOLEAN) {
    throw std::invalid_argument("sizeOfTypeKind dos not apply to boolean");
  }
  return VELOX_DYNAMIC_SCALAR_TYPE_DISPATCH(sizeOfTypeKindHelper, kind);
}

// Helper for using folly::to with StringView.
template <typename T, typename U>
static inline T to(U value) {
  return folly::to<T>(value);
}

template <>
inline Timestamp to(std::string value) {
  return Timestamp(0, 0);
}

template <>
inline UnknownValue to(std::string /* value */) {
  return UnknownValue();
}

template <>
inline std::string to(Timestamp value) {
  return value.toString();
}

template <>
inline std::string to(velox::StringView value) {
  return std::string(value.data(), value.size());
}

template <>
inline std::string to(ComplexType value) {
  return std::string("ComplexType");
}

template <>
inline ComplexType to(std::string value) {
  return ComplexType();
}

/// Adds custom type to the registry. Type names must be unique.
void registerType(
    const std::string& name,
    std::function<TypePtr(std::vector<TypePtr> childTypes)> factory);

/// Return true if customer type with specified name exists.
bool typeExists(const std::string& name);

/// Returns an instance of a custom type with the specified name and specified
/// child types.
TypePtr getType(const std::string& name, std::vector<TypePtr> childTypes);

// Allows us to transparently use folly::toAppend(), folly::join(), etc.
template <class TString>
void toAppend(
    const std::shared_ptr<const facebook::velox::Type>& type,
    TString* result) {
  result->append(type->toString());
}

} // namespace facebook::velox

namespace folly {
template <>
struct hasher<::facebook::velox::UnknownValue> {
  size_t operator()(const ::facebook::velox::UnknownValue /* value */) const {
    return 0;
  }
};

// Helper functions to allow TypeKind and some common variations to be
// transparently used by folly::sformat.
//
// e.g: folly::sformat("type: {}", typeKind);
template <>
class FormatValue<facebook::velox::TypeKind> {
 public:
  explicit FormatValue(const facebook::velox::TypeKind& type) : type_(type) {}

  template <typename FormatCallback>
  void format(FormatArg& arg, FormatCallback& cb) const {
    return format_value::formatString(
        facebook::velox::mapTypeKindToName(type_), arg, cb);
  }

 private:
  facebook::velox::TypeKind type_;
};

// Prints all types derived from `velox::Type`.
template <typename T>
class FormatValue<
    std::shared_ptr<T>,
    typename std::enable_if<
        std::is_base_of<facebook::velox::Type, T>::value>::type> {
 public:
  explicit FormatValue(const std::shared_ptr<const facebook::velox::Type>& type)
      : type_(type) {}

  template <typename FormatCallback>
  void format(FormatArg& arg, FormatCallback& cb) const {
    return format_value::formatString(type_->toString(), arg, cb);
  }

 private:
  std::shared_ptr<const facebook::velox::Type> type_;
};

} // namespace folly

template <>
struct fmt::formatter<facebook::velox::TypeKind> {
  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const facebook::velox::TypeKind& k, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", facebook::velox::mapTypeKindToName(k));
  }
};

template <typename T>
struct fmt::formatter<
    std::shared_ptr<T>,
    typename std::
        enable_if_t<std::is_base_of_v<facebook::velox::Type, T>, char>> {
  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const std::shared_ptr<T>& k, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", k->toString());
  }
};
