/**
 * @file cyFileDataStream.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Data stream related classes
 */
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>


namespace CYLLENE_SDK
{
using StringStream  = std::stringstream;

using IStringStream = std::istringstream;

using OStringStream = std::ostringstream;

using IfStream = std::ifstream;

using OfStream = std::ofstream;

using FileBuffer = std::filebuf;

using FStream = std::fstream;

using StreamSize    = std::streamsize;

}