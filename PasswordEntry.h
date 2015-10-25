#pragma once

#include <string>

class PasswordEntry
{
public:
	PasswordEntry(const std::string&, const std::string&, const std::string&);
	~PasswordEntry();

	const std::string& GetUsername() const;
	const std::string& GetPassword() const;
	const std::string& GetTag() const;

	void SetUsername(const std::string&);
	void SetPassword(const std::string&);
	void SetTag(const std::string&);
private:
	
	std::string m_tag;
	std::string m_username;
	std::string m_password;
};

