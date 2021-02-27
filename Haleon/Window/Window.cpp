#include "Window.h"
#include "../IO/Log.h"
// Standard C++ Library
#include <vector>
#include <iostream>
#include <string>

namespace Haleon {

	constexpr const int DefaultWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	void BaseWindow::Open(const WindowCreateParameters& Params) {
		using namespace std::literals;
		Window = SDL_CreateWindow(Params.Title.c_str(), Params.Position.x, Params.Position.y, Params.Size.x, Params.Size.y, DefaultWindowFlags | GetAPIFlags());
		DebugLog1(Assertion(Window, "Unable to create window. SDL2 error: "s + SDL_GetError(), "Window succesfully created") << LogFlush);
		WindowID = SDL_GetWindowID(Window);
		DebugLog2(COFFEE_SEVERITY_INFO << "New window ID is: " << WindowID << LogFlush);
		CurrentState.Open = true;
		CreateGraphics();
	}

	void BaseWindow::Open(const std::string& title, int x, int y, int w, int h) {
		WindowCreateParameters Params;
		Params.Title = title;
		Params.Position = glm::ivec2(x, y);
		Params.Size = glm::ivec2(w, h);
		Params.Fullscreen = COFFEE_FULLSCREEN_MODE_WINDOWED;
		Open(Params);
	}

	void BaseWindow::Close(void) {
		DestroyGraphics();
		SDL_DestroyWindow(Window);
	}

	void BaseWindow::PollEvents(void) {
		SDL_PumpEvents();

		std::vector<SDL_Event> Events;
		int Size = SDL_PeepEvents(nullptr, SDL_LASTEVENT, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
		if (Size < 1) {
			return;
		}
		Events.resize(Size);

		SDL_PeepEvents(Events.data(), Size, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

		std::vector<SDL_Event> ProcessibleEvents;
		std::vector<SDL_Event> UnprocessibleEvents;
		ProcessibleEvents.reserve(Size);
		UnprocessibleEvents.reserve(Size);
		for (auto Itr = Events.begin(); Itr != Events.end(); Itr++) {
			if (ShouldProcessEvent(*Itr)) {
				ProcessibleEvents.push_back(*Itr);
			} else {
				UnprocessibleEvents.push_back(*Itr);
			}
		}
		SDL_PeepEvents(UnprocessibleEvents.data(), (int)UnprocessibleEvents.size(), SDL_ADDEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
		for (const SDL_Event& Event : ProcessibleEvents) {
			ProcessEvent(Event);
		}
	}

	bool BaseWindow::ShouldProcessEvent(const SDL_Event& Event) {
		return Event.window.windowID == WindowID;
	}

	void BaseWindow::ProcessEvent(const SDL_Event& Event) {
		// At the moment I only want to process window events
		if (Event.type == SDL_WINDOWEVENT) {
			switch (Event.window.event) {
			case SDL_WINDOWEVENT_SHOWN: {
				DebugLog3(COFFEE_SEVERITY_INFO << "Window " << WindowID << " has been shown" << LogFlush);
			} break;
			case SDL_WINDOWEVENT_EXPOSED: {
				DebugLog4(COFFEE_SEVERITY_INFO << "Window " << WindowID << " needs to be redrawn");
			} break;
			case SDL_WINDOWEVENT_CLOSE: {
				CurrentState.Open = false;
				DebugLog2(COFFEE_SEVERITY_INFO << "Window " << WindowID <<  " has been closed" << LogFlush);
			} break;
			case SDL_WINDOWEVENT_MOVED: {
				CurrentState.Position.x = Event.window.data1;
				CurrentState.Position.y = Event.window.data2;
				DebugLog4(COFFEE_SEVERITY_INFO << "Window " << WindowID << " has been moved to: " << CurrentState.Position << LogFlush);
			} break;
			case SDL_WINDOWEVENT_RESIZED: {
				CurrentState.Size.x = Event.window.data1;
				CurrentState.Size.y = Event.window.data2;
				DebugLog2(COFFEE_SEVERITY_INFO << "Window " << WindowID << " has been resized to: " << CurrentState.Size << LogFlush);
			} break;
			case SDL_WINDOWEVENT_SIZE_CHANGED: {
				CurrentState.Size.x = Event.window.data1;
				CurrentState.Size.y = Event.window.data2;
				DebugLog4(COFFEE_SEVERITY_INFO << "The size of window " << WindowID << " has been changed");
			} break;
			case SDL_WINDOWEVENT_MAXIMIZED: {
				DebugLog4(COFFEE_SEVERITY_INFO << "Window " << WindowID << " has been maximized");
			} break;
			case SDL_WINDOWEVENT_MINIMIZED: {
				DebugLog4(COFFEE_SEVERITY_INFO << "Window " << WindowID << " has been minimized");
			} break;
			case SDL_WINDOWEVENT_RESTORED: {
				DebugLog4(COFFEE_SEVERITY_INFO << "Window " << WindowID << " has been restored");
			} break;
			case SDL_WINDOWEVENT_ENTER: {
				CurrentState.Mouse = true;
			} break;
			case SDL_WINDOWEVENT_LEAVE: {
				CurrentState.Mouse = false;
			} break;
			case SDL_WINDOWEVENT_FOCUS_GAINED: {
				CurrentState.Keyboard = true;
			} break;
			case SDL_WINDOWEVENT_FOCUS_LOST: {
				CurrentState.Keyboard = false;
			} break;
			default: {
				DebugLog1(COFFEE_SEVERITY_WARN << "Unknown window event type sent to window " << WindowID << ": " << Event.window.event << LogFlush);
			} break;
			}
		}
		// Disptach the event to all subsribers
		for (EventSubscriber& Subscriber : Subscribers) {
			Subscriber.DisptachEvent(Event);
		}
	}

	void BaseWindow::AddSubscriber(const EventSubscriber& listener) {
		Subscribers.push_back(listener);
	}

	const WindowState& BaseWindow::GetCurrentWindowState(void) {
		return CurrentState;
	}

	void EventSubscriber::SetFunction(EventDispatchFunction function) {
		SubscriberDispatch = function;
	}

	void EventSubscriber::SetData(void* ptr) {
		SubscriberData = ptr;
	}

	void EventSubscriber::DisptachEvent(const SDL_Event& event) {
		SubscriberDispatch(event, SubscriberData);
	}

}