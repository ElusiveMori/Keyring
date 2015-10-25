#pragma once

#include <vector>
#include "PasswordEntry.h"

class PasswordManager
{
	PasswordManager();
	~PasswordManager();

	const std::vector<PasswordEntry>& GetEntries() const;
	PasswordEntry& GetEntry(size_t);
	void AddEntry(const std::string&, const std::string&, const std::string&);
	void RemoveEntry(size_t);

	void SortByTag();
	void SortByUsername();
private:
	std::vector<PasswordEntry> m_entries;
};