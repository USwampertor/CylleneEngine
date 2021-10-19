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
#include "cyStdHeaders.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>


namespace CYLLENE_SDK {
  // using JSON = rapidjson::Document;
  using JSONValue = rapidjson::Value;

  class JSON : public rapidjson::Document
  {
  public:
    std::string
    stringify() {
      rapidjson::StringBuffer buffer;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      Accept(writer);
      return buffer.GetString();
     
    }

    std::string
    prettyString() {
      rapidjson::StringBuffer buffer;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
      Accept(writer);
      return buffer.GetString();
    }
  };
}