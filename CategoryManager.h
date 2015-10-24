#pragma once

#include <vector>
#include <string>

#include "PasswordCategory.h"

class CategoryManager
{
public:
	CategoryManager();
	~CategoryManager();

	const std::vector<PasswordCategory>& GetCategories() const;
	PasswordCategory& GetCategory(size_t);

	size_t AddCategory(const std::string&);
	void RemoveCategory(size_t);
private:
	std::vector<PasswordCategory> m_categories;
};

