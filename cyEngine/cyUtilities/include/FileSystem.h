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

#define USING_CPPFS

#ifdef USING_CPPFS

#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <cppfs/FilePath.h>
#include <cppfs/LoginCredentials.h>
#include <cppfs/fs.h>
#endif // USING_CPPFS

namespace CYLLENE_SDK
{
#ifdef USING_CPPFS

  using File          = cppfs::FileHandle;
  using Path          = cppfs::FilePath;
  using FileIterator  = cppfs::FileIterator;
  using Credentials   = cppfs::LoginCredentials;

#endif // USING_CPPFS

  class CY_UTILITY_EXPORT FileSystem
  {
  public:
    static File
    open(const String& fileName) {
      return cppfs::fs::open(fileName);
    }
  };

}