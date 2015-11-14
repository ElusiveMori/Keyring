#pragma once

#include <modes.h>
#include <aes.h>
#include <osrng.h>
#include <sha.h>
#include <pwdbased.h>
#include <base64.h>
#include <files.h>
#include <fstream>
#include <sstream>

#include <boost/filesystem/fstream.hpp>
#include "PasswordManager.h"

namespace Security {
	using namespace CryptoPP;
	using namespace boost;

	bool LoadManager(std::istream& stream, PasswordManager& manager, const std::string& password);
	void SaveManager(std::ostream& stream, const PasswordManager& manager, const std::string& password);

	void DeriveKey(const std::string&, const byte*, size_t, byte*, size_t);
	void DecipherData(const byte*, byte*, size_t, const byte*, size_t, const byte*, size_t);
	void EncipherData(const byte*, byte*, size_t, const byte*, size_t, const byte*, size_t);

	bool CompareHash(const byte*, const byte*, size_t);

	const int32_t passwordIterations = 100000;
};

