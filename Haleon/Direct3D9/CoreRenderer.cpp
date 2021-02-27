#include "CoreRenderer.h"
#include <wrl.h>

namespace Haleon {

	struct Direct3D9Interface {
	public:
		Direct3D9Interface(void) : Interface(Direct3DCreate9(D3D_SDK_VERSION)) {}
		~Direct3D9Interface(void) {
			Interface->Release();
		}
		IDirect3D9* operator->(void) {
			return Interface;
		}
	private:
		IDirect3D9* Interface;
	};
	
	static Direct3D9Interface Direct3D9;

	void CoreRenderer::Create(GraphicsWindow* Window) {
		D3DPRESENT_PARAMETERS PresentParameters;
		ZeroMemory(&PresentParameters, sizeof(PresentParameters));

		PresentParameters.Windowed = TRUE;
		PresentParameters.hDeviceWindow = Window->DriverInfo.info.win.window;
		PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

		Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, PresentParameters.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentParameters, &Device);
	}

	void CoreRenderer::Free(void) {
		Device->Release();
	}

	void CoreRenderer::StartFrame(Frame* NewFrame) {
		ClearBuffer();
		Device->BeginScene();

		CurrentFrame.ID++;
		*NewFrame = CurrentFrame;
	}

	void CoreRenderer::EndFrame(Frame* CurrentFrame) {
		Device->EndScene();
		SwapBuffers();

		// Until I get a more advanced system that can detect runtime errors and recover, it's either true or exit(-1) for now
		this->CurrentFrame.Success = true;
		*CurrentFrame = this->CurrentFrame;
	}

	void CoreRenderer::ClearBuffer(void) {
		Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0xFF);
	}

	void CoreRenderer::SwapBuffers(void) {
		Device->Present(NULL, NULL, NULL, NULL);
	}

}