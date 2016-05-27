#pragma once

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include "KeyPress.h"

class KeyRing;
class ConsoleInterface;
class PasswordManager;

namespace ConState {
	class Base {
		bool m_finished;
	protected:
		KeyRing& m_master;
		ConsoleInterface& m_interface;
		PasswordManager& m_manager;

		void SetFinished(bool);
	public:
		Base(KeyRing& _master);
		bool IsFinished() const;
		virtual void OnInputString(const std::string& input) = 0;
		virtual void OnInputSelection(int num) = 0;
		virtual void OnPressKey(const KeyPress& key) = 0;
		virtual void OnActivated() = 0;
	};

	class LoadDB : public Base {
	private:
		enum State {
			CHOOSE_FILE,
			CREATE_FILE,
			ENTER_PASSWORD,
			NEW_PASSWORD,
			NEW_PASSWORD_CONFIRM
		};

		State m_state;
		std::vector<boost::filesystem::path> m_databases;
		std::string m_databaseName;
		std::string m_response;

		bool ValidatePath(const std::string&) const;
		void SwitchState(State state);
	public:
		LoadDB(KeyRing& _master);
		virtual void OnInputString(const std::string& input);
		virtual void OnInputSelection(int num);
		virtual void OnPressKey(const KeyPress& key);
		virtual void OnActivated();
	};

	class ViewPasswords : public Base {
	public:
		ViewPasswords(KeyRing& _master);
		virtual void OnInputString(const std::string& input);
		virtual void OnInputSelection(int num);
		virtual void OnPressKey(const KeyPress& key);
		virtual void OnActivated();
	};

	class ViewPassword : public Base {
		enum State {
			VIEW,
			EDIT_TAG,
			EDIT_USERNAME,
			EDIT_PASSWORD,
		};
	public:
		ViewPassword(KeyRing& _master);
		virtual void OnInputString(const std::string& input);
		virtual void OnInputSelection(int num);
		virtual void OnPressKey(const KeyPress& key);
		virtual void OnActivated();
	};

	class NewPassword : public Base {
		enum State {
			ENTER_TAG,
			ENTER_USERNAME,
			ENTER_PASSWORD
		};
	public:
		NewPassword(KeyRing& _master);
		virtual void OnInputString(const std::string& input);
		virtual void OnInputSelection(int num);
		virtual void OnPressKey(const KeyPress& key);
		virtual void OnActivated();
	};
}