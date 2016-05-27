#include "KeyRing.h"
#include "ConsoleInterface.h"

ConsoleInterface::ConsoleInterface(KeyRing& keyring) : m_keyring(keyring) {}

ConsoleInterface::~ConsoleInterface() {}

ScreenState ConsoleInterface::GetState() const { return m_state; }

void ConsoleInterface::SetOptionList(std::vector<std::string>&& options) {
	m_options = options;
	m_selectedOption = 0;
	m_scrollOffset = 0;

	for (auto& str : m_options)
		if (str.length() > SCREEN_WIDTH)
			str.resize(SCREEN_WIDTH);
}

void ConsoleInterface::SetEntry(const PasswordEntry* entry) { m_entry = entry; }
void ConsoleInterface::SwitchState(ScreenState state)       { m_state = state; }
void ConsoleInterface::SetObfuscateInput(bool b)            { m_obfuscateInput = b; }
void ConsoleInterface::SetFooter(const std::string& str)    { m_footer = str; }
void ConsoleInterface::SetHeader(const std::string& str)    { m_header = str; }

void ConsoleInterface::Draw() {
	m_screen.ClearScreen();
	int y = 0;

	// draw the header
	switch (m_state) {
		case VIEW_OPTION_LIST:
		case INPUT_STRING:
		{
			y = m_screen.WriteRow(y, m_header);
			break;
		}
		default:
			break;
	}

	// draw the footer
	m_screen.WriteRow(SCREEN_HEIGHT - 1, m_footer);

	int availableRows = GetAvailableRows();

	switch (m_state) {
		case VIEW_OPTION_LIST:
		{
			for (int i = m_scrollOffset, j = 0; i < m_options.size() && j < availableRows; ++i, ++j) {
				const auto& str = m_options[i];
				m_screen.WriteRow(j + y + 1, str);
			}

			m_screen.SetHighlightRow(m_selectedOption - m_scrollOffset + y + 1, true);
			int scrollSegments = m_options.size() - availableRows;

			if (scrollSegments > 0) {
				for (int i = y; i < availableRows + y + 1; i++)
					m_screen.WriteCell(SCREEN_WIDTH - 1, i, '|');

				int scrollbarPos = floor((float)(availableRows - 1) * ((float)m_scrollOffset / (float)scrollSegments));
				m_screen.WriteCell(SCREEN_WIDTH - 1, y + 1 + scrollbarPos, '*');
			}

			break;
		}
		case INPUT_STRING:
		{
			if (m_obfuscateInput)
				m_screen.WriteRow(y, std::string(m_input.size(), '*').insert(0, 2, ' '));
			else
				m_screen.WriteRow(y, std::string(m_input).insert(0, 2, ' '));

			break;
		}
		case VIEW_DATA:
		{
			if (m_entry) {
				y += m_screen.WriteRow(y, m_entry->GetTag());
				y += 1 + m_screen.WriteRow(y + 1, "Username: " + m_entry->GetUsername());
				y += m_screen.WriteRow(y, "Password: " + std::string(m_entry->GetPassword().size(), '*'));
			}

			break;
		}
	}

	m_screen.SwapBuffers();
}

int ConsoleInterface::GetAvailableRows() {
	return SCREEN_HEIGHT - 3 - (m_header.length() / SCREEN_WIDTH + 1);
}

void ConsoleInterface::InputLoop() {
	while (true) {
		KeyPress keyPress;
		m_screen.ProcessInput(keyPress);

		switch (m_state) {
			case INPUT_STRING:
			{
				if (keyPress.isVirtual) {
					switch (keyPress.key.virtualCode) {
						case VK_RETURN:
						{
							m_keyring.OnInputString(m_input);
							m_input.clear();

							break;
						}
						case VK_BACK:
						{
							if (m_input.size() > 0)
								m_input.pop_back();

							break;
						}
					}
				}
				else if (m_input.size() < MAX_INPUT_LENGTH)
					m_input.push_back(keyPress.key.charCode);
				
				break;
			}
			case VIEW_OPTION_LIST:
			{
				if (keyPress.isVirtual) {
					switch (keyPress.key.virtualCode) {
						case VK_UP:
						{
							if (m_selectedOption > 0) {
								if (m_selectedOption == m_scrollOffset)
									m_scrollOffset--;

								m_selectedOption--;
							}

							break;
						}						
						case VK_DOWN:
						{
							if (m_selectedOption < m_options.size() - 1) {
								if (m_selectedOption == (m_scrollOffset + GetAvailableRows() - 1))
									m_scrollOffset++;

								m_selectedOption++;
							}

							break;
						}
						case VK_RETURN:
						{
							m_keyring.OnInputSelection(m_selectedOption);

							break;
						}
					}
				}
				else m_keyring.OnPressKey(keyPress);

				break;
			}
			case VIEW_DATA:
			{
				m_keyring.OnPressKey(keyPress);

				break;
			}
		}
	
		m_keyring.CheckState();
		
		if (m_keyring.ShouldContinue()) {
			Draw();
		}
		else {
			return;
		}
	}
}