#pragma once

#include "CategoryManager.h"
#include "Security.h"

class KeyRing
{
public:
	KeyRing();
	~KeyRing();

private:
	CategoryManager m_manager;
};