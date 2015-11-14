#pragma once

#include <vector>
#include "PasswordEntry.h"

class PasswordManager {
public:
	PasswordManager();
	~PasswordManager();

	const std::vector<PasswordEntry>& GetEntries() const;
	PasswordEntry& GetEntry(size_t);
	void AddEntry(const std::string&, const std::string&, const std::string&);
	void RemoveEntry(size_t);

	void SortByTag();
	void SortByUsername();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version);
private:
	std::vector<PasswordEntry> m_entries;
};