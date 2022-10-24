#pragma once

#include <json/json.h>
#include <json/value.h>
#include <string>
#include <optional>

namespace JsonUtil {

std::optional<std::string> JsonTypeToString(const Json::ValueType& type) {
  switch (type) {
    case Json::ValueType::nullValue:
      return std::nullopt;
    case Json::ValueType::intValue:
      return "SignedIntValue";
    case Json::ValueType::uintValue:
      return "UnsignedIntValue";
    case Json::ValueType::realValue:
      return "DoubleValue";
    case Json::ValueType::stringValue:
      return "StringValue";
    case Json::ValueType::booleanValue:
      return "BooleanValue";
    case Json::ValueType::arrayValue:
      return "ArrayValue";
    case Json::ValueType::objectValue:
      return "DictValue";
  }
}

bool IsNull(const Json::Value& json_value) {
  return json_value.type() == Json::ValueType::nullValue;
}

bool IsSignedIntValue(const Json::Value& json_value) {
  return json_value.type() == Json::ValueType::intValue;
}

bool IsUnsignedIntValue(const Json::Value& json_value) {
  return json_value.type() == Json::ValueType::uintValue;
}

bool IsArray(const Json::Value& json_value) {
    return
  json_value.type() == Json::ValueType::arrayValue;
}

};  // namespace JsonUtil