#pragma once

class GUI
{
	static GUI *instancePtr;

public:
	GUI();
	~GUI();

	static GUI &instance() { return *instancePtr; }

	void BeginFrame();
	void EndFrame();
};