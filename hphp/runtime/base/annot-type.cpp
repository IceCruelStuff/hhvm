/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/annot-type.h"

#include <folly/Optional.h>
#include <folly/MapUtil.h>
#include "hphp/runtime/base/string-data.h"
#include "hphp/runtime/base/static-string-table.h"
#include "hphp/runtime/vm/runtime.h"
#include "hphp/util/hash-map.h"

namespace HPHP {

///////////////////////////////////////////////////////////////////////////////

using HhvmStrToTypeMap = hphp_hash_map<
  const StringData*, AnnotType, string_data_hash, string_data_isame
>;

using StdStrToTypeMap = hphp_string_imap<AnnotType>;

const StaticString
  s_HH_Traversable("HH\\Traversable"),
  s_HH_RX_Traversable("HH\\Rx\\Traversable"),
  s_HH_KeyedTraversable("HH\\KeyedTraversable"),
  s_HH_RX_KeyedTraversable("HH\\Rx\\KeyedTraversable"),
  s_HH_Container("HH\\Container"),
  s_HH_KeyedContainer("HH\\KeyedContainer"),
  s_XHPChild("XHPChild"),
  s_Stringish("Stringish");

MaybeDataType nameToMaybeDataType(const StringData* typeName) {
  auto const* type = nameToAnnotType(typeName);
  return type ? MaybeDataType(getAnnotDataType(*type)) : folly::none;
}

MaybeDataType nameToMaybeDataType(const std::string& typeName) {
  auto const* type = nameToAnnotType(typeName);
  return type ? MaybeDataType(getAnnotDataType(*type)) : folly::none;
}

/**
 * This is the authoritative map that determines which typehints require
 * special handling. Any typehint not on this list is assumed to be normal
 * "class-name" typehint.
 */
static const std::pair<HhvmStrToTypeMap, StdStrToTypeMap>& getAnnotTypeMaps() {
  static const std::pair<HhvmStrToTypeMap, StdStrToTypeMap> mapPair = []() {
    std::pair<HhvmStrToTypeMap, StdStrToTypeMap> mappedPairs;
    const struct Pair {
      const char* name;
      AnnotType type;
    } pairs[] = {
      { "HH\\nothing",  AnnotType::Nothing },
      { "HH\\noreturn", AnnotType::NoReturn },
      { "HH\\null",     AnnotType::Null },
      { "HH\\void",     AnnotType::Null },
      { "HH\\bool",     AnnotType::Bool },
      { "HH\\int",      AnnotType::Int },
      { "HH\\float",    AnnotType::Float },
      { "HH\\string",   AnnotType::String },
      { "array",        AnnotType::Array },
      { "HH\\resource", AnnotType::Resource },
      { "HH\\mixed",    AnnotType::Mixed },
      { "HH\\nonnull",  AnnotType::Nonnull },
      { "HH\\num",      AnnotType::Number },
      { "HH\\arraykey", AnnotType::ArrayKey },
      { "HH\\this",     AnnotType::This },
      { "self",         AnnotType::Self },
      { "parent",       AnnotType::Parent },
      { "callable",     AnnotType::Callable },
      { "HH\\dict",     AnnotType::Dict },
      { "HH\\vec",      AnnotType::Vec },
      { "HH\\keyset",   AnnotType::Keyset },
      {
        "HH\\varray",
        RuntimeOption::EvalHackArrDVArrs ? AnnotType::Vec : AnnotType::VArray
      },
      {
        "HH\\darray",
        RuntimeOption::EvalHackArrDVArrs ? AnnotType::Dict : AnnotType::DArray
      },
      {
        "HH\\varray_or_darray",
        RuntimeOption::EvalHackArrDVArrs
          ? AnnotType::VecOrDict : AnnotType::VArrOrDArr
      },
      { "HH\\vec_or_dict", AnnotType::VecOrDict },
      { "HH\\arraylike", AnnotType::ArrayLike },
    };
    for (unsigned i = 0; i < sizeof(pairs) / sizeof(Pair); ++i) {
      mappedPairs.first[makeStaticString(pairs[i].name)] = pairs[i].type;
      mappedPairs.second[pairs[i].name] = pairs[i].type;
    }
    return mappedPairs;
  }();
  return mapPair;
}

const AnnotType* nameToAnnotType(const StringData* typeName) {
  assertx(typeName);
  auto const& mapPair = getAnnotTypeMaps();
  return folly::get_ptr(mapPair.first, typeName);
}

const AnnotType* nameToAnnotType(const std::string& typeName) {
  auto const& mapPair = getAnnotTypeMaps();
  auto const* at = folly::get_ptr(mapPair.second, typeName);
  assertx(!at || *at != AnnotType::Object);
  return at;
}

namespace {

bool isame(folly::StringPiece a, folly::StringPiece b) {
  return a.size() == b.size() && !strcasecmp(a.data(), b.data());
}

}

bool interface_supports_non_objects(const StringData* s) {
  return interface_supports_non_objects(s->slice());
}

bool interface_supports_non_objects(folly::StringPiece s) {
  return interface_supports_arrlike(s) ||
         isame(s, s_Stringish.slice());
}

bool interface_supports_arrlike(const StringData* s) {
  return interface_supports_arrlike(s->slice());
}

bool interface_supports_arrlike(folly::StringPiece s) {
  return isame(s, s_HH_Traversable.slice()) ||
         isame(s, s_HH_KeyedTraversable.slice()) ||
         isame(s, s_HH_RX_Traversable.slice()) ||
         isame(s, s_HH_RX_KeyedTraversable.slice()) ||
         isame(s, s_HH_Container.slice()) ||
         isame(s, s_HH_KeyedContainer.slice()) ||
         isame(s, s_XHPChild.slice());
}

bool interface_supports_string(const StringData* s) {
  return interface_supports_string(s->slice());
}

bool interface_supports_string(folly::StringPiece s) {
  return isame(s, s_XHPChild.slice()) ||
         isame(s, s_Stringish.slice());
}

bool interface_supports_int(const StringData* s) {
  return interface_supports_int(s->slice());
}

bool interface_supports_int(folly::StringPiece s) {
  return isame(s, s_XHPChild.slice());
}

bool interface_supports_double(const StringData* s) {
  return interface_supports_double(s->slice());
}

bool interface_supports_double(folly::StringPiece s) {
  return isame(s, s_XHPChild.slice());
}

///////////////////////////////////////////////////////////////////////////////

TypedValue annotDefaultValue(AnnotType at) {
  switch (at) {
    case AnnotType::Mixed:
    case AnnotType::Self:
    case AnnotType::Parent:
    case AnnotType::This:
    case AnnotType::Callable:
    case AnnotType::Resource:
    case AnnotType::Object:
    case AnnotType::Nothing:
    case AnnotType::Record:
    case AnnotType::NoReturn:
    case AnnotType::Null:     return make_tv<KindOfNull>();
    case AnnotType::Nonnull:
    case AnnotType::Number:
    case AnnotType::ArrayKey:
    case AnnotType::Int:      return make_tv<KindOfInt64>(0);
    case AnnotType::Bool:     return make_tv<KindOfBoolean>(false);
    case AnnotType::Float:    return make_tv<KindOfDouble>(0);
    case AnnotType::DArray:
      return make_persistent_array_like_tv(staticEmptyDArray());
    case AnnotType::VArray:
    case AnnotType::VArrOrDArr:
      return make_persistent_array_like_tv(staticEmptyVArray());
    case AnnotType::ArrayLike:
    case AnnotType::VecOrDict:
    case AnnotType::Vec:
      return make_tv<KindOfPersistentVec>(staticEmptyVec());
    case AnnotType::String:
      return make_tv<KindOfPersistentString>(staticEmptyString());
    case AnnotType::Array:
      return make_persistent_array_like_tv(staticEmptyArray());
    case AnnotType::Dict:
      return make_tv<KindOfPersistentDict>(staticEmptyDictArray());
    case AnnotType::Keyset:
      return make_tv<KindOfPersistentKeyset>(staticEmptyKeysetArray());
  }
  always_assert(false);
}

///////////////////////////////////////////////////////////////////////////////

}
