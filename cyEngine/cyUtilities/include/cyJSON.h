/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyJSON.h
 * @author Marco "Swampy" Millan
 * @date 10/18/2021
 * @brief JSON utility for serialization
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace CYLLENE_SDK {
  using JSON = rapidjson::Document;
  using JSONValue = rapidjson::Value;

  class CY_UTILITY_EXPORT JSONUtility
  {
  public:

    static JSON
    fromString(const String& jsonString) {
      JSON toReturn;
      toReturn.Parse(jsonString.c_str());
      return toReturn;
    }

    static String
    stringify(const JSON& json) {
      rapidjson::StringBuffer buffer;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      json.Accept(writer);
      String toReturn = buffer.GetString();
      return toReturn;
    }

    static String
    prettyString(const JSON& json) {
      rapidjson::StringBuffer buffer;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
      json.Accept(writer);
      return buffer.GetString();
    }

  };
}