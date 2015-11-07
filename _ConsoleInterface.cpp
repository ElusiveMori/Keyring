#include "ConsoleInterface.h"

#define COMMAND_MAX 256

using namespace boost;

ConsoleInterface::ConsoleInterface(CategoryManager* manager) : m_manager(manager), m_currentCategory(nullptr) {
	m_infoString += "Type help for command list.\nType load to load password file or create a new one.\n";

	while (!b_exit) {
		try {
			ProcessCommand();
		}
		catch (std::exception& e) {
			m_infoString += "ERROR: ";
			m_infoString += e.what();
			m_infoString += "\n";
		}
	}
}

ConsoleInterface::~ConsoleInterface() {}

void ConsoleInterface::PrintInfo() {
	std::cout << "Cryptographic Password Manager.\n";
	std::cout << "Written by Samuel Moriarty.\n\n";

	if (b_passwordsLoaded) {
		std::cout << "> Passwords are loaded.\n\n";
		
		if (m_currentCategory) {
			std::cout << "> Entries in category " << m_currentCategory->GetName() << ":\n";

			const auto& entries = m_currentCategory->GetEntries();
			if (entries.empty()) 
				std::cout << "\tNone.\n";
			else
				for (size_t index = 0; index < entries.size(); ++index)
					std::cout << index + 1 << ". " << entries[index].GetUsername() << '\n';
			
			std::cout << '\n';
		}
		else {
			std::cout << "> Available categories:\n";

			const auto& categories = m_manager->GetCategories();
			if (categories.empty())
				std::cout << "\tNone.\n";
			else
				for (size_t index = 0; index < categories.size(); ++index)
					std::cout << index + 1 << ". " << categories[index].GetName() << '\n';

			std::cout << "\n";
		}
	}
	else
		std::cout << "> Passwords are not loaded.\n\n";

	if (!m_infoString.empty())
		std::cout << m_infoString << "\n";

	std::cout << "Enter command > ";
}

void ConsoleInterface::ProcessCommand() {
	Clear();
	PrintInfo();
	ClearInfoString();

	if (std::cin.good()) {
		std::string command = ReadInput();

		if (command == "help")
			Cmd_Help();
		else if (command == "load")
			Cmd_Load();
		else if (command == "enter")
			Cmd_Enter();
		else if (command == "view")
			Cmd_View();
		else if (command == "new")
			Cmd_New();
		else if (command == "remove")
			Cmd_Remove();
		else if (command == "exit")
			Cmd_Exit();
		else if (command == "password")
			Cmd_ChangePassword();
		else if (command == "write")
			Cmd_Write();
		else {
			m_infoString += "This isn't a valid command.\n";
		}
	}
	else {
		b_exit = true;
	}
}

void ConsoleInterface::Clear() {
	std::system("cls");
}

void ConsoleInterface::ClearInfoString() {
	m_infoString.clear();
}

void ConsoleInterface::Cmd_Help() {
	m_infoString += "Commands available:\n";
	m_infoString += "help - print this info message\n";
	m_infoString += "load - load and decode the password file (pw.db)\n";
	m_infoString += "enter - enters a category\n";
	m_infoString += "exit - exits the current category or program\n";
	m_infoString += "view - views an entry\n";
	m_infoString += "new - creates a new category if no directory is active, otherwise creates a password entry\n";
	m_infoString += "remove - removes a category or password entry\n";
	m_infoString += "password - changes the current password\n";
	m_infoString += "write - writes current password table to password file\n";
}

void ConsoleInterface::Cmd_Exit() {
	if (m_currentCategory) {
		m_currentCategory = nullptr;
	}
	else {
		std::cout << "\nDo you want to exit the program? y/n: ";

		std::string command = ReadInput();
		if (command == "y" || command == "yes") {
			b_exit = true;
		}
	}
}

void ConsoleInterface::Cmd_Load() {
	if (b_passwordsLoaded) {
		m_infoString += "Passwords already loaded.\n";
		return;
	}

	if (filesystem::exists(passwordPath)) {
		std::cout << "Enter password: ";
		std::string password = ReadInput();

		filesystem::ifstream in("pw.db", std::ios::binary);
		if (Security::LoadCategories(in, *m_manager, password)) {
			UpdateHash(password);
			b_passwordsLoaded = true;
		}
		else
			m_infoString += "Invalid password.\n";
	}
	else {
		std::cout << "No password file. A new one will be created.\n";
		
		std::string passOne, passTwo;
		do {
			std::cout << "Choose a password: ";
			passOne = ReadInput();
			std::cout << "Re-enter password: ";
			passTwo = ReadInput();
			
			if (passOne != passTwo)
				std::cout << "Passwords do not match.\n";
		} while (passOne != passTwo);

		UpdateHash(passOne);
		b_passwordsLoaded = true;

		m_infoString += "New password has been set.\n";
	}
}

