#include "PasswordEntry.h"

PasswordEntry::PasswordEntry(const std::string& tag, const std::string& username, const std::string& password)
							:  m_tag(tag), m_username(username), m_password(password) {}
PasswordEntry::PasswordEntry() {}
PasswordEntry::~PasswordEntry() {}

const std::string& PasswordEntry::GetUsername() const {
	return m_username;
}

const std::string& PasswordEntry::GetPassword() const {
	return m_password;
}

const std::string& PasswordEntry::GetTag() const {
	return m_tag;
}

void PasswordEntry::SetUsername(const std::string& username) {
	m_username = username;
}

void PasswordEntry::SetPassword(const std::string& password) {
	m_password = password;
}

void PasswordEntry::SetTag(const std::string& tag) {
	m_tag = tag;
}

template<class Archive>
void PasswordEntry::serialize(Archive & ar, const unsigned int version) {
	ar & m_tag & m_username & m_password;
}

template void PasswordEntry::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, const unsigned int);
template void PasswordEntry::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, const unsigned int);