#include "GamePCH.h"

#include "GameSettings.h"
#include "Application/Application.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define APP_Sandbox

int Game()
{
	// Startup
	auto *app = new Application;

	// Scene Creation
	app->SetScene(std::make_shared<Scene>(std::filesystem::current_path().string() + R"(\save\world.glc)"));
	LOG_TRACE("Scene Created");

	// Runtime
	int result = app->Run();
	LOG_TRACE("RUN");

	// Shutdown
	delete app;

	return result;
}

int Sandbox()
{
	ma_result result;
	ma_engine engine;

	const std::string audioSrc = Settings::ProjectPath + R"(\resources\audio\test.wav)";

	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS)
	{
		return -1;
	}

	ma_engine_play_sound(&engine, audioSrc.c_str(), NULL);

	printf("Press Enter to quit...");
	getchar();

	ma_engine_uninit(&engine);

	return 0;
}

int main(int argc, char **argv)
{
	return Game();
	//return Sandbox();
}