#include "KeyRing.h"
#include <boost/filesystem.hpp>

using namespace boost;

ConState::LoadDB::LoadDB(KeyRing& _master) : Base(_master) {}

bool ConState::LoadDB::ValidatePath(const std::string& path) const {
	for (char c : path) {
		switch (c) {
			case '\\':
			case '/':
			case '?':
			case '%':
			case '*':
			case ':':
			case '|':
			case '"':
			case '<':
			case '>':
				return false;
			default:
				break;
		}
	}

	return true;
}

void ConState::LoadDB::SwitchState(State state) {
	ConsoleInterface& conInterface = m_master.GetInterface();
	m_state = state;

	std::string header;
	std::string footer;

	switch (state) {
		case CHOOSE_FILE:
		{
			m_databases.clear();

			for (filesystem::directory_entry& entry : filesystem::directory_iterator(filesystem::current_path())) {
				if (filesystem::is_regular_file(entry.status()) || filesystem::is_symlink(entry.status())) {
					if (entry.path().extension() == "keyring") {
						m_databases.push_back(entry.path().filename());
					}
				}
			}

			conInterface.SwitchState(VIEW_OPTION_LIST);
			header = "Select a keyring to load: ";
			footer = "enter - load keyring | n - create keyring | ESC - exit";

			std::vector<std::string> temp(m_databases.size());

			for (auto it = m_databases.begin(); it != m_databases.end(); ++it) {
				temp[it - m_databases.begin()] = it->string();
			}

			conInterface.SetOptionList(std::move(temp));

			break;
		}
		case CREATE_FILE:
		{
			header = "Choose a name for this keyring: ";
			footer = "ESC - back";
			conInterface.SetObfuscateInput(false);
			conInterface.SwitchState(INPUT_STRING);

			break;
		}
		case ENTER_PASSWORD: 
		{
			header = "Enter the password: ";
			footer = "ESC - back";
			conInterface.SetObfuscateInput(true);
			conInterface.SwitchState(INPUT_STRING);

			break;
		}
		case NEW_PASSWORD:
		{
			header = "Choose a password: ";
			footer = "ESC - back";
			conInterface.SetObfuscateInput(true);
			conInterface.SwitchState(INPUT_STRING);

			break;
		}
		case NEW_PASSWORD_CONFIRM:
		{
			header = "Confirm the password: ";
			footer = "ESC - back";
			conInterface.SetObfuscateInput(true);
			conInterface.SwitchState(INPUT_STRING);

			break;
		}
	}

	conInterface.SetHeader(header);
	conInterface.SetFooter(footer);
}

void ConState::LoadDB::OnActivated() {
	SwitchState(CHOOSE_FILE);
}

void ConState::LoadDB::OnInputSelection(int num) {
	if (m_state == CHOOSE_FILE) {
		m_databaseName = m_databases[num].string();

		SwitchState(ENTER_PASSWORD);
	}
}

void ConState::LoadDB::OnPressKey(const KeyPress& key) {
	if (key.key.virtualCode == VK_ESCAPE) {
		SwitchState(CHOOSE_FILE);
	}
}

void ConState::LoadDB::OnInputString(const std::string& input) {
	ConsoleInterface conInterface = m_master.GetInterface();

	switch (m_state) {
		case CREATE_FILE:
		{
			if (ValidatePath(input)) {

			}
			else {

			}

			break;
		}
	}
}
