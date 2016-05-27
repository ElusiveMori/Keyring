#include "KeyRing.h"
#include <iostream>
#include <cstdint>

KeyRing::KeyRing() : m_interface(*this)
{
}

KeyRing::~KeyRing()
{
}

void KeyRing::ValidateState() {
	if (m_states.empty())
		throw std::runtime_error("No active state.");
}

void KeyRing::CheckState() {
	ValidateState();

	if (m_states.top()->IsFinished()) {
		PopState();
	}
}

bool KeyRing::ShouldContinue() const {
	if (m_states.empty())
		return false;
	else
		return true;
}

PasswordManager& KeyRing::GetManager() { return m_manager; };
ConsoleInterface& KeyRing::GetInterface() { return m_interface; }

ConState::Base* KeyRing::GetActiveState() {
	if (m_states.empty())
		return nullptr;

	return m_states.top();
}

void KeyRing::OnInputString(const std::string& input) {
	ValidateState();
	m_states.top()->OnInputString(input);
	CheckState();
}

void KeyRing::OnInputSelection(int num) {
	ValidateState();
	m_states.top()->OnInputSelection(num);
	CheckState();
}

void KeyRing::OnPressKey(const KeyPress& key) {
	ValidateState();
	m_states.top()->OnPressKey(key);
	CheckState();
}

void KeyRing::PushState(ConState::Base* state) {
	m_states.push(state);

	state->OnActivated();
	CheckState();
}

void KeyRing::PopState() {
	ValidateState();
	ConState::Base* state = m_states.top();
	m_states.pop();

	delete state;
}