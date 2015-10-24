#pragma once

#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

#include "KeyRing.h"

class ConsoleInterface
{
public:
	ConsoleInterface(CategoryManager*);
	~ConsoleInterface();

private:
	void PrintInfo();
	void ProcessCommand();
	void Clear();

	void ClearInfoString();

	void Cmd_Help();
	void Cmd_Load();
	void Cmd_Exit();
	void Cmd_Enter();
	void Cmd_View();
	void Cmd_New();
	void Cmd_Remove();
	void Cmd_ChangePassword();
	void Cmd_Write();

	void UpdateHash(const std::string&);
	bool CompareHash(const std::string&);

	std::string ReadInput();

	PasswordCategory* m_currentCategory;
	CategoryManager* m_manager;
	bool b_passwordsLoaded;
	bool b_exit;
	byte m_passwordHash[CryptoPP::SHA256::DIGESTSIZE];
	std::string m_infoString;

	const boost::filesystem::path passwordPath = "pw.db";
};

