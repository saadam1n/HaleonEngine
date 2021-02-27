// Copyright 2021 fuzdex
#include <Haleon/Kernel/Haleon.h>
#include <Haleon/IO/Log.h>
#include <Haleon/Graphics/GraphicsWindow.h>
#include <Haleon/Graphics/CoreRenderer.h>

static const float Triangle[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

int main() {
	Haleon::StreamOut.AddOutput(std::cout);
	Haleon::Initialize();
	Haleon::GraphicsWindow Window;
	Window.Open("Haleon Demo", 100, 100, 1280, 720);
	Haleon::CoreRenderer Renderer;
	Renderer.Create(&Window);
	Haleon::Frame CurrentFrame;
	while (Window.GetCurrentWindowState().Open) {
		Renderer.StartFrame(&CurrentFrame);
		Renderer.EndFrame(&CurrentFrame);
		Window.PollEvents();
	}
	Renderer.Free();
	Haleon::Terminate();
}
