#pragma once
#include "WindowContainer.h"
class Engine : WindowContainer{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_calss, int width, int height);
	bool ProcessMessages();

	void Update();
};