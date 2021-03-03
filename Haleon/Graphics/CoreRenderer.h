#ifndef HALEON_GRAPHICS_CORE_RENDERER_H
#define HALEON_GRAPHICS_CORE_RENDERER_H

#if defined(HALEON_GRAPHICS_API_OPENGL3)
#include "../OpenGL3/GLCoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D9)
#include "../Direct3D9/D3D9CoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D10)
#include "../Direct3D10/D3D10CoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D11)
#include "../Direct3D11/D3D11CoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D12)
#include "../Direct3D12/D3D12CoreRenderer.h"
#else
#include "../Photon/PhotonCoreRenderer.h"
#endif

#endif

