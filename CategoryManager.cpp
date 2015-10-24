#include "CategoryManager.h"

CategoryManager::CategoryManager() : m_categories() {}

CategoryManager::~CategoryManager() {}

const std::vector<PasswordCategory>& CategoryManager::GetCategories() const {
	return m_categories;
}

PasswordCategory& CategoryManager::GetCategory(size_t index) {
	if (index < m_categories.size())
		return m_categories[index];
	else
		throw std::invalid_argument("Category index out of range.");
}

size_t CategoryManager::AddCategory(const std::string& name) {
	m_categories.emplace_back(name);
	
	return m_categories.size() - 1;
}

void CategoryManager::RemoveCategory(size_t index) {
	if (index < m_categories.size())
		m_categories.erase(m_categories.begin() + index);
	else
		throw std::invalid_argument("Category index out of range.");
}