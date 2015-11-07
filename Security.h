#pragma once

#include <modes.h>
#include <aes.h>
#include <osrng.h>
#include <sha.h>
#include <pwdbased.h>
#include <fstream>
#include <sstream>

#include <boost/filesystem/fstream.hpp>
#include "PasswordManager.h"

namespace Security {
	using namespace CryptoPP;
	using namespace boost;

	bool LoadCategories(std::istream&, PasswordManager&, const std::string&);
	void SaveCategories(std::ostream&, const PasswordManager&, const std::string&);

	void DeriveKey(const std::string&, const byte*, size_t, byte*, size_t);
	void DecodeData(const byte*, byte*, size_t, const byte*, size_t, const byte*, size_t);
	void EncodeData(const byte*, byte*, size_t, const byte*, size_t, const byte*, size_t);

	bool CompareHash(const byte*, const byte*, size_t);

	void Deserialize(PasswordManager&, std::istream&);
	void Serialize(const PasswordManager&, std::ostream&);
	size_t GetSerializedSize(const PasswordManager&);
	
	size_t ReadSize(std::istream&);
	std::string ReadString(std::istream&);

	void WriteSize(std::ostream&, size_t);
	void WriteString(std::ostream&, const std::string&);

	const int32_t passwordIterations = 100000;
};

