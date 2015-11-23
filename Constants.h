#pragma once

enum ScreenState {
	VIEW_OPTION_LIST,
	VIEW_DATA,
	INPUT_STRING,
};

enum OptionType {
	OPTION_YES,
	OPTION_NO,
	OPTION_ENTRY,
};

const int MaxInputLength = 64;
const int ScreenWidth = 80;
const int ScreenHeight = 25;