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

// using JSONDocument = rapidjson::Document;

using JSONFileReader = rapidjson::FileReadStream;

using JSONFileWriter = rapidjson::FileWriteStream;

using JSONIStream = rapidjson::IStreamWrapper;

using JSONOStream = rapidjson::OStreamWrapper;

template<typename T>
using JSONPrettyWriter = rapidjson::PrettyWriter<T>;

using JSONValue = rapidjson::Value;

template<typename T>
using JSONWriter = rapidjson::Writer<T>;

using JSONStringBuffer = rapidjson::StringBuffer;

class JSONDocument : public rapidjson::Document
{
public:

  const CYLLENE_SDK::String
  stringify() {
    JSONStringBuffer buffer;
    JSONWriter<JSONStringBuffer> writer(buffer);
    Accept(writer);
    return buffer.GetString();
  }
  
  const CYLLENE_SDK::String
  prettyString() {
    JSONStringBuffer buffer;
    JSONPrettyWriter<JSONStringBuffer> writer(buffer);
    Accept(writer);
    return buffer.GetString();
  }
};
}