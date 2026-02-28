/**
 * @file cyJSON.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for JSON.
 */

#pragma once

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "cyUtilitiesPrerequisites.h"

#include "cyStdHeaders.h"

namespace CYLLENE_SDK {

// Redefinitions
using JSONFileReader    = rapidjson::FileReadStream;

using JSONFileWriter    = rapidjson::FileWriteStream;

using JSONIStream       = rapidjson::IStreamWrapper;

using JSONOStream       = rapidjson::OStreamWrapper;

template<typename T>
using JSONPrettyWriter  = rapidjson::PrettyWriter<T>;

using JSONValue         = rapidjson::Value;

template<typename T>
using JSONWriter        = rapidjson::Writer<T>;

using JSONStringBuffer  = rapidjson::StringBuffer;

/**
 * @class JSONDocument
 * @brief RapidJSON document wrapper with string serialization helpers.
 */
class JSONDocument : public rapidjson::Document
{
public:

  /**
   * @brief Serializes document to compact JSON string.
   * @return Compact JSON string.
   */
  const CYLLENE_SDK::String
  stringify() {
    JSONStringBuffer buffer;
    JSONWriter<JSONStringBuffer> writer(buffer);
    Accept(writer);
    return buffer.GetString();
  }
  
  /**
   * @brief Serializes document to pretty formatted JSON string.
   * @return Pretty JSON string.
   */
  const CYLLENE_SDK::String
  prettyString() {
    JSONStringBuffer buffer;
    JSONPrettyWriter<JSONStringBuffer> writer(buffer);
    Accept(writer);
    return buffer.GetString();
  }
};
}
