#include "ConsoleInterface.h"

ConsoleInterface::ConsoleInterface() {}

ConsoleInterface::~ConsoleInterface() {}

ScreenState ConsoleInterface::GetState() const { return m_state; }

void ConsoleInterface::SetOptionList(std::vector<std::string>&& options) {
	m_options = options;
	m_selectedOption = 0;
	m_scrollOffset = 0;

	for (auto& str : m_options)
		if (str.length() > ScreenWidth)
			str.resize(ScreenWidth);

	Draw();
}

void ConsoleInterface::SetEntry(const PasswordEntry* entry) {
	m_entry = entry;
	Draw();
}

void ConsoleInterface::SwitchState(ScreenState state) {
	m_state = state;

	Draw();
}

void ConsoleInterface::SetObfuscateInput(bool b) { m_obfuscateInput = b; Draw(); }
void ConsoleInterface::SetFooter(const std::string& str) { m_footer = str; Draw(); }
void ConsoleInterface::SetHeader(const std::string& str) { m_header = str; Draw(); }

void ConsoleInterface::Draw() {
	m_screen.ClearScreenHighlight();
	int y = 0;

	// draw the header
	switch (m_state) {
		case VIEW_OPTION_LIST:
		case INPUT_STRING:
		{
			y = m_screen.WriteRow(y, m_header) + 1;
			break;
		}
		default:
			break;
	}

	// draw the footer
	m_screen.WriteRow(ScreenHeight - 1, m_footer);

	int availableRows = GetAvailableRows();

	switch (m_state) {
		case VIEW_OPTION_LIST:
		{
			for (int i = m_scrollOffset, j = 0; i < m_options.size() && j < availableRows; ++i, ++j) {
				const auto& str = m_options[i];
				m_screen.WriteRow(j + y, str);
			}

			m_screen.SetHighlightRow(m_selectedOption - m_scrollOffset + y, true);
		}
		default:
			break;
	}

	m_screen.SwapBuffers();
}

int ConsoleInterface::GetAvailableRows() {
	return ScreenHeight - 3 - (m_header.length() / ScreenWidth + 1);
}

void ConsoleInterface::InputLoop() {
	while (true) {
		KeyPress keyPress;
		m_screen.ProcessInput(keyPress);

		if (keyPress.isVirtual && keyPress.key.virtualCode == VK_ESCAPE)
			return;

		switch (m_state) {
			case INPUT_STRING:
			{
				if (keyPress.isVirtual) {
					switch (keyPress.key.virtualCode) {
						case VK_RETURN:
							// clear string and return to controlling state
							break;
						case VK_BACK:
						{
							if (m_input.size() > 0)
								m_input.pop_back();
							break;
						}
					}
				}
				else {
					if (m_input.size() < MaxInputLength)
						m_input.push_back(keyPress.key.charCode);
				}

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
							// return selection to controlling state

							break;
						}
					}
				}
				else {

				}

				break;
			}
			case VIEW_DATA:
			{
				break;
			}
		}
	
		Draw();
	}
}