void ConsoleInterface::Cmd_Enter() {
	if (b_passwordsLoaded) {
		if (!m_currentCategory) {
			std::cout << "Enter category #";
			std::string number = ReadInput();
			int i = std::stoi(number);

			m_currentCategory = &(m_manager->GetCategory(i - 1));
		}
		else
			m_infoString += "Exit the current directory first.\n";
	}
	else
		m_infoString += "Load passwords first.\n";
}

void ConsoleInterface::Cmd_View() {
	if (b_passwordsLoaded) {
		if (m_currentCategory) {
			std::cout << "View entry #";
			std::string number = ReadInput();
			int i = std::stoi(number);

			m_infoString += "Username: " + m_currentCategory->GetEntry(i - 1).GetUsername() + "\n";
			m_infoString += "Password: " + m_currentCategory->GetEntry(i - 1).GetPassword() + "\n";
		}
		else {
			m_infoString += "Enter a category first.\n";
		}
	} else
		m_infoString += "Load passwords first.\n";
}

void ConsoleInterface::Cmd_New() {
	if (b_passwordsLoaded) {
		if (m_currentCategory) {
			std::cout << "Enter entry username: ";
			std::string username = ReadInput();
			std::cout << "Enter entry password: ";
			std::string password = ReadInput();

			m_currentCategory->AddEntry(username, password);
		}
		else {
			std::cout << "Enter category name: ";
			std::string name = ReadInput();

			m_manager->AddCategory(name);
		}
	} else
		m_infoString += "Load passwords first.\n";
}

void ConsoleInterface::Cmd_Remove() {
	if (b_passwordsLoaded) {
		if (m_currentCategory) {
			std::cout << "Remove entry #";
			std::string number = ReadInput();
			int i = std::stoi(number);

			m_currentCategory->RemoveEntry(i - 1);
		}
		else {
			std::cout << "Remove category #";
			std::string number = ReadInput();
			int i = std::stoi(number);

			m_manager->RemoveCategory(i - 1);
		}
	} else
		m_infoString += "Load passwords first.\n";
}

void ConsoleInterface::Cmd_ChangePassword() {
	if (b_passwordsLoaded) {
		std::cout << "Enter old password: ";
		std::string oldPassword = ReadInput();

		if (CompareHash(oldPassword)) {
			std::cout << "Enter new password: ";
			std::string password = ReadInput();

			std::cout << "Re-enter password: ";

			if (password == ReadInput()) {
				filesystem::ofstream out("pw.db", std::ios::binary);
				
				Security::SaveCategories(out, *m_manager, password);
				
				UpdateHash(password);

				m_infoString += "Password updated.\nPassword table rewritten.\n";
			}
			else
				m_infoString += "Passwords mismatched.\n";
		}
		else
			m_infoString += "Invalid password.\n";
	}
	else
		m_infoString += "Load passwords first.\n";
}

void ConsoleInterface::Cmd_Write() {
	if (b_passwordsLoaded) {
		std::cout << "Enter password to save password table: ";
		std::string password = ReadInput();

		if (CompareHash(password)) {	
			filesystem::ofstream out("pw.db", std::ios::binary);

			Security::SaveCategories(out, *m_manager, password);

			m_infoString += "Passwords table written.\n";
		}
		else
			m_infoString += "Invalid password.\n";
	}
	else
		m_infoString += "Load passwords first.\n";
}

void ConsoleInterface::UpdateHash(const std::string& password) {
	CryptoPP::SHA256().CalculateDigest(m_passwordHash, reinterpret_cast<const byte*>(password.data()), password.size());
}

bool ConsoleInterface::CompareHash(const std::string& password) {
	return CryptoPP::SHA256().VerifyDigest(m_passwordHash, reinterpret_cast<const byte*>(password.data()), password.size());
}

std::string ConsoleInterface::ReadInput() {
	char cmd[COMMAND_MAX];

	std::cin.getline(cmd, COMMAND_MAX, '\n');

	return std::string(cmd);
}