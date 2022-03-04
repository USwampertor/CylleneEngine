/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyFileSystem.h
 * @author Marco "Swampy" Millan
 * @date 8/4/2021
 * @brief 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <cppfs/FilePath.h>
#include <cppfs/LoginCredentials.h>
#include <cppfs/fs.h>
#include <cpplocate/cpplocate.h>

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK
{
// Redefinition
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

  static unsigned char*
  openBinary(const String& fileName) {
    IfStream file;
    file.open(fileName, IfStream::binary | IfStream::in | IfStream::ate);
    const int32 file_length = static_cast<const int>(file.tellg());

    unsigned char* data = new unsigned char[file_length];
    file.seekg(file.beg);
    file.read(reinterpret_cast<char*>(data), file_length); // error PBYTE is incompatibe with char*
    return data;
  }

  static bool
  exists(const String& filePath) {
    return cppfs::fs::open(filePath).exists();
  }

  static File
  createFile(const String& filePath) {
    std::fstream newFile;
    newFile.open(filePath, std::fstream::binary | std::fstream::trunc | std::fstream::out);
    newFile.close();
    return cppfs::fs::open(filePath);
  }

  static bool
  createFolder(const String& folderPath) {
    File f = cppfs::fs::open(folderPath);
    if (!f.exists()) {
      return f.createDirectory();
    }
    return false;
  }

  static void
  deleteFolder(const String& folderPath) {
    File f = cppfs::fs::open(folderPath);
    if (f.isDirectory()) {
      f.removeDirectoryRec();
    }
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
  getWorkingDirectory(){
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

  static Path
  profileDir() {
    return Path(cpplocate::profileDir());
  }

  static Path
  tempDir() {
    return Path(cpplocate::tempDir(getExecutablePath().baseName()));
  }

  static Path
  configDir() {
    return Path(cpplocate::configDir(getExecutablePath().baseName()));
  }

  static Path
  localDir() {
    return Path(cpplocate::localDir(getExecutablePath().baseName()));
  }

  static Path
  roamingDir() {
    return Path(cpplocate::roamingDir(getExecutablePath().baseName()));
  }
};
}
