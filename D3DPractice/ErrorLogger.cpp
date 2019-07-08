#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string message) {
	std::string error_msg = "Error: " + message;
	MessageBoxA(NULL, error_msg.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string message) {
	_com_error error(hr);
	std::wstring error_msg = L"Error: " + StringConverter::StringToWide(message) + L'\n' + error.ErrorMessage();
	MessageBoxW(NULL, error_msg.c_str(), L"Error", MB_ICONERROR);
}