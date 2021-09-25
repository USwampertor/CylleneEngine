/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyFileSystem.h
 * @author Marco "Swampy" Millan
 * @date 8/4/2021
 * @brief 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include "cyUtilitiesPrerequisites.h"

#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <cppfs/FilePath.h>
#include <cppfs/LoginCredentials.h>
#include <cppfs/fs.h>

namespace CYLLENE_SDK
{
  using File          = cppfs::FileHandle;
  using Path          = cppfs::FilePath;
  using FileIterator  = cppfs::FileIterator;
  using Credentials   = cppfs::LoginCredentials;
  using FileEvent     = cppfs::FileEvent;

  struct CY_UTILITY_EXPORT FileSystem
  {
   public:
    static File
    open(const String& fileName) {
      return cppfs::fs::open(fileName);
    }

    static String
    toBase64(const String& fileName) {
      return cppfs::fs::base64(fileName);
    }

    static String
    fromBase64(const String& fileName) {
      return cppfs::fs::fromBase64(fileName);
    }

    static String
    hashToString(const unsigned char* fileName) {
      return cppfs::fs::hashToString(fileName);
    }

    static String
    stringToHash(const String& fileName) {
      return cppfs::fs::sha1(fileName);
    }
  };
}