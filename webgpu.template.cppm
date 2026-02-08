module;

{{webgpu_includes}}

#include <atomic>
#include <cstddef>
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
#include <concepts>
#include <cmath>
#include <memory>
#include <new>
#include <initializer_list>
#include <string_view>
#include <span>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>

export module webgpu;

{{begin_inject}}
typename StringView:
    StringView(const std::string_view& sv) : data(sv.data()), length(sv.size()) {}
    StringView(const char* str) : data(str), length(WGPU_STRLEN) {}
    operator std::string_view() const {
        return length == WGPU_STRLEN ? std::string_view(data) : std::string_view(data, length);
    }
typename Instance:
    WEBGPU_CPP_NAMESPACE::Adapter requestAdapter(const RequestAdapterOptions& options) const;
typename Adapter:
    WEBGPU_CPP_NAMESPACE::Device requestDevice(const DeviceDescriptor& descriptor) const;
typename Color:
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
typename Extent3D:
    Extent3D(uint32_t width, uint32_t height, uint32_t depthOrArrayLayers = 1) : width(width), height(height), depthOrArrayLayers(depthOrArrayLayers) {}
typename Origin3D:
    Origin3D(uint32_t x, uint32_t y, uint32_t z) : x(x), y(y), z(z) {}
{{end_inject}}

export {

{{type_aliases}}

{{enums}}

{{structs_decl}}
{{handles_decl}}
{{callbacks_decl}}

{{handles}}
{{callbacks}}
{{structs}}

// Non member functions
{{functions_decl}}

}

{{structs_template_impl}}
{{handles_template_impl}}
{{callbacks_template_impl}}

{{enums_impl}}
{{structs_impl}}
{{handles_impl}}
{{callbacks_impl}}
{{functions_impl}}

namespace WEBGPU_CPP_NAMESPACE
{
#ifdef WEBGPU_CPP_USE_RAII
namespace raw 
#endif
{
WEBGPU_CPP_NAMESPACE::Adapter Instance::requestAdapter(const RequestAdapterOptions& options) const {
	struct Context {
		Adapter adapter = nullptr;
		bool requestEnded = false;
	};
	Context context;

	WGPURequestAdapterCallbackInfo callbackInfo;
	callbackInfo.nextInChain = nullptr;
	callbackInfo.userdata1 = &context;
	callbackInfo.callback = [](
		WGPURequestAdapterStatus status,
		WGPUAdapter adapter,
		WGPUStringView message,
		void* userdata1,
		[[maybe_unused]] void* userdata2
	) {
		Context& context = *reinterpret_cast<Context*>(userdata1);
		if (status == WGPURequestAdapterStatus_Success) {
			context.adapter = adapter;
		}
		else {
			std::cout << "Could not get WebGPU adapter: " << std::string_view(StringView(message)) << std::endl;
		}
		context.requestEnded = true;
	};
	callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
	wgpuInstanceRequestAdapter(*this, reinterpret_cast<const WGPURequestAdapterOptions*>(&options), callbackInfo);

#if __EMSCRIPTEN__
	while (!context.requestEnded) {
		emscripten_sleep(50);
	}
#endif

	assert(context.requestEnded);
	return context.adapter;
}
WEBGPU_CPP_NAMESPACE::Device Adapter::requestDevice(const DeviceDescriptor& descriptor) const {
	struct Context {
		Device device = nullptr;
		bool requestEnded = false;
	};
	Context context;

	WGPURequestDeviceCallbackInfo callbackInfo;
	callbackInfo.nextInChain = nullptr;
	callbackInfo.userdata1 = &context;
	callbackInfo.callback = [](
		WGPURequestDeviceStatus status,
		WGPUDevice device,
		WGPUStringView message,
		void* userdata1,
		[[maybe_unused]] void* userdata2
	) {
		Context& context = *reinterpret_cast<Context*>(userdata1);
		if (status == WGPURequestDeviceStatus_Success) {
			context.device = device;
		}
		else {
			std::cout << "Could not get WebGPU device: " << std::string_view(StringView(message)) << std::endl;
		}
		context.requestEnded = true;
	};
	callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
	wgpuAdapterRequestDevice(*this, reinterpret_cast<const WGPUDeviceDescriptor*>(&descriptor), callbackInfo);

#if __EMSCRIPTEN__
	while (!context.requestEnded) {
		emscripten_sleep(50);
	}
#endif

	assert(context.requestEnded);
	return context.device;
}
}
}