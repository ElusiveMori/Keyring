#include "Security.h"

using namespace CryptoPP;
using namespace boost;

bool Security::LoadManager(std::istream& in, PasswordManager& manager, const std::string& password) {
	if (in.good()) {
		byte iv[AES::BLOCKSIZE];
		byte salt[AES::MAX_KEYLENGTH];
		byte key[AES::MAX_KEYLENGTH];
		byte hash[SHA256::DIGESTSIZE];

		std::stringstream decoded;
		FileSource(in, true, new Base64Decoder(new FileSink(decoded)));

		decoded.read(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);
		decoded.read(reinterpret_cast<char*>(salt), AES::MAX_KEYLENGTH);
		decoded.read(reinterpret_cast<char*>(hash), SHA256::DIGESTSIZE);

		DeriveKey(password, salt, AES::MAX_KEYLENGTH, key, AES::MAX_KEYLENGTH);

		auto start = decoded.tellg();
		auto end = decoded.seekg(0, decoded.end).tellg();

		if (start < 0 || end < 0)
			throw std::runtime_error("Unknown stream error.");

		size_t size = static_cast<size_t>(abs(end - start));
		decoded.seekg(start);

		std::unique_ptr<char[]> cipheredData(new char[size]);
		std::unique_ptr<char[]> decipheredData(new char[size]);
		
		decoded.read(cipheredData.get(), size);

		DecipherData(reinterpret_cast<byte*>(cipheredData.get()), reinterpret_cast<byte*>(decipheredData.get()), size, key, AES::MAX_KEYLENGTH, iv, AES::BLOCKSIZE);
		
		if (CompareHash(hash, reinterpret_cast<byte*>(decipheredData.get()), size)) {
			std::stringstream stream;
			stream.write(decipheredData.get(), size);
			boost::archive::text_iarchive ar(stream);
			ar & manager;

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

void Security::SaveManager(std::ostream& out, const PasswordManager& manager, const std::string& password) {
	if (out.good()) {
		byte iv[AES::BLOCKSIZE];
		byte salt[AES::MAX_KEYLENGTH];
		byte key[AES::MAX_KEYLENGTH];
		byte hash[SHA256::DIGESTSIZE];

		AutoSeededRandomPool random;

		random.GenerateBlock(iv, AES::BLOCKSIZE);
		random.GenerateBlock(salt, AES::MAX_KEYLENGTH);
		DeriveKey(password, salt, AES::MAX_KEYLENGTH, key, AES::MAX_KEYLENGTH);

		Base64Encoder encoder(new FileSink(out), false);

		std::stringstream stream;
		boost::archive::text_oarchive archive(stream);
		archive & manager;

		auto size = stream.str().size();
		SHA256().CalculateDigest(hash, reinterpret_cast<const byte*>(stream.str().c_str()), size);
		std::unique_ptr<char[]> cipheredData(new char[size]);
		EncipherData(reinterpret_cast<const byte*>(stream.str().c_str()), reinterpret_cast<byte*>(cipheredData.get()), size, key, AES::MAX_KEYLENGTH, iv, AES::BLOCKSIZE);

		encoder.Put(iv, AES::BLOCKSIZE);
		encoder.Put(salt, AES::MAX_KEYLENGTH);
		encoder.Put(hash, SHA256::DIGESTSIZE);
		encoder.Put(reinterpret_cast<byte*>(cipheredData.get()), size);
		encoder.MessageEnd();
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

void Security::DecipherData(const byte* ciphered, byte* deciphered, size_t size, const byte* key, size_t keySize, const byte* iv, size_t ivSize) {
	CTR_Mode<AES>::Decryption decryptor;
	decryptor.SetKeyWithIV(key, keySize, iv, ivSize);
	decryptor.ProcessData(deciphered, ciphered, size);
}

void Security::EncipherData(const byte* deciphered, byte* ciphered, size_t size, const byte* key, size_t keySize, const byte* iv, size_t ivSize) {
	CTR_Mode<AES>::Encryption encryptor;
	encryptor.SetKeyWithIV(key, keySize, iv, ivSize);
	encryptor.ProcessData(ciphered, deciphered, size);
}

bool Security::CompareHash(const byte* hash, const byte* data, size_t size) {
	return SHA256().VerifyDigest(hash, data, size);
}