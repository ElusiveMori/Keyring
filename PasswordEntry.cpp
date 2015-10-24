#include "PasswordEntry.h"

PasswordEntry::PasswordEntry(const std::string& username, const std::string& password) : m_username(username), m_password(password) {}
PasswordEntry::~PasswordEntry() {}

const std::string& PasswordEntry::GetUsername() const {
	return m_username;
}

const std::string& PasswordEntry::GetPassword() const {
	return m_password;
}