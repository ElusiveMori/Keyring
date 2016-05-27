#pragma once

#include "Security.h"
#include "ConsoleInterface.h"
#include "ConsoleStates.h"
#include <stack>

class KeyRing {
public:
	KeyRing();
	~KeyRing();

	void OnInputString(const std::string& input);
	void OnInputSelection(int num);
	void OnPressKey(const KeyPress& key);
	void PushState(ConState::Base*);
	void PopState();
	void CheckState();
	bool ShouldContinue() const;
	PasswordManager& GetManager();
	ConsoleInterface& GetInterface();
private:
	void ValidateState();
	ConState::Base* GetActiveState();

	PasswordManager m_manager;
	ConsoleInterface m_interface;
	std::stack<ConState::Base*> m_states;
};