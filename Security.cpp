#include "Security.h"

using namespace CryptoPP;
using namespace boost;

bool Security::LoadCategories(filesystem::ifstream& in, CategoryManager& manager, const std::string& password) {
	if (in.good()) {
		byte iv[AES::BLOCKSIZE];
		byte salt[AES::MAX_KEYLENGTH];
		byte key[AES::MAX_KEYLENGTH];
		byte hash[SHA256::DIGESTSIZE];

		in.read(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);
		in.read(reinterpret_cast<char*>(salt), AES::MAX_KEYLENGTH);
		in.read(reinterpret_cast<char*>(hash), SHA256::DIGESTSIZE);

		DeriveKey(password, salt, AES::MAX_KEYLENGTH, key, AES::MAX_KEYLENGTH);

		size_t start = in.tellg();
		size_t end = in.seekg(0, in.end).tellg();
		size_t size = end - start;
		in.seekg(start);

		std::unique_ptr<char[]> cipheredData(new char[size]);
		std::unique_ptr<char[]> decipheredData(new char[size]);
		
		in.read(cipheredData.get(), size);

		DecodeData(reinterpret_cast<byte*>(cipheredData.get()), reinterpret_cast<byte*>(decipheredData.get()), size, key, AES::MAX_KEYLENGTH, iv, AES::BLOCKSIZE);
		
		if (CompareHash(hash, reinterpret_cast<byte*>(decipheredData.get()), size)) {
			std::stringstream stream;
			stream.write(decipheredData.get(), size);

			Deserialize(manager, stream);

			return true;
		}
		else {
			return false;
		}
	}
	else {
		throw std::runtime_error("File is in bad state.");
	}
}

void Security::SaveCategories(filesystem::ofstream& out, const CategoryManager& manager, const std::string& password) {
	if (out.good()) {
		byte iv[AES::BLOCKSIZE];
		byte salt[AES::MAX_KEYLENGTH];
		byte key[AES::MAX_KEYLENGTH];
		byte hash[SHA256::DIGESTSIZE];

		AutoSeededRandomPool random;

		random.GenerateBlock(iv, AES::BLOCKSIZE);
		random.GenerateBlock(salt, AES::MAX_KEYLENGTH);
		DeriveKey(password, salt, AES::MAX_KEYLENGTH, key, AES::MAX_KEYLENGTH);

		out.write(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);
		out.write(reinterpret_cast<char*>(salt), AES::MAX_KEYLENGTH);

		std::stringstream stream;
		Serialize(manager, stream);
		size_t size = GetSerializedSize(manager);

		std::unique_ptr<char[]> cipheredData(new char[size]);
		std::unique_ptr<char[]> decipheredData(new char[size]);

		stream.read(decipheredData.get(), size);
		SHA256().CalculateDigest(hash, reinterpret_cast<byte*>(decipheredData.get()), size);
		
		EncodeData(reinterpret_cast<byte*>(decipheredData.get()), reinterpret_cast<byte*>(cipheredData.get()), size, key, AES::MAX_KEYLENGTH, iv, AES::BLOCKSIZE);

		out.write(reinterpret_cast<char*>(hash), SHA256::DIGESTSIZE);
		out.write(cipheredData.get(), size);
	}
	else {
		throw std::runtime_error("File is in bad state.");
	}
}

void Security::DeriveKey(const std::string& password, const byte* salt, size_t saltSize, byte* derived, size_t derivedSize) {
	PKCS5_PBKDF2_HMAC<SHA256>().DeriveKey(
		derived,
		derivedSize,
		0,
		reinterpret_cast<const byte*>(password.data()),
		password.size(),
		salt,
		saltSize,
		passwordIterations);
}

void Security::DecodeData(const byte* ciphered, byte* deciphered, size_t size, const byte* key, size_t keySize, const byte* iv, size_t ivSize) {
	CTR_Mode<AES>::Decryption decryptor;
	decryptor.SetKeyWithIV(key, keySize, iv, ivSize);
	decryptor.ProcessData(deciphered, ciphered, size);
}

void Security::EncodeData(const byte* deciphered, byte* ciphered, size_t size, const byte* key, size_t keySize, const byte* iv, size_t ivSize) {
	CTR_Mode<AES>::Encryption encryptor;
	encryptor.SetKeyWithIV(key, keySize, iv, ivSize);
	encryptor.ProcessData(ciphered, deciphered, size);
}

bool Security::CompareHash(const byte* hash, const byte* data, size_t size) {
	return SHA256().VerifyDigest(hash, data, size);
}

void Security::Deserialize(CategoryManager& manager, std::stringstream& stream) {
	size_t nCategories = ReadSize(stream);

	for (size_t i = 0; i < nCategories; ++i) {
		std::string categoryName = ReadString(stream);
		size_t nEntries = ReadSize(stream);

		size_t categoryIndex = manager.AddCategory(categoryName);

		for (size_t j = 0; j < nEntries; ++j) {
			std::string entryUsername = ReadString(stream);
			std::string entryPassword = ReadString(stream);

			manager.GetCategory(categoryIndex).AddEntry(entryUsername, entryPassword);
		}
	}
}

void Security::Serialize(const CategoryManager& manager, std::stringstream& stream) {
	const auto& categories = manager.GetCategories();
	WriteSize(stream, categories.size());

	for (auto&& category : categories) {
		WriteString(stream, category.GetName());

		const auto& entries = category.GetEntries();
		WriteSize(stream, entries.size());
		for (auto&& entry = entries.begin(); entry != entries.end(); ++entry) {
			WriteString(stream, entry->GetUsername());
			WriteString(stream, entry->GetPassword());
		}
	}
}

size_t Security::GetSerializedSize(const CategoryManager& manager) {
	size_t size = 0;

	size += 4; // category count

	const auto& categories = manager.GetCategories();

	for (auto&& category : categories) {
		size += 4 + category.GetName().size() + 4; // category count + string size + string

		const auto& entries = category.GetEntries();
		for (auto&& entry : entries)
			size += entry.GetUsername().size() + entry.GetPassword().size() + 8; // username string + password string + 2 string sizes
	}

	return size;
}

size_t Security::ReadSize(std::stringstream& stream) {
	char buf[sizeof(size_t)];
	stream.read(buf, sizeof(size_t));
	return *(reinterpret_cast<size_t*>(buf));
}

std::string Security::ReadString(std::stringstream& stream) {
	size_t size = ReadSize(stream);
	std::unique_ptr<char[]> data(new char[size]);
	stream.read(data.get(), size);
	return std::string(data.get(), size);
}

void Security::WriteSize(std::stringstream& stream, size_t n) {
	stream.write(reinterpret_cast<char*>(&n), sizeof(size_t));
}

void Security::WriteString(std::stringstream& stream, const std::string& string) {
	WriteSize(stream, string.size());
	stream.write(string.c_str(), string.size());
}