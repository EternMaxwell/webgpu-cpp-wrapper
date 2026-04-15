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
#include <string>
#include <string_view>
#include <span>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>

export module webgpu;

{{begin_inject}}
typename StringView:
    StringView(const std::string_view& sv) : owned_(sv) {}
    StringView(const char* str) : owned_(str ? str : "") {}
    StringView(const StringView&) = default;
    StringView(StringView&&) = default;
    StringView& operator=(const StringView&) = default;
    StringView& operator=(StringView&&) = default;
    operator std::string_view() const { return std::string_view(owned_); }
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

// Type aliases
{{type_aliases}}

// Enums
{{enums}}

// Struct declarations
{{structs_decl}}
// Handle declarations
{{handles_decl}}
// Callback declarations
{{callbacks_decl}}

// Handles
{{handles}}
// Callbacks
{{callbacks}}
// Structs
{{structs}}

// Non member functions
{{functions_decl}}

}

// Struct template implementations
{{structs_template_impl}}
// Handle template implementations
{{handles_template_impl}}
// Callback template implementations
{{callbacks_template_impl}}

// Enum implementations
{{enums_impl}}
// Struct implementations
{{structs_impl}}
// Handle implementations
{{handles_impl}}
// Callback implementations
{{callbacks_impl}}
// Non member function implementations
{{functions_impl}}

namespace WEBGPU_CPP_NAMESPACE
{
#ifdef WEBGPU_CPP_USE_RAII
namespace raw 
#endif
{
WEBGPU_CPP_NAMESPACE::Adapter Instance::requestAdapter(const RequestAdapterOptions& options) const {
    struct Context {
        WEBGPU_CPP_NAMESPACE::Adapter adapter = nullptr;
        bool requestEnded = false;
    };
    Context context;

    RequestAdapterCallbackInfo callbackInfo;
    callbackInfo.callback = [&](
        RequestAdapterStatus status,
        WEBGPU_CPP_NAMESPACE::Adapter adapter,
        StringView message
    ) {
        if (status == RequestAdapterStatus::eSuccess) {
            context.adapter = std::move(adapter);
        }
        else {
            std::cout << "Could not get WebGPU adapter: " << std::string_view(StringView(message)) << std::endl;
        }
        context.requestEnded = true;
    };
    callbackInfo.mode = CallbackMode::eAllowSpontaneous;
    RequestAdapterOptions::CStruct options_c;
    options.to_cstruct(&options_c);
    RequestAdapterCallbackInfo::CStruct callbackInfo_c;
    callbackInfo.to_cstruct(&callbackInfo_c);
    wgpuInstanceRequestAdapter(*this, &options_c, callbackInfo_c);

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
        WEBGPU_CPP_NAMESPACE::Device device = nullptr;
        bool requestEnded = false;
    };
    Context context;

    RequestDeviceCallbackInfo callbackInfo;
    callbackInfo.callback = [&](
        RequestDeviceStatus status,
        WEBGPU_CPP_NAMESPACE::Device device,
        StringView message
    ) {
        if (status == RequestDeviceStatus::eSuccess) {
            context.device = std::move(device);
        }
        else {
            std::cout << "Could not get WebGPU device: " << std::string_view(StringView(message)) << std::endl;
        }
        context.requestEnded = true;
    };
    callbackInfo.mode = CallbackMode::eAllowSpontaneous;
    DeviceDescriptor::CStruct descriptor_c;
    descriptor.to_cstruct(&descriptor_c);
    RequestDeviceCallbackInfo::CStruct callbackInfo_c;
    callbackInfo.to_cstruct(&callbackInfo_c);
    wgpuAdapterRequestDevice(*this, &descriptor_c, callbackInfo_c);

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