#include "ConsoleControls.h"

ConsoleControls::ConsoleControls() : m_screen(*this) {
	m_screen.SetFooter("test");
	m_screen.SetHeader("test");
	m_screen.Draw();
}


ConsoleControls::~ConsoleControls() {}
