// Copyright 2021 fuzdex
#ifndef COFFEE_WINDOW_H
#define COFFEE_WINDOW_H

// Haleon
#include "../Kernel/Haleon.h"
#include "../Kernel/Object.h"
// Simple DirectMedia Layer
#include <SDL_video.h>
#include <SDL_events.h>
// OpenGL Mathematics
#include <glm/vec2.hpp>
// Standard C++ Library
#include <string>
#include <functional>
#include <vector>

namespace Haleon {

	enum FullscreenMode {
		COFFEE_FULLSCREEN_MODE_WINDOWED,
		COFFEE_FULLSCREEN_MODE_FULLSCREEN_WINDOWED,
		COFFEE_FULLSCREEN_MODE_FULLSCREEN
	};

	enum WindowGraphicsAPI {
		COFFEE_WINDOW_GRAPHICS_API_OPENGL = SDL_WINDOW_OPENGL
	};

	struct WindowCreateParameters : public BaseObject {
		std::string Title;
		glm::ivec2 Position;
		glm::ivec2 Size;
		FullscreenMode Fullscreen;
	};

	struct WindowState : public BaseObject {
		typedef bool WindowFocus;
		bool Open;
		WindowFocus Mouse;
		WindowFocus Keyboard;
		glm::ivec2 Position;
		glm::ivec2 Size;
	};

	struct EventSubscriber : public BaseObject {
	public:
		typedef std::function<void(const SDL_Event&, void*)> EventDispatchFunction;
		void SetFunction(EventDispatchFunction function);
		void SetData(void* ptr);
		void DisptachEvent(const SDL_Event& event);
	private:
		EventDispatchFunction SubscriberDispatch;
		void* SubscriberData;
	};

	class BaseWindow : public BaseObject {
	public:
		void Open(const WindowCreateParameters& params);
		void Open(const std::string& title, int x, int y, int w, int h);
		void Close(void);
		void PollEvents(void);
		const WindowState& GetCurrentWindowState(void);
		void AddSubscriber(const EventSubscriber& listener);
	protected:
		bool ShouldProcessEvent(const SDL_Event& Event);
		void ProcessEvent(const SDL_Event& Event);
		// I wish there was a better solution to this instead of using virtual methods
		virtual uint32_t GetAPIFlags(void) = 0;
		virtual void CreateGraphics(void) = 0;
		virtual void DestroyGraphics(void) = 0;
		SDL_Window* Window;
		uint32_t WindowID;
		WindowState CurrentState;
		std::vector<EventSubscriber> Subscribers;
	};

}

#endif