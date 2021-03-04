#ifndef HALEON_GRAPHICS_CORE_RENDERER_H
#define HALEON_GRAPHICS_CORE_RENDERER_H

#if defined(HALEON_GRAPHICS_API_OPENGL3)
#include "Backend/OpenGL3/GL3CoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D9)
#include "Backend/Direct3D9/D3D9CoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D10)
#include "Backend/Direct3D10/D3D10CoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D11)
#include "Backend/Direct3D11/D3D11CoreRenderer.h"
#elif defined(HALEON_GRAPHICS_API_DIRECT3D12)
#include "Backend/Direct3D12/D3D12CoreRenderer.h"
#else
#include "Backend/Photon/PhotonCoreRenderer.h"
#endif

#endif

