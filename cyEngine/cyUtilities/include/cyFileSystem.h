/**
 * @file cyFileSystem.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for FileSystem.
 */

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

/**
 * @struct FileSystem
 * @brief Static filesystem and path utility wrapper.
 */
struct CY_UTILITY_EXPORT FileSystem
{
 public:
  /**
   * @brief Opens a file handle for the given path.
   * @param fileName File path.
   * @return Opened file handle.
   */
  static File
  open(const String& fileName) {
    return cppfs::fs::open(fileName);
  }

  /**
   * @brief Loads an entire file as a binary buffer.
   * @param fileName File path.
   * @return Heap-allocated binary data buffer.
   */
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

  /**
   * @brief Checks whether a path exists.
   * @param filePath Path to test.
   * @return True if path exists.
   */
  static bool
  exists(const String& filePath) {
    return cppfs::fs::open(filePath).exists();
  }

  /**
   * @brief Creates or truncates a file and returns its handle.
   * @param filePath File path to create.
   * @return File handle for created file.
   */
  static File
  createFile(const String& filePath) {
    std::fstream newFile;
    newFile.open(filePath, std::fstream::binary | std::fstream::trunc | std::fstream::out);
    newFile.close();
    return cppfs::fs::open(filePath);
  }

  /**
   * @brief Creates a directory if it does not exist.
   * @param folderPath Directory path.
   * @return True if folder was created.
   */
  static bool
  createFolder(const String& folderPath) {
    File f = cppfs::fs::open(folderPath);
    if (!f.exists()) {
      return f.createDirectory();
    }
    return false;
  }

  /**
   * @brief Deletes a directory recursively.
   * @param folderPath Directory path.
   */
  static void
  deleteFolder(const String& folderPath) {
    File f = cppfs::fs::open(folderPath);
    if (f.isDirectory()) {
      f.removeDirectoryRec();
    }
  }

  /**
   * @brief Encodes file contents to Base64.
   * @param fileName File path.
   * @return Base64 string.
   */
  static String
  toBase64(const String& fileName) {
    return cppfs::fs::base64(fileName);
  }

  /**
   * @brief Decodes Base64 content from file source.
   * @param fileName File path.
   * @return Decoded string.
   */
  static String
  fromBase64(const String& fileName) {
    return cppfs::fs::fromBase64(fileName);
  }

  /**
   * @brief Converts hash bytes to string.
   * @param fileName Hash byte data.
   * @return Hash string.
   */
  static String
  hashToString(const unsigned char* fileName) {
    return cppfs::fs::hashToString(fileName);
  }

  /**
   * @brief Calculates SHA1 hash string for input text.
   * @param fileName Input text.
   * @return SHA1 hash.
   */
  static String
  stringToHash(const String& fileName) {
    return cppfs::fs::sha1(fileName);
  }

  /**
   * @brief Returns executable path.
   * @return Executable path.
   */
  static Path
  getExecutablePath() {
    return Path(cpplocate::getExecutablePath());
  }

  /**
   * @brief Returns process working directory.
   * @return Working directory path.
   */
  static Path
  getWorkingDirectory(){
    return Path(cpplocate::getModulePath());
  }

  /**
   * @brief Returns bundle path.
   * @return Bundle path.
   */
  static Path
  getBundlePath() {
    return Path(cpplocate::getBundlePath());
  }

  /**
   * @brief Returns path of a loaded library.
   * @param library Library handle.
   * @return Library file path.
   */
  static Path
  getLibraryPath(void* library) {
    return Path(cpplocate::getLibraryPath(library));
  }

  /**
   * @brief Locates a path from module context and relative location.
   * @param relPath Relative path.
   * @param systemDir System directory hint.
   * @param symbol Symbol used for location context.
   * @return Resolved path.
   */
  static Path
  locatePath(const String& relPath, const String& systemDir, void* symbol) {
    return Path(cpplocate::locatePath(relPath, systemDir, symbol));
  }

  /**
   * @brief Returns user home directory.
   * @return Home directory path.
   */
  static Path
  homeDir() {
    return Path(cpplocate::homeDir());
  }

  /**
   * @brief Returns user documents directory.
   * @return Documents directory path.
   */
  static Path
  documentsDir() {
    return Path(cpplocate::documentDir());
  }

  /**
   * @brief Returns user profile directory.
   * @return Profile directory path.
   */
  static Path
  profileDir() {
    return Path(cpplocate::profileDir());
  }

  /**
   * @brief Returns temporary directory scoped to executable.
   * @return Temp directory path.
   */
  static Path
  tempDir() {
    return Path(cpplocate::tempDir(getExecutablePath().baseName()));
  }

  /**
   * @brief Returns configuration directory scoped to executable.
   * @return Config directory path.
   */
  static Path
  configDir() {
    return Path(cpplocate::configDir(getExecutablePath().baseName()));
  }

  /**
   * @brief Returns local data directory scoped to executable.
   * @return Local data directory path.
   */
  static Path
  localDir() {
    return Path(cpplocate::localDir(getExecutablePath().baseName()));
  }

  /**
   * @brief Returns roaming data directory scoped to executable.
   * @return Roaming data directory path.
   */
  static Path
  roamingDir() {
    return Path(cpplocate::roamingDir(getExecutablePath().baseName()));
  }
};

/**
 * @struct FileExt
 * @brief Common file extension constants used by the engine.
 */
struct FileExt
{
  /**
   * @brief JSON extension string.
   */
  static String JSON;
  /**
   * @brief Scene extension string.
   */
  static String SCENE;
  /**
   * @brief Package extension string.
   */
  static String PACKAGE;
  /**
   * @brief Asset extension string.
   */
  static String ASSET;
};

}

