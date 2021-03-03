#include "PhotonCoreRenderer.h"
#include <GL/glew.h>
#include <iostream>

/*
0x11103ACED60C582
0x1110291DE2BF582
*/

/*
Presenting the CPU-side back buffer to the screen is based on OpenGL 1.1 Red Book chapters 7 and 9
https://www.glprogramming.com/red/chapter07.html
https://www.glprogramming.com/red/chapter09.html
*/

namespace Haleon {

	BackBuffer::BackBuffer(void) : Data(nullptr), Width(0), Height(0) {}

	void BackBuffer::Create(uint32_t x, uint32_t y) {
		Width = x;
		Height = y;
		size_t PixelCount = (size_t)(Width * Height);
		Data = new ColorRGBA[PixelCount];
	}

	// Looks better than having the cast in the actual code
	void BackBuffer::Create(int32_t x, int32_t y) {
		uint32_t U32X = (uint32_t)x;
		uint32_t U32Y = (uint32_t)y;
		Create(U32X, U32Y);
	}

	void BackBuffer::Free(void) {
		delete[] Data;
	}

	ColorRGBA BackBuffer::Sample(uint32_t x, uint32_t y) {
		return Data[GetIndex(x, y)];
	}

	void BackBuffer::Write(uint32_t x, uint32_t y, ColorRGBA Color) {
		Data[GetIndex(x, y)] = Color;
	}

	void BackBuffer::Write(uint32_t x, uint32_t y, unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
		Write(x, y, { R, G, B, A });
	}

	uint64_t BackBuffer::GetIndex(uint32_t X, uint32_t Y) {
		uint32_t U32 = Width * Y + X;
		return (uint64_t)U32;
	}

	void CoreRenderer::Create(GraphicsWindow* Window) {
		WindowContext = Window;

		// Create framebuffer
		BackBuffer.Create(Window->CurrentState.Size.x, Window->CurrentState.Size.y);

		// Enable texturing and create the texture
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &ScreenTexture);
		glBindTexture(GL_TEXTURE_2D, ScreenTexture);
		// Preallocate memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BackBuffer.Width, BackBuffer.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		// Set settings of the texture
		const static float BorderColor[] = {0.5f, 0.5f, 0.5f, 0.5f};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor);
		// GL_CLAMP is the legacy version of GL_CLAMP_TO_BORDER. Using GL_CLAMP_TO_BORDER breaks the second triangle, so then you would have to use GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		// Create the display list that is used for rendering the fullscreen textured quad.
		PresentList = glGenLists(1);
		glNewList(PresentList, GL_COMPILE); {
			// I could use GL_QUADS and only 4 verticies but quads are much slower than triangles and I don't want to rely on the driver to turn the quad into triangles
			glBegin(GL_TRIANGLES);

			glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);

			glVertex2f(1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f);

			glVertex2f(1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f);

			glVertex2f(1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f);

			glVertex2f(-1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f);

			glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);

			glEnd();
		} glEndList();

		// Set up basic construction stuff
		CurrentFrame.ID = 0;
		CurrentFrame.Success = false;
	}
	
	void CoreRenderer::Free(void) {
		glDeleteTextures(1, &ScreenTexture);
		glDisable(GL_TEXTURE_2D);
		BackBuffer.Free();
	}

	void CoreRenderer::StartFrame(Frame* NewFrame) {
		ClearBuffer();

		CurrentFrame.ID++;
		*NewFrame = CurrentFrame;
	}

	void CoreRenderer::EndFrame(Frame* CurrentFrame) {
		SwapBuffers();
		// Until I get a more advanced system that can detect runtime errors and recover, it's either true or exit(-1) for now
		this->CurrentFrame.Success = true; 
		*CurrentFrame = this->CurrentFrame;
	}

	void CoreRenderer::ClearBuffer(void) {
		ColorRGBA DefaultColor{ 0, 0, 0, 255 };
		/*
		Loop y first then x to improve cache efficiency
		The color array is a linear array of y-levels
		By looping x first and then y we jump an entire y level, which could be thousands of pixels or a few kilobytes apart
		But by looping y first and then x we write adjacent pixels
		*/
		for (uint32_t y = 0; y < BackBuffer.Height; y++) {
			for (uint32_t x = 0; x < BackBuffer.Width; x++) {
				BackBuffer.Write(x, y, DefaultColor);
			}
		}
	}

	void CoreRenderer::SwapBuffers(void) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, BackBuffer.Width, BackBuffer.Height, GL_RGBA, GL_UNSIGNED_BYTE, BackBuffer.Data);
		glCallList(PresentList);
		SDL_GL_SwapWindow(WindowContext->Window);
	}

}