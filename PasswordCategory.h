#pragma once

#include "PasswordEntry.h"

#include <vector>
#include <string>
#include <memory>

class PasswordCategory
{
public:
	PasswordCategory(const std::string&);
	~PasswordCategory();

	const std::string& GetName() const;

	void AddEntry(const std::string&, const std::string&);
	void RemoveEntry(size_t index);

	const PasswordEntry& GetEntry(size_t) const;
	const std::vector<PasswordEntry>& GetEntries() const;
private:
	
	std::string m_name;
	std::vector<PasswordEntry> m_entries;
};

