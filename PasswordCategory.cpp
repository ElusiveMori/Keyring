#include "PasswordCategory.h"
#include <cstdint>

PasswordCategory::PasswordCategory(const std::string& name) : m_name(name), m_entries() {}
PasswordCategory::~PasswordCategory() {}

const std::string& PasswordCategory::GetName() const {
	return m_name;
}

void PasswordCategory::AddEntry(const std::string& username, const std::string& password) {
	std::unique_ptr<PasswordEntry> entry(new PasswordEntry(username, password));

	m_entries.emplace_back(username, password);
}

void PasswordCategory::RemoveEntry(size_t index) {
	if (index < m_entries.size())
		m_entries.erase(m_entries.begin() + index);
	else
		throw std::invalid_argument("Entry index out of range.");
}

const PasswordEntry& PasswordCategory::GetEntry(size_t index) const {
	if (index < m_entries.size())
		return m_entries[index];
	else
		throw std::invalid_argument("Entry index out of range.");
}

const std::vector<PasswordEntry>& PasswordCategory::GetEntries() const {
	return m_entries;
}