#pragma once

#include <modes.h>
#include <aes.h>
#include <osrng.h>
#include <sha.h>
#include <pwdbased.h>
#include <fstream>
#include <sstream>

#include <boost/filesystem/fstream.hpp>
#include "CategoryManager.h"

namespace Security {
	using namespace CryptoPP;
	using namespace boost;

	bool LoadCategories(filesystem::ifstream&, CategoryManager&, const std::string&);
	void SaveCategories(filesystem::ofstream&, const CategoryManager&, const std::string&);

	void DeriveKey(const std::string&, const byte*, size_t, byte*, size_t);
	void DecodeData(const byte*, byte*, size_t, const byte*, size_t, const byte*, size_t);
	void EncodeData(const byte*, byte*, size_t, const byte*, size_t, const byte*, size_t);

	bool CompareHash(const byte*, const byte*, size_t);

	void Deserialize(CategoryManager&, std::stringstream&);
	void Serialize(const CategoryManager&, std::stringstream&);
	size_t GetSerializedSize(const CategoryManager&);
	
	size_t ReadSize(std::stringstream&);
	std::string ReadString(std::stringstream&);

	void WriteSize(std::stringstream&, size_t);
	void WriteString(std::stringstream&, const std::string&);

	const int32_t passwordIterations = 10000;
};

