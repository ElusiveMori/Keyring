#include "Security.h"
#include "StreamUtils.h"

using namespace CryptoPP;
using namespace boost;

bool Security::LoadCategories(std::istream& in, PasswordManager& manager, const std::string& password) {
	if (in.good()) {
		byte iv[AES::BLOCKSIZE];
		byte salt[AES::MAX_KEYLENGTH];
		byte key[AES::MAX_KEYLENGTH];
		byte hash[SHA256::DIGESTSIZE];

		in.read(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);
		in.read(reinterpret_cast<char*>(salt), AES::MAX_KEYLENGTH);
		in.read(reinterpret_cast<char*>(hash), SHA256::DIGESTSIZE);

		DeriveKey(password, salt, AES::MAX_KEYLENGTH, key, AES::MAX_KEYLENGTH);

		auto start = in.tellg();
		auto end = in.seekg(0, in.end).tellg();

		if (start < 0 || end < 0)
			throw std::runtime_error("Unknown stream error.");

		size_t size = static_cast<size_t>(abs(end - start));
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
		throw std::runtime_error("Stream is in a bad state.");
	}
}

void Security::SaveCategories(std::ostream& out, const PasswordManager& manager, const std::string& password) {
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

void Security::Deserialize(PasswordManager& manager, std::istream& stream) {
	uint16_t nEntries = ReadType<uint16_t>(stream);

	for (uint16_t i = 0; i < nEntries; ++i) {
		std::string entryTag = ReadType<std::string>(stream);
		std::string entryUsername = ReadType<std::string>(stream);
		std::string entryPassword = ReadType<std::string>(stream);

		manager.AddEntry(entryTag, entryUsername, entryPassword);
	}
}

void Security::Serialize(const PasswordManager& manager, std::ostream& stream) {
	const auto& entries = manager.GetEntries();

	if (entries.size() >= std::numeric_limits<uint16_t>::max())
		throw std::runtime_error("Max entry limit reached. Cannot save.");

	WriteType(stream, static_cast<uint16_t>(entries.size()));

	for (auto& entry = entries.begin(); entry != entries.end(); ++entry) {
		WriteType(stream, entry->GetTag());
		WriteType(stream, entry->GetUsername());
		WriteType(stream, entry->GetPassword());
	}
}

size_t Security::GetSerializedSize(const PasswordManager& manager) {
	size_t size = 4; // entry count
	const auto& entries = manager.GetEntries();

	for (auto& entry : entries)
		size += entry.GetTag().size() + entry.GetUsername().size() + entry.GetPassword().size() + 12; // username string + password string + 3 string sizes

	return size;
}