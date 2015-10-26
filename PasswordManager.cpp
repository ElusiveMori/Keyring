#include <algorithm>
#include "PasswordManager.h"

PasswordManager::PasswordManager() : m_entries() {}

PasswordManager::~PasswordManager() {}

const std::vector<PasswordEntry>& PasswordManager::GetEntries() const {
	return m_entries;
}

PasswordEntry& PasswordManager::GetEntry(size_t index) {
	if (index < m_entries.size())
		return m_entries[index];
	else
		throw std::invalid_argument("Password index out of range.");
}

void PasswordManager::AddEntry(const std::string& tag, const std::string& username, const std::string& password) {
	m_entries.emplace_back(tag, username, password);
}

void PasswordManager::RemoveEntry(size_t index) {
	if (index < m_entries.size())
		m_entries.erase(m_entries.begin() + index);
	else
		throw std::invalid_argument("Password index out of range.");
}

void PasswordManager::SortByTag() {
	std::stable_sort(m_entries.begin(), m_entries.end(),
		[](const PasswordEntry& a, const PasswordEntry& b)->bool{ a.GetTag() > b.GetTag(); });
}

void PasswordManager::SortByUsername() {
	std::stable_sort(m_entries.begin(), m_entries.end(),
		[](const PasswordEntry& a, const PasswordEntry& b)->bool{ a.GetUsername() > b.GetUsername(); });
}