#pragma once

#include "BackendSystem.h"

class ApplicationController {
public:
	ApplicationController() = default;

	void RequireRestart();
	bool Restart();

	void InitBackend();
	void Run();	

private:
    void CreateSettingWidget();
    void CreateMainWidget();

private:
	BackendSystem m_Backend;
	bool m_Restart = true;

	int m_SelectedRender{};
};
