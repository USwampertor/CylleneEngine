/**
 * @file cyFileSystem.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for FileSystem.
 */

#pragma once


#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>

#include <cpplocate/cpplocate.h>

#include <filesystem>

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK
{
// Redefinition
using File          = cppfs::FileHandle;
// using Path          = cppfs::FilePath;
// using FileIterator  = cppfs::FileIterator;
// using Credentials   = cppfs::LoginCredentials;
// using FileEvent     = cppfs::FileEvent;

using Path = std::filesystem::path;
using FileEntry = std::filesystem::directory_entry;
using FileIterator = std::filesystem::directory_iterator;



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
  open(Stringview fileName) {
    return cppfs::fs::open(fileName.data());
  }

  static File
  open(Path fileName) {
    return cppfs::fs::open(fileName.string());
  }

  /**
    * @brief Loads an entire file as a binary buffer.
    * @param fileName File path.
    * @param outFileSize Receives the number of bytes loaded.
    * @return Heap-allocated binary data buffer (free with cy_deleteN).
    */
  static unsigned char*
  openBinary(Stringview fileName, size_t& outFileSize) {
    std::fstream file;
    file.open(fileName.data(), IfStream::binary | IfStream::in | IfStream::ate);
    const size_t file_length = static_cast<size_t>(file.tellg());

    unsigned char* data = cy_newN<unsigned char>(file_length);
    file.seekg(file.beg);
    file.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(file_length));
    outFileSize = file_length;
    return data;
  }

  static unsigned char*
  openBinary(const Path& fileName, size_t& outFileSize) {
    std::fstream file;
    file.open(fileName.string(), IfStream::binary | IfStream::in | IfStream::ate);
    const size_t file_length = static_cast<size_t>(file.tellg());

    unsigned char* data = cy_newN<unsigned char>(file_length);
    file.seekg(file.beg);
    file.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(file_length));
    outFileSize = file_length;
    return data;
  }

  /**
   * @brief Checks whether a path exists.
   * @param filePath Path to test.
   * @return True if path exists.
   */
  static bool
  exists(Stringview filePath) {
    return std::filesystem::exists(filePath);
  }

  static bool
  exists(const Path& filePath) {
    return std::filesystem::exists(filePath);
  }

  /**
   * @brief Creates or truncates a file and returns its handle.
   * @param filePath File path to create.
   * @return File handle for created file.
   */
  static File
  createFile(Stringview filePath) {
    IfStream newFile;
    newFile.open(filePath.data(), std::fstream::binary | std::fstream::trunc | std::fstream::out);
    newFile.close();
    return cppfs::fs::open(filePath.data());
  }

  static File
  createFile(const Path& filePath) {
    IfStream newFile;
    newFile.open(filePath, std::fstream::binary | std::fstream::trunc | std::fstream::out);
    newFile.close();
    return cppfs::fs::open(filePath.string());
  }

  /**
   * @brief Creates a directory if it does not exist.
   * @param folderPath Directory path.
   * @return True if folder was created.
   */
  static bool
  createFolder(Stringview folderPath) {
    File f = FileSystem::open(folderPath);
    if (!f.exists()) {
      return f.createDirectory();
    }
    return false;
  }

  static bool
  createFolder(const Path& folderPath) {
    File f = FileSystem::open(folderPath);
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
  deleteFolder(Stringview folderPath) {
    File f = FileSystem::open(folderPath);
    if (f.isDirectory()) {
      f.removeDirectoryRec();
    }
  }

  // @see deleteFolder
  static void
  deleteFolder(const Path& folderPath) {
    File f = FileSystem::open(folderPath);
    if (f.isDirectory()) {
      f.removeDirectoryRec();
    }
  }

  /**
   * @brief Deletes a directory recursively.
   * @param folderPath Directory path.
   */
  static void
  deleteFile(Stringview filePath) {
    File f = FileSystem::open(filePath);
    if (f.isFile()) {
      f.remove();
    }
  }

  static void
  deleteFile(const Path& filePath) {
    File f = FileSystem::open(filePath);
    if (f.isFile()) {
      f.remove();
    }
  }

  /**
   * @brief Encodes file contents to Base64.
   * @param fileName File path.
   * @return Base64 string.
   */
  static String
  toBase64(Stringview fileName) {
    return cppfs::fs::base64(fileName.data());
  }

  /**
   * @brief Decodes Base64 content from file source.
   * @param fileName File path.
   * @return Decoded string.
   */
  static String
  fromBase64(Stringview fileName) {
    return cppfs::fs::fromBase64(fileName.data());
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
  stringToHash(Stringview fileName) {
    return cppfs::fs::sha1(fileName.data());
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
  locatePath(Stringview relPath, Stringview systemDir, void* symbol) {
    return Path(cpplocate::locatePath(relPath.data(), systemDir.data(), symbol));
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
    return Path(cpplocate::tempDir(getExecutablePath().stem().string()));
  }

  /**
   * @brief Returns configuration directory scoped to executable.
   * @return Config directory path.
   */
  static Path
  configDir() {
    return Path(cpplocate::configDir(getExecutablePath().stem().string()));
  }

  /**
   * @brief Returns local data directory scoped to executable.
   * @return Local data directory path.
   */
  static Path
  localDir() {
    return Path(cpplocate::localDir(getExecutablePath().stem().string()));
  }

  /**
   * @brief Returns roaming data directory scoped to executable.
   * @return Roaming data directory path.
   */
  static Path
  roamingDir() {
    return Path(cpplocate::roamingDir(getExecutablePath().stem().string()));
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

