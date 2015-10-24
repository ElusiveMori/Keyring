#pragma once

#include <string>
#include <iostream>

class PasswordEntry
{
public:
	PasswordEntry(const std::string&, const std::string&);
	~PasswordEntry();

	const std::string& GetUsername() const;
	const std::string& GetPassword() const;
private:
	
	std::string m_username;
	std::string m_password;
};

