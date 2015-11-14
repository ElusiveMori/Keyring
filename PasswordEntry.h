#pragma once

#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

class PasswordEntry {
public:
	PasswordEntry(const std::string&, const std::string&, const std::string&);
	PasswordEntry();
	~PasswordEntry();

	const std::string& GetUsername() const;
	const std::string& GetPassword() const;
	const std::string& GetTag() const;

	void SetUsername(const std::string&);
	void SetPassword(const std::string&);
	void SetTag(const std::string&);

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version);

private:
	
	std::string m_tag;
	std::string m_username;
	std::string m_password;
};

