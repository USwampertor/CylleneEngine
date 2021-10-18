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
#include "cyModule.h"

#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <cppfs/FilePath.h>
#include <cppfs/LoginCredentials.h>
#include <cppfs/fs.h>
#include <cpplocate/cpplocate.h>

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

    static Path
    getExecutablePath() {
      return Path(cpplocate::getExecutablePath());
    }

    static Path
    getBasePath(){
      return Path(cpplocate::getModulePath());
    }

    static Path
    getBundlePath() {
      return Path(cpplocate::getBundlePath());
    }

    static Path
    getLibraryPath(void* library) {
      return Path(cpplocate::getLibraryPath(library));
    }

    static Path
    locatePath(const String& relPath, const String& systemDir, void* symbol) {
      return Path(cpplocate::locatePath(relPath, systemDir, symbol));
    }

    static Path
    homeDir() {
      return Path(cpplocate::homeDir());
    }

    static Path
    documentsDir() {
      return Path(cpplocate::documentDir());
    }

  };
}