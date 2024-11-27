#pragma once

#ifndef UNICODE
#define UNICODE 1
#endif // !UNICODE

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl/client.h>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <set>
#include <mutex>
#include <atomic>
#include <functional>
#include <exception>
#include <numbers>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_2.h>
#include <DirectXMath.h>

#include "core/utils/exception.hpp"
#include "core/thread/thread_manager.hpp"

using Microsoft::WRL::ComPtr;
