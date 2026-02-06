module;
#include <webgpu/webgpu.h>
#include <atomic>
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
#include <concepts>
#include <cmath>
#include <memory>
#include <string_view>
#include <span>
#include <optional>
#include <ranges>
export module webgpu;
export {
namespace wgpu {
using Flags = uint64_t;
using Bool = uint32_t;
}
namespace wgpu {
enum class AdapterType {
    eDiscreteGPU = WGPUAdapterType_DiscreteGPU,
    eIntegratedGPU = WGPUAdapterType_IntegratedGPU,
    eCPU = WGPUAdapterType_CPU,
    eUnknown = WGPUAdapterType_Unknown,
    eForce32 = WGPUAdapterType_Force32,
};
enum class AddressMode {
    eUndefined = WGPUAddressMode_Undefined,
    eClampToEdge = WGPUAddressMode_ClampToEdge,
    eRepeat = WGPUAddressMode_Repeat,
    eMirrorRepeat = WGPUAddressMode_MirrorRepeat,
    eForce32 = WGPUAddressMode_Force32,
};
enum class BackendType {
    eUndefined = WGPUBackendType_Undefined,
    eNull = WGPUBackendType_Null,
    eWebGPU = WGPUBackendType_WebGPU,
    eD3D11 = WGPUBackendType_D3D11,
    eD3D12 = WGPUBackendType_D3D12,
    eMetal = WGPUBackendType_Metal,
    eVulkan = WGPUBackendType_Vulkan,
    eOpenGL = WGPUBackendType_OpenGL,
    eOpenGLES = WGPUBackendType_OpenGLES,
    eForce32 = WGPUBackendType_Force32,
};
enum class BlendFactor {
    eUndefined = WGPUBlendFactor_Undefined,
    eZero = WGPUBlendFactor_Zero,
    eOne = WGPUBlendFactor_One,
    eSrc = WGPUBlendFactor_Src,
    eOneMinusSrc = WGPUBlendFactor_OneMinusSrc,
    eSrcAlpha = WGPUBlendFactor_SrcAlpha,
    eOneMinusSrcAlpha = WGPUBlendFactor_OneMinusSrcAlpha,
    eDst = WGPUBlendFactor_Dst,
    eOneMinusDst = WGPUBlendFactor_OneMinusDst,
    eDstAlpha = WGPUBlendFactor_DstAlpha,
    eOneMinusDstAlpha = WGPUBlendFactor_OneMinusDstAlpha,
    eSrcAlphaSaturated = WGPUBlendFactor_SrcAlphaSaturated,
    eConstant = WGPUBlendFactor_Constant,
    eOneMinusConstant = WGPUBlendFactor_OneMinusConstant,
    eSrc1 = WGPUBlendFactor_Src1,
    eOneMinusSrc1 = WGPUBlendFactor_OneMinusSrc1,
    eSrc1Alpha = WGPUBlendFactor_Src1Alpha,
    eOneMinusSrc1Alpha = WGPUBlendFactor_OneMinusSrc1Alpha,
    eForce32 = WGPUBlendFactor_Force32,
};
enum class BlendOperation {
    eUndefined = WGPUBlendOperation_Undefined,
    eAdd = WGPUBlendOperation_Add,
    eSubtract = WGPUBlendOperation_Subtract,
    eReverseSubtract = WGPUBlendOperation_ReverseSubtract,
    eMin = WGPUBlendOperation_Min,
    eMax = WGPUBlendOperation_Max,
    eForce32 = WGPUBlendOperation_Force32,
};
enum class BufferBindingType {
    eBindingNotUsed = WGPUBufferBindingType_BindingNotUsed,
    eUndefined = WGPUBufferBindingType_Undefined,
    eUniform = WGPUBufferBindingType_Uniform,
    eStorage = WGPUBufferBindingType_Storage,
    eReadOnlyStorage = WGPUBufferBindingType_ReadOnlyStorage,
    eForce32 = WGPUBufferBindingType_Force32,
};
enum class BufferMapState {
    eUnmapped = WGPUBufferMapState_Unmapped,
    ePending = WGPUBufferMapState_Pending,
    eMapped = WGPUBufferMapState_Mapped,
    eForce32 = WGPUBufferMapState_Force32,
};
enum class CallbackMode {
    eWaitAnyOnly = WGPUCallbackMode_WaitAnyOnly,
    eAllowProcessEvents = WGPUCallbackMode_AllowProcessEvents,
    eAllowSpontaneous = WGPUCallbackMode_AllowSpontaneous,
    eForce32 = WGPUCallbackMode_Force32,
};
enum class CompareFunction {
    eUndefined = WGPUCompareFunction_Undefined,
    eNever = WGPUCompareFunction_Never,
    eLess = WGPUCompareFunction_Less,
    eEqual = WGPUCompareFunction_Equal,
    eLessEqual = WGPUCompareFunction_LessEqual,
    eGreater = WGPUCompareFunction_Greater,
    eNotEqual = WGPUCompareFunction_NotEqual,
    eGreaterEqual = WGPUCompareFunction_GreaterEqual,
    eAlways = WGPUCompareFunction_Always,
    eForce32 = WGPUCompareFunction_Force32,
};
enum class CompilationInfoRequestStatus {
    eSuccess = WGPUCompilationInfoRequestStatus_Success,
    eCallbackCancelled = WGPUCompilationInfoRequestStatus_CallbackCancelled,
    eForce32 = WGPUCompilationInfoRequestStatus_Force32,
};
enum class CompilationMessageType {
    eError = WGPUCompilationMessageType_Error,
    eWarning = WGPUCompilationMessageType_Warning,
    eInfo = WGPUCompilationMessageType_Info,
    eForce32 = WGPUCompilationMessageType_Force32,
};
enum class ComponentSwizzle {
    eUndefined = WGPUComponentSwizzle_Undefined,
    eZero = WGPUComponentSwizzle_Zero,
    eOne = WGPUComponentSwizzle_One,
    eR = WGPUComponentSwizzle_R,
    eG = WGPUComponentSwizzle_G,
    eB = WGPUComponentSwizzle_B,
    eA = WGPUComponentSwizzle_A,
    eForce32 = WGPUComponentSwizzle_Force32,
};
enum class CompositeAlphaMode {
    eAuto = WGPUCompositeAlphaMode_Auto,
    eOpaque = WGPUCompositeAlphaMode_Opaque,
    ePremultiplied = WGPUCompositeAlphaMode_Premultiplied,
    eUnpremultiplied = WGPUCompositeAlphaMode_Unpremultiplied,
    eInherit = WGPUCompositeAlphaMode_Inherit,
    eForce32 = WGPUCompositeAlphaMode_Force32,
};
enum class CreatePipelineAsyncStatus {
    eSuccess = WGPUCreatePipelineAsyncStatus_Success,
    eCallbackCancelled = WGPUCreatePipelineAsyncStatus_CallbackCancelled,
    eValidationError = WGPUCreatePipelineAsyncStatus_ValidationError,
    eInternalError = WGPUCreatePipelineAsyncStatus_InternalError,
    eForce32 = WGPUCreatePipelineAsyncStatus_Force32,
};
enum class CullMode {
    eUndefined = WGPUCullMode_Undefined,
    eNone = WGPUCullMode_None,
    eFront = WGPUCullMode_Front,
    eBack = WGPUCullMode_Back,
    eForce32 = WGPUCullMode_Force32,
};
enum class DeviceLostReason {
    eUnknown = WGPUDeviceLostReason_Unknown,
    eDestroyed = WGPUDeviceLostReason_Destroyed,
    eCallbackCancelled = WGPUDeviceLostReason_CallbackCancelled,
    eFailedCreation = WGPUDeviceLostReason_FailedCreation,
    eForce32 = WGPUDeviceLostReason_Force32,
};
enum class ErrorFilter {
    eValidation = WGPUErrorFilter_Validation,
    eOutOfMemory = WGPUErrorFilter_OutOfMemory,
    eInternal = WGPUErrorFilter_Internal,
    eForce32 = WGPUErrorFilter_Force32,
};
enum class ErrorType {
    eNoError = WGPUErrorType_NoError,
    eValidation = WGPUErrorType_Validation,
    eOutOfMemory = WGPUErrorType_OutOfMemory,
    eInternal = WGPUErrorType_Internal,
    eUnknown = WGPUErrorType_Unknown,
    eForce32 = WGPUErrorType_Force32,
};
enum class FeatureLevel {
    eUndefined = WGPUFeatureLevel_Undefined,
    eCompatibility = WGPUFeatureLevel_Compatibility,
    eCore = WGPUFeatureLevel_Core,
    eForce32 = WGPUFeatureLevel_Force32,
};
enum class FeatureName {
    eCoreFeaturesAndLimits = WGPUFeatureName_CoreFeaturesAndLimits,
    eDepthClipControl = WGPUFeatureName_DepthClipControl,
    eDepth32FloatStencil8 = WGPUFeatureName_Depth32FloatStencil8,
    eTextureCompressionBC = WGPUFeatureName_TextureCompressionBC,
    eTextureCompressionBCSliced3D = WGPUFeatureName_TextureCompressionBCSliced3D,
    eTextureCompressionETC2 = WGPUFeatureName_TextureCompressionETC2,
    eTextureCompressionASTC = WGPUFeatureName_TextureCompressionASTC,
    eTextureCompressionASTCSliced3D = WGPUFeatureName_TextureCompressionASTCSliced3D,
    eTimestampQuery = WGPUFeatureName_TimestampQuery,
    eIndirectFirstInstance = WGPUFeatureName_IndirectFirstInstance,
    eShaderF16 = WGPUFeatureName_ShaderF16,
    eRG11B10UfloatRenderable = WGPUFeatureName_RG11B10UfloatRenderable,
    eBGRA8UnormStorage = WGPUFeatureName_BGRA8UnormStorage,
    eFloat32Filterable = WGPUFeatureName_Float32Filterable,
    eFloat32Blendable = WGPUFeatureName_Float32Blendable,
    eClipDistances = WGPUFeatureName_ClipDistances,
    eDualSourceBlending = WGPUFeatureName_DualSourceBlending,
    eSubgroups = WGPUFeatureName_Subgroups,
    eTextureFormatsTier1 = WGPUFeatureName_TextureFormatsTier1,
    eTextureFormatsTier2 = WGPUFeatureName_TextureFormatsTier2,
    ePrimitiveIndex = WGPUFeatureName_PrimitiveIndex,
    eTextureComponentSwizzle = WGPUFeatureName_TextureComponentSwizzle,
    eForce32 = WGPUFeatureName_Force32,
};
enum class FilterMode {
    eUndefined = WGPUFilterMode_Undefined,
    eNearest = WGPUFilterMode_Nearest,
    eLinear = WGPUFilterMode_Linear,
    eForce32 = WGPUFilterMode_Force32,
};
enum class FrontFace {
    eUndefined = WGPUFrontFace_Undefined,
    eCCW = WGPUFrontFace_CCW,
    eCW = WGPUFrontFace_CW,
    eForce32 = WGPUFrontFace_Force32,
};
enum class IndexFormat {
    eUndefined = WGPUIndexFormat_Undefined,
    eUint16 = WGPUIndexFormat_Uint16,
    eUint32 = WGPUIndexFormat_Uint32,
    eForce32 = WGPUIndexFormat_Force32,
};
enum class InstanceFeatureName {
    eTimedWaitAny = WGPUInstanceFeatureName_TimedWaitAny,
    eShaderSourceSPIRV = WGPUInstanceFeatureName_ShaderSourceSPIRV,
    eMultipleDevicesPerAdapter = WGPUInstanceFeatureName_MultipleDevicesPerAdapter,
    eForce32 = WGPUInstanceFeatureName_Force32,
};
enum class LoadOp {
    eUndefined = WGPULoadOp_Undefined,
    eLoad = WGPULoadOp_Load,
    eClear = WGPULoadOp_Clear,
    eForce32 = WGPULoadOp_Force32,
};
enum class MapAsyncStatus {
    eSuccess = WGPUMapAsyncStatus_Success,
    eCallbackCancelled = WGPUMapAsyncStatus_CallbackCancelled,
    eError = WGPUMapAsyncStatus_Error,
    eAborted = WGPUMapAsyncStatus_Aborted,
    eForce32 = WGPUMapAsyncStatus_Force32,
};
enum class MipmapFilterMode {
    eUndefined = WGPUMipmapFilterMode_Undefined,
    eNearest = WGPUMipmapFilterMode_Nearest,
    eLinear = WGPUMipmapFilterMode_Linear,
    eForce32 = WGPUMipmapFilterMode_Force32,
};
enum class OptionalBool {
    eFalse = WGPUOptionalBool_False,
    eTrue = WGPUOptionalBool_True,
    eUndefined = WGPUOptionalBool_Undefined,
    eForce32 = WGPUOptionalBool_Force32,
};
enum class PopErrorScopeStatus {
    eSuccess = WGPUPopErrorScopeStatus_Success,
    eCallbackCancelled = WGPUPopErrorScopeStatus_CallbackCancelled,
    eError = WGPUPopErrorScopeStatus_Error,
    eForce32 = WGPUPopErrorScopeStatus_Force32,
};
enum class PowerPreference {
    eUndefined = WGPUPowerPreference_Undefined,
    eLowPower = WGPUPowerPreference_LowPower,
    eHighPerformance = WGPUPowerPreference_HighPerformance,
    eForce32 = WGPUPowerPreference_Force32,
};
enum class PredefinedColorSpace {
    eSRGB = WGPUPredefinedColorSpace_SRGB,
    eDisplayP3 = WGPUPredefinedColorSpace_DisplayP3,
    eForce32 = WGPUPredefinedColorSpace_Force32,
};
enum class PresentMode {
    eUndefined = WGPUPresentMode_Undefined,
    eFifo = WGPUPresentMode_Fifo,
    eFifoRelaxed = WGPUPresentMode_FifoRelaxed,
    eImmediate = WGPUPresentMode_Immediate,
    eMailbox = WGPUPresentMode_Mailbox,
    eForce32 = WGPUPresentMode_Force32,
};
enum class PrimitiveTopology {
    eUndefined = WGPUPrimitiveTopology_Undefined,
    ePointList = WGPUPrimitiveTopology_PointList,
    eLineList = WGPUPrimitiveTopology_LineList,
    eLineStrip = WGPUPrimitiveTopology_LineStrip,
    eTriangleList = WGPUPrimitiveTopology_TriangleList,
    eTriangleStrip = WGPUPrimitiveTopology_TriangleStrip,
    eForce32 = WGPUPrimitiveTopology_Force32,
};
enum class QueryType {
    eOcclusion = WGPUQueryType_Occlusion,
    eTimestamp = WGPUQueryType_Timestamp,
    eForce32 = WGPUQueryType_Force32,
};
enum class QueueWorkDoneStatus {
    eSuccess = WGPUQueueWorkDoneStatus_Success,
    eCallbackCancelled = WGPUQueueWorkDoneStatus_CallbackCancelled,
    eError = WGPUQueueWorkDoneStatus_Error,
    eForce32 = WGPUQueueWorkDoneStatus_Force32,
};
enum class RequestAdapterStatus {
    eSuccess = WGPURequestAdapterStatus_Success,
    eCallbackCancelled = WGPURequestAdapterStatus_CallbackCancelled,
    eUnavailable = WGPURequestAdapterStatus_Unavailable,
    eError = WGPURequestAdapterStatus_Error,
    eForce32 = WGPURequestAdapterStatus_Force32,
};
enum class RequestDeviceStatus {
    eSuccess = WGPURequestDeviceStatus_Success,
    eCallbackCancelled = WGPURequestDeviceStatus_CallbackCancelled,
    eError = WGPURequestDeviceStatus_Error,
    eForce32 = WGPURequestDeviceStatus_Force32,
};
enum class SamplerBindingType {
    eBindingNotUsed = WGPUSamplerBindingType_BindingNotUsed,
    eUndefined = WGPUSamplerBindingType_Undefined,
    eFiltering = WGPUSamplerBindingType_Filtering,
    eNonFiltering = WGPUSamplerBindingType_NonFiltering,
    eComparison = WGPUSamplerBindingType_Comparison,
    eForce32 = WGPUSamplerBindingType_Force32,
};
enum class Status {
    eSuccess = WGPUStatus_Success,
    eError = WGPUStatus_Error,
    eForce32 = WGPUStatus_Force32,
};
enum class StencilOperation {
    eUndefined = WGPUStencilOperation_Undefined,
    eKeep = WGPUStencilOperation_Keep,
    eZero = WGPUStencilOperation_Zero,
    eReplace = WGPUStencilOperation_Replace,
    eInvert = WGPUStencilOperation_Invert,
    eIncrementClamp = WGPUStencilOperation_IncrementClamp,
    eDecrementClamp = WGPUStencilOperation_DecrementClamp,
    eIncrementWrap = WGPUStencilOperation_IncrementWrap,
    eDecrementWrap = WGPUStencilOperation_DecrementWrap,
    eForce32 = WGPUStencilOperation_Force32,
};
enum class StorageTextureAccess {
    eBindingNotUsed = WGPUStorageTextureAccess_BindingNotUsed,
    eUndefined = WGPUStorageTextureAccess_Undefined,
    eWriteOnly = WGPUStorageTextureAccess_WriteOnly,
    eReadOnly = WGPUStorageTextureAccess_ReadOnly,
    eReadWrite = WGPUStorageTextureAccess_ReadWrite,
    eForce32 = WGPUStorageTextureAccess_Force32,
};
enum class StoreOp {
    eUndefined = WGPUStoreOp_Undefined,
    eStore = WGPUStoreOp_Store,
    eDiscard = WGPUStoreOp_Discard,
    eForce32 = WGPUStoreOp_Force32,
};
enum class SType {
    eShaderSourceSPIRV = WGPUSType_ShaderSourceSPIRV,
    eShaderSourceWGSL = WGPUSType_ShaderSourceWGSL,
    eRenderPassMaxDrawCount = WGPUSType_RenderPassMaxDrawCount,
    eSurfaceSourceMetalLayer = WGPUSType_SurfaceSourceMetalLayer,
    eSurfaceSourceWindowsHWND = WGPUSType_SurfaceSourceWindowsHWND,
    eSurfaceSourceXlibWindow = WGPUSType_SurfaceSourceXlibWindow,
    eSurfaceSourceWaylandSurface = WGPUSType_SurfaceSourceWaylandSurface,
    eSurfaceSourceAndroidNativeWindow = WGPUSType_SurfaceSourceAndroidNativeWindow,
    eSurfaceSourceXCBWindow = WGPUSType_SurfaceSourceXCBWindow,
    eSurfaceColorManagement = WGPUSType_SurfaceColorManagement,
    eRequestAdapterWebXROptions = WGPUSType_RequestAdapterWebXROptions,
    eTextureComponentSwizzleDescriptor = WGPUSType_TextureComponentSwizzleDescriptor,
    eExternalTextureBindingLayout = WGPUSType_ExternalTextureBindingLayout,
    eExternalTextureBindingEntry = WGPUSType_ExternalTextureBindingEntry,
    eForce32 = WGPUSType_Force32,
};
enum class SurfaceGetCurrentTextureStatus {
    eSuccessOptimal = WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal,
    eSuccessSuboptimal = WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal,
    eTimeout = WGPUSurfaceGetCurrentTextureStatus_Timeout,
    eOutdated = WGPUSurfaceGetCurrentTextureStatus_Outdated,
    eLost = WGPUSurfaceGetCurrentTextureStatus_Lost,
    eError = WGPUSurfaceGetCurrentTextureStatus_Error,
    eForce32 = WGPUSurfaceGetCurrentTextureStatus_Force32,
};
enum class TextureAspect {
    eUndefined = WGPUTextureAspect_Undefined,
    eAll = WGPUTextureAspect_All,
    eStencilOnly = WGPUTextureAspect_StencilOnly,
    eDepthOnly = WGPUTextureAspect_DepthOnly,
    eForce32 = WGPUTextureAspect_Force32,
};
enum class TextureDimension {
    eUndefined = WGPUTextureDimension_Undefined,
    e1D = WGPUTextureDimension_1D,
    e2D = WGPUTextureDimension_2D,
    e3D = WGPUTextureDimension_3D,
    eForce32 = WGPUTextureDimension_Force32,
};
enum class TextureFormat {
    eUndefined = WGPUTextureFormat_Undefined,
    eR8Unorm = WGPUTextureFormat_R8Unorm,
    eR8Snorm = WGPUTextureFormat_R8Snorm,
    eR8Uint = WGPUTextureFormat_R8Uint,
    eR8Sint = WGPUTextureFormat_R8Sint,
    eR16Unorm = WGPUTextureFormat_R16Unorm,
    eR16Snorm = WGPUTextureFormat_R16Snorm,
    eR16Uint = WGPUTextureFormat_R16Uint,
    eR16Sint = WGPUTextureFormat_R16Sint,
    eR16Float = WGPUTextureFormat_R16Float,
    eRG8Unorm = WGPUTextureFormat_RG8Unorm,
    eRG8Snorm = WGPUTextureFormat_RG8Snorm,
    eRG8Uint = WGPUTextureFormat_RG8Uint,
    eRG8Sint = WGPUTextureFormat_RG8Sint,
    eR32Float = WGPUTextureFormat_R32Float,
    eR32Uint = WGPUTextureFormat_R32Uint,
    eR32Sint = WGPUTextureFormat_R32Sint,
    eRG16Unorm = WGPUTextureFormat_RG16Unorm,
    eRG16Snorm = WGPUTextureFormat_RG16Snorm,
    eRG16Uint = WGPUTextureFormat_RG16Uint,
    eRG16Sint = WGPUTextureFormat_RG16Sint,
    eRG16Float = WGPUTextureFormat_RG16Float,
    eRGBA8Unorm = WGPUTextureFormat_RGBA8Unorm,
    eRGBA8UnormSrgb = WGPUTextureFormat_RGBA8UnormSrgb,
    eRGBA8Snorm = WGPUTextureFormat_RGBA8Snorm,
    eRGBA8Uint = WGPUTextureFormat_RGBA8Uint,
    eRGBA8Sint = WGPUTextureFormat_RGBA8Sint,
    eBGRA8Unorm = WGPUTextureFormat_BGRA8Unorm,
    eBGRA8UnormSrgb = WGPUTextureFormat_BGRA8UnormSrgb,
    eRGB10A2Uint = WGPUTextureFormat_RGB10A2Uint,
    eRGB10A2Unorm = WGPUTextureFormat_RGB10A2Unorm,
    eRG11B10Ufloat = WGPUTextureFormat_RG11B10Ufloat,
    eRGB9E5Ufloat = WGPUTextureFormat_RGB9E5Ufloat,
    eRG32Float = WGPUTextureFormat_RG32Float,
    eRG32Uint = WGPUTextureFormat_RG32Uint,
    eRG32Sint = WGPUTextureFormat_RG32Sint,
    eRGBA16Unorm = WGPUTextureFormat_RGBA16Unorm,
    eRGBA16Snorm = WGPUTextureFormat_RGBA16Snorm,
    eRGBA16Uint = WGPUTextureFormat_RGBA16Uint,
    eRGBA16Sint = WGPUTextureFormat_RGBA16Sint,
    eRGBA16Float = WGPUTextureFormat_RGBA16Float,
    eRGBA32Float = WGPUTextureFormat_RGBA32Float,
    eRGBA32Uint = WGPUTextureFormat_RGBA32Uint,
    eRGBA32Sint = WGPUTextureFormat_RGBA32Sint,
    eStencil8 = WGPUTextureFormat_Stencil8,
    eDepth16Unorm = WGPUTextureFormat_Depth16Unorm,
    eDepth24Plus = WGPUTextureFormat_Depth24Plus,
    eDepth24PlusStencil8 = WGPUTextureFormat_Depth24PlusStencil8,
    eDepth32Float = WGPUTextureFormat_Depth32Float,
    eDepth32FloatStencil8 = WGPUTextureFormat_Depth32FloatStencil8,
    eBC1RGBAUnorm = WGPUTextureFormat_BC1RGBAUnorm,
    eBC1RGBAUnormSrgb = WGPUTextureFormat_BC1RGBAUnormSrgb,
    eBC2RGBAUnorm = WGPUTextureFormat_BC2RGBAUnorm,
    eBC2RGBAUnormSrgb = WGPUTextureFormat_BC2RGBAUnormSrgb,
    eBC3RGBAUnorm = WGPUTextureFormat_BC3RGBAUnorm,
    eBC3RGBAUnormSrgb = WGPUTextureFormat_BC3RGBAUnormSrgb,
    eBC4RUnorm = WGPUTextureFormat_BC4RUnorm,
    eBC4RSnorm = WGPUTextureFormat_BC4RSnorm,
    eBC5RGUnorm = WGPUTextureFormat_BC5RGUnorm,
    eBC5RGSnorm = WGPUTextureFormat_BC5RGSnorm,
    eBC6HRGBUfloat = WGPUTextureFormat_BC6HRGBUfloat,
    eBC6HRGBFloat = WGPUTextureFormat_BC6HRGBFloat,
    eBC7RGBAUnorm = WGPUTextureFormat_BC7RGBAUnorm,
    eBC7RGBAUnormSrgb = WGPUTextureFormat_BC7RGBAUnormSrgb,
    eETC2RGB8Unorm = WGPUTextureFormat_ETC2RGB8Unorm,
    eETC2RGB8UnormSrgb = WGPUTextureFormat_ETC2RGB8UnormSrgb,
    eETC2RGB8A1Unorm = WGPUTextureFormat_ETC2RGB8A1Unorm,
    eETC2RGB8A1UnormSrgb = WGPUTextureFormat_ETC2RGB8A1UnormSrgb,
    eETC2RGBA8Unorm = WGPUTextureFormat_ETC2RGBA8Unorm,
    eETC2RGBA8UnormSrgb = WGPUTextureFormat_ETC2RGBA8UnormSrgb,
    eEACR11Unorm = WGPUTextureFormat_EACR11Unorm,
    eEACR11Snorm = WGPUTextureFormat_EACR11Snorm,
    eEACRG11Unorm = WGPUTextureFormat_EACRG11Unorm,
    eEACRG11Snorm = WGPUTextureFormat_EACRG11Snorm,
    eASTC4x4Unorm = WGPUTextureFormat_ASTC4x4Unorm,
    eASTC4x4UnormSrgb = WGPUTextureFormat_ASTC4x4UnormSrgb,
    eASTC5x4Unorm = WGPUTextureFormat_ASTC5x4Unorm,
    eASTC5x4UnormSrgb = WGPUTextureFormat_ASTC5x4UnormSrgb,
    eASTC5x5Unorm = WGPUTextureFormat_ASTC5x5Unorm,
    eASTC5x5UnormSrgb = WGPUTextureFormat_ASTC5x5UnormSrgb,
    eASTC6x5Unorm = WGPUTextureFormat_ASTC6x5Unorm,
    eASTC6x5UnormSrgb = WGPUTextureFormat_ASTC6x5UnormSrgb,
    eASTC6x6Unorm = WGPUTextureFormat_ASTC6x6Unorm,
    eASTC6x6UnormSrgb = WGPUTextureFormat_ASTC6x6UnormSrgb,
    eASTC8x5Unorm = WGPUTextureFormat_ASTC8x5Unorm,
    eASTC8x5UnormSrgb = WGPUTextureFormat_ASTC8x5UnormSrgb,
    eASTC8x6Unorm = WGPUTextureFormat_ASTC8x6Unorm,
    eASTC8x6UnormSrgb = WGPUTextureFormat_ASTC8x6UnormSrgb,
    eASTC8x8Unorm = WGPUTextureFormat_ASTC8x8Unorm,
    eASTC8x8UnormSrgb = WGPUTextureFormat_ASTC8x8UnormSrgb,
    eASTC10x5Unorm = WGPUTextureFormat_ASTC10x5Unorm,
    eASTC10x5UnormSrgb = WGPUTextureFormat_ASTC10x5UnormSrgb,
    eASTC10x6Unorm = WGPUTextureFormat_ASTC10x6Unorm,
    eASTC10x6UnormSrgb = WGPUTextureFormat_ASTC10x6UnormSrgb,
    eASTC10x8Unorm = WGPUTextureFormat_ASTC10x8Unorm,
    eASTC10x8UnormSrgb = WGPUTextureFormat_ASTC10x8UnormSrgb,
    eASTC10x10Unorm = WGPUTextureFormat_ASTC10x10Unorm,
    eASTC10x10UnormSrgb = WGPUTextureFormat_ASTC10x10UnormSrgb,
    eASTC12x10Unorm = WGPUTextureFormat_ASTC12x10Unorm,
    eASTC12x10UnormSrgb = WGPUTextureFormat_ASTC12x10UnormSrgb,
    eASTC12x12Unorm = WGPUTextureFormat_ASTC12x12Unorm,
    eASTC12x12UnormSrgb = WGPUTextureFormat_ASTC12x12UnormSrgb,
    eForce32 = WGPUTextureFormat_Force32,
};
enum class TextureSampleType {
    eBindingNotUsed = WGPUTextureSampleType_BindingNotUsed,
    eUndefined = WGPUTextureSampleType_Undefined,
    eFloat = WGPUTextureSampleType_Float,
    eUnfilterableFloat = WGPUTextureSampleType_UnfilterableFloat,
    eDepth = WGPUTextureSampleType_Depth,
    eSint = WGPUTextureSampleType_Sint,
    eUint = WGPUTextureSampleType_Uint,
    eForce32 = WGPUTextureSampleType_Force32,
};
enum class TextureViewDimension {
    eUndefined = WGPUTextureViewDimension_Undefined,
    e1D = WGPUTextureViewDimension_1D,
    e2D = WGPUTextureViewDimension_2D,
    e2DArray = WGPUTextureViewDimension_2DArray,
    eCube = WGPUTextureViewDimension_Cube,
    eCubeArray = WGPUTextureViewDimension_CubeArray,
    e3D = WGPUTextureViewDimension_3D,
    eForce32 = WGPUTextureViewDimension_Force32,
};
enum class ToneMappingMode {
    eStandard = WGPUToneMappingMode_Standard,
    eExtended = WGPUToneMappingMode_Extended,
    eForce32 = WGPUToneMappingMode_Force32,
};
enum class VertexFormat {
    eUint8 = WGPUVertexFormat_Uint8,
    eUint8x2 = WGPUVertexFormat_Uint8x2,
    eUint8x4 = WGPUVertexFormat_Uint8x4,
    eSint8 = WGPUVertexFormat_Sint8,
    eSint8x2 = WGPUVertexFormat_Sint8x2,
    eSint8x4 = WGPUVertexFormat_Sint8x4,
    eUnorm8 = WGPUVertexFormat_Unorm8,
    eUnorm8x2 = WGPUVertexFormat_Unorm8x2,
    eUnorm8x4 = WGPUVertexFormat_Unorm8x4,
    eSnorm8 = WGPUVertexFormat_Snorm8,
    eSnorm8x2 = WGPUVertexFormat_Snorm8x2,
    eSnorm8x4 = WGPUVertexFormat_Snorm8x4,
    eUint16 = WGPUVertexFormat_Uint16,
    eUint16x2 = WGPUVertexFormat_Uint16x2,
    eUint16x4 = WGPUVertexFormat_Uint16x4,
    eSint16 = WGPUVertexFormat_Sint16,
    eSint16x2 = WGPUVertexFormat_Sint16x2,
    eSint16x4 = WGPUVertexFormat_Sint16x4,
    eUnorm16 = WGPUVertexFormat_Unorm16,
    eUnorm16x2 = WGPUVertexFormat_Unorm16x2,
    eUnorm16x4 = WGPUVertexFormat_Unorm16x4,
    eSnorm16 = WGPUVertexFormat_Snorm16,
    eSnorm16x2 = WGPUVertexFormat_Snorm16x2,
    eSnorm16x4 = WGPUVertexFormat_Snorm16x4,
    eFloat16 = WGPUVertexFormat_Float16,
    eFloat16x2 = WGPUVertexFormat_Float16x2,
    eFloat16x4 = WGPUVertexFormat_Float16x4,
    eFloat32 = WGPUVertexFormat_Float32,
    eFloat32x2 = WGPUVertexFormat_Float32x2,
    eFloat32x3 = WGPUVertexFormat_Float32x3,
    eFloat32x4 = WGPUVertexFormat_Float32x4,
    eUint32 = WGPUVertexFormat_Uint32,
    eUint32x2 = WGPUVertexFormat_Uint32x2,
    eUint32x3 = WGPUVertexFormat_Uint32x3,
    eUint32x4 = WGPUVertexFormat_Uint32x4,
    eSint32 = WGPUVertexFormat_Sint32,
    eSint32x2 = WGPUVertexFormat_Sint32x2,
    eSint32x3 = WGPUVertexFormat_Sint32x3,
    eSint32x4 = WGPUVertexFormat_Sint32x4,
    eUnorm10_10_10_2 = WGPUVertexFormat_Unorm10_10_10_2,
    eUnorm8x4BGRA = WGPUVertexFormat_Unorm8x4BGRA,
    eForce32 = WGPUVertexFormat_Force32,
};
enum class VertexStepMode {
    eUndefined = WGPUVertexStepMode_Undefined,
    eVertex = WGPUVertexStepMode_Vertex,
    eInstance = WGPUVertexStepMode_Instance,
    eForce32 = WGPUVertexStepMode_Force32,
};
enum class WaitStatus {
    eSuccess = WGPUWaitStatus_Success,
    eTimedOut = WGPUWaitStatus_TimedOut,
    eError = WGPUWaitStatus_Error,
    eForce32 = WGPUWaitStatus_Force32,
};
enum class WGSLLanguageFeatureName {
    eReadonlyAndReadwriteStorageTextures = WGPUWGSLLanguageFeatureName_ReadonlyAndReadwriteStorageTextures,
    ePacked4x8IntegerDotProduct = WGPUWGSLLanguageFeatureName_Packed4x8IntegerDotProduct,
    eUnrestrictedPointerParameters = WGPUWGSLLanguageFeatureName_UnrestrictedPointerParameters,
    ePointerCompositeAccess = WGPUWGSLLanguageFeatureName_PointerCompositeAccess,
    eUniformBufferStandardLayout = WGPUWGSLLanguageFeatureName_UniformBufferStandardLayout,
    eSubgroupId = WGPUWGSLLanguageFeatureName_SubgroupId,
    eTextureAndSamplerLet = WGPUWGSLLanguageFeatureName_TextureAndSamplerLet,
    eSubgroupUniformity = WGPUWGSLLanguageFeatureName_SubgroupUniformity,
    eForce32 = WGPUWGSLLanguageFeatureName_Force32,
};
enum class BufferUsage : Flags {
    eNone = WGPUBufferUsage_None,
    eMapRead = WGPUBufferUsage_MapRead,
    eMapWrite = WGPUBufferUsage_MapWrite,
    eCopySrc = WGPUBufferUsage_CopySrc,
    eCopyDst = WGPUBufferUsage_CopyDst,
    eIndex = WGPUBufferUsage_Index,
    eVertex = WGPUBufferUsage_Vertex,
    eUniform = WGPUBufferUsage_Uniform,
    eStorage = WGPUBufferUsage_Storage,
    eIndirect = WGPUBufferUsage_Indirect,
    eQueryResolve = WGPUBufferUsage_QueryResolve,
};
enum class ColorWriteMask : Flags {
    eNone = WGPUColorWriteMask_None,
    eRed = WGPUColorWriteMask_Red,
    eGreen = WGPUColorWriteMask_Green,
    eBlue = WGPUColorWriteMask_Blue,
    eAlpha = WGPUColorWriteMask_Alpha,
    eAll = WGPUColorWriteMask_All,
};
enum class MapMode : Flags {
    eNone = WGPUMapMode_None,
    eRead = WGPUMapMode_Read,
    eWrite = WGPUMapMode_Write,
};
enum class ShaderStage : Flags {
    eNone = WGPUShaderStage_None,
    eVertex = WGPUShaderStage_Vertex,
    eFragment = WGPUShaderStage_Fragment,
    eCompute = WGPUShaderStage_Compute,
};
enum class TextureUsage : Flags {
    eNone = WGPUTextureUsage_None,
    eCopySrc = WGPUTextureUsage_CopySrc,
    eCopyDst = WGPUTextureUsage_CopyDst,
    eTextureBinding = WGPUTextureUsage_TextureBinding,
    eStorageBinding = WGPUTextureUsage_StorageBinding,
    eRenderAttachment = WGPUTextureUsage_RenderAttachment,
    eTransientAttachment = WGPUTextureUsage_TransientAttachment,
};
std::string_view to_string(AdapterType value);
std::string_view to_string(AddressMode value);
std::string_view to_string(BackendType value);
std::string_view to_string(BlendFactor value);
std::string_view to_string(BlendOperation value);
std::string_view to_string(BufferBindingType value);
std::string_view to_string(BufferMapState value);
std::string_view to_string(CallbackMode value);
std::string_view to_string(CompareFunction value);
std::string_view to_string(CompilationInfoRequestStatus value);
std::string_view to_string(CompilationMessageType value);
std::string_view to_string(ComponentSwizzle value);
std::string_view to_string(CompositeAlphaMode value);
std::string_view to_string(CreatePipelineAsyncStatus value);
std::string_view to_string(CullMode value);
std::string_view to_string(DeviceLostReason value);
std::string_view to_string(ErrorFilter value);
std::string_view to_string(ErrorType value);
std::string_view to_string(FeatureLevel value);
std::string_view to_string(FeatureName value);
std::string_view to_string(FilterMode value);
std::string_view to_string(FrontFace value);
std::string_view to_string(IndexFormat value);
std::string_view to_string(InstanceFeatureName value);
std::string_view to_string(LoadOp value);
std::string_view to_string(MapAsyncStatus value);
std::string_view to_string(MipmapFilterMode value);
std::string_view to_string(OptionalBool value);
std::string_view to_string(PopErrorScopeStatus value);
std::string_view to_string(PowerPreference value);
std::string_view to_string(PredefinedColorSpace value);
std::string_view to_string(PresentMode value);
std::string_view to_string(PrimitiveTopology value);
std::string_view to_string(QueryType value);
std::string_view to_string(QueueWorkDoneStatus value);
std::string_view to_string(RequestAdapterStatus value);
std::string_view to_string(RequestDeviceStatus value);
std::string_view to_string(SamplerBindingType value);
std::string_view to_string(Status value);
std::string_view to_string(StencilOperation value);
std::string_view to_string(StorageTextureAccess value);
std::string_view to_string(StoreOp value);
std::string_view to_string(SType value);
std::string_view to_string(SurfaceGetCurrentTextureStatus value);
std::string_view to_string(TextureAspect value);
std::string_view to_string(TextureDimension value);
std::string_view to_string(TextureFormat value);
std::string_view to_string(TextureSampleType value);
std::string_view to_string(TextureViewDimension value);
std::string_view to_string(ToneMappingMode value);
std::string_view to_string(VertexFormat value);
std::string_view to_string(VertexStepMode value);
std::string_view to_string(WaitStatus value);
std::string_view to_string(WGSLLanguageFeatureName value);
std::string_view to_string(BufferUsage value);
std::string_view to_string(ColorWriteMask value);
std::string_view to_string(MapMode value);
std::string_view to_string(ShaderStage value);
std::string_view to_string(TextureUsage value);
BufferUsage operator|(BufferUsage lhs, BufferUsage rhs);
ColorWriteMask operator|(ColorWriteMask lhs, ColorWriteMask rhs);
MapMode operator|(MapMode lhs, MapMode rhs);
ShaderStage operator|(ShaderStage lhs, ShaderStage rhs);
TextureUsage operator|(TextureUsage lhs, TextureUsage rhs);
}
namespace wgpu {
struct StringView;
struct ChainedStruct;
struct BufferMapCallbackInfo;
struct CompilationInfoCallbackInfo;
struct CreateComputePipelineAsyncCallbackInfo;
struct CreateRenderPipelineAsyncCallbackInfo;
struct DeviceLostCallbackInfo;
struct PopErrorScopeCallbackInfo;
struct QueueWorkDoneCallbackInfo;
struct RequestAdapterCallbackInfo;
struct RequestDeviceCallbackInfo;
struct UncapturedErrorCallbackInfo;
struct AdapterInfo;
struct BlendComponent;
struct BufferBindingLayout;
struct BufferDescriptor;
struct Color;
struct CommandBufferDescriptor;
struct CommandEncoderDescriptor;
struct CompilationMessage;
struct ConstantEntry;
struct Extent3D;
struct ExternalTextureBindingEntry;
struct ExternalTextureBindingLayout;
struct Future;
struct InstanceLimits;
struct Limits;
struct MultisampleState;
struct Origin3D;
struct PassTimestampWrites;
struct PipelineLayoutDescriptor;
struct PrimitiveState;
struct QuerySetDescriptor;
struct QueueDescriptor;
struct RenderBundleDescriptor;
struct RenderBundleEncoderDescriptor;
struct RenderPassDepthStencilAttachment;
struct RenderPassMaxDrawCount;
struct RequestAdapterWebXROptions;
struct SamplerBindingLayout;
struct SamplerDescriptor;
struct ShaderSourceSPIRV;
struct ShaderSourceWGSL;
struct StencilFaceState;
struct StorageTextureBindingLayout;
struct SupportedFeatures;
struct SupportedInstanceFeatures;
struct SupportedWGSLLanguageFeatures;
struct SurfaceCapabilities;
struct SurfaceColorManagement;
struct SurfaceConfiguration;
struct SurfaceSourceAndroidNativeWindow;
struct SurfaceSourceMetalLayer;
struct SurfaceSourceWaylandSurface;
struct SurfaceSourceWindowsHWND;
struct SurfaceSourceXCBWindow;
struct SurfaceSourceXlibWindow;
struct SurfaceTexture;
struct TexelCopyBufferLayout;
struct TextureBindingLayout;
struct TextureComponentSwizzle;
struct TextureViewDescriptor;
struct VertexAttribute;
struct BindGroupEntry;
struct BindGroupLayoutEntry;
struct BlendState;
struct CompilationInfo;
struct ComputePassDescriptor;
struct ComputeState;
struct DepthStencilState;
struct DeviceDescriptor;
struct FutureWaitInfo;
struct InstanceDescriptor;
struct RenderPassColorAttachment;
struct RequestAdapterOptions;
struct ShaderModuleDescriptor;
struct SurfaceDescriptor;
struct TexelCopyBufferInfo;
struct TexelCopyTextureInfo;
struct TextureComponentSwizzleDescriptor;
struct TextureDescriptor;
struct VertexBufferLayout;
struct BindGroupDescriptor;
struct BindGroupLayoutDescriptor;
struct ColorTargetState;
struct ComputePipelineDescriptor;
struct RenderPassDescriptor;
struct VertexState;
struct FragmentState;
struct RenderPipelineDescriptor;
}
namespace wgpu::raw {
class Adapter;
class BindGroup;
class BindGroupLayout;
class Buffer;
class CommandBuffer;
class CommandEncoder;
class ComputePassEncoder;
class ComputePipeline;
class Device;
class ExternalTexture;
class Instance;
class PipelineLayout;
class QuerySet;
class Queue;
class RenderBundle;
class RenderBundleEncoder;
class RenderPassEncoder;
class RenderPipeline;
class Sampler;
class ShaderModule;
class Surface;
class Texture;
class TextureView;
}
namespace wgpu {
class Adapter;
class BindGroup;
class BindGroupLayout;
class Buffer;
class CommandBuffer;
class CommandEncoder;
class ComputePassEncoder;
class ComputePipeline;
class Device;
class ExternalTexture;
class Instance;
class PipelineLayout;
class QuerySet;
class Queue;
class RenderBundle;
class RenderBundleEncoder;
class RenderPassEncoder;
class RenderPipeline;
class Sampler;
class ShaderModule;
class Surface;
class Texture;
class TextureView;
}
namespace wgpu {
struct BufferMapCallback;
struct CompilationInfoCallback;
struct CreateComputePipelineAsyncCallback;
struct CreateRenderPipelineAsyncCallback;
struct DeviceLostCallback;
struct PopErrorScopeCallback;
struct QueueWorkDoneCallback;
struct RequestAdapterCallback;
struct RequestDeviceCallback;
struct UncapturedErrorCallback;
}
namespace wgpu::raw {
class Adapter {
public:
    using wgpu_type = WGPUAdapter;
    Adapter() : m_raw(nullptr) {}
    Adapter(WGPUAdapter raw) : m_raw(raw) {}
    operator WGPUAdapter() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Adapter& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Adapter& other) const { return m_raw != other.m_raw; }
    Adapter& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void getFeatures(wgpu::SupportedFeatures& features) const;
    wgpu::Status getInfo(wgpu::AdapterInfo& info) const;
    wgpu::Status getLimits(wgpu::Limits& limits) const;
    wgpu::Bool hasFeature(wgpu::FeatureName feature) const;
    wgpu::Future requestDevice(wgpu::DeviceDescriptor const* descriptor, wgpu::RequestDeviceCallbackInfo callbackInfo) const;
    void addRef() const;
    void release() const;
private:
    WGPUAdapter m_raw;
};
class BindGroup {
public:
    using wgpu_type = WGPUBindGroup;
    BindGroup() : m_raw(nullptr) {}
    BindGroup(WGPUBindGroup raw) : m_raw(raw) {}
    operator WGPUBindGroup() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const BindGroup& other) const { return m_raw == other.m_raw; }
    bool operator!=(const BindGroup& other) const { return m_raw != other.m_raw; }
    BindGroup& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUBindGroup m_raw;
};
class BindGroupLayout {
public:
    using wgpu_type = WGPUBindGroupLayout;
    BindGroupLayout() : m_raw(nullptr) {}
    BindGroupLayout(WGPUBindGroupLayout raw) : m_raw(raw) {}
    operator WGPUBindGroupLayout() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const BindGroupLayout& other) const { return m_raw == other.m_raw; }
    bool operator!=(const BindGroupLayout& other) const { return m_raw != other.m_raw; }
    BindGroupLayout& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUBindGroupLayout m_raw;
};
class Buffer {
public:
    using wgpu_type = WGPUBuffer;
    Buffer() : m_raw(nullptr) {}
    Buffer(WGPUBuffer raw) : m_raw(raw) {}
    operator WGPUBuffer() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Buffer& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Buffer& other) const { return m_raw != other.m_raw; }
    Buffer& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void destroy() const;
    void const * getConstMappedRange(size_t offset, size_t size) const;
    void * getMappedRange(size_t offset, size_t size) const;
    wgpu::BufferMapState getMapState() const;
    uint64_t getSize() const;
    wgpu::BufferUsage getUsage() const;
    wgpu::Future mapAsync(wgpu::MapMode mode, size_t offset, size_t size, wgpu::BufferMapCallbackInfo callbackInfo) const;
    wgpu::Status readMappedRange(size_t offset, void* data, size_t size) const;
    void setLabel(wgpu::StringView label) const;
    void unmap() const;
    wgpu::Status writeMappedRange(size_t offset, void const* data, size_t size) const;
    void addRef() const;
    void release() const;
private:
    WGPUBuffer m_raw;
};
class CommandBuffer {
public:
    using wgpu_type = WGPUCommandBuffer;
    CommandBuffer() : m_raw(nullptr) {}
    CommandBuffer(WGPUCommandBuffer raw) : m_raw(raw) {}
    operator WGPUCommandBuffer() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const CommandBuffer& other) const { return m_raw == other.m_raw; }
    bool operator!=(const CommandBuffer& other) const { return m_raw != other.m_raw; }
    CommandBuffer& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUCommandBuffer m_raw;
};
class CommandEncoder {
public:
    using wgpu_type = WGPUCommandEncoder;
    CommandEncoder() : m_raw(nullptr) {}
    CommandEncoder(WGPUCommandEncoder raw) : m_raw(raw) {}
    operator WGPUCommandEncoder() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const CommandEncoder& other) const { return m_raw == other.m_raw; }
    bool operator!=(const CommandEncoder& other) const { return m_raw != other.m_raw; }
    CommandEncoder& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::ComputePassEncoder beginComputePass(wgpu::ComputePassDescriptor const& descriptor) const;
    wgpu::ComputePassEncoder beginComputePass() const;
    wgpu::RenderPassEncoder beginRenderPass(wgpu::RenderPassDescriptor const& descriptor) const;
    void clearBuffer(wgpu::Buffer const& buffer, uint64_t offset, uint64_t size) const;
    void copyBufferToBuffer(wgpu::Buffer const& source, uint64_t sourceOffset, wgpu::Buffer const& destination, uint64_t destinationOffset, uint64_t size) const;
    void copyBufferToTexture(wgpu::TexelCopyBufferInfo const& source, wgpu::TexelCopyTextureInfo const& destination, wgpu::Extent3D const& copySize) const;
    void copyTextureToBuffer(wgpu::TexelCopyTextureInfo const& source, wgpu::TexelCopyBufferInfo const& destination, wgpu::Extent3D const& copySize) const;
    void copyTextureToTexture(wgpu::TexelCopyTextureInfo const& source, wgpu::TexelCopyTextureInfo const& destination, wgpu::Extent3D const& copySize) const;
    wgpu::CommandBuffer finish(wgpu::CommandBufferDescriptor const& descriptor) const;
    wgpu::CommandBuffer finish() const;
    void insertDebugMarker(wgpu::StringView markerLabel) const;
    void popDebugGroup() const;
    void pushDebugGroup(wgpu::StringView groupLabel) const;
    void resolveQuerySet(wgpu::QuerySet const& querySet, uint32_t firstQuery, uint32_t queryCount, wgpu::Buffer const& destination, uint64_t destinationOffset) const;
    void setLabel(wgpu::StringView label) const;
    void writeTimestamp(wgpu::QuerySet const& querySet, uint32_t queryIndex) const;
    void addRef() const;
    void release() const;
private:
    WGPUCommandEncoder m_raw;
};
class ComputePassEncoder {
public:
    using wgpu_type = WGPUComputePassEncoder;
    ComputePassEncoder() : m_raw(nullptr) {}
    ComputePassEncoder(WGPUComputePassEncoder raw) : m_raw(raw) {}
    operator WGPUComputePassEncoder() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const ComputePassEncoder& other) const { return m_raw == other.m_raw; }
    bool operator!=(const ComputePassEncoder& other) const { return m_raw != other.m_raw; }
    ComputePassEncoder& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void dispatchWorkgroups(uint32_t workgroupCountX, uint32_t workgroupCountY, uint32_t workgroupCountZ) const;
    void dispatchWorkgroupsIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const;
    void end() const;
    void insertDebugMarker(wgpu::StringView markerLabel) const;
    void popDebugGroup() const;
    void pushDebugGroup(wgpu::StringView groupLabel) const;
    void setBindGroup(uint32_t groupIndex, wgpu::BindGroup const& group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets) const;
    void setLabel(wgpu::StringView label) const;
    void setPipeline(wgpu::ComputePipeline const& pipeline) const;
    void addRef() const;
    void release() const;
private:
    WGPUComputePassEncoder m_raw;
};
class ComputePipeline {
public:
    using wgpu_type = WGPUComputePipeline;
    ComputePipeline() : m_raw(nullptr) {}
    ComputePipeline(WGPUComputePipeline raw) : m_raw(raw) {}
    operator WGPUComputePipeline() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const ComputePipeline& other) const { return m_raw == other.m_raw; }
    bool operator!=(const ComputePipeline& other) const { return m_raw != other.m_raw; }
    ComputePipeline& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::BindGroupLayout getBindGroupLayout(uint32_t groupIndex) const;
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUComputePipeline m_raw;
};
class Device {
public:
    using wgpu_type = WGPUDevice;
    Device() : m_raw(nullptr) {}
    Device(WGPUDevice raw) : m_raw(raw) {}
    operator WGPUDevice() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Device& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Device& other) const { return m_raw != other.m_raw; }
    Device& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::BindGroup createBindGroup(wgpu::BindGroupDescriptor const& descriptor) const;
    wgpu::BindGroupLayout createBindGroupLayout(wgpu::BindGroupLayoutDescriptor const& descriptor) const;
    wgpu::Buffer createBuffer(wgpu::BufferDescriptor const& descriptor) const;
    wgpu::CommandEncoder createCommandEncoder(wgpu::CommandEncoderDescriptor const& descriptor) const;
    wgpu::CommandEncoder createCommandEncoder() const;
    wgpu::ComputePipeline createComputePipeline(wgpu::ComputePipelineDescriptor const& descriptor) const;
    wgpu::Future createComputePipelineAsync(wgpu::ComputePipelineDescriptor const& descriptor, wgpu::CreateComputePipelineAsyncCallbackInfo callbackInfo) const;
    wgpu::PipelineLayout createPipelineLayout(wgpu::PipelineLayoutDescriptor const& descriptor) const;
    wgpu::QuerySet createQuerySet(wgpu::QuerySetDescriptor const& descriptor) const;
    wgpu::RenderBundleEncoder createRenderBundleEncoder(wgpu::RenderBundleEncoderDescriptor const& descriptor) const;
    wgpu::RenderPipeline createRenderPipeline(wgpu::RenderPipelineDescriptor const& descriptor) const;
    wgpu::Future createRenderPipelineAsync(wgpu::RenderPipelineDescriptor const& descriptor, wgpu::CreateRenderPipelineAsyncCallbackInfo callbackInfo) const;
    wgpu::Sampler createSampler(wgpu::SamplerDescriptor const& descriptor) const;
    wgpu::Sampler createSampler() const;
    wgpu::ShaderModule createShaderModule(wgpu::ShaderModuleDescriptor const& descriptor) const;
    wgpu::Texture createTexture(wgpu::TextureDescriptor const& descriptor) const;
    void destroy() const;
    wgpu::Status getAdapterInfo(wgpu::AdapterInfo& adapterInfo) const;
    void getFeatures(wgpu::SupportedFeatures& features) const;
    wgpu::Status getLimits(wgpu::Limits& limits) const;
    wgpu::Future getLostFuture() const;
    wgpu::Queue getQueue() const;
    wgpu::Bool hasFeature(wgpu::FeatureName feature) const;
    wgpu::Future popErrorScope(wgpu::PopErrorScopeCallbackInfo callbackInfo) const;
    void pushErrorScope(wgpu::ErrorFilter filter) const;
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUDevice m_raw;
};
class ExternalTexture {
public:
    using wgpu_type = WGPUExternalTexture;
    ExternalTexture() : m_raw(nullptr) {}
    ExternalTexture(WGPUExternalTexture raw) : m_raw(raw) {}
    operator WGPUExternalTexture() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const ExternalTexture& other) const { return m_raw == other.m_raw; }
    bool operator!=(const ExternalTexture& other) const { return m_raw != other.m_raw; }
    ExternalTexture& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUExternalTexture m_raw;
};
class Instance {
public:
    using wgpu_type = WGPUInstance;
    Instance() : m_raw(nullptr) {}
    Instance(WGPUInstance raw) : m_raw(raw) {}
    operator WGPUInstance() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Instance& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Instance& other) const { return m_raw != other.m_raw; }
    Instance& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::Surface createSurface(wgpu::SurfaceDescriptor const& descriptor) const;
    void getWGSLLanguageFeatures(wgpu::SupportedWGSLLanguageFeatures& features) const;
    wgpu::Bool hasWGSLLanguageFeature(wgpu::WGSLLanguageFeatureName feature) const;
    void processEvents() const;
    wgpu::Future requestAdapter(wgpu::RequestAdapterOptions const* options, wgpu::RequestAdapterCallbackInfo callbackInfo) const;
    wgpu::WaitStatus waitAny(size_t futureCount, wgpu::FutureWaitInfo* futures, uint64_t timeoutNS) const;
    void addRef() const;
    void release() const;
private:
    WGPUInstance m_raw;
};
class PipelineLayout {
public:
    using wgpu_type = WGPUPipelineLayout;
    PipelineLayout() : m_raw(nullptr) {}
    PipelineLayout(WGPUPipelineLayout raw) : m_raw(raw) {}
    operator WGPUPipelineLayout() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const PipelineLayout& other) const { return m_raw == other.m_raw; }
    bool operator!=(const PipelineLayout& other) const { return m_raw != other.m_raw; }
    PipelineLayout& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUPipelineLayout m_raw;
};
class QuerySet {
public:
    using wgpu_type = WGPUQuerySet;
    QuerySet() : m_raw(nullptr) {}
    QuerySet(WGPUQuerySet raw) : m_raw(raw) {}
    operator WGPUQuerySet() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const QuerySet& other) const { return m_raw == other.m_raw; }
    bool operator!=(const QuerySet& other) const { return m_raw != other.m_raw; }
    QuerySet& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void destroy() const;
    uint32_t getCount() const;
    wgpu::QueryType getType() const;
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUQuerySet m_raw;
};
class Queue {
public:
    using wgpu_type = WGPUQueue;
    Queue() : m_raw(nullptr) {}
    Queue(WGPUQueue raw) : m_raw(raw) {}
    operator WGPUQueue() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Queue& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Queue& other) const { return m_raw != other.m_raw; }
    Queue& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::Future onSubmittedWorkDone(wgpu::QueueWorkDoneCallbackInfo callbackInfo) const;
    void setLabel(wgpu::StringView label) const;
    void submit(size_t commandCount, wgpu::CommandBuffer const& commands) const;
    void writeBuffer(wgpu::Buffer const& buffer, uint64_t bufferOffset, void const* data, size_t size) const;
    void writeTexture(wgpu::TexelCopyTextureInfo const& destination, void const* data, size_t dataSize, wgpu::TexelCopyBufferLayout const& dataLayout, wgpu::Extent3D const& writeSize) const;
    void addRef() const;
    void release() const;
private:
    WGPUQueue m_raw;
};
class RenderBundle {
public:
    using wgpu_type = WGPURenderBundle;
    RenderBundle() : m_raw(nullptr) {}
    RenderBundle(WGPURenderBundle raw) : m_raw(raw) {}
    operator WGPURenderBundle() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const RenderBundle& other) const { return m_raw == other.m_raw; }
    bool operator!=(const RenderBundle& other) const { return m_raw != other.m_raw; }
    RenderBundle& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPURenderBundle m_raw;
};
class RenderBundleEncoder {
public:
    using wgpu_type = WGPURenderBundleEncoder;
    RenderBundleEncoder() : m_raw(nullptr) {}
    RenderBundleEncoder(WGPURenderBundleEncoder raw) : m_raw(raw) {}
    operator WGPURenderBundleEncoder() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const RenderBundleEncoder& other) const { return m_raw == other.m_raw; }
    bool operator!=(const RenderBundleEncoder& other) const { return m_raw != other.m_raw; }
    RenderBundleEncoder& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
    void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) const;
    void drawIndexedIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const;
    void drawIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const;
    wgpu::RenderBundle finish(wgpu::RenderBundleDescriptor const& descriptor) const;
    wgpu::RenderBundle finish() const;
    void insertDebugMarker(wgpu::StringView markerLabel) const;
    void popDebugGroup() const;
    void pushDebugGroup(wgpu::StringView groupLabel) const;
    void setBindGroup(uint32_t groupIndex, wgpu::BindGroup const& group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets) const;
    void setIndexBuffer(wgpu::Buffer const& buffer, wgpu::IndexFormat format, uint64_t offset, uint64_t size) const;
    void setLabel(wgpu::StringView label) const;
    void setPipeline(wgpu::RenderPipeline const& pipeline) const;
    void setVertexBuffer(uint32_t slot, wgpu::Buffer const& buffer, uint64_t offset, uint64_t size) const;
    void addRef() const;
    void release() const;
private:
    WGPURenderBundleEncoder m_raw;
};
class RenderPassEncoder {
public:
    using wgpu_type = WGPURenderPassEncoder;
    RenderPassEncoder() : m_raw(nullptr) {}
    RenderPassEncoder(WGPURenderPassEncoder raw) : m_raw(raw) {}
    operator WGPURenderPassEncoder() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const RenderPassEncoder& other) const { return m_raw == other.m_raw; }
    bool operator!=(const RenderPassEncoder& other) const { return m_raw != other.m_raw; }
    RenderPassEncoder& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void beginOcclusionQuery(uint32_t queryIndex) const;
    void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
    void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) const;
    void drawIndexedIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const;
    void drawIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const;
    void end() const;
    void endOcclusionQuery() const;
    void executeBundles(size_t bundleCount, wgpu::RenderBundle const& bundles) const;
    void insertDebugMarker(wgpu::StringView markerLabel) const;
    void popDebugGroup() const;
    void pushDebugGroup(wgpu::StringView groupLabel) const;
    void setBindGroup(uint32_t groupIndex, wgpu::BindGroup const& group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets) const;
    void setBlendConstant(wgpu::Color const& color) const;
    void setIndexBuffer(wgpu::Buffer const& buffer, wgpu::IndexFormat format, uint64_t offset, uint64_t size) const;
    void setLabel(wgpu::StringView label) const;
    void setPipeline(wgpu::RenderPipeline const& pipeline) const;
    void setScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const;
    void setStencilReference(uint32_t reference) const;
    void setVertexBuffer(uint32_t slot, wgpu::Buffer const& buffer, uint64_t offset, uint64_t size) const;
    void setViewport(float x, float y, float width, float height, float minDepth, float maxDepth) const;
    void addRef() const;
    void release() const;
private:
    WGPURenderPassEncoder m_raw;
};
class RenderPipeline {
public:
    using wgpu_type = WGPURenderPipeline;
    RenderPipeline() : m_raw(nullptr) {}
    RenderPipeline(WGPURenderPipeline raw) : m_raw(raw) {}
    operator WGPURenderPipeline() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const RenderPipeline& other) const { return m_raw == other.m_raw; }
    bool operator!=(const RenderPipeline& other) const { return m_raw != other.m_raw; }
    RenderPipeline& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::BindGroupLayout getBindGroupLayout(uint32_t groupIndex) const;
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPURenderPipeline m_raw;
};
class Sampler {
public:
    using wgpu_type = WGPUSampler;
    Sampler() : m_raw(nullptr) {}
    Sampler(WGPUSampler raw) : m_raw(raw) {}
    operator WGPUSampler() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Sampler& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Sampler& other) const { return m_raw != other.m_raw; }
    Sampler& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUSampler m_raw;
};
class ShaderModule {
public:
    using wgpu_type = WGPUShaderModule;
    ShaderModule() : m_raw(nullptr) {}
    ShaderModule(WGPUShaderModule raw) : m_raw(raw) {}
    operator WGPUShaderModule() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const ShaderModule& other) const { return m_raw == other.m_raw; }
    bool operator!=(const ShaderModule& other) const { return m_raw != other.m_raw; }
    ShaderModule& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::Future getCompilationInfo(wgpu::CompilationInfoCallbackInfo callbackInfo) const;
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUShaderModule m_raw;
};
class Surface {
public:
    using wgpu_type = WGPUSurface;
    Surface() : m_raw(nullptr) {}
    Surface(WGPUSurface raw) : m_raw(raw) {}
    operator WGPUSurface() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Surface& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Surface& other) const { return m_raw != other.m_raw; }
    Surface& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void configure(wgpu::SurfaceConfiguration const& config) const;
    wgpu::Status getCapabilities(wgpu::Adapter const& adapter, wgpu::SurfaceCapabilities& capabilities) const;
    void getCurrentTexture(wgpu::SurfaceTexture& surfaceTexture) const;
    wgpu::Status present() const;
    void setLabel(wgpu::StringView label) const;
    void unconfigure() const;
    void addRef() const;
    void release() const;
private:
    WGPUSurface m_raw;
};
class Texture {
public:
    using wgpu_type = WGPUTexture;
    Texture() : m_raw(nullptr) {}
    Texture(WGPUTexture raw) : m_raw(raw) {}
    operator WGPUTexture() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const Texture& other) const { return m_raw == other.m_raw; }
    bool operator!=(const Texture& other) const { return m_raw != other.m_raw; }
    Texture& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    wgpu::TextureView createView(wgpu::TextureViewDescriptor const& descriptor) const;
    wgpu::TextureView createView() const;
    void destroy() const;
    uint32_t getDepthOrArrayLayers() const;
    wgpu::TextureDimension getDimension() const;
    wgpu::TextureFormat getFormat() const;
    uint32_t getHeight() const;
    uint32_t getMipLevelCount() const;
    uint32_t getSampleCount() const;
    wgpu::TextureViewDimension getTextureBindingViewDimension() const;
    wgpu::TextureUsage getUsage() const;
    uint32_t getWidth() const;
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUTexture m_raw;
};
class TextureView {
public:
    using wgpu_type = WGPUTextureView;
    TextureView() : m_raw(nullptr) {}
    TextureView(WGPUTextureView raw) : m_raw(raw) {}
    operator WGPUTextureView() const { return m_raw; }
    operator bool() const { return m_raw != nullptr; }
    bool operator==(const TextureView& other) const { return m_raw == other.m_raw; }
    bool operator!=(const TextureView& other) const { return m_raw != other.m_raw; }
    TextureView& operator=(std::nullptr_t) { m_raw = nullptr; return *this; }
    void setLabel(wgpu::StringView label) const;
    void addRef() const;
    void release() const;
private:
    WGPUTextureView m_raw;
};
}
#define WEBGPU_RAII_FRIENDS \
    friend class raw::Adapter; \
    friend class raw::BindGroup; \
    friend class raw::BindGroupLayout; \
    friend class raw::Buffer; \
    friend class raw::CommandBuffer; \
    friend class raw::CommandEncoder; \
    friend class raw::ComputePassEncoder; \
    friend class raw::ComputePipeline; \
    friend class raw::Device; \
    friend class raw::ExternalTexture; \
    friend class raw::Instance; \
    friend class raw::PipelineLayout; \
    friend class raw::QuerySet; \
    friend class raw::Queue; \
    friend class raw::RenderBundle; \
    friend class raw::RenderBundleEncoder; \
    friend class raw::RenderPassEncoder; \
    friend class raw::RenderPipeline; \
    friend class raw::Sampler; \
    friend class raw::ShaderModule; \
    friend class raw::Surface; \
    friend class raw::Texture; \
    friend class raw::TextureView; \
    friend struct AdapterInfo; \
    friend struct Future; \
    friend struct InstanceLimits; \
    friend struct Limits; \
    friend struct SupportedFeatures; \
    friend struct SupportedInstanceFeatures; \
    friend struct SupportedWGSLLanguageFeatures; \
    friend struct SurfaceCapabilities; \
    friend struct SurfaceTexture; \
    friend struct FutureWaitInfo; \
    friend wgpu::Instance createInstance(wgpu::InstanceDescriptor const& descriptor); \
    friend wgpu::Instance createInstance(); \
    friend void getInstanceFeatures(wgpu::SupportedInstanceFeatures& features); \
    friend wgpu::Status getInstanceLimits(wgpu::InstanceLimits& limits); \
    friend wgpu::Bool hasInstanceFeature(wgpu::InstanceFeatureName feature);
namespace wgpu {
class Adapter : public raw::Adapter {
public:
    using base_type = raw::Adapter;
    using wgpu_type = WGPUAdapter;
    Adapter() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Adapter(wgpu_type raw) : base_type(raw) {}
    Adapter(base_type raw) : base_type(raw) {}
    Adapter& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Adapter& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Adapter(const Adapter& other) : base_type(other) { if (*this) this->addRef(); }
    Adapter(Adapter&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Adapter& operator=(const Adapter& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Adapter& operator=(Adapter&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Adapter() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Adapter& other) const { return base_type::operator==(other); }
    bool operator!=(const Adapter& other) const { return base_type::operator!=(other); }
    Adapter clone() const { this->addRef(); return Adapter((const base_type&)*this); }
};
class BindGroup : public raw::BindGroup {
public:
    using base_type = raw::BindGroup;
    using wgpu_type = WGPUBindGroup;
    BindGroup() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    BindGroup(wgpu_type raw) : base_type(raw) {}
    BindGroup(base_type raw) : base_type(raw) {}
    BindGroup& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    BindGroup& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    BindGroup(const BindGroup& other) : base_type(other) { if (*this) this->addRef(); }
    BindGroup(BindGroup&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    BindGroup& operator=(const BindGroup& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    BindGroup& operator=(BindGroup&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~BindGroup() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const BindGroup& other) const { return base_type::operator==(other); }
    bool operator!=(const BindGroup& other) const { return base_type::operator!=(other); }
    BindGroup clone() const { this->addRef(); return BindGroup((const base_type&)*this); }
};
class BindGroupLayout : public raw::BindGroupLayout {
public:
    using base_type = raw::BindGroupLayout;
    using wgpu_type = WGPUBindGroupLayout;
    BindGroupLayout() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    BindGroupLayout(wgpu_type raw) : base_type(raw) {}
    BindGroupLayout(base_type raw) : base_type(raw) {}
    BindGroupLayout& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    BindGroupLayout& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    BindGroupLayout(const BindGroupLayout& other) : base_type(other) { if (*this) this->addRef(); }
    BindGroupLayout(BindGroupLayout&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    BindGroupLayout& operator=(const BindGroupLayout& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    BindGroupLayout& operator=(BindGroupLayout&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~BindGroupLayout() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const BindGroupLayout& other) const { return base_type::operator==(other); }
    bool operator!=(const BindGroupLayout& other) const { return base_type::operator!=(other); }
    BindGroupLayout clone() const { this->addRef(); return BindGroupLayout((const base_type&)*this); }
};
class Buffer : public raw::Buffer {
public:
    using base_type = raw::Buffer;
    using wgpu_type = WGPUBuffer;
    Buffer() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Buffer(wgpu_type raw) : base_type(raw) {}
    Buffer(base_type raw) : base_type(raw) {}
    Buffer& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Buffer& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Buffer(const Buffer& other) : base_type(other) { if (*this) this->addRef(); }
    Buffer(Buffer&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Buffer& operator=(const Buffer& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Buffer& operator=(Buffer&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Buffer() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Buffer& other) const { return base_type::operator==(other); }
    bool operator!=(const Buffer& other) const { return base_type::operator!=(other); }
    Buffer clone() const { this->addRef(); return Buffer((const base_type&)*this); }
};
class CommandBuffer : public raw::CommandBuffer {
public:
    using base_type = raw::CommandBuffer;
    using wgpu_type = WGPUCommandBuffer;
    CommandBuffer() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    CommandBuffer(wgpu_type raw) : base_type(raw) {}
    CommandBuffer(base_type raw) : base_type(raw) {}
    CommandBuffer& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    CommandBuffer& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    CommandBuffer(const CommandBuffer& other) : base_type(other) { if (*this) this->addRef(); }
    CommandBuffer(CommandBuffer&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    CommandBuffer& operator=(const CommandBuffer& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    CommandBuffer& operator=(CommandBuffer&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~CommandBuffer() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const CommandBuffer& other) const { return base_type::operator==(other); }
    bool operator!=(const CommandBuffer& other) const { return base_type::operator!=(other); }
    CommandBuffer clone() const { this->addRef(); return CommandBuffer((const base_type&)*this); }
};
class CommandEncoder : public raw::CommandEncoder {
public:
    using base_type = raw::CommandEncoder;
    using wgpu_type = WGPUCommandEncoder;
    CommandEncoder() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    CommandEncoder(wgpu_type raw) : base_type(raw) {}
    CommandEncoder(base_type raw) : base_type(raw) {}
    CommandEncoder& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    CommandEncoder& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    CommandEncoder(const CommandEncoder& other) : base_type(other) { if (*this) this->addRef(); }
    CommandEncoder(CommandEncoder&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    CommandEncoder& operator=(const CommandEncoder& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    CommandEncoder& operator=(CommandEncoder&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~CommandEncoder() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const CommandEncoder& other) const { return base_type::operator==(other); }
    bool operator!=(const CommandEncoder& other) const { return base_type::operator!=(other); }
    CommandEncoder clone() const { this->addRef(); return CommandEncoder((const base_type&)*this); }
};
class ComputePassEncoder : public raw::ComputePassEncoder {
public:
    using base_type = raw::ComputePassEncoder;
    using wgpu_type = WGPUComputePassEncoder;
    ComputePassEncoder() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    ComputePassEncoder(wgpu_type raw) : base_type(raw) {}
    ComputePassEncoder(base_type raw) : base_type(raw) {}
    ComputePassEncoder& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    ComputePassEncoder& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    ComputePassEncoder(const ComputePassEncoder& other) : base_type(other) { if (*this) this->addRef(); }
    ComputePassEncoder(ComputePassEncoder&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    ComputePassEncoder& operator=(const ComputePassEncoder& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    ComputePassEncoder& operator=(ComputePassEncoder&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~ComputePassEncoder() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const ComputePassEncoder& other) const { return base_type::operator==(other); }
    bool operator!=(const ComputePassEncoder& other) const { return base_type::operator!=(other); }
    ComputePassEncoder clone() const { this->addRef(); return ComputePassEncoder((const base_type&)*this); }
};
class ComputePipeline : public raw::ComputePipeline {
public:
    using base_type = raw::ComputePipeline;
    using wgpu_type = WGPUComputePipeline;
    ComputePipeline() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    ComputePipeline(wgpu_type raw) : base_type(raw) {}
    ComputePipeline(base_type raw) : base_type(raw) {}
    ComputePipeline& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    ComputePipeline& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    ComputePipeline(const ComputePipeline& other) : base_type(other) { if (*this) this->addRef(); }
    ComputePipeline(ComputePipeline&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    ComputePipeline& operator=(const ComputePipeline& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    ComputePipeline& operator=(ComputePipeline&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~ComputePipeline() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const ComputePipeline& other) const { return base_type::operator==(other); }
    bool operator!=(const ComputePipeline& other) const { return base_type::operator!=(other); }
    ComputePipeline clone() const { this->addRef(); return ComputePipeline((const base_type&)*this); }
};
class Device : public raw::Device {
public:
    using base_type = raw::Device;
    using wgpu_type = WGPUDevice;
    Device() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Device(wgpu_type raw) : base_type(raw) {}
    Device(base_type raw) : base_type(raw) {}
    Device& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Device& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Device(const Device& other) : base_type(other) { if (*this) this->addRef(); }
    Device(Device&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Device& operator=(const Device& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Device& operator=(Device&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Device() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Device& other) const { return base_type::operator==(other); }
    bool operator!=(const Device& other) const { return base_type::operator!=(other); }
    Device clone() const { this->addRef(); return Device((const base_type&)*this); }
};
class ExternalTexture : public raw::ExternalTexture {
public:
    using base_type = raw::ExternalTexture;
    using wgpu_type = WGPUExternalTexture;
    ExternalTexture() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    ExternalTexture(wgpu_type raw) : base_type(raw) {}
    ExternalTexture(base_type raw) : base_type(raw) {}
    ExternalTexture& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    ExternalTexture& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    ExternalTexture(const ExternalTexture& other) : base_type(other) { if (*this) this->addRef(); }
    ExternalTexture(ExternalTexture&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    ExternalTexture& operator=(const ExternalTexture& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    ExternalTexture& operator=(ExternalTexture&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~ExternalTexture() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const ExternalTexture& other) const { return base_type::operator==(other); }
    bool operator!=(const ExternalTexture& other) const { return base_type::operator!=(other); }
    ExternalTexture clone() const { this->addRef(); return ExternalTexture((const base_type&)*this); }
};
class Instance : public raw::Instance {
public:
    using base_type = raw::Instance;
    using wgpu_type = WGPUInstance;
    Instance() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Instance(wgpu_type raw) : base_type(raw) {}
    Instance(base_type raw) : base_type(raw) {}
    Instance& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Instance& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Instance(const Instance& other) : base_type(other) { if (*this) this->addRef(); }
    Instance(Instance&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Instance& operator=(const Instance& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Instance& operator=(Instance&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Instance() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Instance& other) const { return base_type::operator==(other); }
    bool operator!=(const Instance& other) const { return base_type::operator!=(other); }
    Instance clone() const { this->addRef(); return Instance((const base_type&)*this); }
};
class PipelineLayout : public raw::PipelineLayout {
public:
    using base_type = raw::PipelineLayout;
    using wgpu_type = WGPUPipelineLayout;
    PipelineLayout() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    PipelineLayout(wgpu_type raw) : base_type(raw) {}
    PipelineLayout(base_type raw) : base_type(raw) {}
    PipelineLayout& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    PipelineLayout& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    PipelineLayout(const PipelineLayout& other) : base_type(other) { if (*this) this->addRef(); }
    PipelineLayout(PipelineLayout&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    PipelineLayout& operator=(const PipelineLayout& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    PipelineLayout& operator=(PipelineLayout&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~PipelineLayout() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const PipelineLayout& other) const { return base_type::operator==(other); }
    bool operator!=(const PipelineLayout& other) const { return base_type::operator!=(other); }
    PipelineLayout clone() const { this->addRef(); return PipelineLayout((const base_type&)*this); }
};
class QuerySet : public raw::QuerySet {
public:
    using base_type = raw::QuerySet;
    using wgpu_type = WGPUQuerySet;
    QuerySet() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    QuerySet(wgpu_type raw) : base_type(raw) {}
    QuerySet(base_type raw) : base_type(raw) {}
    QuerySet& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    QuerySet& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    QuerySet(const QuerySet& other) : base_type(other) { if (*this) this->addRef(); }
    QuerySet(QuerySet&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    QuerySet& operator=(const QuerySet& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    QuerySet& operator=(QuerySet&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~QuerySet() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const QuerySet& other) const { return base_type::operator==(other); }
    bool operator!=(const QuerySet& other) const { return base_type::operator!=(other); }
    QuerySet clone() const { this->addRef(); return QuerySet((const base_type&)*this); }
};
class Queue : public raw::Queue {
public:
    using base_type = raw::Queue;
    using wgpu_type = WGPUQueue;
    Queue() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Queue(wgpu_type raw) : base_type(raw) {}
    Queue(base_type raw) : base_type(raw) {}
    Queue& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Queue& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Queue(const Queue& other) : base_type(other) { if (*this) this->addRef(); }
    Queue(Queue&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Queue& operator=(const Queue& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Queue& operator=(Queue&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Queue() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Queue& other) const { return base_type::operator==(other); }
    bool operator!=(const Queue& other) const { return base_type::operator!=(other); }
    Queue clone() const { this->addRef(); return Queue((const base_type&)*this); }
};
class RenderBundle : public raw::RenderBundle {
public:
    using base_type = raw::RenderBundle;
    using wgpu_type = WGPURenderBundle;
    RenderBundle() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    RenderBundle(wgpu_type raw) : base_type(raw) {}
    RenderBundle(base_type raw) : base_type(raw) {}
    RenderBundle& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    RenderBundle& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    RenderBundle(const RenderBundle& other) : base_type(other) { if (*this) this->addRef(); }
    RenderBundle(RenderBundle&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    RenderBundle& operator=(const RenderBundle& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    RenderBundle& operator=(RenderBundle&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~RenderBundle() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const RenderBundle& other) const { return base_type::operator==(other); }
    bool operator!=(const RenderBundle& other) const { return base_type::operator!=(other); }
    RenderBundle clone() const { this->addRef(); return RenderBundle((const base_type&)*this); }
};
class RenderBundleEncoder : public raw::RenderBundleEncoder {
public:
    using base_type = raw::RenderBundleEncoder;
    using wgpu_type = WGPURenderBundleEncoder;
    RenderBundleEncoder() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    RenderBundleEncoder(wgpu_type raw) : base_type(raw) {}
    RenderBundleEncoder(base_type raw) : base_type(raw) {}
    RenderBundleEncoder& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    RenderBundleEncoder& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    RenderBundleEncoder(const RenderBundleEncoder& other) : base_type(other) { if (*this) this->addRef(); }
    RenderBundleEncoder(RenderBundleEncoder&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    RenderBundleEncoder& operator=(const RenderBundleEncoder& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    RenderBundleEncoder& operator=(RenderBundleEncoder&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~RenderBundleEncoder() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const RenderBundleEncoder& other) const { return base_type::operator==(other); }
    bool operator!=(const RenderBundleEncoder& other) const { return base_type::operator!=(other); }
    RenderBundleEncoder clone() const { this->addRef(); return RenderBundleEncoder((const base_type&)*this); }
};
class RenderPassEncoder : public raw::RenderPassEncoder {
public:
    using base_type = raw::RenderPassEncoder;
    using wgpu_type = WGPURenderPassEncoder;
    RenderPassEncoder() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    RenderPassEncoder(wgpu_type raw) : base_type(raw) {}
    RenderPassEncoder(base_type raw) : base_type(raw) {}
    RenderPassEncoder& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    RenderPassEncoder& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    RenderPassEncoder(const RenderPassEncoder& other) : base_type(other) { if (*this) this->addRef(); }
    RenderPassEncoder(RenderPassEncoder&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    RenderPassEncoder& operator=(const RenderPassEncoder& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    RenderPassEncoder& operator=(RenderPassEncoder&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~RenderPassEncoder() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const RenderPassEncoder& other) const { return base_type::operator==(other); }
    bool operator!=(const RenderPassEncoder& other) const { return base_type::operator!=(other); }
    RenderPassEncoder clone() const { this->addRef(); return RenderPassEncoder((const base_type&)*this); }
};
class RenderPipeline : public raw::RenderPipeline {
public:
    using base_type = raw::RenderPipeline;
    using wgpu_type = WGPURenderPipeline;
    RenderPipeline() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    RenderPipeline(wgpu_type raw) : base_type(raw) {}
    RenderPipeline(base_type raw) : base_type(raw) {}
    RenderPipeline& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    RenderPipeline& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    RenderPipeline(const RenderPipeline& other) : base_type(other) { if (*this) this->addRef(); }
    RenderPipeline(RenderPipeline&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    RenderPipeline& operator=(const RenderPipeline& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    RenderPipeline& operator=(RenderPipeline&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~RenderPipeline() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const RenderPipeline& other) const { return base_type::operator==(other); }
    bool operator!=(const RenderPipeline& other) const { return base_type::operator!=(other); }
    RenderPipeline clone() const { this->addRef(); return RenderPipeline((const base_type&)*this); }
};
class Sampler : public raw::Sampler {
public:
    using base_type = raw::Sampler;
    using wgpu_type = WGPUSampler;
    Sampler() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Sampler(wgpu_type raw) : base_type(raw) {}
    Sampler(base_type raw) : base_type(raw) {}
    Sampler& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Sampler& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Sampler(const Sampler& other) : base_type(other) { if (*this) this->addRef(); }
    Sampler(Sampler&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Sampler& operator=(const Sampler& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Sampler& operator=(Sampler&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Sampler() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Sampler& other) const { return base_type::operator==(other); }
    bool operator!=(const Sampler& other) const { return base_type::operator!=(other); }
    Sampler clone() const { this->addRef(); return Sampler((const base_type&)*this); }
};
class ShaderModule : public raw::ShaderModule {
public:
    using base_type = raw::ShaderModule;
    using wgpu_type = WGPUShaderModule;
    ShaderModule() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    ShaderModule(wgpu_type raw) : base_type(raw) {}
    ShaderModule(base_type raw) : base_type(raw) {}
    ShaderModule& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    ShaderModule& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    ShaderModule(const ShaderModule& other) : base_type(other) { if (*this) this->addRef(); }
    ShaderModule(ShaderModule&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    ShaderModule& operator=(const ShaderModule& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    ShaderModule& operator=(ShaderModule&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~ShaderModule() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const ShaderModule& other) const { return base_type::operator==(other); }
    bool operator!=(const ShaderModule& other) const { return base_type::operator!=(other); }
    ShaderModule clone() const { this->addRef(); return ShaderModule((const base_type&)*this); }
};
class Surface : public raw::Surface {
public:
    using base_type = raw::Surface;
    using wgpu_type = WGPUSurface;
    Surface() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Surface(wgpu_type raw) : base_type(raw) {}
    Surface(base_type raw) : base_type(raw) {}
    Surface& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Surface& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Surface(const Surface& other) : base_type(other) { if (*this) this->addRef(); }
    Surface(Surface&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Surface& operator=(const Surface& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Surface& operator=(Surface&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Surface() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Surface& other) const { return base_type::operator==(other); }
    bool operator!=(const Surface& other) const { return base_type::operator!=(other); }
    Surface clone() const { this->addRef(); return Surface((const base_type&)*this); }
};
class Texture : public raw::Texture {
public:
    using base_type = raw::Texture;
    using wgpu_type = WGPUTexture;
    Texture() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    Texture(wgpu_type raw) : base_type(raw) {}
    Texture(base_type raw) : base_type(raw) {}
    Texture& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    Texture& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    Texture(const Texture& other) : base_type(other) { if (*this) this->addRef(); }
    Texture(Texture&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    Texture& operator=(const Texture& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    Texture& operator=(Texture&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~Texture() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const Texture& other) const { return base_type::operator==(other); }
    bool operator!=(const Texture& other) const { return base_type::operator!=(other); }
    Texture clone() const { this->addRef(); return Texture((const base_type&)*this); }
};
class TextureView : public raw::TextureView {
public:
    using base_type = raw::TextureView;
    using wgpu_type = WGPUTextureView;
    TextureView() : base_type() {}
    WEBGPU_RAII_FRIENDS
private:
    TextureView(wgpu_type raw) : base_type(raw) {}
    TextureView(base_type raw) : base_type(raw) {}
    TextureView& operator=(const base_type& raw) { if (*this) this->release(); base_type::operator=(raw); return *this; }
public:
    TextureView& operator=(std::nullptr_t) { if (*this) this->release(); base_type::operator=(nullptr); return *this; }
    TextureView(const TextureView& other) : base_type(other) { if (*this) this->addRef(); }
    TextureView(TextureView&& other) : base_type(other) { (base_type&)(other) = nullptr; }
    TextureView& operator=(const TextureView& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); } return *this; }
    TextureView& operator=(TextureView&& other) { if (this != &other) { if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; } return *this; }
    ~TextureView() { if (*this) this->release(); }
    operator bool() const { return base_type::operator bool(); }
    bool operator==(const TextureView& other) const { return base_type::operator==(other); }
    bool operator!=(const TextureView& other) const { return base_type::operator!=(other); }
    TextureView clone() const { this->addRef(); return TextureView((const base_type&)*this); }
};
}
#undef WEBGPU_RAII_FRIENDS
namespace wgpu {
struct BufferMapCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::MapAsyncStatus status, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPUMapAsyncStatus status, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::MapAsyncStatus status, wgpu::StringView message) const override;
    };
    Control* data;
public:
    BufferMapCallback() : data(nullptr) {}
    BufferMapCallback(WGPUBufferMapCallback native, void* userdata1, void* userdata2);
    BufferMapCallback(const BufferMapCallback& other) : data(other.data) { if (data) ++data->count; }
    BufferMapCallback(BufferMapCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    BufferMapCallback& operator=(const BufferMapCallback& other);
    BufferMapCallback& operator=(BufferMapCallback&& other);
    BufferMapCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~BufferMapCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::MapAsyncStatus, wgpu::StringView> F>
    BufferMapCallback(const F& f);
    void operator()(wgpu::MapAsyncStatus status, wgpu::StringView message) const;
    void operator()(WGPUMapAsyncStatus status, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct CompilationInfoCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::CompilationInfoRequestStatus status, wgpu::CompilationInfo const& compilationInfo) const = 0;
        virtual void invoke_c(WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const* compilationInfo) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::CompilationInfoRequestStatus status, wgpu::CompilationInfo const& compilationInfo) const override;
    };
    Control* data;
public:
    CompilationInfoCallback() : data(nullptr) {}
    CompilationInfoCallback(WGPUCompilationInfoCallback native, void* userdata1, void* userdata2);
    CompilationInfoCallback(const CompilationInfoCallback& other) : data(other.data) { if (data) ++data->count; }
    CompilationInfoCallback(CompilationInfoCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    CompilationInfoCallback& operator=(const CompilationInfoCallback& other);
    CompilationInfoCallback& operator=(CompilationInfoCallback&& other);
    CompilationInfoCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~CompilationInfoCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::CompilationInfoRequestStatus, wgpu::CompilationInfo const&> F>
    CompilationInfoCallback(const F& f);
    void operator()(wgpu::CompilationInfoRequestStatus status, wgpu::CompilationInfo const& compilationInfo) const;
    void operator()(WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const* compilationInfo) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct CreateComputePipelineAsyncCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::ComputePipeline const& pipeline, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::ComputePipeline const& pipeline, wgpu::StringView message) const override;
    };
    Control* data;
public:
    CreateComputePipelineAsyncCallback() : data(nullptr) {}
    CreateComputePipelineAsyncCallback(WGPUCreateComputePipelineAsyncCallback native, void* userdata1, void* userdata2);
    CreateComputePipelineAsyncCallback(const CreateComputePipelineAsyncCallback& other) : data(other.data) { if (data) ++data->count; }
    CreateComputePipelineAsyncCallback(CreateComputePipelineAsyncCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    CreateComputePipelineAsyncCallback& operator=(const CreateComputePipelineAsyncCallback& other);
    CreateComputePipelineAsyncCallback& operator=(CreateComputePipelineAsyncCallback&& other);
    CreateComputePipelineAsyncCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~CreateComputePipelineAsyncCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::CreatePipelineAsyncStatus, wgpu::ComputePipeline const&, wgpu::StringView> F>
    CreateComputePipelineAsyncCallback(const F& f);
    void operator()(wgpu::CreatePipelineAsyncStatus status, wgpu::ComputePipeline const& pipeline, wgpu::StringView message) const;
    void operator()(WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct CreateRenderPipelineAsyncCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::RenderPipeline const& pipeline, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::RenderPipeline const& pipeline, wgpu::StringView message) const override;
    };
    Control* data;
public:
    CreateRenderPipelineAsyncCallback() : data(nullptr) {}
    CreateRenderPipelineAsyncCallback(WGPUCreateRenderPipelineAsyncCallback native, void* userdata1, void* userdata2);
    CreateRenderPipelineAsyncCallback(const CreateRenderPipelineAsyncCallback& other) : data(other.data) { if (data) ++data->count; }
    CreateRenderPipelineAsyncCallback(CreateRenderPipelineAsyncCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    CreateRenderPipelineAsyncCallback& operator=(const CreateRenderPipelineAsyncCallback& other);
    CreateRenderPipelineAsyncCallback& operator=(CreateRenderPipelineAsyncCallback&& other);
    CreateRenderPipelineAsyncCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~CreateRenderPipelineAsyncCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::CreatePipelineAsyncStatus, wgpu::RenderPipeline const&, wgpu::StringView> F>
    CreateRenderPipelineAsyncCallback(const F& f);
    void operator()(wgpu::CreatePipelineAsyncStatus status, wgpu::RenderPipeline const& pipeline, wgpu::StringView message) const;
    void operator()(WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct DeviceLostCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::Device const& device, wgpu::DeviceLostReason reason, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::Device const& device, wgpu::DeviceLostReason reason, wgpu::StringView message) const override;
    };
    Control* data;
public:
    DeviceLostCallback() : data(nullptr) {}
    DeviceLostCallback(WGPUDeviceLostCallback native, void* userdata1, void* userdata2);
    DeviceLostCallback(const DeviceLostCallback& other) : data(other.data) { if (data) ++data->count; }
    DeviceLostCallback(DeviceLostCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    DeviceLostCallback& operator=(const DeviceLostCallback& other);
    DeviceLostCallback& operator=(DeviceLostCallback&& other);
    DeviceLostCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~DeviceLostCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::Device const&, wgpu::DeviceLostReason, wgpu::StringView> F>
    DeviceLostCallback(const F& f);
    void operator()(wgpu::Device const& device, wgpu::DeviceLostReason reason, wgpu::StringView message) const;
    void operator()(WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct PopErrorScopeCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::PopErrorScopeStatus status, wgpu::ErrorType type, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPUPopErrorScopeStatus status, WGPUErrorType type, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::PopErrorScopeStatus status, wgpu::ErrorType type, wgpu::StringView message) const override;
    };
    Control* data;
public:
    PopErrorScopeCallback() : data(nullptr) {}
    PopErrorScopeCallback(WGPUPopErrorScopeCallback native, void* userdata1, void* userdata2);
    PopErrorScopeCallback(const PopErrorScopeCallback& other) : data(other.data) { if (data) ++data->count; }
    PopErrorScopeCallback(PopErrorScopeCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    PopErrorScopeCallback& operator=(const PopErrorScopeCallback& other);
    PopErrorScopeCallback& operator=(PopErrorScopeCallback&& other);
    PopErrorScopeCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~PopErrorScopeCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::PopErrorScopeStatus, wgpu::ErrorType, wgpu::StringView> F>
    PopErrorScopeCallback(const F& f);
    void operator()(wgpu::PopErrorScopeStatus status, wgpu::ErrorType type, wgpu::StringView message) const;
    void operator()(WGPUPopErrorScopeStatus status, WGPUErrorType type, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct QueueWorkDoneCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::QueueWorkDoneStatus status, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPUQueueWorkDoneStatus status, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::QueueWorkDoneStatus status, wgpu::StringView message) const override;
    };
    Control* data;
public:
    QueueWorkDoneCallback() : data(nullptr) {}
    QueueWorkDoneCallback(WGPUQueueWorkDoneCallback native, void* userdata1, void* userdata2);
    QueueWorkDoneCallback(const QueueWorkDoneCallback& other) : data(other.data) { if (data) ++data->count; }
    QueueWorkDoneCallback(QueueWorkDoneCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    QueueWorkDoneCallback& operator=(const QueueWorkDoneCallback& other);
    QueueWorkDoneCallback& operator=(QueueWorkDoneCallback&& other);
    QueueWorkDoneCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~QueueWorkDoneCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::QueueWorkDoneStatus, wgpu::StringView> F>
    QueueWorkDoneCallback(const F& f);
    void operator()(wgpu::QueueWorkDoneStatus status, wgpu::StringView message) const;
    void operator()(WGPUQueueWorkDoneStatus status, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct RequestAdapterCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::RequestAdapterStatus status, wgpu::Adapter const& adapter, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::RequestAdapterStatus status, wgpu::Adapter const& adapter, wgpu::StringView message) const override;
    };
    Control* data;
public:
    RequestAdapterCallback() : data(nullptr) {}
    RequestAdapterCallback(WGPURequestAdapterCallback native, void* userdata1, void* userdata2);
    RequestAdapterCallback(const RequestAdapterCallback& other) : data(other.data) { if (data) ++data->count; }
    RequestAdapterCallback(RequestAdapterCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    RequestAdapterCallback& operator=(const RequestAdapterCallback& other);
    RequestAdapterCallback& operator=(RequestAdapterCallback&& other);
    RequestAdapterCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~RequestAdapterCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::RequestAdapterStatus, wgpu::Adapter const&, wgpu::StringView> F>
    RequestAdapterCallback(const F& f);
    void operator()(wgpu::RequestAdapterStatus status, wgpu::Adapter const& adapter, wgpu::StringView message) const;
    void operator()(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct RequestDeviceCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::RequestDeviceStatus status, wgpu::Device const& device, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::RequestDeviceStatus status, wgpu::Device const& device, wgpu::StringView message) const override;
    };
    Control* data;
public:
    RequestDeviceCallback() : data(nullptr) {}
    RequestDeviceCallback(WGPURequestDeviceCallback native, void* userdata1, void* userdata2);
    RequestDeviceCallback(const RequestDeviceCallback& other) : data(other.data) { if (data) ++data->count; }
    RequestDeviceCallback(RequestDeviceCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    RequestDeviceCallback& operator=(const RequestDeviceCallback& other);
    RequestDeviceCallback& operator=(RequestDeviceCallback&& other);
    RequestDeviceCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~RequestDeviceCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::RequestDeviceStatus, wgpu::Device const&, wgpu::StringView> F>
    RequestDeviceCallback(const F& f);
    void operator()(wgpu::RequestDeviceStatus status, wgpu::Device const& device, wgpu::StringView message) const;
    void operator()(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
struct UncapturedErrorCallback {
    struct Control {
        std::atomic<std::size_t> count{1};
        virtual ~Control() = default;
        virtual void invoke(wgpu::Device const& device, wgpu::ErrorType type, wgpu::StringView message) const = 0;
        virtual void invoke_c(WGPUDevice const* device, WGPUErrorType type, WGPUStringView message) const;
    };
private:
    template <typename F>
    struct ControlImpl : Control {
        F func;
        ControlImpl(const F& f) : func(f) {}
        void invoke(wgpu::Device const& device, wgpu::ErrorType type, wgpu::StringView message) const override;
    };
    Control* data;
public:
    UncapturedErrorCallback() : data(nullptr) {}
    UncapturedErrorCallback(WGPUUncapturedErrorCallback native, void* userdata1, void* userdata2);
    UncapturedErrorCallback(const UncapturedErrorCallback& other) : data(other.data) { if (data) ++data->count; }
    UncapturedErrorCallback(UncapturedErrorCallback&& other) noexcept : data(other.data) { other.data = nullptr; }
    UncapturedErrorCallback& operator=(const UncapturedErrorCallback& other);
    UncapturedErrorCallback& operator=(UncapturedErrorCallback&& other);
    UncapturedErrorCallback& operator=(std::nullptr_t) { reset(); return *this; }
    ~UncapturedErrorCallback() { if (data && --data->count == 0) { delete data; } }
    template <std::invocable<wgpu::Device const&, wgpu::ErrorType, wgpu::StringView> F>
    UncapturedErrorCallback(const F& f);
    void operator()(wgpu::Device const& device, wgpu::ErrorType type, wgpu::StringView message) const;
    void operator()(WGPUDevice const* device, WGPUErrorType type, WGPUStringView message) const;
    void reset() { if (data && --data->count == 0) { delete data; } data = nullptr; }
    operator bool() const { return data != nullptr; }
};
}
namespace wgpu {
struct NextInChainBase {
    std::atomic<std::size_t> ref_count{1};
    virtual WGPUChainedStruct* getNextInChain() const = 0;
    virtual ~NextInChainBase() = default;
};
template <typename Struct>
struct NextInChainImpl : Struct, NextInChainBase {
    mutable typename Struct::CStruct cstruct;
    NextInChainImpl(const Struct& s) : Struct(s) {
        cstruct = this->to_cstruct();
    }
    NextInChainImpl(Struct&& s) : Struct(std::move(s)) {
        cstruct = this->to_cstruct();
    }
    WGPUChainedStruct* getNextInChain() const override {
        cstruct = this->to_cstruct();
        return reinterpret_cast<WGPUChainedStruct*>(&cstruct);
    }
    void updateFromCStruct() {
        (Struct&)(*this) = static_cast<Struct>(cstruct);
    }
};
struct NextInChainNative : NextInChainBase {
    WGPUChainedStruct* next;
    NextInChainNative(WGPUChainedStruct* n) : next(n) {}
    WGPUChainedStruct* getNextInChain() const override { return next; }
};
struct NextInChain {
public:
    NextInChain() : data(nullptr) {}
    NextInChain(const NextInChain& other) : data(other.data) { if (data) ++data->ref_count; }
    NextInChain(NextInChain&& other) noexcept : data(other.data) { other.data = nullptr; }
    NextInChain& operator=(const NextInChain& other) {
        if (this != &other) {
            if (data && --data->ref_count == 0) {
                delete data;
            }
            data = other.data;
            if (data) ++data->ref_count;
        }
        return *this;
    }
    NextInChain& operator=(NextInChain&& other) noexcept {
        if (this != &other) {
            if (data && --data->ref_count == 0) {
                delete data;
            }
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
    ~NextInChain() { reset(); }
    template <typename Struct>
    void setNext(Struct&& s) {
        reset();
        data = new NextInChainImpl<std::decay_t<Struct>>(std::forward<Struct>(s));
    }
    void setNext(WGPUChainedStruct* next) {
        reset();
        data = new NextInChainNative(next);
    }
    WGPUChainedStruct* getNext() const {
        if (data) {
            return data->getNextInChain();
        }
        return nullptr;
    }
    template <typename Struct>
    Struct* getAs() const {
        if (data) {
            if (auto impl = dynamic_cast<NextInChainImpl<Struct>*>(data)) {
                impl->updateFromCStruct();
                return static_cast<Struct*>(impl);
            }
        }
        return nullptr;
    }
    void reset() {
        if (data && --data->ref_count == 0) {
            delete data;
        }
        data = nullptr;
    }
private:
    NextInChainBase* data;
};
struct StringView {
    struct CStruct : public WGPUStringView {
    };
    StringView(const WGPUStringView& native);
    StringView() : StringView(WGPU_STRING_VIEW_INIT) {};
    CStruct to_cstruct() const;
    StringView& setData(char const* value) &;
    StringView&& setData(char const* value) &&;
    StringView& setLength(size_t value) &;
    StringView&& setLength(size_t value) &&;
    char const* data;
    size_t length;
};
struct ChainedStruct {
    struct CStruct : public WGPUChainedStruct {
    };
    ChainedStruct(const WGPUChainedStruct& native);
    ChainedStruct() {};
    CStruct to_cstruct() const;
    template <typename T>
    ChainedStruct& setNext(T&& value) &;
    template <typename T>
    ChainedStruct&& setNext(T&& value) &&;
    ChainedStruct& setSType(wgpu::SType value) &;
    ChainedStruct&& setSType(wgpu::SType value) &&;
    NextInChain next;
    wgpu::SType sType;
};
struct BufferMapCallbackInfo {
    struct CStruct : public WGPUBufferMapCallbackInfo {
    };
    BufferMapCallbackInfo(const WGPUBufferMapCallbackInfo& native);
    BufferMapCallbackInfo() : BufferMapCallbackInfo(WGPU_BUFFER_MAP_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    BufferMapCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    BufferMapCallbackInfo&& setNextInChain(T&& value) &&;
    BufferMapCallbackInfo& setMode(wgpu::CallbackMode value) &;
    BufferMapCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    BufferMapCallbackInfo& setCallback(wgpu::BufferMapCallback value) &;
    BufferMapCallbackInfo&& setCallback(wgpu::BufferMapCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::BufferMapCallback callback;
};
struct CompilationInfoCallbackInfo {
    struct CStruct : public WGPUCompilationInfoCallbackInfo {
    };
    CompilationInfoCallbackInfo(const WGPUCompilationInfoCallbackInfo& native);
    CompilationInfoCallbackInfo() : CompilationInfoCallbackInfo(WGPU_COMPILATION_INFO_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    CompilationInfoCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    CompilationInfoCallbackInfo&& setNextInChain(T&& value) &&;
    CompilationInfoCallbackInfo& setMode(wgpu::CallbackMode value) &;
    CompilationInfoCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    CompilationInfoCallbackInfo& setCallback(wgpu::CompilationInfoCallback value) &;
    CompilationInfoCallbackInfo&& setCallback(wgpu::CompilationInfoCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::CompilationInfoCallback callback;
};
struct CreateComputePipelineAsyncCallbackInfo {
    struct CStruct : public WGPUCreateComputePipelineAsyncCallbackInfo {
    };
    CreateComputePipelineAsyncCallbackInfo(const WGPUCreateComputePipelineAsyncCallbackInfo& native);
    CreateComputePipelineAsyncCallbackInfo() : CreateComputePipelineAsyncCallbackInfo(WGPU_CREATE_COMPUTE_PIPELINE_ASYNC_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    CreateComputePipelineAsyncCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    CreateComputePipelineAsyncCallbackInfo&& setNextInChain(T&& value) &&;
    CreateComputePipelineAsyncCallbackInfo& setMode(wgpu::CallbackMode value) &;
    CreateComputePipelineAsyncCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    CreateComputePipelineAsyncCallbackInfo& setCallback(wgpu::CreateComputePipelineAsyncCallback value) &;
    CreateComputePipelineAsyncCallbackInfo&& setCallback(wgpu::CreateComputePipelineAsyncCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::CreateComputePipelineAsyncCallback callback;
};
struct CreateRenderPipelineAsyncCallbackInfo {
    struct CStruct : public WGPUCreateRenderPipelineAsyncCallbackInfo {
    };
    CreateRenderPipelineAsyncCallbackInfo(const WGPUCreateRenderPipelineAsyncCallbackInfo& native);
    CreateRenderPipelineAsyncCallbackInfo() : CreateRenderPipelineAsyncCallbackInfo(WGPU_CREATE_RENDER_PIPELINE_ASYNC_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    CreateRenderPipelineAsyncCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    CreateRenderPipelineAsyncCallbackInfo&& setNextInChain(T&& value) &&;
    CreateRenderPipelineAsyncCallbackInfo& setMode(wgpu::CallbackMode value) &;
    CreateRenderPipelineAsyncCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    CreateRenderPipelineAsyncCallbackInfo& setCallback(wgpu::CreateRenderPipelineAsyncCallback value) &;
    CreateRenderPipelineAsyncCallbackInfo&& setCallback(wgpu::CreateRenderPipelineAsyncCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::CreateRenderPipelineAsyncCallback callback;
};
struct DeviceLostCallbackInfo {
    struct CStruct : public WGPUDeviceLostCallbackInfo {
    };
    DeviceLostCallbackInfo(const WGPUDeviceLostCallbackInfo& native);
    DeviceLostCallbackInfo() : DeviceLostCallbackInfo(WGPU_DEVICE_LOST_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    DeviceLostCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    DeviceLostCallbackInfo&& setNextInChain(T&& value) &&;
    DeviceLostCallbackInfo& setMode(wgpu::CallbackMode value) &;
    DeviceLostCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    DeviceLostCallbackInfo& setCallback(wgpu::DeviceLostCallback value) &;
    DeviceLostCallbackInfo&& setCallback(wgpu::DeviceLostCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::DeviceLostCallback callback;
};
struct PopErrorScopeCallbackInfo {
    struct CStruct : public WGPUPopErrorScopeCallbackInfo {
    };
    PopErrorScopeCallbackInfo(const WGPUPopErrorScopeCallbackInfo& native);
    PopErrorScopeCallbackInfo() : PopErrorScopeCallbackInfo(WGPU_POP_ERROR_SCOPE_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    PopErrorScopeCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    PopErrorScopeCallbackInfo&& setNextInChain(T&& value) &&;
    PopErrorScopeCallbackInfo& setMode(wgpu::CallbackMode value) &;
    PopErrorScopeCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    PopErrorScopeCallbackInfo& setCallback(wgpu::PopErrorScopeCallback value) &;
    PopErrorScopeCallbackInfo&& setCallback(wgpu::PopErrorScopeCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::PopErrorScopeCallback callback;
};
struct QueueWorkDoneCallbackInfo {
    struct CStruct : public WGPUQueueWorkDoneCallbackInfo {
    };
    QueueWorkDoneCallbackInfo(const WGPUQueueWorkDoneCallbackInfo& native);
    QueueWorkDoneCallbackInfo() : QueueWorkDoneCallbackInfo(WGPU_QUEUE_WORK_DONE_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    QueueWorkDoneCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    QueueWorkDoneCallbackInfo&& setNextInChain(T&& value) &&;
    QueueWorkDoneCallbackInfo& setMode(wgpu::CallbackMode value) &;
    QueueWorkDoneCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    QueueWorkDoneCallbackInfo& setCallback(wgpu::QueueWorkDoneCallback value) &;
    QueueWorkDoneCallbackInfo&& setCallback(wgpu::QueueWorkDoneCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::QueueWorkDoneCallback callback;
};
struct RequestAdapterCallbackInfo {
    struct CStruct : public WGPURequestAdapterCallbackInfo {
    };
    RequestAdapterCallbackInfo(const WGPURequestAdapterCallbackInfo& native);
    RequestAdapterCallbackInfo() : RequestAdapterCallbackInfo(WGPU_REQUEST_ADAPTER_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RequestAdapterCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    RequestAdapterCallbackInfo&& setNextInChain(T&& value) &&;
    RequestAdapterCallbackInfo& setMode(wgpu::CallbackMode value) &;
    RequestAdapterCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    RequestAdapterCallbackInfo& setCallback(wgpu::RequestAdapterCallback value) &;
    RequestAdapterCallbackInfo&& setCallback(wgpu::RequestAdapterCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::RequestAdapterCallback callback;
};
struct RequestDeviceCallbackInfo {
    struct CStruct : public WGPURequestDeviceCallbackInfo {
    };
    RequestDeviceCallbackInfo(const WGPURequestDeviceCallbackInfo& native);
    RequestDeviceCallbackInfo() : RequestDeviceCallbackInfo(WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RequestDeviceCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    RequestDeviceCallbackInfo&& setNextInChain(T&& value) &&;
    RequestDeviceCallbackInfo& setMode(wgpu::CallbackMode value) &;
    RequestDeviceCallbackInfo&& setMode(wgpu::CallbackMode value) &&;
    RequestDeviceCallbackInfo& setCallback(wgpu::RequestDeviceCallback value) &;
    RequestDeviceCallbackInfo&& setCallback(wgpu::RequestDeviceCallback value) &&;
    NextInChain nextInChain;
    wgpu::CallbackMode mode;
    wgpu::RequestDeviceCallback callback;
};
struct UncapturedErrorCallbackInfo {
    struct CStruct : public WGPUUncapturedErrorCallbackInfo {
    };
    UncapturedErrorCallbackInfo(const WGPUUncapturedErrorCallbackInfo& native);
    UncapturedErrorCallbackInfo() : UncapturedErrorCallbackInfo(WGPU_UNCAPTURED_ERROR_CALLBACK_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    UncapturedErrorCallbackInfo& setNextInChain(T&& value) &;
    template <typename T>
    UncapturedErrorCallbackInfo&& setNextInChain(T&& value) &&;
    UncapturedErrorCallbackInfo& setCallback(wgpu::UncapturedErrorCallback value) &;
    UncapturedErrorCallbackInfo&& setCallback(wgpu::UncapturedErrorCallback value) &&;
    NextInChain nextInChain;
    wgpu::UncapturedErrorCallback callback;
};
struct AdapterInfo {
    struct CStruct : public WGPUAdapterInfo {
    };
    AdapterInfo(const WGPUAdapterInfo& native);
    AdapterInfo() : AdapterInfo(WGPU_ADAPTER_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    AdapterInfo& setNextInChain(T&& value) &;
    template <typename T>
    AdapterInfo&& setNextInChain(T&& value) &&;
    AdapterInfo& setVendor(const wgpu::StringView& value) &;
    AdapterInfo&& setVendor(const wgpu::StringView& value) &&;
    AdapterInfo& setVendor(wgpu::StringView&& value) &;
    AdapterInfo&& setVendor(wgpu::StringView&& value) &&;
    AdapterInfo& setArchitecture(const wgpu::StringView& value) &;
    AdapterInfo&& setArchitecture(const wgpu::StringView& value) &&;
    AdapterInfo& setArchitecture(wgpu::StringView&& value) &;
    AdapterInfo&& setArchitecture(wgpu::StringView&& value) &&;
    AdapterInfo& setDevice(const wgpu::StringView& value) &;
    AdapterInfo&& setDevice(const wgpu::StringView& value) &&;
    AdapterInfo& setDevice(wgpu::StringView&& value) &;
    AdapterInfo&& setDevice(wgpu::StringView&& value) &&;
    AdapterInfo& setDescription(const wgpu::StringView& value) &;
    AdapterInfo&& setDescription(const wgpu::StringView& value) &&;
    AdapterInfo& setDescription(wgpu::StringView&& value) &;
    AdapterInfo&& setDescription(wgpu::StringView&& value) &&;
    AdapterInfo& setBackendType(wgpu::BackendType value) &;
    AdapterInfo&& setBackendType(wgpu::BackendType value) &&;
    AdapterInfo& setAdapterType(wgpu::AdapterType value) &;
    AdapterInfo&& setAdapterType(wgpu::AdapterType value) &&;
    AdapterInfo& setVendorID(uint32_t value) &;
    AdapterInfo&& setVendorID(uint32_t value) &&;
    AdapterInfo& setDeviceID(uint32_t value) &;
    AdapterInfo&& setDeviceID(uint32_t value) &&;
    AdapterInfo& setSubgroupMinSize(uint32_t value) &;
    AdapterInfo&& setSubgroupMinSize(uint32_t value) &&;
    AdapterInfo& setSubgroupMaxSize(uint32_t value) &;
    AdapterInfo&& setSubgroupMaxSize(uint32_t value) &&;
    NextInChain nextInChain;
    wgpu::StringView vendor;
    wgpu::StringView architecture;
    wgpu::StringView device;
    wgpu::StringView description;
    wgpu::BackendType backendType;
    wgpu::AdapterType adapterType;
    uint32_t vendorID;
    uint32_t deviceID;
    uint32_t subgroupMinSize;
    uint32_t subgroupMaxSize;
};
struct BlendComponent {
    struct CStruct : public WGPUBlendComponent {
    };
    BlendComponent(const WGPUBlendComponent& native);
    BlendComponent() : BlendComponent(WGPU_BLEND_COMPONENT_INIT) {};
    CStruct to_cstruct() const;
    BlendComponent& setOperation(wgpu::BlendOperation value) &;
    BlendComponent&& setOperation(wgpu::BlendOperation value) &&;
    BlendComponent& setSrcFactor(wgpu::BlendFactor value) &;
    BlendComponent&& setSrcFactor(wgpu::BlendFactor value) &&;
    BlendComponent& setDstFactor(wgpu::BlendFactor value) &;
    BlendComponent&& setDstFactor(wgpu::BlendFactor value) &&;
    wgpu::BlendOperation operation;
    wgpu::BlendFactor srcFactor;
    wgpu::BlendFactor dstFactor;
};
struct BufferBindingLayout {
    struct CStruct : public WGPUBufferBindingLayout {
    };
    BufferBindingLayout(const WGPUBufferBindingLayout& native);
    BufferBindingLayout() : BufferBindingLayout(WGPU_BUFFER_BINDING_LAYOUT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    BufferBindingLayout& setNextInChain(T&& value) &;
    template <typename T>
    BufferBindingLayout&& setNextInChain(T&& value) &&;
    BufferBindingLayout& setType(wgpu::BufferBindingType value) &;
    BufferBindingLayout&& setType(wgpu::BufferBindingType value) &&;
    BufferBindingLayout& setHasDynamicOffset(wgpu::Bool value) &;
    BufferBindingLayout&& setHasDynamicOffset(wgpu::Bool value) &&;
    BufferBindingLayout& setMinBindingSize(uint64_t value) &;
    BufferBindingLayout&& setMinBindingSize(uint64_t value) &&;
    NextInChain nextInChain;
    wgpu::BufferBindingType type;
    wgpu::Bool hasDynamicOffset;
    uint64_t minBindingSize;
};
struct BufferDescriptor {
    struct CStruct : public WGPUBufferDescriptor {
    };
    BufferDescriptor(const WGPUBufferDescriptor& native);
    BufferDescriptor() : BufferDescriptor(WGPU_BUFFER_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    BufferDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    BufferDescriptor&& setNextInChain(T&& value) &&;
    BufferDescriptor& setLabel(const wgpu::StringView& value) &;
    BufferDescriptor&& setLabel(const wgpu::StringView& value) &&;
    BufferDescriptor& setLabel(wgpu::StringView&& value) &;
    BufferDescriptor&& setLabel(wgpu::StringView&& value) &&;
    BufferDescriptor& setUsage(wgpu::BufferUsage value) &;
    BufferDescriptor&& setUsage(wgpu::BufferUsage value) &&;
    BufferDescriptor& setSize(uint64_t value) &;
    BufferDescriptor&& setSize(uint64_t value) &&;
    BufferDescriptor& setMappedAtCreation(wgpu::Bool value) &;
    BufferDescriptor&& setMappedAtCreation(wgpu::Bool value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::BufferUsage usage;
    uint64_t size;
    wgpu::Bool mappedAtCreation;
};
struct Color {
    struct CStruct : public WGPUColor {
    };
    Color(const WGPUColor& native);
    Color() : Color(WGPU_COLOR_INIT) {};
    CStruct to_cstruct() const;
    Color& setR(double value) &;
    Color&& setR(double value) &&;
    Color& setG(double value) &;
    Color&& setG(double value) &&;
    Color& setB(double value) &;
    Color&& setB(double value) &&;
    Color& setA(double value) &;
    Color&& setA(double value) &&;
    double r;
    double g;
    double b;
    double a;
};
struct CommandBufferDescriptor {
    struct CStruct : public WGPUCommandBufferDescriptor {
    };
    CommandBufferDescriptor(const WGPUCommandBufferDescriptor& native);
    CommandBufferDescriptor() : CommandBufferDescriptor(WGPU_COMMAND_BUFFER_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    CommandBufferDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    CommandBufferDescriptor&& setNextInChain(T&& value) &&;
    CommandBufferDescriptor& setLabel(const wgpu::StringView& value) &;
    CommandBufferDescriptor&& setLabel(const wgpu::StringView& value) &&;
    CommandBufferDescriptor& setLabel(wgpu::StringView&& value) &;
    CommandBufferDescriptor&& setLabel(wgpu::StringView&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
};
struct CommandEncoderDescriptor {
    struct CStruct : public WGPUCommandEncoderDescriptor {
    };
    CommandEncoderDescriptor(const WGPUCommandEncoderDescriptor& native);
    CommandEncoderDescriptor() : CommandEncoderDescriptor(WGPU_COMMAND_ENCODER_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    CommandEncoderDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    CommandEncoderDescriptor&& setNextInChain(T&& value) &&;
    CommandEncoderDescriptor& setLabel(const wgpu::StringView& value) &;
    CommandEncoderDescriptor&& setLabel(const wgpu::StringView& value) &&;
    CommandEncoderDescriptor& setLabel(wgpu::StringView&& value) &;
    CommandEncoderDescriptor&& setLabel(wgpu::StringView&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
};
struct CompilationMessage {
    struct CStruct : public WGPUCompilationMessage {
    };
    CompilationMessage(const WGPUCompilationMessage& native);
    CompilationMessage() : CompilationMessage(WGPU_COMPILATION_MESSAGE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    CompilationMessage& setNextInChain(T&& value) &;
    template <typename T>
    CompilationMessage&& setNextInChain(T&& value) &&;
    CompilationMessage& setMessage(const wgpu::StringView& value) &;
    CompilationMessage&& setMessage(const wgpu::StringView& value) &&;
    CompilationMessage& setMessage(wgpu::StringView&& value) &;
    CompilationMessage&& setMessage(wgpu::StringView&& value) &&;
    CompilationMessage& setType(wgpu::CompilationMessageType value) &;
    CompilationMessage&& setType(wgpu::CompilationMessageType value) &&;
    CompilationMessage& setLineNum(uint64_t value) &;
    CompilationMessage&& setLineNum(uint64_t value) &&;
    CompilationMessage& setLinePos(uint64_t value) &;
    CompilationMessage&& setLinePos(uint64_t value) &&;
    CompilationMessage& setOffset(uint64_t value) &;
    CompilationMessage&& setOffset(uint64_t value) &&;
    CompilationMessage& setLength(uint64_t value) &;
    CompilationMessage&& setLength(uint64_t value) &&;
    NextInChain nextInChain;
    wgpu::StringView message;
    wgpu::CompilationMessageType type;
    uint64_t lineNum;
    uint64_t linePos;
    uint64_t offset;
    uint64_t length;
};
struct ConstantEntry {
    struct CStruct : public WGPUConstantEntry {
    };
    ConstantEntry(const WGPUConstantEntry& native);
    ConstantEntry() : ConstantEntry(WGPU_CONSTANT_ENTRY_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ConstantEntry& setNextInChain(T&& value) &;
    template <typename T>
    ConstantEntry&& setNextInChain(T&& value) &&;
    ConstantEntry& setKey(const wgpu::StringView& value) &;
    ConstantEntry&& setKey(const wgpu::StringView& value) &&;
    ConstantEntry& setKey(wgpu::StringView&& value) &;
    ConstantEntry&& setKey(wgpu::StringView&& value) &&;
    ConstantEntry& setValue(double value) &;
    ConstantEntry&& setValue(double value) &&;
    NextInChain nextInChain;
    wgpu::StringView key;
    double value;
};
struct Extent3D {
    struct CStruct : public WGPUExtent3D {
    };
    Extent3D(const WGPUExtent3D& native);
    Extent3D() {};
    CStruct to_cstruct() const;
    Extent3D& setWidth(uint32_t value) &;
    Extent3D&& setWidth(uint32_t value) &&;
    Extent3D& setHeight(uint32_t value) &;
    Extent3D&& setHeight(uint32_t value) &&;
    Extent3D& setDepthOrArrayLayers(uint32_t value) &;
    Extent3D&& setDepthOrArrayLayers(uint32_t value) &&;
    uint32_t width;
    uint32_t height;
    uint32_t depthOrArrayLayers;
};
struct ExternalTextureBindingEntry {
    struct CStruct : public WGPUExternalTextureBindingEntry {
    };
    ExternalTextureBindingEntry(const WGPUExternalTextureBindingEntry& native);
    ExternalTextureBindingEntry() : ExternalTextureBindingEntry(WGPU_EXTERNAL_TEXTURE_BINDING_ENTRY_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ExternalTextureBindingEntry& setNext(T&& value) &;
    template <typename T>
    ExternalTextureBindingEntry&& setNext(T&& value) &&;
    ExternalTextureBindingEntry& setExternalTexture(wgpu::raw::ExternalTexture value) &;
    ExternalTextureBindingEntry&& setExternalTexture(wgpu::raw::ExternalTexture value) &&;
    wgpu::ChainedStruct chain;
    wgpu::raw::ExternalTexture externalTexture;
};
struct ExternalTextureBindingLayout {
    struct CStruct : public WGPUExternalTextureBindingLayout {
    };
    ExternalTextureBindingLayout(const WGPUExternalTextureBindingLayout& native);
    ExternalTextureBindingLayout() : ExternalTextureBindingLayout(WGPU_EXTERNAL_TEXTURE_BINDING_LAYOUT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ExternalTextureBindingLayout& setNext(T&& value) &;
    template <typename T>
    ExternalTextureBindingLayout&& setNext(T&& value) &&;
    wgpu::ChainedStruct chain;
};
struct Future {
    struct CStruct : public WGPUFuture {
    };
    Future(const WGPUFuture& native);
    Future() : Future(WGPU_FUTURE_INIT) {};
    CStruct to_cstruct() const;
    Future& setId(uint64_t value) &;
    Future&& setId(uint64_t value) &&;
    uint64_t id;
};
struct InstanceLimits {
    struct CStruct : public WGPUInstanceLimits {
    };
    InstanceLimits(const WGPUInstanceLimits& native);
    InstanceLimits() : InstanceLimits(WGPU_INSTANCE_LIMITS_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    InstanceLimits& setNextInChain(T&& value) &;
    template <typename T>
    InstanceLimits&& setNextInChain(T&& value) &&;
    InstanceLimits& setTimedWaitAnyMaxCount(size_t value) &;
    InstanceLimits&& setTimedWaitAnyMaxCount(size_t value) &&;
    NextInChain nextInChain;
    size_t timedWaitAnyMaxCount;
};
struct Limits {
    struct CStruct : public WGPULimits {
    };
    Limits(const WGPULimits& native);
    Limits() : Limits(WGPU_LIMITS_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    Limits& setNextInChain(T&& value) &;
    template <typename T>
    Limits&& setNextInChain(T&& value) &&;
    Limits& setMaxTextureDimension1D(uint32_t value) &;
    Limits&& setMaxTextureDimension1D(uint32_t value) &&;
    Limits& setMaxTextureDimension2D(uint32_t value) &;
    Limits&& setMaxTextureDimension2D(uint32_t value) &&;
    Limits& setMaxTextureDimension3D(uint32_t value) &;
    Limits&& setMaxTextureDimension3D(uint32_t value) &&;
    Limits& setMaxTextureArrayLayers(uint32_t value) &;
    Limits&& setMaxTextureArrayLayers(uint32_t value) &&;
    Limits& setMaxBindGroups(uint32_t value) &;
    Limits&& setMaxBindGroups(uint32_t value) &&;
    Limits& setMaxBindGroupsPlusVertexBuffers(uint32_t value) &;
    Limits&& setMaxBindGroupsPlusVertexBuffers(uint32_t value) &&;
    Limits& setMaxBindingsPerBindGroup(uint32_t value) &;
    Limits&& setMaxBindingsPerBindGroup(uint32_t value) &&;
    Limits& setMaxDynamicUniformBuffersPerPipelineLayout(uint32_t value) &;
    Limits&& setMaxDynamicUniformBuffersPerPipelineLayout(uint32_t value) &&;
    Limits& setMaxDynamicStorageBuffersPerPipelineLayout(uint32_t value) &;
    Limits&& setMaxDynamicStorageBuffersPerPipelineLayout(uint32_t value) &&;
    Limits& setMaxSampledTexturesPerShaderStage(uint32_t value) &;
    Limits&& setMaxSampledTexturesPerShaderStage(uint32_t value) &&;
    Limits& setMaxSamplersPerShaderStage(uint32_t value) &;
    Limits&& setMaxSamplersPerShaderStage(uint32_t value) &&;
    Limits& setMaxStorageBuffersPerShaderStage(uint32_t value) &;
    Limits&& setMaxStorageBuffersPerShaderStage(uint32_t value) &&;
    Limits& setMaxStorageTexturesPerShaderStage(uint32_t value) &;
    Limits&& setMaxStorageTexturesPerShaderStage(uint32_t value) &&;
    Limits& setMaxUniformBuffersPerShaderStage(uint32_t value) &;
    Limits&& setMaxUniformBuffersPerShaderStage(uint32_t value) &&;
    Limits& setMaxUniformBufferBindingSize(uint64_t value) &;
    Limits&& setMaxUniformBufferBindingSize(uint64_t value) &&;
    Limits& setMaxStorageBufferBindingSize(uint64_t value) &;
    Limits&& setMaxStorageBufferBindingSize(uint64_t value) &&;
    Limits& setMinUniformBufferOffsetAlignment(uint32_t value) &;
    Limits&& setMinUniformBufferOffsetAlignment(uint32_t value) &&;
    Limits& setMinStorageBufferOffsetAlignment(uint32_t value) &;
    Limits&& setMinStorageBufferOffsetAlignment(uint32_t value) &&;
    Limits& setMaxVertexBuffers(uint32_t value) &;
    Limits&& setMaxVertexBuffers(uint32_t value) &&;
    Limits& setMaxBufferSize(uint64_t value) &;
    Limits&& setMaxBufferSize(uint64_t value) &&;
    Limits& setMaxVertexAttributes(uint32_t value) &;
    Limits&& setMaxVertexAttributes(uint32_t value) &&;
    Limits& setMaxVertexBufferArrayStride(uint32_t value) &;
    Limits&& setMaxVertexBufferArrayStride(uint32_t value) &&;
    Limits& setMaxInterStageShaderVariables(uint32_t value) &;
    Limits&& setMaxInterStageShaderVariables(uint32_t value) &&;
    Limits& setMaxColorAttachments(uint32_t value) &;
    Limits&& setMaxColorAttachments(uint32_t value) &&;
    Limits& setMaxColorAttachmentBytesPerSample(uint32_t value) &;
    Limits&& setMaxColorAttachmentBytesPerSample(uint32_t value) &&;
    Limits& setMaxComputeWorkgroupStorageSize(uint32_t value) &;
    Limits&& setMaxComputeWorkgroupStorageSize(uint32_t value) &&;
    Limits& setMaxComputeInvocationsPerWorkgroup(uint32_t value) &;
    Limits&& setMaxComputeInvocationsPerWorkgroup(uint32_t value) &&;
    Limits& setMaxComputeWorkgroupSizeX(uint32_t value) &;
    Limits&& setMaxComputeWorkgroupSizeX(uint32_t value) &&;
    Limits& setMaxComputeWorkgroupSizeY(uint32_t value) &;
    Limits&& setMaxComputeWorkgroupSizeY(uint32_t value) &&;
    Limits& setMaxComputeWorkgroupSizeZ(uint32_t value) &;
    Limits&& setMaxComputeWorkgroupSizeZ(uint32_t value) &&;
    Limits& setMaxComputeWorkgroupsPerDimension(uint32_t value) &;
    Limits&& setMaxComputeWorkgroupsPerDimension(uint32_t value) &&;
    Limits& setMaxImmediateSize(uint32_t value) &;
    Limits&& setMaxImmediateSize(uint32_t value) &&;
    NextInChain nextInChain;
    uint32_t maxTextureDimension1D;
    uint32_t maxTextureDimension2D;
    uint32_t maxTextureDimension3D;
    uint32_t maxTextureArrayLayers;
    uint32_t maxBindGroups;
    uint32_t maxBindGroupsPlusVertexBuffers;
    uint32_t maxBindingsPerBindGroup;
    uint32_t maxDynamicUniformBuffersPerPipelineLayout;
    uint32_t maxDynamicStorageBuffersPerPipelineLayout;
    uint32_t maxSampledTexturesPerShaderStage;
    uint32_t maxSamplersPerShaderStage;
    uint32_t maxStorageBuffersPerShaderStage;
    uint32_t maxStorageTexturesPerShaderStage;
    uint32_t maxUniformBuffersPerShaderStage;
    uint64_t maxUniformBufferBindingSize;
    uint64_t maxStorageBufferBindingSize;
    uint32_t minUniformBufferOffsetAlignment;
    uint32_t minStorageBufferOffsetAlignment;
    uint32_t maxVertexBuffers;
    uint64_t maxBufferSize;
    uint32_t maxVertexAttributes;
    uint32_t maxVertexBufferArrayStride;
    uint32_t maxInterStageShaderVariables;
    uint32_t maxColorAttachments;
    uint32_t maxColorAttachmentBytesPerSample;
    uint32_t maxComputeWorkgroupStorageSize;
    uint32_t maxComputeInvocationsPerWorkgroup;
    uint32_t maxComputeWorkgroupSizeX;
    uint32_t maxComputeWorkgroupSizeY;
    uint32_t maxComputeWorkgroupSizeZ;
    uint32_t maxComputeWorkgroupsPerDimension;
    uint32_t maxImmediateSize;
};
struct MultisampleState {
    struct CStruct : public WGPUMultisampleState {
    };
    MultisampleState(const WGPUMultisampleState& native);
    MultisampleState() : MultisampleState(WGPU_MULTISAMPLE_STATE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    MultisampleState& setNextInChain(T&& value) &;
    template <typename T>
    MultisampleState&& setNextInChain(T&& value) &&;
    MultisampleState& setCount(uint32_t value) &;
    MultisampleState&& setCount(uint32_t value) &&;
    MultisampleState& setMask(uint32_t value) &;
    MultisampleState&& setMask(uint32_t value) &&;
    MultisampleState& setAlphaToCoverageEnabled(wgpu::Bool value) &;
    MultisampleState&& setAlphaToCoverageEnabled(wgpu::Bool value) &&;
    NextInChain nextInChain;
    uint32_t count;
    uint32_t mask;
    wgpu::Bool alphaToCoverageEnabled;
};
struct Origin3D {
    struct CStruct : public WGPUOrigin3D {
    };
    Origin3D(const WGPUOrigin3D& native);
    Origin3D() {};
    CStruct to_cstruct() const;
    Origin3D& setX(uint32_t value) &;
    Origin3D&& setX(uint32_t value) &&;
    Origin3D& setY(uint32_t value) &;
    Origin3D&& setY(uint32_t value) &&;
    Origin3D& setZ(uint32_t value) &;
    Origin3D&& setZ(uint32_t value) &&;
    uint32_t x;
    uint32_t y;
    uint32_t z;
};
struct PassTimestampWrites {
    struct CStruct : public WGPUPassTimestampWrites {
    };
    PassTimestampWrites(const WGPUPassTimestampWrites& native);
    PassTimestampWrites() : PassTimestampWrites(WGPU_PASS_TIMESTAMP_WRITES_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    PassTimestampWrites& setNextInChain(T&& value) &;
    template <typename T>
    PassTimestampWrites&& setNextInChain(T&& value) &&;
    PassTimestampWrites& setQuerySet(wgpu::raw::QuerySet value) &;
    PassTimestampWrites&& setQuerySet(wgpu::raw::QuerySet value) &&;
    PassTimestampWrites& setBeginningOfPassWriteIndex(uint32_t value) &;
    PassTimestampWrites&& setBeginningOfPassWriteIndex(uint32_t value) &&;
    PassTimestampWrites& setEndOfPassWriteIndex(uint32_t value) &;
    PassTimestampWrites&& setEndOfPassWriteIndex(uint32_t value) &&;
    NextInChain nextInChain;
    wgpu::raw::QuerySet querySet;
    uint32_t beginningOfPassWriteIndex;
    uint32_t endOfPassWriteIndex;
};
struct PipelineLayoutDescriptor {
    struct CStruct : public WGPUPipelineLayoutDescriptor {
    };
    PipelineLayoutDescriptor(const WGPUPipelineLayoutDescriptor& native);
    PipelineLayoutDescriptor() : PipelineLayoutDescriptor(WGPU_PIPELINE_LAYOUT_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    PipelineLayoutDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    PipelineLayoutDescriptor&& setNextInChain(T&& value) &&;
    PipelineLayoutDescriptor& setLabel(const wgpu::StringView& value) &;
    PipelineLayoutDescriptor&& setLabel(const wgpu::StringView& value) &&;
    PipelineLayoutDescriptor& setLabel(wgpu::StringView&& value) &;
    PipelineLayoutDescriptor&& setLabel(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::raw::BindGroupLayout>
    PipelineLayoutDescriptor& setBindGroupLayouts(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::raw::BindGroupLayout>
    PipelineLayoutDescriptor&& setBindGroupLayouts(T&& values) &&;
    PipelineLayoutDescriptor& setImmediateSize(uint32_t value) &;
    PipelineLayoutDescriptor&& setImmediateSize(uint32_t value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    std::vector<wgpu::raw::BindGroupLayout> bindGroupLayouts;
    uint32_t immediateSize;
};
struct PrimitiveState {
    struct CStruct : public WGPUPrimitiveState {
    };
    PrimitiveState(const WGPUPrimitiveState& native);
    PrimitiveState() : PrimitiveState(WGPU_PRIMITIVE_STATE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    PrimitiveState& setNextInChain(T&& value) &;
    template <typename T>
    PrimitiveState&& setNextInChain(T&& value) &&;
    PrimitiveState& setTopology(wgpu::PrimitiveTopology value) &;
    PrimitiveState&& setTopology(wgpu::PrimitiveTopology value) &&;
    PrimitiveState& setStripIndexFormat(wgpu::IndexFormat value) &;
    PrimitiveState&& setStripIndexFormat(wgpu::IndexFormat value) &&;
    PrimitiveState& setFrontFace(wgpu::FrontFace value) &;
    PrimitiveState&& setFrontFace(wgpu::FrontFace value) &&;
    PrimitiveState& setCullMode(wgpu::CullMode value) &;
    PrimitiveState&& setCullMode(wgpu::CullMode value) &&;
    PrimitiveState& setUnclippedDepth(wgpu::Bool value) &;
    PrimitiveState&& setUnclippedDepth(wgpu::Bool value) &&;
    NextInChain nextInChain;
    wgpu::PrimitiveTopology topology;
    wgpu::IndexFormat stripIndexFormat;
    wgpu::FrontFace frontFace;
    wgpu::CullMode cullMode;
    wgpu::Bool unclippedDepth;
};
struct QuerySetDescriptor {
    struct CStruct : public WGPUQuerySetDescriptor {
    };
    QuerySetDescriptor(const WGPUQuerySetDescriptor& native);
    QuerySetDescriptor() : QuerySetDescriptor(WGPU_QUERY_SET_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    QuerySetDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    QuerySetDescriptor&& setNextInChain(T&& value) &&;
    QuerySetDescriptor& setLabel(const wgpu::StringView& value) &;
    QuerySetDescriptor&& setLabel(const wgpu::StringView& value) &&;
    QuerySetDescriptor& setLabel(wgpu::StringView&& value) &;
    QuerySetDescriptor&& setLabel(wgpu::StringView&& value) &&;
    QuerySetDescriptor& setType(wgpu::QueryType value) &;
    QuerySetDescriptor&& setType(wgpu::QueryType value) &&;
    QuerySetDescriptor& setCount(uint32_t value) &;
    QuerySetDescriptor&& setCount(uint32_t value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::QueryType type;
    uint32_t count;
};
struct QueueDescriptor {
    struct CStruct : public WGPUQueueDescriptor {
    };
    QueueDescriptor(const WGPUQueueDescriptor& native);
    QueueDescriptor() : QueueDescriptor(WGPU_QUEUE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    QueueDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    QueueDescriptor&& setNextInChain(T&& value) &&;
    QueueDescriptor& setLabel(const wgpu::StringView& value) &;
    QueueDescriptor&& setLabel(const wgpu::StringView& value) &&;
    QueueDescriptor& setLabel(wgpu::StringView&& value) &;
    QueueDescriptor&& setLabel(wgpu::StringView&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
};
struct RenderBundleDescriptor {
    struct CStruct : public WGPURenderBundleDescriptor {
    };
    RenderBundleDescriptor(const WGPURenderBundleDescriptor& native);
    RenderBundleDescriptor() : RenderBundleDescriptor(WGPU_RENDER_BUNDLE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RenderBundleDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    RenderBundleDescriptor&& setNextInChain(T&& value) &&;
    RenderBundleDescriptor& setLabel(const wgpu::StringView& value) &;
    RenderBundleDescriptor&& setLabel(const wgpu::StringView& value) &&;
    RenderBundleDescriptor& setLabel(wgpu::StringView&& value) &;
    RenderBundleDescriptor&& setLabel(wgpu::StringView&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
};
struct RenderBundleEncoderDescriptor {
    struct CStruct : public WGPURenderBundleEncoderDescriptor {
    };
    RenderBundleEncoderDescriptor(const WGPURenderBundleEncoderDescriptor& native);
    RenderBundleEncoderDescriptor() : RenderBundleEncoderDescriptor(WGPU_RENDER_BUNDLE_ENCODER_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RenderBundleEncoderDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    RenderBundleEncoderDescriptor&& setNextInChain(T&& value) &&;
    RenderBundleEncoderDescriptor& setLabel(const wgpu::StringView& value) &;
    RenderBundleEncoderDescriptor&& setLabel(const wgpu::StringView& value) &&;
    RenderBundleEncoderDescriptor& setLabel(wgpu::StringView&& value) &;
    RenderBundleEncoderDescriptor&& setLabel(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    RenderBundleEncoderDescriptor& setColorFormats(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    RenderBundleEncoderDescriptor&& setColorFormats(T&& values) &&;
    RenderBundleEncoderDescriptor& setDepthStencilFormat(wgpu::TextureFormat value) &;
    RenderBundleEncoderDescriptor&& setDepthStencilFormat(wgpu::TextureFormat value) &&;
    RenderBundleEncoderDescriptor& setSampleCount(uint32_t value) &;
    RenderBundleEncoderDescriptor&& setSampleCount(uint32_t value) &&;
    RenderBundleEncoderDescriptor& setDepthReadOnly(wgpu::Bool value) &;
    RenderBundleEncoderDescriptor&& setDepthReadOnly(wgpu::Bool value) &&;
    RenderBundleEncoderDescriptor& setStencilReadOnly(wgpu::Bool value) &;
    RenderBundleEncoderDescriptor&& setStencilReadOnly(wgpu::Bool value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    std::vector<wgpu::TextureFormat> colorFormats;
    wgpu::TextureFormat depthStencilFormat;
    uint32_t sampleCount;
    wgpu::Bool depthReadOnly;
    wgpu::Bool stencilReadOnly;
};
struct RenderPassDepthStencilAttachment {
    struct CStruct : public WGPURenderPassDepthStencilAttachment {
    };
    RenderPassDepthStencilAttachment(const WGPURenderPassDepthStencilAttachment& native);
    RenderPassDepthStencilAttachment() : RenderPassDepthStencilAttachment(WGPU_RENDER_PASS_DEPTH_STENCIL_ATTACHMENT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RenderPassDepthStencilAttachment& setNextInChain(T&& value) &;
    template <typename T>
    RenderPassDepthStencilAttachment&& setNextInChain(T&& value) &&;
    RenderPassDepthStencilAttachment& setView(wgpu::raw::TextureView value) &;
    RenderPassDepthStencilAttachment&& setView(wgpu::raw::TextureView value) &&;
    RenderPassDepthStencilAttachment& setDepthLoadOp(wgpu::LoadOp value) &;
    RenderPassDepthStencilAttachment&& setDepthLoadOp(wgpu::LoadOp value) &&;
    RenderPassDepthStencilAttachment& setDepthStoreOp(wgpu::StoreOp value) &;
    RenderPassDepthStencilAttachment&& setDepthStoreOp(wgpu::StoreOp value) &&;
    RenderPassDepthStencilAttachment& setDepthClearValue(float value) &;
    RenderPassDepthStencilAttachment&& setDepthClearValue(float value) &&;
    RenderPassDepthStencilAttachment& setDepthReadOnly(wgpu::Bool value) &;
    RenderPassDepthStencilAttachment&& setDepthReadOnly(wgpu::Bool value) &&;
    RenderPassDepthStencilAttachment& setStencilLoadOp(wgpu::LoadOp value) &;
    RenderPassDepthStencilAttachment&& setStencilLoadOp(wgpu::LoadOp value) &&;
    RenderPassDepthStencilAttachment& setStencilStoreOp(wgpu::StoreOp value) &;
    RenderPassDepthStencilAttachment&& setStencilStoreOp(wgpu::StoreOp value) &&;
    RenderPassDepthStencilAttachment& setStencilClearValue(uint32_t value) &;
    RenderPassDepthStencilAttachment&& setStencilClearValue(uint32_t value) &&;
    RenderPassDepthStencilAttachment& setStencilReadOnly(wgpu::Bool value) &;
    RenderPassDepthStencilAttachment&& setStencilReadOnly(wgpu::Bool value) &&;
    NextInChain nextInChain;
    wgpu::raw::TextureView view;
    wgpu::LoadOp depthLoadOp;
    wgpu::StoreOp depthStoreOp;
    float depthClearValue;
    wgpu::Bool depthReadOnly;
    wgpu::LoadOp stencilLoadOp;
    wgpu::StoreOp stencilStoreOp;
    uint32_t stencilClearValue;
    wgpu::Bool stencilReadOnly;
};
struct RenderPassMaxDrawCount {
    struct CStruct : public WGPURenderPassMaxDrawCount {
    };
    RenderPassMaxDrawCount(const WGPURenderPassMaxDrawCount& native);
    RenderPassMaxDrawCount() : RenderPassMaxDrawCount(WGPU_RENDER_PASS_MAX_DRAW_COUNT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RenderPassMaxDrawCount& setNext(T&& value) &;
    template <typename T>
    RenderPassMaxDrawCount&& setNext(T&& value) &&;
    RenderPassMaxDrawCount& setMaxDrawCount(uint64_t value) &;
    RenderPassMaxDrawCount&& setMaxDrawCount(uint64_t value) &&;
    wgpu::ChainedStruct chain;
    uint64_t maxDrawCount;
};
struct RequestAdapterWebXROptions {
    struct CStruct : public WGPURequestAdapterWebXROptions {
    };
    RequestAdapterWebXROptions(const WGPURequestAdapterWebXROptions& native);
    RequestAdapterWebXROptions() {};
    CStruct to_cstruct() const;
    template <typename T>
    RequestAdapterWebXROptions& setNext(T&& value) &;
    template <typename T>
    RequestAdapterWebXROptions&& setNext(T&& value) &&;
    RequestAdapterWebXROptions& setXrCompatible(wgpu::Bool value) &;
    RequestAdapterWebXROptions&& setXrCompatible(wgpu::Bool value) &&;
    wgpu::ChainedStruct chain;
    wgpu::Bool xrCompatible;
};
struct SamplerBindingLayout {
    struct CStruct : public WGPUSamplerBindingLayout {
    };
    SamplerBindingLayout(const WGPUSamplerBindingLayout& native);
    SamplerBindingLayout() : SamplerBindingLayout(WGPU_SAMPLER_BINDING_LAYOUT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SamplerBindingLayout& setNextInChain(T&& value) &;
    template <typename T>
    SamplerBindingLayout&& setNextInChain(T&& value) &&;
    SamplerBindingLayout& setType(wgpu::SamplerBindingType value) &;
    SamplerBindingLayout&& setType(wgpu::SamplerBindingType value) &&;
    NextInChain nextInChain;
    wgpu::SamplerBindingType type;
};
struct SamplerDescriptor {
    struct CStruct : public WGPUSamplerDescriptor {
    };
    SamplerDescriptor(const WGPUSamplerDescriptor& native);
    SamplerDescriptor() : SamplerDescriptor(WGPU_SAMPLER_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SamplerDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    SamplerDescriptor&& setNextInChain(T&& value) &&;
    SamplerDescriptor& setLabel(const wgpu::StringView& value) &;
    SamplerDescriptor&& setLabel(const wgpu::StringView& value) &&;
    SamplerDescriptor& setLabel(wgpu::StringView&& value) &;
    SamplerDescriptor&& setLabel(wgpu::StringView&& value) &&;
    SamplerDescriptor& setAddressModeU(wgpu::AddressMode value) &;
    SamplerDescriptor&& setAddressModeU(wgpu::AddressMode value) &&;
    SamplerDescriptor& setAddressModeV(wgpu::AddressMode value) &;
    SamplerDescriptor&& setAddressModeV(wgpu::AddressMode value) &&;
    SamplerDescriptor& setAddressModeW(wgpu::AddressMode value) &;
    SamplerDescriptor&& setAddressModeW(wgpu::AddressMode value) &&;
    SamplerDescriptor& setMagFilter(wgpu::FilterMode value) &;
    SamplerDescriptor&& setMagFilter(wgpu::FilterMode value) &&;
    SamplerDescriptor& setMinFilter(wgpu::FilterMode value) &;
    SamplerDescriptor&& setMinFilter(wgpu::FilterMode value) &&;
    SamplerDescriptor& setMipmapFilter(wgpu::MipmapFilterMode value) &;
    SamplerDescriptor&& setMipmapFilter(wgpu::MipmapFilterMode value) &&;
    SamplerDescriptor& setLodMinClamp(float value) &;
    SamplerDescriptor&& setLodMinClamp(float value) &&;
    SamplerDescriptor& setLodMaxClamp(float value) &;
    SamplerDescriptor&& setLodMaxClamp(float value) &&;
    SamplerDescriptor& setCompare(wgpu::CompareFunction value) &;
    SamplerDescriptor&& setCompare(wgpu::CompareFunction value) &&;
    SamplerDescriptor& setMaxAnisotropy(uint16_t value) &;
    SamplerDescriptor&& setMaxAnisotropy(uint16_t value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::AddressMode addressModeU;
    wgpu::AddressMode addressModeV;
    wgpu::AddressMode addressModeW;
    wgpu::FilterMode magFilter;
    wgpu::FilterMode minFilter;
    wgpu::MipmapFilterMode mipmapFilter;
    float lodMinClamp;
    float lodMaxClamp;
    wgpu::CompareFunction compare;
    uint16_t maxAnisotropy;
};
struct ShaderSourceSPIRV {
    struct CStruct : public WGPUShaderSourceSPIRV {
    };
    ShaderSourceSPIRV(const WGPUShaderSourceSPIRV& native);
    ShaderSourceSPIRV() {};
    CStruct to_cstruct() const;
    template <typename T>
    ShaderSourceSPIRV& setNext(T&& value) &;
    template <typename T>
    ShaderSourceSPIRV&& setNext(T&& value) &&;
    ShaderSourceSPIRV& setCodeSize(uint32_t value) &;
    ShaderSourceSPIRV&& setCodeSize(uint32_t value) &&;
    ShaderSourceSPIRV& setCode(uint32_t const* value) &;
    ShaderSourceSPIRV&& setCode(uint32_t const* value) &&;
    wgpu::ChainedStruct chain;
    uint32_t codeSize;
    uint32_t const* code;
};
struct ShaderSourceWGSL {
    struct CStruct : public WGPUShaderSourceWGSL {
    };
    ShaderSourceWGSL(const WGPUShaderSourceWGSL& native);
    ShaderSourceWGSL() {};
    CStruct to_cstruct() const;
    template <typename T>
    ShaderSourceWGSL& setNext(T&& value) &;
    template <typename T>
    ShaderSourceWGSL&& setNext(T&& value) &&;
    ShaderSourceWGSL& setCode(const wgpu::StringView& value) &;
    ShaderSourceWGSL&& setCode(const wgpu::StringView& value) &&;
    ShaderSourceWGSL& setCode(wgpu::StringView&& value) &;
    ShaderSourceWGSL&& setCode(wgpu::StringView&& value) &&;
    wgpu::ChainedStruct chain;
    wgpu::StringView code;
};
struct StencilFaceState {
    struct CStruct : public WGPUStencilFaceState {
    };
    StencilFaceState(const WGPUStencilFaceState& native);
    StencilFaceState() : StencilFaceState(WGPU_STENCIL_FACE_STATE_INIT) {};
    CStruct to_cstruct() const;
    StencilFaceState& setCompare(wgpu::CompareFunction value) &;
    StencilFaceState&& setCompare(wgpu::CompareFunction value) &&;
    StencilFaceState& setFailOp(wgpu::StencilOperation value) &;
    StencilFaceState&& setFailOp(wgpu::StencilOperation value) &&;
    StencilFaceState& setDepthFailOp(wgpu::StencilOperation value) &;
    StencilFaceState&& setDepthFailOp(wgpu::StencilOperation value) &&;
    StencilFaceState& setPassOp(wgpu::StencilOperation value) &;
    StencilFaceState&& setPassOp(wgpu::StencilOperation value) &&;
    wgpu::CompareFunction compare;
    wgpu::StencilOperation failOp;
    wgpu::StencilOperation depthFailOp;
    wgpu::StencilOperation passOp;
};
struct StorageTextureBindingLayout {
    struct CStruct : public WGPUStorageTextureBindingLayout {
    };
    StorageTextureBindingLayout(const WGPUStorageTextureBindingLayout& native);
    StorageTextureBindingLayout() : StorageTextureBindingLayout(WGPU_STORAGE_TEXTURE_BINDING_LAYOUT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    StorageTextureBindingLayout& setNextInChain(T&& value) &;
    template <typename T>
    StorageTextureBindingLayout&& setNextInChain(T&& value) &&;
    StorageTextureBindingLayout& setAccess(wgpu::StorageTextureAccess value) &;
    StorageTextureBindingLayout&& setAccess(wgpu::StorageTextureAccess value) &&;
    StorageTextureBindingLayout& setFormat(wgpu::TextureFormat value) &;
    StorageTextureBindingLayout&& setFormat(wgpu::TextureFormat value) &&;
    StorageTextureBindingLayout& setViewDimension(wgpu::TextureViewDimension value) &;
    StorageTextureBindingLayout&& setViewDimension(wgpu::TextureViewDimension value) &&;
    NextInChain nextInChain;
    wgpu::StorageTextureAccess access;
    wgpu::TextureFormat format;
    wgpu::TextureViewDimension viewDimension;
};
struct SupportedFeatures {
    struct CStruct : public WGPUSupportedFeatures {
    };
    SupportedFeatures(const WGPUSupportedFeatures& native);
    SupportedFeatures() : SupportedFeatures(WGPU_SUPPORTED_FEATURES_INIT) {};
    CStruct to_cstruct() const;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
    SupportedFeatures& setFeatures(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
    SupportedFeatures&& setFeatures(T&& values) &&;
    std::vector<wgpu::FeatureName> features;
};
struct SupportedInstanceFeatures {
    struct CStruct : public WGPUSupportedInstanceFeatures {
    };
    SupportedInstanceFeatures(const WGPUSupportedInstanceFeatures& native);
    SupportedInstanceFeatures() : SupportedInstanceFeatures(WGPU_SUPPORTED_INSTANCE_FEATURES_INIT) {};
    CStruct to_cstruct() const;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
    SupportedInstanceFeatures& setFeatures(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
    SupportedInstanceFeatures&& setFeatures(T&& values) &&;
    std::vector<wgpu::InstanceFeatureName> features;
};
struct SupportedWGSLLanguageFeatures {
    struct CStruct : public WGPUSupportedWGSLLanguageFeatures {
    };
    SupportedWGSLLanguageFeatures(const WGPUSupportedWGSLLanguageFeatures& native);
    SupportedWGSLLanguageFeatures() {};
    CStruct to_cstruct() const;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::WGSLLanguageFeatureName>
    SupportedWGSLLanguageFeatures& setFeatures(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::WGSLLanguageFeatureName>
    SupportedWGSLLanguageFeatures&& setFeatures(T&& values) &&;
    std::vector<wgpu::WGSLLanguageFeatureName> features;
};
struct SurfaceCapabilities {
    struct CStruct : public WGPUSurfaceCapabilities {
    };
    SurfaceCapabilities(const WGPUSurfaceCapabilities& native);
    SurfaceCapabilities() : SurfaceCapabilities(WGPU_SURFACE_CAPABILITIES_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceCapabilities& setNextInChain(T&& value) &;
    template <typename T>
    SurfaceCapabilities&& setNextInChain(T&& value) &&;
    SurfaceCapabilities& setUsages(wgpu::TextureUsage value) &;
    SurfaceCapabilities&& setUsages(wgpu::TextureUsage value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    SurfaceCapabilities& setFormats(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    SurfaceCapabilities&& setFormats(T&& values) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::PresentMode>
    SurfaceCapabilities& setPresentModes(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::PresentMode>
    SurfaceCapabilities&& setPresentModes(T&& values) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompositeAlphaMode>
    SurfaceCapabilities& setAlphaModes(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompositeAlphaMode>
    SurfaceCapabilities&& setAlphaModes(T&& values) &&;
    NextInChain nextInChain;
    wgpu::TextureUsage usages;
    std::vector<wgpu::TextureFormat> formats;
    std::vector<wgpu::PresentMode> presentModes;
    std::vector<wgpu::CompositeAlphaMode> alphaModes;
};
struct SurfaceColorManagement {
    struct CStruct : public WGPUSurfaceColorManagement {
    };
    SurfaceColorManagement(const WGPUSurfaceColorManagement& native);
    SurfaceColorManagement() : SurfaceColorManagement(WGPU_SURFACE_COLOR_MANAGEMENT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceColorManagement& setNext(T&& value) &;
    template <typename T>
    SurfaceColorManagement&& setNext(T&& value) &&;
    SurfaceColorManagement& setColorSpace(wgpu::PredefinedColorSpace value) &;
    SurfaceColorManagement&& setColorSpace(wgpu::PredefinedColorSpace value) &&;
    SurfaceColorManagement& setToneMappingMode(wgpu::ToneMappingMode value) &;
    SurfaceColorManagement&& setToneMappingMode(wgpu::ToneMappingMode value) &&;
    wgpu::ChainedStruct chain;
    wgpu::PredefinedColorSpace colorSpace;
    wgpu::ToneMappingMode toneMappingMode;
};
struct SurfaceConfiguration {
    struct CStruct : public WGPUSurfaceConfiguration {
    };
    SurfaceConfiguration(const WGPUSurfaceConfiguration& native);
    SurfaceConfiguration() : SurfaceConfiguration(WGPU_SURFACE_CONFIGURATION_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceConfiguration& setNextInChain(T&& value) &;
    template <typename T>
    SurfaceConfiguration&& setNextInChain(T&& value) &&;
    SurfaceConfiguration& setDevice(wgpu::raw::Device value) &;
    SurfaceConfiguration&& setDevice(wgpu::raw::Device value) &&;
    SurfaceConfiguration& setFormat(wgpu::TextureFormat value) &;
    SurfaceConfiguration&& setFormat(wgpu::TextureFormat value) &&;
    SurfaceConfiguration& setUsage(wgpu::TextureUsage value) &;
    SurfaceConfiguration&& setUsage(wgpu::TextureUsage value) &&;
    SurfaceConfiguration& setWidth(uint32_t value) &;
    SurfaceConfiguration&& setWidth(uint32_t value) &&;
    SurfaceConfiguration& setHeight(uint32_t value) &;
    SurfaceConfiguration&& setHeight(uint32_t value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    SurfaceConfiguration& setViewFormats(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    SurfaceConfiguration&& setViewFormats(T&& values) &&;
    SurfaceConfiguration& setAlphaMode(wgpu::CompositeAlphaMode value) &;
    SurfaceConfiguration&& setAlphaMode(wgpu::CompositeAlphaMode value) &&;
    SurfaceConfiguration& setPresentMode(wgpu::PresentMode value) &;
    SurfaceConfiguration&& setPresentMode(wgpu::PresentMode value) &&;
    NextInChain nextInChain;
    wgpu::raw::Device device;
    wgpu::TextureFormat format;
    wgpu::TextureUsage usage;
    uint32_t width;
    uint32_t height;
    std::vector<wgpu::TextureFormat> viewFormats;
    wgpu::CompositeAlphaMode alphaMode;
    wgpu::PresentMode presentMode;
};
struct SurfaceSourceAndroidNativeWindow {
    struct CStruct : public WGPUSurfaceSourceAndroidNativeWindow {
    };
    SurfaceSourceAndroidNativeWindow(const WGPUSurfaceSourceAndroidNativeWindow& native);
    SurfaceSourceAndroidNativeWindow() : SurfaceSourceAndroidNativeWindow(WGPU_SURFACE_SOURCE_ANDROID_NATIVE_WINDOW_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceSourceAndroidNativeWindow& setNext(T&& value) &;
    template <typename T>
    SurfaceSourceAndroidNativeWindow&& setNext(T&& value) &&;
    SurfaceSourceAndroidNativeWindow& setWindow(void* value) &;
    SurfaceSourceAndroidNativeWindow&& setWindow(void* value) &&;
    wgpu::ChainedStruct chain;
    void* window;
};
struct SurfaceSourceMetalLayer {
    struct CStruct : public WGPUSurfaceSourceMetalLayer {
    };
    SurfaceSourceMetalLayer(const WGPUSurfaceSourceMetalLayer& native);
    SurfaceSourceMetalLayer() : SurfaceSourceMetalLayer(WGPU_SURFACE_SOURCE_METAL_LAYER_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceSourceMetalLayer& setNext(T&& value) &;
    template <typename T>
    SurfaceSourceMetalLayer&& setNext(T&& value) &&;
    SurfaceSourceMetalLayer& setLayer(void* value) &;
    SurfaceSourceMetalLayer&& setLayer(void* value) &&;
    wgpu::ChainedStruct chain;
    void* layer;
};
struct SurfaceSourceWaylandSurface {
    struct CStruct : public WGPUSurfaceSourceWaylandSurface {
    };
    SurfaceSourceWaylandSurface(const WGPUSurfaceSourceWaylandSurface& native);
    SurfaceSourceWaylandSurface() : SurfaceSourceWaylandSurface(WGPU_SURFACE_SOURCE_WAYLAND_SURFACE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceSourceWaylandSurface& setNext(T&& value) &;
    template <typename T>
    SurfaceSourceWaylandSurface&& setNext(T&& value) &&;
    SurfaceSourceWaylandSurface& setDisplay(void* value) &;
    SurfaceSourceWaylandSurface&& setDisplay(void* value) &&;
    SurfaceSourceWaylandSurface& setSurface(void* value) &;
    SurfaceSourceWaylandSurface&& setSurface(void* value) &&;
    wgpu::ChainedStruct chain;
    void* display;
    void* surface;
};
struct SurfaceSourceWindowsHWND {
    struct CStruct : public WGPUSurfaceSourceWindowsHWND {
    };
    SurfaceSourceWindowsHWND(const WGPUSurfaceSourceWindowsHWND& native);
    SurfaceSourceWindowsHWND() {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceSourceWindowsHWND& setNext(T&& value) &;
    template <typename T>
    SurfaceSourceWindowsHWND&& setNext(T&& value) &&;
    SurfaceSourceWindowsHWND& setHinstance(void* value) &;
    SurfaceSourceWindowsHWND&& setHinstance(void* value) &&;
    SurfaceSourceWindowsHWND& setHwnd(void* value) &;
    SurfaceSourceWindowsHWND&& setHwnd(void* value) &&;
    wgpu::ChainedStruct chain;
    void* hinstance;
    void* hwnd;
};
struct SurfaceSourceXCBWindow {
    struct CStruct : public WGPUSurfaceSourceXCBWindow {
    };
    SurfaceSourceXCBWindow(const WGPUSurfaceSourceXCBWindow& native);
    SurfaceSourceXCBWindow() {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceSourceXCBWindow& setNext(T&& value) &;
    template <typename T>
    SurfaceSourceXCBWindow&& setNext(T&& value) &&;
    SurfaceSourceXCBWindow& setConnection(void* value) &;
    SurfaceSourceXCBWindow&& setConnection(void* value) &&;
    SurfaceSourceXCBWindow& setWindow(uint32_t value) &;
    SurfaceSourceXCBWindow&& setWindow(uint32_t value) &&;
    wgpu::ChainedStruct chain;
    void* connection;
    uint32_t window;
};
struct SurfaceSourceXlibWindow {
    struct CStruct : public WGPUSurfaceSourceXlibWindow {
    };
    SurfaceSourceXlibWindow(const WGPUSurfaceSourceXlibWindow& native);
    SurfaceSourceXlibWindow() : SurfaceSourceXlibWindow(WGPU_SURFACE_SOURCE_XLIB_WINDOW_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceSourceXlibWindow& setNext(T&& value) &;
    template <typename T>
    SurfaceSourceXlibWindow&& setNext(T&& value) &&;
    SurfaceSourceXlibWindow& setDisplay(void* value) &;
    SurfaceSourceXlibWindow&& setDisplay(void* value) &&;
    SurfaceSourceXlibWindow& setWindow(uint64_t value) &;
    SurfaceSourceXlibWindow&& setWindow(uint64_t value) &&;
    wgpu::ChainedStruct chain;
    void* display;
    uint64_t window;
};
struct SurfaceTexture {
    struct CStruct : public WGPUSurfaceTexture {
    };
    SurfaceTexture(const WGPUSurfaceTexture& native);
    SurfaceTexture() : SurfaceTexture(WGPU_SURFACE_TEXTURE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceTexture& setNextInChain(T&& value) &;
    template <typename T>
    SurfaceTexture&& setNextInChain(T&& value) &&;
    SurfaceTexture& setTexture(wgpu::Texture value) &;
    SurfaceTexture&& setTexture(wgpu::Texture value) &&;
    SurfaceTexture& setStatus(wgpu::SurfaceGetCurrentTextureStatus value) &;
    SurfaceTexture&& setStatus(wgpu::SurfaceGetCurrentTextureStatus value) &&;
    NextInChain nextInChain;
    wgpu::Texture texture;
    wgpu::SurfaceGetCurrentTextureStatus status;
};
struct TexelCopyBufferLayout {
    struct CStruct : public WGPUTexelCopyBufferLayout {
    };
    TexelCopyBufferLayout(const WGPUTexelCopyBufferLayout& native);
    TexelCopyBufferLayout() : TexelCopyBufferLayout(WGPU_TEXEL_COPY_BUFFER_LAYOUT_INIT) {};
    CStruct to_cstruct() const;
    TexelCopyBufferLayout& setOffset(uint64_t value) &;
    TexelCopyBufferLayout&& setOffset(uint64_t value) &&;
    TexelCopyBufferLayout& setBytesPerRow(uint32_t value) &;
    TexelCopyBufferLayout&& setBytesPerRow(uint32_t value) &&;
    TexelCopyBufferLayout& setRowsPerImage(uint32_t value) &;
    TexelCopyBufferLayout&& setRowsPerImage(uint32_t value) &&;
    uint64_t offset;
    uint32_t bytesPerRow;
    uint32_t rowsPerImage;
};
struct TextureBindingLayout {
    struct CStruct : public WGPUTextureBindingLayout {
    };
    TextureBindingLayout(const WGPUTextureBindingLayout& native);
    TextureBindingLayout() : TextureBindingLayout(WGPU_TEXTURE_BINDING_LAYOUT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    TextureBindingLayout& setNextInChain(T&& value) &;
    template <typename T>
    TextureBindingLayout&& setNextInChain(T&& value) &&;
    TextureBindingLayout& setSampleType(wgpu::TextureSampleType value) &;
    TextureBindingLayout&& setSampleType(wgpu::TextureSampleType value) &&;
    TextureBindingLayout& setViewDimension(wgpu::TextureViewDimension value) &;
    TextureBindingLayout&& setViewDimension(wgpu::TextureViewDimension value) &&;
    TextureBindingLayout& setMultisampled(wgpu::Bool value) &;
    TextureBindingLayout&& setMultisampled(wgpu::Bool value) &&;
    NextInChain nextInChain;
    wgpu::TextureSampleType sampleType;
    wgpu::TextureViewDimension viewDimension;
    wgpu::Bool multisampled;
};
struct TextureComponentSwizzle {
    struct CStruct : public WGPUTextureComponentSwizzle {
    };
    TextureComponentSwizzle(const WGPUTextureComponentSwizzle& native);
    TextureComponentSwizzle() : TextureComponentSwizzle(WGPU_TEXTURE_COMPONENT_SWIZZLE_INIT) {};
    CStruct to_cstruct() const;
    TextureComponentSwizzle& setR(wgpu::ComponentSwizzle value) &;
    TextureComponentSwizzle&& setR(wgpu::ComponentSwizzle value) &&;
    TextureComponentSwizzle& setG(wgpu::ComponentSwizzle value) &;
    TextureComponentSwizzle&& setG(wgpu::ComponentSwizzle value) &&;
    TextureComponentSwizzle& setB(wgpu::ComponentSwizzle value) &;
    TextureComponentSwizzle&& setB(wgpu::ComponentSwizzle value) &&;
    TextureComponentSwizzle& setA(wgpu::ComponentSwizzle value) &;
    TextureComponentSwizzle&& setA(wgpu::ComponentSwizzle value) &&;
    wgpu::ComponentSwizzle r;
    wgpu::ComponentSwizzle g;
    wgpu::ComponentSwizzle b;
    wgpu::ComponentSwizzle a;
};
struct TextureViewDescriptor {
    struct CStruct : public WGPUTextureViewDescriptor {
    };
    TextureViewDescriptor(const WGPUTextureViewDescriptor& native);
    TextureViewDescriptor() : TextureViewDescriptor(WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    TextureViewDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    TextureViewDescriptor&& setNextInChain(T&& value) &&;
    TextureViewDescriptor& setLabel(const wgpu::StringView& value) &;
    TextureViewDescriptor&& setLabel(const wgpu::StringView& value) &&;
    TextureViewDescriptor& setLabel(wgpu::StringView&& value) &;
    TextureViewDescriptor&& setLabel(wgpu::StringView&& value) &&;
    TextureViewDescriptor& setFormat(wgpu::TextureFormat value) &;
    TextureViewDescriptor&& setFormat(wgpu::TextureFormat value) &&;
    TextureViewDescriptor& setDimension(wgpu::TextureViewDimension value) &;
    TextureViewDescriptor&& setDimension(wgpu::TextureViewDimension value) &&;
    TextureViewDescriptor& setBaseMipLevel(uint32_t value) &;
    TextureViewDescriptor&& setBaseMipLevel(uint32_t value) &&;
    TextureViewDescriptor& setMipLevelCount(uint32_t value) &;
    TextureViewDescriptor&& setMipLevelCount(uint32_t value) &&;
    TextureViewDescriptor& setBaseArrayLayer(uint32_t value) &;
    TextureViewDescriptor&& setBaseArrayLayer(uint32_t value) &&;
    TextureViewDescriptor& setArrayLayerCount(uint32_t value) &;
    TextureViewDescriptor&& setArrayLayerCount(uint32_t value) &&;
    TextureViewDescriptor& setAspect(wgpu::TextureAspect value) &;
    TextureViewDescriptor&& setAspect(wgpu::TextureAspect value) &&;
    TextureViewDescriptor& setUsage(wgpu::TextureUsage value) &;
    TextureViewDescriptor&& setUsage(wgpu::TextureUsage value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::TextureFormat format;
    wgpu::TextureViewDimension dimension;
    uint32_t baseMipLevel;
    uint32_t mipLevelCount;
    uint32_t baseArrayLayer;
    uint32_t arrayLayerCount;
    wgpu::TextureAspect aspect;
    wgpu::TextureUsage usage;
};
struct VertexAttribute {
    struct CStruct : public WGPUVertexAttribute {
    };
    VertexAttribute(const WGPUVertexAttribute& native);
    VertexAttribute() : VertexAttribute(WGPU_VERTEX_ATTRIBUTE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    VertexAttribute& setNextInChain(T&& value) &;
    template <typename T>
    VertexAttribute&& setNextInChain(T&& value) &&;
    VertexAttribute& setFormat(wgpu::VertexFormat value) &;
    VertexAttribute&& setFormat(wgpu::VertexFormat value) &&;
    VertexAttribute& setOffset(uint64_t value) &;
    VertexAttribute&& setOffset(uint64_t value) &&;
    VertexAttribute& setShaderLocation(uint32_t value) &;
    VertexAttribute&& setShaderLocation(uint32_t value) &&;
    NextInChain nextInChain;
    wgpu::VertexFormat format;
    uint64_t offset;
    uint32_t shaderLocation;
};
struct BindGroupEntry {
    struct CStruct : public WGPUBindGroupEntry {
    };
    BindGroupEntry(const WGPUBindGroupEntry& native);
    BindGroupEntry() : BindGroupEntry(WGPU_BIND_GROUP_ENTRY_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    BindGroupEntry& setNextInChain(T&& value) &;
    template <typename T>
    BindGroupEntry&& setNextInChain(T&& value) &&;
    BindGroupEntry& setBinding(uint32_t value) &;
    BindGroupEntry&& setBinding(uint32_t value) &&;
    BindGroupEntry& setBuffer(wgpu::raw::Buffer value) &;
    BindGroupEntry&& setBuffer(wgpu::raw::Buffer value) &&;
    BindGroupEntry& setOffset(uint64_t value) &;
    BindGroupEntry&& setOffset(uint64_t value) &&;
    BindGroupEntry& setSize(uint64_t value) &;
    BindGroupEntry&& setSize(uint64_t value) &&;
    BindGroupEntry& setSampler(wgpu::raw::Sampler value) &;
    BindGroupEntry&& setSampler(wgpu::raw::Sampler value) &&;
    BindGroupEntry& setTextureView(wgpu::raw::TextureView value) &;
    BindGroupEntry&& setTextureView(wgpu::raw::TextureView value) &&;
    NextInChain nextInChain;
    uint32_t binding;
    wgpu::raw::Buffer buffer;
    uint64_t offset;
    uint64_t size;
    wgpu::raw::Sampler sampler;
    wgpu::raw::TextureView textureView;
};
struct BindGroupLayoutEntry {
    struct CStruct : public WGPUBindGroupLayoutEntry {
    };
    BindGroupLayoutEntry(const WGPUBindGroupLayoutEntry& native);
    BindGroupLayoutEntry() : BindGroupLayoutEntry(WGPU_BIND_GROUP_LAYOUT_ENTRY_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    BindGroupLayoutEntry& setNextInChain(T&& value) &;
    template <typename T>
    BindGroupLayoutEntry&& setNextInChain(T&& value) &&;
    BindGroupLayoutEntry& setBinding(uint32_t value) &;
    BindGroupLayoutEntry&& setBinding(uint32_t value) &&;
    BindGroupLayoutEntry& setVisibility(wgpu::ShaderStage value) &;
    BindGroupLayoutEntry&& setVisibility(wgpu::ShaderStage value) &&;
    BindGroupLayoutEntry& setBindingArraySize(uint32_t value) &;
    BindGroupLayoutEntry&& setBindingArraySize(uint32_t value) &&;
    BindGroupLayoutEntry& setBuffer(const wgpu::BufferBindingLayout& value) &;
    BindGroupLayoutEntry&& setBuffer(const wgpu::BufferBindingLayout& value) &&;
    BindGroupLayoutEntry& setBuffer(wgpu::BufferBindingLayout&& value) &;
    BindGroupLayoutEntry&& setBuffer(wgpu::BufferBindingLayout&& value) &&;
    BindGroupLayoutEntry& setSampler(const wgpu::SamplerBindingLayout& value) &;
    BindGroupLayoutEntry&& setSampler(const wgpu::SamplerBindingLayout& value) &&;
    BindGroupLayoutEntry& setSampler(wgpu::SamplerBindingLayout&& value) &;
    BindGroupLayoutEntry&& setSampler(wgpu::SamplerBindingLayout&& value) &&;
    BindGroupLayoutEntry& setTexture(const wgpu::TextureBindingLayout& value) &;
    BindGroupLayoutEntry&& setTexture(const wgpu::TextureBindingLayout& value) &&;
    BindGroupLayoutEntry& setTexture(wgpu::TextureBindingLayout&& value) &;
    BindGroupLayoutEntry&& setTexture(wgpu::TextureBindingLayout&& value) &&;
    BindGroupLayoutEntry& setStorageTexture(const wgpu::StorageTextureBindingLayout& value) &;
    BindGroupLayoutEntry&& setStorageTexture(const wgpu::StorageTextureBindingLayout& value) &&;
    BindGroupLayoutEntry& setStorageTexture(wgpu::StorageTextureBindingLayout&& value) &;
    BindGroupLayoutEntry&& setStorageTexture(wgpu::StorageTextureBindingLayout&& value) &&;
    NextInChain nextInChain;
    uint32_t binding;
    wgpu::ShaderStage visibility;
    uint32_t bindingArraySize;
    wgpu::BufferBindingLayout buffer;
    wgpu::SamplerBindingLayout sampler;
    wgpu::TextureBindingLayout texture;
    wgpu::StorageTextureBindingLayout storageTexture;
};
struct BlendState {
    struct CStruct : public WGPUBlendState {
    };
    BlendState(const WGPUBlendState& native);
    BlendState() : BlendState(WGPU_BLEND_STATE_INIT) {};
    CStruct to_cstruct() const;
    BlendState& setColor(const wgpu::BlendComponent& value) &;
    BlendState&& setColor(const wgpu::BlendComponent& value) &&;
    BlendState& setColor(wgpu::BlendComponent&& value) &;
    BlendState&& setColor(wgpu::BlendComponent&& value) &&;
    BlendState& setAlpha(const wgpu::BlendComponent& value) &;
    BlendState&& setAlpha(const wgpu::BlendComponent& value) &&;
    BlendState& setAlpha(wgpu::BlendComponent&& value) &;
    BlendState&& setAlpha(wgpu::BlendComponent&& value) &&;
    wgpu::BlendComponent color;
    wgpu::BlendComponent alpha;
};
struct CompilationInfo {
    struct CStruct : public WGPUCompilationInfo {
        std::vector<WGPUCompilationMessage> messages_vec;
    };
    CompilationInfo(const WGPUCompilationInfo& native);
    CompilationInfo() : CompilationInfo(WGPU_COMPILATION_INFO_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    CompilationInfo& setNextInChain(T&& value) &;
    template <typename T>
    CompilationInfo&& setNextInChain(T&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompilationMessage>
    CompilationInfo& setMessages(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompilationMessage>
    CompilationInfo&& setMessages(T&& values) &&;
    NextInChain nextInChain;
    std::vector<wgpu::CompilationMessage> messages;
};
struct ComputePassDescriptor {
    struct CStruct : public WGPUComputePassDescriptor {
        wgpu::PassTimestampWrites::CStruct timestampWrites_cstruct;
    };
    ComputePassDescriptor(const WGPUComputePassDescriptor& native);
    ComputePassDescriptor() : ComputePassDescriptor(WGPU_COMPUTE_PASS_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ComputePassDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    ComputePassDescriptor&& setNextInChain(T&& value) &&;
    ComputePassDescriptor& setLabel(const wgpu::StringView& value) &;
    ComputePassDescriptor&& setLabel(const wgpu::StringView& value) &&;
    ComputePassDescriptor& setLabel(wgpu::StringView&& value) &;
    ComputePassDescriptor&& setLabel(wgpu::StringView&& value) &&;
    ComputePassDescriptor& setTimestampWrites(const wgpu::PassTimestampWrites& value) &;
    ComputePassDescriptor&& setTimestampWrites(const wgpu::PassTimestampWrites& value) &&;
    ComputePassDescriptor& setTimestampWrites(wgpu::PassTimestampWrites&& value) &;
    ComputePassDescriptor&& setTimestampWrites(wgpu::PassTimestampWrites&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    std::optional<wgpu::PassTimestampWrites> timestampWrites;
};
struct ComputeState {
    struct CStruct : public WGPUComputeState {
        std::vector<WGPUConstantEntry> constants_vec;
    };
    ComputeState(const WGPUComputeState& native);
    ComputeState() : ComputeState(WGPU_COMPUTE_STATE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ComputeState& setNextInChain(T&& value) &;
    template <typename T>
    ComputeState&& setNextInChain(T&& value) &&;
    ComputeState& setModule(wgpu::raw::ShaderModule value) &;
    ComputeState&& setModule(wgpu::raw::ShaderModule value) &&;
    ComputeState& setEntryPoint(const wgpu::StringView& value) &;
    ComputeState&& setEntryPoint(const wgpu::StringView& value) &&;
    ComputeState& setEntryPoint(wgpu::StringView&& value) &;
    ComputeState&& setEntryPoint(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
    ComputeState& setConstants(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
    ComputeState&& setConstants(T&& values) &&;
    NextInChain nextInChain;
    wgpu::raw::ShaderModule module;
    wgpu::StringView entryPoint;
    std::vector<wgpu::ConstantEntry> constants;
};
struct DepthStencilState {
    struct CStruct : public WGPUDepthStencilState {
    };
    DepthStencilState(const WGPUDepthStencilState& native);
    DepthStencilState() : DepthStencilState(WGPU_DEPTH_STENCIL_STATE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    DepthStencilState& setNextInChain(T&& value) &;
    template <typename T>
    DepthStencilState&& setNextInChain(T&& value) &&;
    DepthStencilState& setFormat(wgpu::TextureFormat value) &;
    DepthStencilState&& setFormat(wgpu::TextureFormat value) &&;
    DepthStencilState& setDepthWriteEnabled(wgpu::OptionalBool value) &;
    DepthStencilState&& setDepthWriteEnabled(wgpu::OptionalBool value) &&;
    DepthStencilState& setDepthCompare(wgpu::CompareFunction value) &;
    DepthStencilState&& setDepthCompare(wgpu::CompareFunction value) &&;
    DepthStencilState& setStencilFront(const wgpu::StencilFaceState& value) &;
    DepthStencilState&& setStencilFront(const wgpu::StencilFaceState& value) &&;
    DepthStencilState& setStencilFront(wgpu::StencilFaceState&& value) &;
    DepthStencilState&& setStencilFront(wgpu::StencilFaceState&& value) &&;
    DepthStencilState& setStencilBack(const wgpu::StencilFaceState& value) &;
    DepthStencilState&& setStencilBack(const wgpu::StencilFaceState& value) &&;
    DepthStencilState& setStencilBack(wgpu::StencilFaceState&& value) &;
    DepthStencilState&& setStencilBack(wgpu::StencilFaceState&& value) &&;
    DepthStencilState& setStencilReadMask(uint32_t value) &;
    DepthStencilState&& setStencilReadMask(uint32_t value) &&;
    DepthStencilState& setStencilWriteMask(uint32_t value) &;
    DepthStencilState&& setStencilWriteMask(uint32_t value) &&;
    DepthStencilState& setDepthBias(int32_t value) &;
    DepthStencilState&& setDepthBias(int32_t value) &&;
    DepthStencilState& setDepthBiasSlopeScale(float value) &;
    DepthStencilState&& setDepthBiasSlopeScale(float value) &&;
    DepthStencilState& setDepthBiasClamp(float value) &;
    DepthStencilState&& setDepthBiasClamp(float value) &&;
    NextInChain nextInChain;
    wgpu::TextureFormat format;
    wgpu::OptionalBool depthWriteEnabled;
    wgpu::CompareFunction depthCompare;
    wgpu::StencilFaceState stencilFront;
    wgpu::StencilFaceState stencilBack;
    uint32_t stencilReadMask;
    uint32_t stencilWriteMask;
    int32_t depthBias;
    float depthBiasSlopeScale;
    float depthBiasClamp;
};
struct DeviceDescriptor {
    struct CStruct : public WGPUDeviceDescriptor {
        wgpu::Limits::CStruct requiredLimits_cstruct;
    };
    DeviceDescriptor(const WGPUDeviceDescriptor& native);
    DeviceDescriptor() : DeviceDescriptor(WGPU_DEVICE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    DeviceDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    DeviceDescriptor&& setNextInChain(T&& value) &&;
    DeviceDescriptor& setLabel(const wgpu::StringView& value) &;
    DeviceDescriptor&& setLabel(const wgpu::StringView& value) &&;
    DeviceDescriptor& setLabel(wgpu::StringView&& value) &;
    DeviceDescriptor&& setLabel(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
    DeviceDescriptor& setRequiredFeatures(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
    DeviceDescriptor&& setRequiredFeatures(T&& values) &&;
    DeviceDescriptor& setRequiredLimits(const wgpu::Limits& value) &;
    DeviceDescriptor&& setRequiredLimits(const wgpu::Limits& value) &&;
    DeviceDescriptor& setRequiredLimits(wgpu::Limits&& value) &;
    DeviceDescriptor&& setRequiredLimits(wgpu::Limits&& value) &&;
    DeviceDescriptor& setDefaultQueue(const wgpu::QueueDescriptor& value) &;
    DeviceDescriptor&& setDefaultQueue(const wgpu::QueueDescriptor& value) &&;
    DeviceDescriptor& setDefaultQueue(wgpu::QueueDescriptor&& value) &;
    DeviceDescriptor&& setDefaultQueue(wgpu::QueueDescriptor&& value) &&;
    DeviceDescriptor& setDeviceLostCallbackInfo(const wgpu::DeviceLostCallbackInfo& value) &;
    DeviceDescriptor&& setDeviceLostCallbackInfo(const wgpu::DeviceLostCallbackInfo& value) &&;
    DeviceDescriptor& setDeviceLostCallbackInfo(wgpu::DeviceLostCallbackInfo&& value) &;
    DeviceDescriptor&& setDeviceLostCallbackInfo(wgpu::DeviceLostCallbackInfo&& value) &&;
    DeviceDescriptor& setUncapturedErrorCallbackInfo(const wgpu::UncapturedErrorCallbackInfo& value) &;
    DeviceDescriptor&& setUncapturedErrorCallbackInfo(const wgpu::UncapturedErrorCallbackInfo& value) &&;
    DeviceDescriptor& setUncapturedErrorCallbackInfo(wgpu::UncapturedErrorCallbackInfo&& value) &;
    DeviceDescriptor&& setUncapturedErrorCallbackInfo(wgpu::UncapturedErrorCallbackInfo&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    std::vector<wgpu::FeatureName> requiredFeatures;
    std::optional<wgpu::Limits> requiredLimits;
    wgpu::QueueDescriptor defaultQueue;
    wgpu::DeviceLostCallbackInfo deviceLostCallbackInfo;
    wgpu::UncapturedErrorCallbackInfo uncapturedErrorCallbackInfo;
};
struct FutureWaitInfo {
    struct CStruct : public WGPUFutureWaitInfo {
    };
    FutureWaitInfo(const WGPUFutureWaitInfo& native);
    FutureWaitInfo() : FutureWaitInfo(WGPU_FUTURE_WAIT_INFO_INIT) {};
    CStruct to_cstruct() const;
    FutureWaitInfo& setFuture(const wgpu::Future& value) &;
    FutureWaitInfo&& setFuture(const wgpu::Future& value) &&;
    FutureWaitInfo& setFuture(wgpu::Future&& value) &;
    FutureWaitInfo&& setFuture(wgpu::Future&& value) &&;
    FutureWaitInfo& setCompleted(wgpu::Bool value) &;
    FutureWaitInfo&& setCompleted(wgpu::Bool value) &&;
    wgpu::Future future;
    wgpu::Bool completed;
};
struct InstanceDescriptor {
    struct CStruct : public WGPUInstanceDescriptor {
        wgpu::InstanceLimits::CStruct requiredLimits_cstruct;
    };
    InstanceDescriptor(const WGPUInstanceDescriptor& native);
    InstanceDescriptor() : InstanceDescriptor(WGPU_INSTANCE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    InstanceDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    InstanceDescriptor&& setNextInChain(T&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
    InstanceDescriptor& setRequiredFeatures(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
    InstanceDescriptor&& setRequiredFeatures(T&& values) &&;
    InstanceDescriptor& setRequiredLimits(const wgpu::InstanceLimits& value) &;
    InstanceDescriptor&& setRequiredLimits(const wgpu::InstanceLimits& value) &&;
    InstanceDescriptor& setRequiredLimits(wgpu::InstanceLimits&& value) &;
    InstanceDescriptor&& setRequiredLimits(wgpu::InstanceLimits&& value) &&;
    NextInChain nextInChain;
    std::vector<wgpu::InstanceFeatureName> requiredFeatures;
    std::optional<wgpu::InstanceLimits> requiredLimits;
};
struct RenderPassColorAttachment {
    struct CStruct : public WGPURenderPassColorAttachment {
    };
    RenderPassColorAttachment(const WGPURenderPassColorAttachment& native);
    RenderPassColorAttachment() : RenderPassColorAttachment(WGPU_RENDER_PASS_COLOR_ATTACHMENT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RenderPassColorAttachment& setNextInChain(T&& value) &;
    template <typename T>
    RenderPassColorAttachment&& setNextInChain(T&& value) &&;
    RenderPassColorAttachment& setView(wgpu::raw::TextureView value) &;
    RenderPassColorAttachment&& setView(wgpu::raw::TextureView value) &&;
    RenderPassColorAttachment& setDepthSlice(uint32_t value) &;
    RenderPassColorAttachment&& setDepthSlice(uint32_t value) &&;
    RenderPassColorAttachment& setResolveTarget(wgpu::raw::TextureView value) &;
    RenderPassColorAttachment&& setResolveTarget(wgpu::raw::TextureView value) &&;
    RenderPassColorAttachment& setLoadOp(wgpu::LoadOp value) &;
    RenderPassColorAttachment&& setLoadOp(wgpu::LoadOp value) &&;
    RenderPassColorAttachment& setStoreOp(wgpu::StoreOp value) &;
    RenderPassColorAttachment&& setStoreOp(wgpu::StoreOp value) &&;
    RenderPassColorAttachment& setClearValue(const wgpu::Color& value) &;
    RenderPassColorAttachment&& setClearValue(const wgpu::Color& value) &&;
    RenderPassColorAttachment& setClearValue(wgpu::Color&& value) &;
    RenderPassColorAttachment&& setClearValue(wgpu::Color&& value) &&;
    NextInChain nextInChain;
    wgpu::raw::TextureView view;
    uint32_t depthSlice;
    wgpu::raw::TextureView resolveTarget;
    wgpu::LoadOp loadOp;
    wgpu::StoreOp storeOp;
    wgpu::Color clearValue;
};
struct RequestAdapterOptions {
    struct CStruct : public WGPURequestAdapterOptions {
    };
    RequestAdapterOptions(const WGPURequestAdapterOptions& native);
    RequestAdapterOptions() : RequestAdapterOptions(WGPU_REQUEST_ADAPTER_OPTIONS_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RequestAdapterOptions& setNextInChain(T&& value) &;
    template <typename T>
    RequestAdapterOptions&& setNextInChain(T&& value) &&;
    RequestAdapterOptions& setFeatureLevel(wgpu::FeatureLevel value) &;
    RequestAdapterOptions&& setFeatureLevel(wgpu::FeatureLevel value) &&;
    RequestAdapterOptions& setPowerPreference(wgpu::PowerPreference value) &;
    RequestAdapterOptions&& setPowerPreference(wgpu::PowerPreference value) &&;
    RequestAdapterOptions& setForceFallbackAdapter(wgpu::Bool value) &;
    RequestAdapterOptions&& setForceFallbackAdapter(wgpu::Bool value) &&;
    RequestAdapterOptions& setBackendType(wgpu::BackendType value) &;
    RequestAdapterOptions&& setBackendType(wgpu::BackendType value) &&;
    RequestAdapterOptions& setCompatibleSurface(wgpu::raw::Surface value) &;
    RequestAdapterOptions&& setCompatibleSurface(wgpu::raw::Surface value) &&;
    NextInChain nextInChain;
    wgpu::FeatureLevel featureLevel;
    wgpu::PowerPreference powerPreference;
    wgpu::Bool forceFallbackAdapter;
    wgpu::BackendType backendType;
    wgpu::raw::Surface compatibleSurface;
};
struct ShaderModuleDescriptor {
    struct CStruct : public WGPUShaderModuleDescriptor {
    };
    ShaderModuleDescriptor(const WGPUShaderModuleDescriptor& native);
    ShaderModuleDescriptor() : ShaderModuleDescriptor(WGPU_SHADER_MODULE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ShaderModuleDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    ShaderModuleDescriptor&& setNextInChain(T&& value) &&;
    ShaderModuleDescriptor& setLabel(const wgpu::StringView& value) &;
    ShaderModuleDescriptor&& setLabel(const wgpu::StringView& value) &&;
    ShaderModuleDescriptor& setLabel(wgpu::StringView&& value) &;
    ShaderModuleDescriptor&& setLabel(wgpu::StringView&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
};
struct SurfaceDescriptor {
    struct CStruct : public WGPUSurfaceDescriptor {
    };
    SurfaceDescriptor(const WGPUSurfaceDescriptor& native);
    SurfaceDescriptor() : SurfaceDescriptor(WGPU_SURFACE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    SurfaceDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    SurfaceDescriptor&& setNextInChain(T&& value) &&;
    SurfaceDescriptor& setLabel(const wgpu::StringView& value) &;
    SurfaceDescriptor&& setLabel(const wgpu::StringView& value) &&;
    SurfaceDescriptor& setLabel(wgpu::StringView&& value) &;
    SurfaceDescriptor&& setLabel(wgpu::StringView&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
};
struct TexelCopyBufferInfo {
    struct CStruct : public WGPUTexelCopyBufferInfo {
    };
    TexelCopyBufferInfo(const WGPUTexelCopyBufferInfo& native);
    TexelCopyBufferInfo() : TexelCopyBufferInfo(WGPU_TEXEL_COPY_BUFFER_INFO_INIT) {};
    CStruct to_cstruct() const;
    TexelCopyBufferInfo& setLayout(const wgpu::TexelCopyBufferLayout& value) &;
    TexelCopyBufferInfo&& setLayout(const wgpu::TexelCopyBufferLayout& value) &&;
    TexelCopyBufferInfo& setLayout(wgpu::TexelCopyBufferLayout&& value) &;
    TexelCopyBufferInfo&& setLayout(wgpu::TexelCopyBufferLayout&& value) &&;
    TexelCopyBufferInfo& setBuffer(wgpu::raw::Buffer value) &;
    TexelCopyBufferInfo&& setBuffer(wgpu::raw::Buffer value) &&;
    wgpu::TexelCopyBufferLayout layout;
    wgpu::raw::Buffer buffer;
};
struct TexelCopyTextureInfo {
    struct CStruct : public WGPUTexelCopyTextureInfo {
    };
    TexelCopyTextureInfo(const WGPUTexelCopyTextureInfo& native);
    TexelCopyTextureInfo() : TexelCopyTextureInfo(WGPU_TEXEL_COPY_TEXTURE_INFO_INIT) {};
    CStruct to_cstruct() const;
    TexelCopyTextureInfo& setTexture(wgpu::raw::Texture value) &;
    TexelCopyTextureInfo&& setTexture(wgpu::raw::Texture value) &&;
    TexelCopyTextureInfo& setMipLevel(uint32_t value) &;
    TexelCopyTextureInfo&& setMipLevel(uint32_t value) &&;
    TexelCopyTextureInfo& setOrigin(const wgpu::Origin3D& value) &;
    TexelCopyTextureInfo&& setOrigin(const wgpu::Origin3D& value) &&;
    TexelCopyTextureInfo& setOrigin(wgpu::Origin3D&& value) &;
    TexelCopyTextureInfo&& setOrigin(wgpu::Origin3D&& value) &&;
    TexelCopyTextureInfo& setAspect(wgpu::TextureAspect value) &;
    TexelCopyTextureInfo&& setAspect(wgpu::TextureAspect value) &&;
    wgpu::raw::Texture texture;
    uint32_t mipLevel;
    wgpu::Origin3D origin;
    wgpu::TextureAspect aspect;
};
struct TextureComponentSwizzleDescriptor {
    struct CStruct : public WGPUTextureComponentSwizzleDescriptor {
    };
    TextureComponentSwizzleDescriptor(const WGPUTextureComponentSwizzleDescriptor& native);
    TextureComponentSwizzleDescriptor() : TextureComponentSwizzleDescriptor(WGPU_TEXTURE_COMPONENT_SWIZZLE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    TextureComponentSwizzleDescriptor& setNext(T&& value) &;
    template <typename T>
    TextureComponentSwizzleDescriptor&& setNext(T&& value) &&;
    TextureComponentSwizzleDescriptor& setSwizzle(const wgpu::TextureComponentSwizzle& value) &;
    TextureComponentSwizzleDescriptor&& setSwizzle(const wgpu::TextureComponentSwizzle& value) &&;
    TextureComponentSwizzleDescriptor& setSwizzle(wgpu::TextureComponentSwizzle&& value) &;
    TextureComponentSwizzleDescriptor&& setSwizzle(wgpu::TextureComponentSwizzle&& value) &&;
    wgpu::ChainedStruct chain;
    wgpu::TextureComponentSwizzle swizzle;
};
struct TextureDescriptor {
    struct CStruct : public WGPUTextureDescriptor {
    };
    TextureDescriptor(const WGPUTextureDescriptor& native);
    TextureDescriptor() : TextureDescriptor(WGPU_TEXTURE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    TextureDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    TextureDescriptor&& setNextInChain(T&& value) &&;
    TextureDescriptor& setLabel(const wgpu::StringView& value) &;
    TextureDescriptor&& setLabel(const wgpu::StringView& value) &&;
    TextureDescriptor& setLabel(wgpu::StringView&& value) &;
    TextureDescriptor&& setLabel(wgpu::StringView&& value) &&;
    TextureDescriptor& setUsage(wgpu::TextureUsage value) &;
    TextureDescriptor&& setUsage(wgpu::TextureUsage value) &&;
    TextureDescriptor& setDimension(wgpu::TextureDimension value) &;
    TextureDescriptor&& setDimension(wgpu::TextureDimension value) &&;
    TextureDescriptor& setSize(const wgpu::Extent3D& value) &;
    TextureDescriptor&& setSize(const wgpu::Extent3D& value) &&;
    TextureDescriptor& setSize(wgpu::Extent3D&& value) &;
    TextureDescriptor&& setSize(wgpu::Extent3D&& value) &&;
    TextureDescriptor& setFormat(wgpu::TextureFormat value) &;
    TextureDescriptor&& setFormat(wgpu::TextureFormat value) &&;
    TextureDescriptor& setMipLevelCount(uint32_t value) &;
    TextureDescriptor&& setMipLevelCount(uint32_t value) &&;
    TextureDescriptor& setSampleCount(uint32_t value) &;
    TextureDescriptor&& setSampleCount(uint32_t value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    TextureDescriptor& setViewFormats(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
    TextureDescriptor&& setViewFormats(T&& values) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::TextureUsage usage;
    wgpu::TextureDimension dimension;
    wgpu::Extent3D size;
    wgpu::TextureFormat format;
    uint32_t mipLevelCount;
    uint32_t sampleCount;
    std::vector<wgpu::TextureFormat> viewFormats;
};
struct VertexBufferLayout {
    struct CStruct : public WGPUVertexBufferLayout {
        std::vector<WGPUVertexAttribute> attributes_vec;
    };
    VertexBufferLayout(const WGPUVertexBufferLayout& native);
    VertexBufferLayout() : VertexBufferLayout(WGPU_VERTEX_BUFFER_LAYOUT_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    VertexBufferLayout& setNextInChain(T&& value) &;
    template <typename T>
    VertexBufferLayout&& setNextInChain(T&& value) &&;
    VertexBufferLayout& setStepMode(wgpu::VertexStepMode value) &;
    VertexBufferLayout&& setStepMode(wgpu::VertexStepMode value) &&;
    VertexBufferLayout& setArrayStride(uint64_t value) &;
    VertexBufferLayout&& setArrayStride(uint64_t value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexAttribute>
    VertexBufferLayout& setAttributes(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexAttribute>
    VertexBufferLayout&& setAttributes(T&& values) &&;
    NextInChain nextInChain;
    wgpu::VertexStepMode stepMode;
    uint64_t arrayStride;
    std::vector<wgpu::VertexAttribute> attributes;
};
struct BindGroupDescriptor {
    struct CStruct : public WGPUBindGroupDescriptor {
        std::vector<WGPUBindGroupEntry> entries_vec;
    };
    BindGroupDescriptor(const WGPUBindGroupDescriptor& native);
    BindGroupDescriptor() : BindGroupDescriptor(WGPU_BIND_GROUP_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    BindGroupDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    BindGroupDescriptor&& setNextInChain(T&& value) &&;
    BindGroupDescriptor& setLabel(const wgpu::StringView& value) &;
    BindGroupDescriptor&& setLabel(const wgpu::StringView& value) &&;
    BindGroupDescriptor& setLabel(wgpu::StringView&& value) &;
    BindGroupDescriptor&& setLabel(wgpu::StringView&& value) &&;
    BindGroupDescriptor& setLayout(wgpu::raw::BindGroupLayout value) &;
    BindGroupDescriptor&& setLayout(wgpu::raw::BindGroupLayout value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupEntry>
    BindGroupDescriptor& setEntries(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupEntry>
    BindGroupDescriptor&& setEntries(T&& values) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::raw::BindGroupLayout layout;
    std::vector<wgpu::BindGroupEntry> entries;
};
struct BindGroupLayoutDescriptor {
    struct CStruct : public WGPUBindGroupLayoutDescriptor {
        std::vector<WGPUBindGroupLayoutEntry> entries_vec;
    };
    BindGroupLayoutDescriptor(const WGPUBindGroupLayoutDescriptor& native);
    BindGroupLayoutDescriptor() : BindGroupLayoutDescriptor(WGPU_BIND_GROUP_LAYOUT_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    BindGroupLayoutDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    BindGroupLayoutDescriptor&& setNextInChain(T&& value) &&;
    BindGroupLayoutDescriptor& setLabel(const wgpu::StringView& value) &;
    BindGroupLayoutDescriptor&& setLabel(const wgpu::StringView& value) &&;
    BindGroupLayoutDescriptor& setLabel(wgpu::StringView&& value) &;
    BindGroupLayoutDescriptor&& setLabel(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupLayoutEntry>
    BindGroupLayoutDescriptor& setEntries(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupLayoutEntry>
    BindGroupLayoutDescriptor&& setEntries(T&& values) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    std::vector<wgpu::BindGroupLayoutEntry> entries;
};
struct ColorTargetState {
    struct CStruct : public WGPUColorTargetState {
    };
    ColorTargetState(const WGPUColorTargetState& native);
    ColorTargetState() : ColorTargetState(WGPU_COLOR_TARGET_STATE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ColorTargetState& setNextInChain(T&& value) &;
    template <typename T>
    ColorTargetState&& setNextInChain(T&& value) &&;
    ColorTargetState& setFormat(wgpu::TextureFormat value) &;
    ColorTargetState&& setFormat(wgpu::TextureFormat value) &&;
    ColorTargetState& setBlend(const wgpu::BlendState& value) &;
    ColorTargetState&& setBlend(const wgpu::BlendState& value) &&;
    ColorTargetState& setBlend(wgpu::BlendState&& value) &;
    ColorTargetState&& setBlend(wgpu::BlendState&& value) &&;
    ColorTargetState& setWriteMask(wgpu::ColorWriteMask value) &;
    ColorTargetState&& setWriteMask(wgpu::ColorWriteMask value) &&;
    NextInChain nextInChain;
    wgpu::TextureFormat format;
    std::optional<wgpu::BlendState> blend;
    wgpu::ColorWriteMask writeMask;
};
struct ComputePipelineDescriptor {
    struct CStruct : public WGPUComputePipelineDescriptor {
        wgpu::ComputeState::CStruct compute_cstruct;
    };
    ComputePipelineDescriptor(const WGPUComputePipelineDescriptor& native);
    ComputePipelineDescriptor() : ComputePipelineDescriptor(WGPU_COMPUTE_PIPELINE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    ComputePipelineDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    ComputePipelineDescriptor&& setNextInChain(T&& value) &&;
    ComputePipelineDescriptor& setLabel(const wgpu::StringView& value) &;
    ComputePipelineDescriptor&& setLabel(const wgpu::StringView& value) &&;
    ComputePipelineDescriptor& setLabel(wgpu::StringView&& value) &;
    ComputePipelineDescriptor&& setLabel(wgpu::StringView&& value) &&;
    ComputePipelineDescriptor& setLayout(wgpu::raw::PipelineLayout value) &;
    ComputePipelineDescriptor&& setLayout(wgpu::raw::PipelineLayout value) &&;
    ComputePipelineDescriptor& setCompute(const wgpu::ComputeState& value) &;
    ComputePipelineDescriptor&& setCompute(const wgpu::ComputeState& value) &&;
    ComputePipelineDescriptor& setCompute(wgpu::ComputeState&& value) &;
    ComputePipelineDescriptor&& setCompute(wgpu::ComputeState&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::raw::PipelineLayout layout;
    wgpu::ComputeState compute;
};
struct RenderPassDescriptor {
    struct CStruct : public WGPURenderPassDescriptor {
        std::vector<WGPURenderPassColorAttachment> colorAttachments_vec;
        wgpu::RenderPassDepthStencilAttachment::CStruct depthStencilAttachment_cstruct;
        wgpu::PassTimestampWrites::CStruct timestampWrites_cstruct;
    };
    RenderPassDescriptor(const WGPURenderPassDescriptor& native);
    RenderPassDescriptor() : RenderPassDescriptor(WGPU_RENDER_PASS_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RenderPassDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    RenderPassDescriptor&& setNextInChain(T&& value) &&;
    RenderPassDescriptor& setLabel(const wgpu::StringView& value) &;
    RenderPassDescriptor&& setLabel(const wgpu::StringView& value) &&;
    RenderPassDescriptor& setLabel(wgpu::StringView&& value) &;
    RenderPassDescriptor&& setLabel(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::RenderPassColorAttachment>
    RenderPassDescriptor& setColorAttachments(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::RenderPassColorAttachment>
    RenderPassDescriptor&& setColorAttachments(T&& values) &&;
    RenderPassDescriptor& setDepthStencilAttachment(const wgpu::RenderPassDepthStencilAttachment& value) &;
    RenderPassDescriptor&& setDepthStencilAttachment(const wgpu::RenderPassDepthStencilAttachment& value) &&;
    RenderPassDescriptor& setDepthStencilAttachment(wgpu::RenderPassDepthStencilAttachment&& value) &;
    RenderPassDescriptor&& setDepthStencilAttachment(wgpu::RenderPassDepthStencilAttachment&& value) &&;
    RenderPassDescriptor& setOcclusionQuerySet(wgpu::raw::QuerySet value) &;
    RenderPassDescriptor&& setOcclusionQuerySet(wgpu::raw::QuerySet value) &&;
    RenderPassDescriptor& setTimestampWrites(const wgpu::PassTimestampWrites& value) &;
    RenderPassDescriptor&& setTimestampWrites(const wgpu::PassTimestampWrites& value) &&;
    RenderPassDescriptor& setTimestampWrites(wgpu::PassTimestampWrites&& value) &;
    RenderPassDescriptor&& setTimestampWrites(wgpu::PassTimestampWrites&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    std::vector<wgpu::RenderPassColorAttachment> colorAttachments;
    std::optional<wgpu::RenderPassDepthStencilAttachment> depthStencilAttachment;
    wgpu::raw::QuerySet occlusionQuerySet;
    std::optional<wgpu::PassTimestampWrites> timestampWrites;
};
struct VertexState {
    struct CStruct : public WGPUVertexState {
        std::vector<WGPUConstantEntry> constants_vec;
        std::vector<wgpu::VertexBufferLayout::CStruct> buffers_cstruct_vec;
        std::vector<WGPUVertexBufferLayout> buffers_vec;
    };
    VertexState(const WGPUVertexState& native);
    VertexState() : VertexState(WGPU_VERTEX_STATE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    VertexState& setNextInChain(T&& value) &;
    template <typename T>
    VertexState&& setNextInChain(T&& value) &&;
    VertexState& setModule(wgpu::raw::ShaderModule value) &;
    VertexState&& setModule(wgpu::raw::ShaderModule value) &&;
    VertexState& setEntryPoint(const wgpu::StringView& value) &;
    VertexState&& setEntryPoint(const wgpu::StringView& value) &&;
    VertexState& setEntryPoint(wgpu::StringView&& value) &;
    VertexState&& setEntryPoint(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
    VertexState& setConstants(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
    VertexState&& setConstants(T&& values) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexBufferLayout>
    VertexState& setBuffers(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexBufferLayout>
    VertexState&& setBuffers(T&& values) &&;
    NextInChain nextInChain;
    wgpu::raw::ShaderModule module;
    wgpu::StringView entryPoint;
    std::vector<wgpu::ConstantEntry> constants;
    std::vector<wgpu::VertexBufferLayout> buffers;
};
struct FragmentState {
    struct CStruct : public WGPUFragmentState {
        std::vector<WGPUConstantEntry> constants_vec;
        std::vector<WGPUColorTargetState> targets_vec;
    };
    FragmentState(const WGPUFragmentState& native);
    FragmentState() : FragmentState(WGPU_FRAGMENT_STATE_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    FragmentState& setNextInChain(T&& value) &;
    template <typename T>
    FragmentState&& setNextInChain(T&& value) &&;
    FragmentState& setModule(wgpu::raw::ShaderModule value) &;
    FragmentState&& setModule(wgpu::raw::ShaderModule value) &&;
    FragmentState& setEntryPoint(const wgpu::StringView& value) &;
    FragmentState&& setEntryPoint(const wgpu::StringView& value) &&;
    FragmentState& setEntryPoint(wgpu::StringView&& value) &;
    FragmentState&& setEntryPoint(wgpu::StringView&& value) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
    FragmentState& setConstants(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
    FragmentState&& setConstants(T&& values) &&;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ColorTargetState>
    FragmentState& setTargets(T&& values) &;
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ColorTargetState>
    FragmentState&& setTargets(T&& values) &&;
    NextInChain nextInChain;
    wgpu::raw::ShaderModule module;
    wgpu::StringView entryPoint;
    std::vector<wgpu::ConstantEntry> constants;
    std::vector<wgpu::ColorTargetState> targets;
};
struct RenderPipelineDescriptor {
    struct CStruct : public WGPURenderPipelineDescriptor {
        wgpu::VertexState::CStruct vertex_cstruct;
        wgpu::DepthStencilState::CStruct depthStencil_cstruct;
        wgpu::FragmentState::CStruct fragment_cstruct;
    };
    RenderPipelineDescriptor(const WGPURenderPipelineDescriptor& native);
    RenderPipelineDescriptor() : RenderPipelineDescriptor(WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT) {};
    CStruct to_cstruct() const;
    template <typename T>
    RenderPipelineDescriptor& setNextInChain(T&& value) &;
    template <typename T>
    RenderPipelineDescriptor&& setNextInChain(T&& value) &&;
    RenderPipelineDescriptor& setLabel(const wgpu::StringView& value) &;
    RenderPipelineDescriptor&& setLabel(const wgpu::StringView& value) &&;
    RenderPipelineDescriptor& setLabel(wgpu::StringView&& value) &;
    RenderPipelineDescriptor&& setLabel(wgpu::StringView&& value) &&;
    RenderPipelineDescriptor& setLayout(wgpu::raw::PipelineLayout value) &;
    RenderPipelineDescriptor&& setLayout(wgpu::raw::PipelineLayout value) &&;
    RenderPipelineDescriptor& setVertex(const wgpu::VertexState& value) &;
    RenderPipelineDescriptor&& setVertex(const wgpu::VertexState& value) &&;
    RenderPipelineDescriptor& setVertex(wgpu::VertexState&& value) &;
    RenderPipelineDescriptor&& setVertex(wgpu::VertexState&& value) &&;
    RenderPipelineDescriptor& setPrimitive(const wgpu::PrimitiveState& value) &;
    RenderPipelineDescriptor&& setPrimitive(const wgpu::PrimitiveState& value) &&;
    RenderPipelineDescriptor& setPrimitive(wgpu::PrimitiveState&& value) &;
    RenderPipelineDescriptor&& setPrimitive(wgpu::PrimitiveState&& value) &&;
    RenderPipelineDescriptor& setDepthStencil(const wgpu::DepthStencilState& value) &;
    RenderPipelineDescriptor&& setDepthStencil(const wgpu::DepthStencilState& value) &&;
    RenderPipelineDescriptor& setDepthStencil(wgpu::DepthStencilState&& value) &;
    RenderPipelineDescriptor&& setDepthStencil(wgpu::DepthStencilState&& value) &&;
    RenderPipelineDescriptor& setMultisample(const wgpu::MultisampleState& value) &;
    RenderPipelineDescriptor&& setMultisample(const wgpu::MultisampleState& value) &&;
    RenderPipelineDescriptor& setMultisample(wgpu::MultisampleState&& value) &;
    RenderPipelineDescriptor&& setMultisample(wgpu::MultisampleState&& value) &&;
    RenderPipelineDescriptor& setFragment(const wgpu::FragmentState& value) &;
    RenderPipelineDescriptor&& setFragment(const wgpu::FragmentState& value) &&;
    RenderPipelineDescriptor& setFragment(wgpu::FragmentState&& value) &;
    RenderPipelineDescriptor&& setFragment(wgpu::FragmentState&& value) &&;
    NextInChain nextInChain;
    wgpu::StringView label;
    wgpu::raw::PipelineLayout layout;
    wgpu::VertexState vertex;
    wgpu::PrimitiveState primitive;
    std::optional<wgpu::DepthStencilState> depthStencil;
    wgpu::MultisampleState multisample;
    std::optional<wgpu::FragmentState> fragment;
};
}
// Non member functions
namespace wgpu {
wgpu::Instance createInstance(wgpu::InstanceDescriptor const& descriptor);
wgpu::Instance createInstance();
void getInstanceFeatures(wgpu::SupportedInstanceFeatures& features);
wgpu::Status getInstanceLimits(wgpu::InstanceLimits& limits);
wgpu::Bool hasInstanceFeature(wgpu::InstanceFeatureName feature);
}
}
namespace wgpu {
template <typename T>
ChainedStruct& ChainedStruct::setNext(T&& value) & {
    this->next.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ChainedStruct&& ChainedStruct::setNext(T&& value) && {
    this->next.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
BufferMapCallbackInfo& BufferMapCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
BufferMapCallbackInfo&& BufferMapCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
CompilationInfoCallbackInfo& CompilationInfoCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
CompilationInfoCallbackInfo&& CompilationInfoCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
CreateComputePipelineAsyncCallbackInfo& CreateComputePipelineAsyncCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
CreateComputePipelineAsyncCallbackInfo&& CreateComputePipelineAsyncCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
CreateRenderPipelineAsyncCallbackInfo& CreateRenderPipelineAsyncCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
CreateRenderPipelineAsyncCallbackInfo&& CreateRenderPipelineAsyncCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
DeviceLostCallbackInfo& DeviceLostCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
DeviceLostCallbackInfo&& DeviceLostCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
PopErrorScopeCallbackInfo& PopErrorScopeCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
PopErrorScopeCallbackInfo&& PopErrorScopeCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
QueueWorkDoneCallbackInfo& QueueWorkDoneCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
QueueWorkDoneCallbackInfo&& QueueWorkDoneCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RequestAdapterCallbackInfo& RequestAdapterCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RequestAdapterCallbackInfo&& RequestAdapterCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RequestDeviceCallbackInfo& RequestDeviceCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RequestDeviceCallbackInfo&& RequestDeviceCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
UncapturedErrorCallbackInfo& UncapturedErrorCallbackInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
UncapturedErrorCallbackInfo&& UncapturedErrorCallbackInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
AdapterInfo& AdapterInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
AdapterInfo&& AdapterInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
BufferBindingLayout& BufferBindingLayout::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
BufferBindingLayout&& BufferBindingLayout::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
BufferDescriptor& BufferDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
BufferDescriptor&& BufferDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
CommandBufferDescriptor& CommandBufferDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
CommandBufferDescriptor&& CommandBufferDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
CommandEncoderDescriptor& CommandEncoderDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
CommandEncoderDescriptor&& CommandEncoderDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
CompilationMessage& CompilationMessage::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
CompilationMessage&& CompilationMessage::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ConstantEntry& ConstantEntry::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ConstantEntry&& ConstantEntry::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ExternalTextureBindingEntry& ExternalTextureBindingEntry::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ExternalTextureBindingEntry&& ExternalTextureBindingEntry::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ExternalTextureBindingLayout& ExternalTextureBindingLayout::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ExternalTextureBindingLayout&& ExternalTextureBindingLayout::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
InstanceLimits& InstanceLimits::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
InstanceLimits&& InstanceLimits::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
Limits& Limits::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
Limits&& Limits::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
MultisampleState& MultisampleState::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
MultisampleState&& MultisampleState::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
PassTimestampWrites& PassTimestampWrites::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
PassTimestampWrites&& PassTimestampWrites::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
PipelineLayoutDescriptor& PipelineLayoutDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
PipelineLayoutDescriptor&& PipelineLayoutDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::raw::BindGroupLayout>
PipelineLayoutDescriptor& PipelineLayoutDescriptor::setBindGroupLayouts(T&& values) & {
    this->bindGroupLayouts = values | std::views::transform([](auto&& e) { return static_cast<wgpu::raw::BindGroupLayout>(e); }) | std::ranges::to<std::vector<wgpu::raw::BindGroupLayout>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::raw::BindGroupLayout>
PipelineLayoutDescriptor&& PipelineLayoutDescriptor::setBindGroupLayouts(T&& values) && {
    this->bindGroupLayouts = values | std::views::transform([](auto&& e) { return static_cast<wgpu::raw::BindGroupLayout>(e); }) | std::ranges::to<std::vector<wgpu::raw::BindGroupLayout>>();
    return std::move(*this);
}
template <typename T>
PrimitiveState& PrimitiveState::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
PrimitiveState&& PrimitiveState::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
QuerySetDescriptor& QuerySetDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
QuerySetDescriptor&& QuerySetDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
QueueDescriptor& QueueDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
QueueDescriptor&& QueueDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RenderBundleDescriptor& RenderBundleDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RenderBundleDescriptor&& RenderBundleDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setColorFormats(T&& values) & {
    this->colorFormats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setColorFormats(T&& values) && {
    this->colorFormats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return std::move(*this);
}
template <typename T>
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RenderPassMaxDrawCount& RenderPassMaxDrawCount::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RenderPassMaxDrawCount&& RenderPassMaxDrawCount::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RequestAdapterWebXROptions& RequestAdapterWebXROptions::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RequestAdapterWebXROptions&& RequestAdapterWebXROptions::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SamplerBindingLayout& SamplerBindingLayout::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SamplerBindingLayout&& SamplerBindingLayout::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SamplerDescriptor& SamplerDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SamplerDescriptor&& SamplerDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ShaderSourceSPIRV& ShaderSourceSPIRV::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ShaderSourceSPIRV&& ShaderSourceSPIRV::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ShaderSourceWGSL& ShaderSourceWGSL::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ShaderSourceWGSL&& ShaderSourceWGSL::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
StorageTextureBindingLayout& StorageTextureBindingLayout::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
StorageTextureBindingLayout&& StorageTextureBindingLayout::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
SupportedFeatures& SupportedFeatures::setFeatures(T&& values) & {
    this->features = values | std::views::transform([](auto&& e) { return static_cast<wgpu::FeatureName>(e); }) | std::ranges::to<std::vector<wgpu::FeatureName>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
SupportedFeatures&& SupportedFeatures::setFeatures(T&& values) && {
    this->features = values | std::views::transform([](auto&& e) { return static_cast<wgpu::FeatureName>(e); }) | std::ranges::to<std::vector<wgpu::FeatureName>>();
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
SupportedInstanceFeatures& SupportedInstanceFeatures::setFeatures(T&& values) & {
    this->features = values | std::views::transform([](auto&& e) { return static_cast<wgpu::InstanceFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::InstanceFeatureName>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
SupportedInstanceFeatures&& SupportedInstanceFeatures::setFeatures(T&& values) && {
    this->features = values | std::views::transform([](auto&& e) { return static_cast<wgpu::InstanceFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::InstanceFeatureName>>();
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::WGSLLanguageFeatureName>
SupportedWGSLLanguageFeatures& SupportedWGSLLanguageFeatures::setFeatures(T&& values) & {
    this->features = values | std::views::transform([](auto&& e) { return static_cast<wgpu::WGSLLanguageFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::WGSLLanguageFeatureName>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::WGSLLanguageFeatureName>
SupportedWGSLLanguageFeatures&& SupportedWGSLLanguageFeatures::setFeatures(T&& values) && {
    this->features = values | std::views::transform([](auto&& e) { return static_cast<wgpu::WGSLLanguageFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::WGSLLanguageFeatureName>>();
    return std::move(*this);
}
template <typename T>
SurfaceCapabilities& SurfaceCapabilities::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceCapabilities&& SurfaceCapabilities::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
SurfaceCapabilities& SurfaceCapabilities::setFormats(T&& values) & {
    this->formats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
SurfaceCapabilities&& SurfaceCapabilities::setFormats(T&& values) && {
    this->formats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::PresentMode>
SurfaceCapabilities& SurfaceCapabilities::setPresentModes(T&& values) & {
    this->presentModes = values | std::views::transform([](auto&& e) { return static_cast<wgpu::PresentMode>(e); }) | std::ranges::to<std::vector<wgpu::PresentMode>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::PresentMode>
SurfaceCapabilities&& SurfaceCapabilities::setPresentModes(T&& values) && {
    this->presentModes = values | std::views::transform([](auto&& e) { return static_cast<wgpu::PresentMode>(e); }) | std::ranges::to<std::vector<wgpu::PresentMode>>();
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompositeAlphaMode>
SurfaceCapabilities& SurfaceCapabilities::setAlphaModes(T&& values) & {
    this->alphaModes = values | std::views::transform([](auto&& e) { return static_cast<wgpu::CompositeAlphaMode>(e); }) | std::ranges::to<std::vector<wgpu::CompositeAlphaMode>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompositeAlphaMode>
SurfaceCapabilities&& SurfaceCapabilities::setAlphaModes(T&& values) && {
    this->alphaModes = values | std::views::transform([](auto&& e) { return static_cast<wgpu::CompositeAlphaMode>(e); }) | std::ranges::to<std::vector<wgpu::CompositeAlphaMode>>();
    return std::move(*this);
}
template <typename T>
SurfaceColorManagement& SurfaceColorManagement::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceColorManagement&& SurfaceColorManagement::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceConfiguration& SurfaceConfiguration::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceConfiguration&& SurfaceConfiguration::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
SurfaceConfiguration& SurfaceConfiguration::setViewFormats(T&& values) & {
    this->viewFormats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
SurfaceConfiguration&& SurfaceConfiguration::setViewFormats(T&& values) && {
    this->viewFormats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return std::move(*this);
}
template <typename T>
SurfaceSourceAndroidNativeWindow& SurfaceSourceAndroidNativeWindow::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceSourceAndroidNativeWindow&& SurfaceSourceAndroidNativeWindow::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceSourceMetalLayer& SurfaceSourceMetalLayer::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceSourceMetalLayer&& SurfaceSourceMetalLayer::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceSourceWaylandSurface& SurfaceSourceWaylandSurface::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceSourceWaylandSurface&& SurfaceSourceWaylandSurface::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceSourceWindowsHWND& SurfaceSourceWindowsHWND::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceSourceWindowsHWND&& SurfaceSourceWindowsHWND::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceSourceXCBWindow& SurfaceSourceXCBWindow::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceSourceXCBWindow&& SurfaceSourceXCBWindow::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceSourceXlibWindow& SurfaceSourceXlibWindow::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceSourceXlibWindow&& SurfaceSourceXlibWindow::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceTexture& SurfaceTexture::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceTexture&& SurfaceTexture::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
TextureBindingLayout& TextureBindingLayout::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
TextureBindingLayout&& TextureBindingLayout::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
TextureViewDescriptor& TextureViewDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
TextureViewDescriptor&& TextureViewDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
VertexAttribute& VertexAttribute::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
VertexAttribute&& VertexAttribute::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
BindGroupEntry& BindGroupEntry::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
BindGroupEntry&& BindGroupEntry::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
BindGroupLayoutEntry& BindGroupLayoutEntry::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
BindGroupLayoutEntry&& BindGroupLayoutEntry::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
CompilationInfo& CompilationInfo::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
CompilationInfo&& CompilationInfo::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompilationMessage>
CompilationInfo& CompilationInfo::setMessages(T&& values) & {
    this->messages = values | std::views::transform([](auto&& e) { return static_cast<wgpu::CompilationMessage>(e); }) | std::ranges::to<std::vector<wgpu::CompilationMessage>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::CompilationMessage>
CompilationInfo&& CompilationInfo::setMessages(T&& values) && {
    this->messages = values | std::views::transform([](auto&& e) { return static_cast<wgpu::CompilationMessage>(e); }) | std::ranges::to<std::vector<wgpu::CompilationMessage>>();
    return std::move(*this);
}
template <typename T>
ComputePassDescriptor& ComputePassDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ComputePassDescriptor&& ComputePassDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ComputeState& ComputeState::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ComputeState&& ComputeState::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
ComputeState& ComputeState::setConstants(T&& values) & {
    this->constants = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
ComputeState&& ComputeState::setConstants(T&& values) && {
    this->constants = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    return std::move(*this);
}
template <typename T>
DepthStencilState& DepthStencilState::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
DepthStencilState&& DepthStencilState::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
DeviceDescriptor& DeviceDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
DeviceDescriptor&& DeviceDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
DeviceDescriptor& DeviceDescriptor::setRequiredFeatures(T&& values) & {
    this->requiredFeatures = values | std::views::transform([](auto&& e) { return static_cast<wgpu::FeatureName>(e); }) | std::ranges::to<std::vector<wgpu::FeatureName>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::FeatureName>
DeviceDescriptor&& DeviceDescriptor::setRequiredFeatures(T&& values) && {
    this->requiredFeatures = values | std::views::transform([](auto&& e) { return static_cast<wgpu::FeatureName>(e); }) | std::ranges::to<std::vector<wgpu::FeatureName>>();
    return std::move(*this);
}
template <typename T>
InstanceDescriptor& InstanceDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
InstanceDescriptor&& InstanceDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
InstanceDescriptor& InstanceDescriptor::setRequiredFeatures(T&& values) & {
    this->requiredFeatures = values | std::views::transform([](auto&& e) { return static_cast<wgpu::InstanceFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::InstanceFeatureName>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::InstanceFeatureName>
InstanceDescriptor&& InstanceDescriptor::setRequiredFeatures(T&& values) && {
    this->requiredFeatures = values | std::views::transform([](auto&& e) { return static_cast<wgpu::InstanceFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::InstanceFeatureName>>();
    return std::move(*this);
}
template <typename T>
RenderPassColorAttachment& RenderPassColorAttachment::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RenderPassColorAttachment&& RenderPassColorAttachment::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RequestAdapterOptions& RequestAdapterOptions::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RequestAdapterOptions&& RequestAdapterOptions::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ShaderModuleDescriptor& ShaderModuleDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ShaderModuleDescriptor&& ShaderModuleDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
SurfaceDescriptor& SurfaceDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
SurfaceDescriptor&& SurfaceDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
TextureComponentSwizzleDescriptor& TextureComponentSwizzleDescriptor::setNext(T&& value) & {
    this->chain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
TextureComponentSwizzleDescriptor&& TextureComponentSwizzleDescriptor::setNext(T&& value) && {
    this->chain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
TextureDescriptor& TextureDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
TextureDescriptor&& TextureDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
TextureDescriptor& TextureDescriptor::setViewFormats(T&& values) & {
    this->viewFormats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::TextureFormat>
TextureDescriptor&& TextureDescriptor::setViewFormats(T&& values) && {
    this->viewFormats = values | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    return std::move(*this);
}
template <typename T>
VertexBufferLayout& VertexBufferLayout::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
VertexBufferLayout&& VertexBufferLayout::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexAttribute>
VertexBufferLayout& VertexBufferLayout::setAttributes(T&& values) & {
    this->attributes = values | std::views::transform([](auto&& e) { return static_cast<wgpu::VertexAttribute>(e); }) | std::ranges::to<std::vector<wgpu::VertexAttribute>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexAttribute>
VertexBufferLayout&& VertexBufferLayout::setAttributes(T&& values) && {
    this->attributes = values | std::views::transform([](auto&& e) { return static_cast<wgpu::VertexAttribute>(e); }) | std::ranges::to<std::vector<wgpu::VertexAttribute>>();
    return std::move(*this);
}
template <typename T>
BindGroupDescriptor& BindGroupDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
BindGroupDescriptor&& BindGroupDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupEntry>
BindGroupDescriptor& BindGroupDescriptor::setEntries(T&& values) & {
    this->entries = values | std::views::transform([](auto&& e) { return static_cast<wgpu::BindGroupEntry>(e); }) | std::ranges::to<std::vector<wgpu::BindGroupEntry>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupEntry>
BindGroupDescriptor&& BindGroupDescriptor::setEntries(T&& values) && {
    this->entries = values | std::views::transform([](auto&& e) { return static_cast<wgpu::BindGroupEntry>(e); }) | std::ranges::to<std::vector<wgpu::BindGroupEntry>>();
    return std::move(*this);
}
template <typename T>
BindGroupLayoutDescriptor& BindGroupLayoutDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
BindGroupLayoutDescriptor&& BindGroupLayoutDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupLayoutEntry>
BindGroupLayoutDescriptor& BindGroupLayoutDescriptor::setEntries(T&& values) & {
    this->entries = values | std::views::transform([](auto&& e) { return static_cast<wgpu::BindGroupLayoutEntry>(e); }) | std::ranges::to<std::vector<wgpu::BindGroupLayoutEntry>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::BindGroupLayoutEntry>
BindGroupLayoutDescriptor&& BindGroupLayoutDescriptor::setEntries(T&& values) && {
    this->entries = values | std::views::transform([](auto&& e) { return static_cast<wgpu::BindGroupLayoutEntry>(e); }) | std::ranges::to<std::vector<wgpu::BindGroupLayoutEntry>>();
    return std::move(*this);
}
template <typename T>
ColorTargetState& ColorTargetState::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ColorTargetState&& ColorTargetState::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
ComputePipelineDescriptor& ComputePipelineDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
ComputePipelineDescriptor&& ComputePipelineDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <typename T>
RenderPassDescriptor& RenderPassDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RenderPassDescriptor&& RenderPassDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::RenderPassColorAttachment>
RenderPassDescriptor& RenderPassDescriptor::setColorAttachments(T&& values) & {
    this->colorAttachments = values | std::views::transform([](auto&& e) { return static_cast<wgpu::RenderPassColorAttachment>(e); }) | std::ranges::to<std::vector<wgpu::RenderPassColorAttachment>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::RenderPassColorAttachment>
RenderPassDescriptor&& RenderPassDescriptor::setColorAttachments(T&& values) && {
    this->colorAttachments = values | std::views::transform([](auto&& e) { return static_cast<wgpu::RenderPassColorAttachment>(e); }) | std::ranges::to<std::vector<wgpu::RenderPassColorAttachment>>();
    return std::move(*this);
}
template <typename T>
VertexState& VertexState::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
VertexState&& VertexState::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
VertexState& VertexState::setConstants(T&& values) & {
    this->constants = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
VertexState&& VertexState::setConstants(T&& values) && {
    this->constants = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexBufferLayout>
VertexState& VertexState::setBuffers(T&& values) & {
    this->buffers = values | std::views::transform([](auto&& e) { return static_cast<wgpu::VertexBufferLayout>(e); }) | std::ranges::to<std::vector<wgpu::VertexBufferLayout>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::VertexBufferLayout>
VertexState&& VertexState::setBuffers(T&& values) && {
    this->buffers = values | std::views::transform([](auto&& e) { return static_cast<wgpu::VertexBufferLayout>(e); }) | std::ranges::to<std::vector<wgpu::VertexBufferLayout>>();
    return std::move(*this);
}
template <typename T>
FragmentState& FragmentState::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
FragmentState&& FragmentState::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
FragmentState& FragmentState::setConstants(T&& values) & {
    this->constants = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ConstantEntry>
FragmentState&& FragmentState::setConstants(T&& values) && {
    this->constants = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    return std::move(*this);
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ColorTargetState>
FragmentState& FragmentState::setTargets(T&& values) & {
    this->targets = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ColorTargetState>(e); }) | std::ranges::to<std::vector<wgpu::ColorTargetState>>();
    return *this;
}
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, wgpu::ColorTargetState>
FragmentState&& FragmentState::setTargets(T&& values) && {
    this->targets = values | std::views::transform([](auto&& e) { return static_cast<wgpu::ColorTargetState>(e); }) | std::ranges::to<std::vector<wgpu::ColorTargetState>>();
    return std::move(*this);
}
template <typename T>
RenderPipelineDescriptor& RenderPipelineDescriptor::setNextInChain(T&& value) & {
    this->nextInChain.setNext(std::forward<T>(value));
    return *this;
}
template <typename T>
RenderPipelineDescriptor&& RenderPipelineDescriptor::setNextInChain(T&& value) && {
    this->nextInChain.setNext(std::forward<T>(value));
    return std::move(*this);
}
}
namespace wgpu::raw {
}
namespace wgpu {
template <std::invocable<wgpu::MapAsyncStatus, wgpu::StringView> F>
BufferMapCallback::BufferMapCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void BufferMapCallback::ControlImpl<F>::invoke(wgpu::MapAsyncStatus status, wgpu::StringView message) const {
    func(status, message);
}
template <std::invocable<wgpu::CompilationInfoRequestStatus, wgpu::CompilationInfo const&> F>
CompilationInfoCallback::CompilationInfoCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void CompilationInfoCallback::ControlImpl<F>::invoke(wgpu::CompilationInfoRequestStatus status, wgpu::CompilationInfo const& compilationInfo) const {
    func(status, compilationInfo);
}
template <std::invocable<wgpu::CreatePipelineAsyncStatus, wgpu::ComputePipeline const&, wgpu::StringView> F>
CreateComputePipelineAsyncCallback::CreateComputePipelineAsyncCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void CreateComputePipelineAsyncCallback::ControlImpl<F>::invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::ComputePipeline const& pipeline, wgpu::StringView message) const {
    func(status, pipeline, message);
}
template <std::invocable<wgpu::CreatePipelineAsyncStatus, wgpu::RenderPipeline const&, wgpu::StringView> F>
CreateRenderPipelineAsyncCallback::CreateRenderPipelineAsyncCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void CreateRenderPipelineAsyncCallback::ControlImpl<F>::invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::RenderPipeline const& pipeline, wgpu::StringView message) const {
    func(status, pipeline, message);
}
template <std::invocable<wgpu::Device const&, wgpu::DeviceLostReason, wgpu::StringView> F>
DeviceLostCallback::DeviceLostCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void DeviceLostCallback::ControlImpl<F>::invoke(wgpu::Device const& device, wgpu::DeviceLostReason reason, wgpu::StringView message) const {
    func(device, reason, message);
}
template <std::invocable<wgpu::PopErrorScopeStatus, wgpu::ErrorType, wgpu::StringView> F>
PopErrorScopeCallback::PopErrorScopeCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void PopErrorScopeCallback::ControlImpl<F>::invoke(wgpu::PopErrorScopeStatus status, wgpu::ErrorType type, wgpu::StringView message) const {
    func(status, type, message);
}
template <std::invocable<wgpu::QueueWorkDoneStatus, wgpu::StringView> F>
QueueWorkDoneCallback::QueueWorkDoneCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void QueueWorkDoneCallback::ControlImpl<F>::invoke(wgpu::QueueWorkDoneStatus status, wgpu::StringView message) const {
    func(status, message);
}
template <std::invocable<wgpu::RequestAdapterStatus, wgpu::Adapter const&, wgpu::StringView> F>
RequestAdapterCallback::RequestAdapterCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void RequestAdapterCallback::ControlImpl<F>::invoke(wgpu::RequestAdapterStatus status, wgpu::Adapter const& adapter, wgpu::StringView message) const {
    func(status, adapter, message);
}
template <std::invocable<wgpu::RequestDeviceStatus, wgpu::Device const&, wgpu::StringView> F>
RequestDeviceCallback::RequestDeviceCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void RequestDeviceCallback::ControlImpl<F>::invoke(wgpu::RequestDeviceStatus status, wgpu::Device const& device, wgpu::StringView message) const {
    func(status, device, message);
}
template <std::invocable<wgpu::Device const&, wgpu::ErrorType, wgpu::StringView> F>
UncapturedErrorCallback::UncapturedErrorCallback(const F& f) {
    data = new ControlImpl<F>(f);
}
template <typename F>
void UncapturedErrorCallback::ControlImpl<F>::invoke(wgpu::Device const& device, wgpu::ErrorType type, wgpu::StringView message) const {
    func(device, type, message);
}
}
namespace wgpu {
std::string_view to_string(AdapterType value) {
    switch (value) {
        case AdapterType::eDiscreteGPU: return "DiscreteGPU";
        case AdapterType::eIntegratedGPU: return "IntegratedGPU";
        case AdapterType::eCPU: return "CPU";
        case AdapterType::eUnknown: return "Unknown";
        case AdapterType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(AddressMode value) {
    switch (value) {
        case AddressMode::eUndefined: return "Undefined";
        case AddressMode::eClampToEdge: return "ClampToEdge";
        case AddressMode::eRepeat: return "Repeat";
        case AddressMode::eMirrorRepeat: return "MirrorRepeat";
        case AddressMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(BackendType value) {
    switch (value) {
        case BackendType::eUndefined: return "Undefined";
        case BackendType::eNull: return "Null";
        case BackendType::eWebGPU: return "WebGPU";
        case BackendType::eD3D11: return "D3D11";
        case BackendType::eD3D12: return "D3D12";
        case BackendType::eMetal: return "Metal";
        case BackendType::eVulkan: return "Vulkan";
        case BackendType::eOpenGL: return "OpenGL";
        case BackendType::eOpenGLES: return "OpenGLES";
        case BackendType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(BlendFactor value) {
    switch (value) {
        case BlendFactor::eUndefined: return "Undefined";
        case BlendFactor::eZero: return "Zero";
        case BlendFactor::eOne: return "One";
        case BlendFactor::eSrc: return "Src";
        case BlendFactor::eOneMinusSrc: return "OneMinusSrc";
        case BlendFactor::eSrcAlpha: return "SrcAlpha";
        case BlendFactor::eOneMinusSrcAlpha: return "OneMinusSrcAlpha";
        case BlendFactor::eDst: return "Dst";
        case BlendFactor::eOneMinusDst: return "OneMinusDst";
        case BlendFactor::eDstAlpha: return "DstAlpha";
        case BlendFactor::eOneMinusDstAlpha: return "OneMinusDstAlpha";
        case BlendFactor::eSrcAlphaSaturated: return "SrcAlphaSaturated";
        case BlendFactor::eConstant: return "Constant";
        case BlendFactor::eOneMinusConstant: return "OneMinusConstant";
        case BlendFactor::eSrc1: return "Src1";
        case BlendFactor::eOneMinusSrc1: return "OneMinusSrc1";
        case BlendFactor::eSrc1Alpha: return "Src1Alpha";
        case BlendFactor::eOneMinusSrc1Alpha: return "OneMinusSrc1Alpha";
        case BlendFactor::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(BlendOperation value) {
    switch (value) {
        case BlendOperation::eUndefined: return "Undefined";
        case BlendOperation::eAdd: return "Add";
        case BlendOperation::eSubtract: return "Subtract";
        case BlendOperation::eReverseSubtract: return "ReverseSubtract";
        case BlendOperation::eMin: return "Min";
        case BlendOperation::eMax: return "Max";
        case BlendOperation::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(BufferBindingType value) {
    switch (value) {
        case BufferBindingType::eBindingNotUsed: return "BindingNotUsed";
        case BufferBindingType::eUndefined: return "Undefined";
        case BufferBindingType::eUniform: return "Uniform";
        case BufferBindingType::eStorage: return "Storage";
        case BufferBindingType::eReadOnlyStorage: return "ReadOnlyStorage";
        case BufferBindingType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(BufferMapState value) {
    switch (value) {
        case BufferMapState::eUnmapped: return "Unmapped";
        case BufferMapState::ePending: return "Pending";
        case BufferMapState::eMapped: return "Mapped";
        case BufferMapState::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(CallbackMode value) {
    switch (value) {
        case CallbackMode::eWaitAnyOnly: return "WaitAnyOnly";
        case CallbackMode::eAllowProcessEvents: return "AllowProcessEvents";
        case CallbackMode::eAllowSpontaneous: return "AllowSpontaneous";
        case CallbackMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(CompareFunction value) {
    switch (value) {
        case CompareFunction::eUndefined: return "Undefined";
        case CompareFunction::eNever: return "Never";
        case CompareFunction::eLess: return "Less";
        case CompareFunction::eEqual: return "Equal";
        case CompareFunction::eLessEqual: return "LessEqual";
        case CompareFunction::eGreater: return "Greater";
        case CompareFunction::eNotEqual: return "NotEqual";
        case CompareFunction::eGreaterEqual: return "GreaterEqual";
        case CompareFunction::eAlways: return "Always";
        case CompareFunction::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(CompilationInfoRequestStatus value) {
    switch (value) {
        case CompilationInfoRequestStatus::eSuccess: return "Success";
        case CompilationInfoRequestStatus::eCallbackCancelled: return "CallbackCancelled";
        case CompilationInfoRequestStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(CompilationMessageType value) {
    switch (value) {
        case CompilationMessageType::eError: return "Error";
        case CompilationMessageType::eWarning: return "Warning";
        case CompilationMessageType::eInfo: return "Info";
        case CompilationMessageType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(ComponentSwizzle value) {
    switch (value) {
        case ComponentSwizzle::eUndefined: return "Undefined";
        case ComponentSwizzle::eZero: return "Zero";
        case ComponentSwizzle::eOne: return "One";
        case ComponentSwizzle::eR: return "R";
        case ComponentSwizzle::eG: return "G";
        case ComponentSwizzle::eB: return "B";
        case ComponentSwizzle::eA: return "A";
        case ComponentSwizzle::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(CompositeAlphaMode value) {
    switch (value) {
        case CompositeAlphaMode::eAuto: return "Auto";
        case CompositeAlphaMode::eOpaque: return "Opaque";
        case CompositeAlphaMode::ePremultiplied: return "Premultiplied";
        case CompositeAlphaMode::eUnpremultiplied: return "Unpremultiplied";
        case CompositeAlphaMode::eInherit: return "Inherit";
        case CompositeAlphaMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(CreatePipelineAsyncStatus value) {
    switch (value) {
        case CreatePipelineAsyncStatus::eSuccess: return "Success";
        case CreatePipelineAsyncStatus::eCallbackCancelled: return "CallbackCancelled";
        case CreatePipelineAsyncStatus::eValidationError: return "ValidationError";
        case CreatePipelineAsyncStatus::eInternalError: return "InternalError";
        case CreatePipelineAsyncStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(CullMode value) {
    switch (value) {
        case CullMode::eUndefined: return "Undefined";
        case CullMode::eNone: return "None";
        case CullMode::eFront: return "Front";
        case CullMode::eBack: return "Back";
        case CullMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(DeviceLostReason value) {
    switch (value) {
        case DeviceLostReason::eUnknown: return "Unknown";
        case DeviceLostReason::eDestroyed: return "Destroyed";
        case DeviceLostReason::eCallbackCancelled: return "CallbackCancelled";
        case DeviceLostReason::eFailedCreation: return "FailedCreation";
        case DeviceLostReason::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(ErrorFilter value) {
    switch (value) {
        case ErrorFilter::eValidation: return "Validation";
        case ErrorFilter::eOutOfMemory: return "OutOfMemory";
        case ErrorFilter::eInternal: return "Internal";
        case ErrorFilter::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(ErrorType value) {
    switch (value) {
        case ErrorType::eNoError: return "NoError";
        case ErrorType::eValidation: return "Validation";
        case ErrorType::eOutOfMemory: return "OutOfMemory";
        case ErrorType::eInternal: return "Internal";
        case ErrorType::eUnknown: return "Unknown";
        case ErrorType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(FeatureLevel value) {
    switch (value) {
        case FeatureLevel::eUndefined: return "Undefined";
        case FeatureLevel::eCompatibility: return "Compatibility";
        case FeatureLevel::eCore: return "Core";
        case FeatureLevel::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(FeatureName value) {
    switch (value) {
        case FeatureName::eCoreFeaturesAndLimits: return "CoreFeaturesAndLimits";
        case FeatureName::eDepthClipControl: return "DepthClipControl";
        case FeatureName::eDepth32FloatStencil8: return "Depth32FloatStencil8";
        case FeatureName::eTextureCompressionBC: return "TextureCompressionBC";
        case FeatureName::eTextureCompressionBCSliced3D: return "TextureCompressionBCSliced3D";
        case FeatureName::eTextureCompressionETC2: return "TextureCompressionETC2";
        case FeatureName::eTextureCompressionASTC: return "TextureCompressionASTC";
        case FeatureName::eTextureCompressionASTCSliced3D: return "TextureCompressionASTCSliced3D";
        case FeatureName::eTimestampQuery: return "TimestampQuery";
        case FeatureName::eIndirectFirstInstance: return "IndirectFirstInstance";
        case FeatureName::eShaderF16: return "ShaderF16";
        case FeatureName::eRG11B10UfloatRenderable: return "RG11B10UfloatRenderable";
        case FeatureName::eBGRA8UnormStorage: return "BGRA8UnormStorage";
        case FeatureName::eFloat32Filterable: return "Float32Filterable";
        case FeatureName::eFloat32Blendable: return "Float32Blendable";
        case FeatureName::eClipDistances: return "ClipDistances";
        case FeatureName::eDualSourceBlending: return "DualSourceBlending";
        case FeatureName::eSubgroups: return "Subgroups";
        case FeatureName::eTextureFormatsTier1: return "TextureFormatsTier1";
        case FeatureName::eTextureFormatsTier2: return "TextureFormatsTier2";
        case FeatureName::ePrimitiveIndex: return "PrimitiveIndex";
        case FeatureName::eTextureComponentSwizzle: return "TextureComponentSwizzle";
        case FeatureName::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(FilterMode value) {
    switch (value) {
        case FilterMode::eUndefined: return "Undefined";
        case FilterMode::eNearest: return "Nearest";
        case FilterMode::eLinear: return "Linear";
        case FilterMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(FrontFace value) {
    switch (value) {
        case FrontFace::eUndefined: return "Undefined";
        case FrontFace::eCCW: return "CCW";
        case FrontFace::eCW: return "CW";
        case FrontFace::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(IndexFormat value) {
    switch (value) {
        case IndexFormat::eUndefined: return "Undefined";
        case IndexFormat::eUint16: return "Uint16";
        case IndexFormat::eUint32: return "Uint32";
        case IndexFormat::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(InstanceFeatureName value) {
    switch (value) {
        case InstanceFeatureName::eTimedWaitAny: return "TimedWaitAny";
        case InstanceFeatureName::eShaderSourceSPIRV: return "ShaderSourceSPIRV";
        case InstanceFeatureName::eMultipleDevicesPerAdapter: return "MultipleDevicesPerAdapter";
        case InstanceFeatureName::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(LoadOp value) {
    switch (value) {
        case LoadOp::eUndefined: return "Undefined";
        case LoadOp::eLoad: return "Load";
        case LoadOp::eClear: return "Clear";
        case LoadOp::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(MapAsyncStatus value) {
    switch (value) {
        case MapAsyncStatus::eSuccess: return "Success";
        case MapAsyncStatus::eCallbackCancelled: return "CallbackCancelled";
        case MapAsyncStatus::eError: return "Error";
        case MapAsyncStatus::eAborted: return "Aborted";
        case MapAsyncStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(MipmapFilterMode value) {
    switch (value) {
        case MipmapFilterMode::eUndefined: return "Undefined";
        case MipmapFilterMode::eNearest: return "Nearest";
        case MipmapFilterMode::eLinear: return "Linear";
        case MipmapFilterMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(OptionalBool value) {
    switch (value) {
        case OptionalBool::eFalse: return "False";
        case OptionalBool::eTrue: return "True";
        case OptionalBool::eUndefined: return "Undefined";
        case OptionalBool::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(PopErrorScopeStatus value) {
    switch (value) {
        case PopErrorScopeStatus::eSuccess: return "Success";
        case PopErrorScopeStatus::eCallbackCancelled: return "CallbackCancelled";
        case PopErrorScopeStatus::eError: return "Error";
        case PopErrorScopeStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(PowerPreference value) {
    switch (value) {
        case PowerPreference::eUndefined: return "Undefined";
        case PowerPreference::eLowPower: return "LowPower";
        case PowerPreference::eHighPerformance: return "HighPerformance";
        case PowerPreference::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(PredefinedColorSpace value) {
    switch (value) {
        case PredefinedColorSpace::eSRGB: return "SRGB";
        case PredefinedColorSpace::eDisplayP3: return "DisplayP3";
        case PredefinedColorSpace::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(PresentMode value) {
    switch (value) {
        case PresentMode::eUndefined: return "Undefined";
        case PresentMode::eFifo: return "Fifo";
        case PresentMode::eFifoRelaxed: return "FifoRelaxed";
        case PresentMode::eImmediate: return "Immediate";
        case PresentMode::eMailbox: return "Mailbox";
        case PresentMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(PrimitiveTopology value) {
    switch (value) {
        case PrimitiveTopology::eUndefined: return "Undefined";
        case PrimitiveTopology::ePointList: return "PointList";
        case PrimitiveTopology::eLineList: return "LineList";
        case PrimitiveTopology::eLineStrip: return "LineStrip";
        case PrimitiveTopology::eTriangleList: return "TriangleList";
        case PrimitiveTopology::eTriangleStrip: return "TriangleStrip";
        case PrimitiveTopology::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(QueryType value) {
    switch (value) {
        case QueryType::eOcclusion: return "Occlusion";
        case QueryType::eTimestamp: return "Timestamp";
        case QueryType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(QueueWorkDoneStatus value) {
    switch (value) {
        case QueueWorkDoneStatus::eSuccess: return "Success";
        case QueueWorkDoneStatus::eCallbackCancelled: return "CallbackCancelled";
        case QueueWorkDoneStatus::eError: return "Error";
        case QueueWorkDoneStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(RequestAdapterStatus value) {
    switch (value) {
        case RequestAdapterStatus::eSuccess: return "Success";
        case RequestAdapterStatus::eCallbackCancelled: return "CallbackCancelled";
        case RequestAdapterStatus::eUnavailable: return "Unavailable";
        case RequestAdapterStatus::eError: return "Error";
        case RequestAdapterStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(RequestDeviceStatus value) {
    switch (value) {
        case RequestDeviceStatus::eSuccess: return "Success";
        case RequestDeviceStatus::eCallbackCancelled: return "CallbackCancelled";
        case RequestDeviceStatus::eError: return "Error";
        case RequestDeviceStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(SamplerBindingType value) {
    switch (value) {
        case SamplerBindingType::eBindingNotUsed: return "BindingNotUsed";
        case SamplerBindingType::eUndefined: return "Undefined";
        case SamplerBindingType::eFiltering: return "Filtering";
        case SamplerBindingType::eNonFiltering: return "NonFiltering";
        case SamplerBindingType::eComparison: return "Comparison";
        case SamplerBindingType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(Status value) {
    switch (value) {
        case Status::eSuccess: return "Success";
        case Status::eError: return "Error";
        case Status::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(StencilOperation value) {
    switch (value) {
        case StencilOperation::eUndefined: return "Undefined";
        case StencilOperation::eKeep: return "Keep";
        case StencilOperation::eZero: return "Zero";
        case StencilOperation::eReplace: return "Replace";
        case StencilOperation::eInvert: return "Invert";
        case StencilOperation::eIncrementClamp: return "IncrementClamp";
        case StencilOperation::eDecrementClamp: return "DecrementClamp";
        case StencilOperation::eIncrementWrap: return "IncrementWrap";
        case StencilOperation::eDecrementWrap: return "DecrementWrap";
        case StencilOperation::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(StorageTextureAccess value) {
    switch (value) {
        case StorageTextureAccess::eBindingNotUsed: return "BindingNotUsed";
        case StorageTextureAccess::eUndefined: return "Undefined";
        case StorageTextureAccess::eWriteOnly: return "WriteOnly";
        case StorageTextureAccess::eReadOnly: return "ReadOnly";
        case StorageTextureAccess::eReadWrite: return "ReadWrite";
        case StorageTextureAccess::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(StoreOp value) {
    switch (value) {
        case StoreOp::eUndefined: return "Undefined";
        case StoreOp::eStore: return "Store";
        case StoreOp::eDiscard: return "Discard";
        case StoreOp::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(SType value) {
    switch (value) {
        case SType::eShaderSourceSPIRV: return "ShaderSourceSPIRV";
        case SType::eShaderSourceWGSL: return "ShaderSourceWGSL";
        case SType::eRenderPassMaxDrawCount: return "RenderPassMaxDrawCount";
        case SType::eSurfaceSourceMetalLayer: return "SurfaceSourceMetalLayer";
        case SType::eSurfaceSourceWindowsHWND: return "SurfaceSourceWindowsHWND";
        case SType::eSurfaceSourceXlibWindow: return "SurfaceSourceXlibWindow";
        case SType::eSurfaceSourceWaylandSurface: return "SurfaceSourceWaylandSurface";
        case SType::eSurfaceSourceAndroidNativeWindow: return "SurfaceSourceAndroidNativeWindow";
        case SType::eSurfaceSourceXCBWindow: return "SurfaceSourceXCBWindow";
        case SType::eSurfaceColorManagement: return "SurfaceColorManagement";
        case SType::eRequestAdapterWebXROptions: return "RequestAdapterWebXROptions";
        case SType::eTextureComponentSwizzleDescriptor: return "TextureComponentSwizzleDescriptor";
        case SType::eExternalTextureBindingLayout: return "ExternalTextureBindingLayout";
        case SType::eExternalTextureBindingEntry: return "ExternalTextureBindingEntry";
        case SType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(SurfaceGetCurrentTextureStatus value) {
    switch (value) {
        case SurfaceGetCurrentTextureStatus::eSuccessOptimal: return "SuccessOptimal";
        case SurfaceGetCurrentTextureStatus::eSuccessSuboptimal: return "SuccessSuboptimal";
        case SurfaceGetCurrentTextureStatus::eTimeout: return "Timeout";
        case SurfaceGetCurrentTextureStatus::eOutdated: return "Outdated";
        case SurfaceGetCurrentTextureStatus::eLost: return "Lost";
        case SurfaceGetCurrentTextureStatus::eError: return "Error";
        case SurfaceGetCurrentTextureStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(TextureAspect value) {
    switch (value) {
        case TextureAspect::eUndefined: return "Undefined";
        case TextureAspect::eAll: return "All";
        case TextureAspect::eStencilOnly: return "StencilOnly";
        case TextureAspect::eDepthOnly: return "DepthOnly";
        case TextureAspect::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(TextureDimension value) {
    switch (value) {
        case TextureDimension::eUndefined: return "Undefined";
        case TextureDimension::e1D: return "1D";
        case TextureDimension::e2D: return "2D";
        case TextureDimension::e3D: return "3D";
        case TextureDimension::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(TextureFormat value) {
    switch (value) {
        case TextureFormat::eUndefined: return "Undefined";
        case TextureFormat::eR8Unorm: return "R8Unorm";
        case TextureFormat::eR8Snorm: return "R8Snorm";
        case TextureFormat::eR8Uint: return "R8Uint";
        case TextureFormat::eR8Sint: return "R8Sint";
        case TextureFormat::eR16Unorm: return "R16Unorm";
        case TextureFormat::eR16Snorm: return "R16Snorm";
        case TextureFormat::eR16Uint: return "R16Uint";
        case TextureFormat::eR16Sint: return "R16Sint";
        case TextureFormat::eR16Float: return "R16Float";
        case TextureFormat::eRG8Unorm: return "RG8Unorm";
        case TextureFormat::eRG8Snorm: return "RG8Snorm";
        case TextureFormat::eRG8Uint: return "RG8Uint";
        case TextureFormat::eRG8Sint: return "RG8Sint";
        case TextureFormat::eR32Float: return "R32Float";
        case TextureFormat::eR32Uint: return "R32Uint";
        case TextureFormat::eR32Sint: return "R32Sint";
        case TextureFormat::eRG16Unorm: return "RG16Unorm";
        case TextureFormat::eRG16Snorm: return "RG16Snorm";
        case TextureFormat::eRG16Uint: return "RG16Uint";
        case TextureFormat::eRG16Sint: return "RG16Sint";
        case TextureFormat::eRG16Float: return "RG16Float";
        case TextureFormat::eRGBA8Unorm: return "RGBA8Unorm";
        case TextureFormat::eRGBA8UnormSrgb: return "RGBA8UnormSrgb";
        case TextureFormat::eRGBA8Snorm: return "RGBA8Snorm";
        case TextureFormat::eRGBA8Uint: return "RGBA8Uint";
        case TextureFormat::eRGBA8Sint: return "RGBA8Sint";
        case TextureFormat::eBGRA8Unorm: return "BGRA8Unorm";
        case TextureFormat::eBGRA8UnormSrgb: return "BGRA8UnormSrgb";
        case TextureFormat::eRGB10A2Uint: return "RGB10A2Uint";
        case TextureFormat::eRGB10A2Unorm: return "RGB10A2Unorm";
        case TextureFormat::eRG11B10Ufloat: return "RG11B10Ufloat";
        case TextureFormat::eRGB9E5Ufloat: return "RGB9E5Ufloat";
        case TextureFormat::eRG32Float: return "RG32Float";
        case TextureFormat::eRG32Uint: return "RG32Uint";
        case TextureFormat::eRG32Sint: return "RG32Sint";
        case TextureFormat::eRGBA16Unorm: return "RGBA16Unorm";
        case TextureFormat::eRGBA16Snorm: return "RGBA16Snorm";
        case TextureFormat::eRGBA16Uint: return "RGBA16Uint";
        case TextureFormat::eRGBA16Sint: return "RGBA16Sint";
        case TextureFormat::eRGBA16Float: return "RGBA16Float";
        case TextureFormat::eRGBA32Float: return "RGBA32Float";
        case TextureFormat::eRGBA32Uint: return "RGBA32Uint";
        case TextureFormat::eRGBA32Sint: return "RGBA32Sint";
        case TextureFormat::eStencil8: return "Stencil8";
        case TextureFormat::eDepth16Unorm: return "Depth16Unorm";
        case TextureFormat::eDepth24Plus: return "Depth24Plus";
        case TextureFormat::eDepth24PlusStencil8: return "Depth24PlusStencil8";
        case TextureFormat::eDepth32Float: return "Depth32Float";
        case TextureFormat::eDepth32FloatStencil8: return "Depth32FloatStencil8";
        case TextureFormat::eBC1RGBAUnorm: return "BC1RGBAUnorm";
        case TextureFormat::eBC1RGBAUnormSrgb: return "BC1RGBAUnormSrgb";
        case TextureFormat::eBC2RGBAUnorm: return "BC2RGBAUnorm";
        case TextureFormat::eBC2RGBAUnormSrgb: return "BC2RGBAUnormSrgb";
        case TextureFormat::eBC3RGBAUnorm: return "BC3RGBAUnorm";
        case TextureFormat::eBC3RGBAUnormSrgb: return "BC3RGBAUnormSrgb";
        case TextureFormat::eBC4RUnorm: return "BC4RUnorm";
        case TextureFormat::eBC4RSnorm: return "BC4RSnorm";
        case TextureFormat::eBC5RGUnorm: return "BC5RGUnorm";
        case TextureFormat::eBC5RGSnorm: return "BC5RGSnorm";
        case TextureFormat::eBC6HRGBUfloat: return "BC6HRGBUfloat";
        case TextureFormat::eBC6HRGBFloat: return "BC6HRGBFloat";
        case TextureFormat::eBC7RGBAUnorm: return "BC7RGBAUnorm";
        case TextureFormat::eBC7RGBAUnormSrgb: return "BC7RGBAUnormSrgb";
        case TextureFormat::eETC2RGB8Unorm: return "ETC2RGB8Unorm";
        case TextureFormat::eETC2RGB8UnormSrgb: return "ETC2RGB8UnormSrgb";
        case TextureFormat::eETC2RGB8A1Unorm: return "ETC2RGB8A1Unorm";
        case TextureFormat::eETC2RGB8A1UnormSrgb: return "ETC2RGB8A1UnormSrgb";
        case TextureFormat::eETC2RGBA8Unorm: return "ETC2RGBA8Unorm";
        case TextureFormat::eETC2RGBA8UnormSrgb: return "ETC2RGBA8UnormSrgb";
        case TextureFormat::eEACR11Unorm: return "EACR11Unorm";
        case TextureFormat::eEACR11Snorm: return "EACR11Snorm";
        case TextureFormat::eEACRG11Unorm: return "EACRG11Unorm";
        case TextureFormat::eEACRG11Snorm: return "EACRG11Snorm";
        case TextureFormat::eASTC4x4Unorm: return "ASTC4x4Unorm";
        case TextureFormat::eASTC4x4UnormSrgb: return "ASTC4x4UnormSrgb";
        case TextureFormat::eASTC5x4Unorm: return "ASTC5x4Unorm";
        case TextureFormat::eASTC5x4UnormSrgb: return "ASTC5x4UnormSrgb";
        case TextureFormat::eASTC5x5Unorm: return "ASTC5x5Unorm";
        case TextureFormat::eASTC5x5UnormSrgb: return "ASTC5x5UnormSrgb";
        case TextureFormat::eASTC6x5Unorm: return "ASTC6x5Unorm";
        case TextureFormat::eASTC6x5UnormSrgb: return "ASTC6x5UnormSrgb";
        case TextureFormat::eASTC6x6Unorm: return "ASTC6x6Unorm";
        case TextureFormat::eASTC6x6UnormSrgb: return "ASTC6x6UnormSrgb";
        case TextureFormat::eASTC8x5Unorm: return "ASTC8x5Unorm";
        case TextureFormat::eASTC8x5UnormSrgb: return "ASTC8x5UnormSrgb";
        case TextureFormat::eASTC8x6Unorm: return "ASTC8x6Unorm";
        case TextureFormat::eASTC8x6UnormSrgb: return "ASTC8x6UnormSrgb";
        case TextureFormat::eASTC8x8Unorm: return "ASTC8x8Unorm";
        case TextureFormat::eASTC8x8UnormSrgb: return "ASTC8x8UnormSrgb";
        case TextureFormat::eASTC10x5Unorm: return "ASTC10x5Unorm";
        case TextureFormat::eASTC10x5UnormSrgb: return "ASTC10x5UnormSrgb";
        case TextureFormat::eASTC10x6Unorm: return "ASTC10x6Unorm";
        case TextureFormat::eASTC10x6UnormSrgb: return "ASTC10x6UnormSrgb";
        case TextureFormat::eASTC10x8Unorm: return "ASTC10x8Unorm";
        case TextureFormat::eASTC10x8UnormSrgb: return "ASTC10x8UnormSrgb";
        case TextureFormat::eASTC10x10Unorm: return "ASTC10x10Unorm";
        case TextureFormat::eASTC10x10UnormSrgb: return "ASTC10x10UnormSrgb";
        case TextureFormat::eASTC12x10Unorm: return "ASTC12x10Unorm";
        case TextureFormat::eASTC12x10UnormSrgb: return "ASTC12x10UnormSrgb";
        case TextureFormat::eASTC12x12Unorm: return "ASTC12x12Unorm";
        case TextureFormat::eASTC12x12UnormSrgb: return "ASTC12x12UnormSrgb";
        case TextureFormat::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(TextureSampleType value) {
    switch (value) {
        case TextureSampleType::eBindingNotUsed: return "BindingNotUsed";
        case TextureSampleType::eUndefined: return "Undefined";
        case TextureSampleType::eFloat: return "Float";
        case TextureSampleType::eUnfilterableFloat: return "UnfilterableFloat";
        case TextureSampleType::eDepth: return "Depth";
        case TextureSampleType::eSint: return "Sint";
        case TextureSampleType::eUint: return "Uint";
        case TextureSampleType::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(TextureViewDimension value) {
    switch (value) {
        case TextureViewDimension::eUndefined: return "Undefined";
        case TextureViewDimension::e1D: return "1D";
        case TextureViewDimension::e2D: return "2D";
        case TextureViewDimension::e2DArray: return "2DArray";
        case TextureViewDimension::eCube: return "Cube";
        case TextureViewDimension::eCubeArray: return "CubeArray";
        case TextureViewDimension::e3D: return "3D";
        case TextureViewDimension::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(ToneMappingMode value) {
    switch (value) {
        case ToneMappingMode::eStandard: return "Standard";
        case ToneMappingMode::eExtended: return "Extended";
        case ToneMappingMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(VertexFormat value) {
    switch (value) {
        case VertexFormat::eUint8: return "Uint8";
        case VertexFormat::eUint8x2: return "Uint8x2";
        case VertexFormat::eUint8x4: return "Uint8x4";
        case VertexFormat::eSint8: return "Sint8";
        case VertexFormat::eSint8x2: return "Sint8x2";
        case VertexFormat::eSint8x4: return "Sint8x4";
        case VertexFormat::eUnorm8: return "Unorm8";
        case VertexFormat::eUnorm8x2: return "Unorm8x2";
        case VertexFormat::eUnorm8x4: return "Unorm8x4";
        case VertexFormat::eSnorm8: return "Snorm8";
        case VertexFormat::eSnorm8x2: return "Snorm8x2";
        case VertexFormat::eSnorm8x4: return "Snorm8x4";
        case VertexFormat::eUint16: return "Uint16";
        case VertexFormat::eUint16x2: return "Uint16x2";
        case VertexFormat::eUint16x4: return "Uint16x4";
        case VertexFormat::eSint16: return "Sint16";
        case VertexFormat::eSint16x2: return "Sint16x2";
        case VertexFormat::eSint16x4: return "Sint16x4";
        case VertexFormat::eUnorm16: return "Unorm16";
        case VertexFormat::eUnorm16x2: return "Unorm16x2";
        case VertexFormat::eUnorm16x4: return "Unorm16x4";
        case VertexFormat::eSnorm16: return "Snorm16";
        case VertexFormat::eSnorm16x2: return "Snorm16x2";
        case VertexFormat::eSnorm16x4: return "Snorm16x4";
        case VertexFormat::eFloat16: return "Float16";
        case VertexFormat::eFloat16x2: return "Float16x2";
        case VertexFormat::eFloat16x4: return "Float16x4";
        case VertexFormat::eFloat32: return "Float32";
        case VertexFormat::eFloat32x2: return "Float32x2";
        case VertexFormat::eFloat32x3: return "Float32x3";
        case VertexFormat::eFloat32x4: return "Float32x4";
        case VertexFormat::eUint32: return "Uint32";
        case VertexFormat::eUint32x2: return "Uint32x2";
        case VertexFormat::eUint32x3: return "Uint32x3";
        case VertexFormat::eUint32x4: return "Uint32x4";
        case VertexFormat::eSint32: return "Sint32";
        case VertexFormat::eSint32x2: return "Sint32x2";
        case VertexFormat::eSint32x3: return "Sint32x3";
        case VertexFormat::eSint32x4: return "Sint32x4";
        case VertexFormat::eUnorm10_10_10_2: return "Unorm10_10_10_2";
        case VertexFormat::eUnorm8x4BGRA: return "Unorm8x4BGRA";
        case VertexFormat::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(VertexStepMode value) {
    switch (value) {
        case VertexStepMode::eUndefined: return "Undefined";
        case VertexStepMode::eVertex: return "Vertex";
        case VertexStepMode::eInstance: return "Instance";
        case VertexStepMode::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(WaitStatus value) {
    switch (value) {
        case WaitStatus::eSuccess: return "Success";
        case WaitStatus::eTimedOut: return "TimedOut";
        case WaitStatus::eError: return "Error";
        case WaitStatus::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(WGSLLanguageFeatureName value) {
    switch (value) {
        case WGSLLanguageFeatureName::eReadonlyAndReadwriteStorageTextures: return "ReadonlyAndReadwriteStorageTextures";
        case WGSLLanguageFeatureName::ePacked4x8IntegerDotProduct: return "Packed4x8IntegerDotProduct";
        case WGSLLanguageFeatureName::eUnrestrictedPointerParameters: return "UnrestrictedPointerParameters";
        case WGSLLanguageFeatureName::ePointerCompositeAccess: return "PointerCompositeAccess";
        case WGSLLanguageFeatureName::eUniformBufferStandardLayout: return "UniformBufferStandardLayout";
        case WGSLLanguageFeatureName::eSubgroupId: return "SubgroupId";
        case WGSLLanguageFeatureName::eTextureAndSamplerLet: return "TextureAndSamplerLet";
        case WGSLLanguageFeatureName::eSubgroupUniformity: return "SubgroupUniformity";
        case WGSLLanguageFeatureName::eForce32: return "Force32";
        default: return "Unknown";
    }
}
std::string_view to_string(BufferUsage value) {
    switch (value) {
        case BufferUsage::eNone: return "None";
        case BufferUsage::eMapRead: return "MapRead";
        case BufferUsage::eMapWrite: return "MapWrite";
        case BufferUsage::eCopySrc: return "CopySrc";
        case BufferUsage::eCopyDst: return "CopyDst";
        case BufferUsage::eIndex: return "Index";
        case BufferUsage::eVertex: return "Vertex";
        case BufferUsage::eUniform: return "Uniform";
        case BufferUsage::eStorage: return "Storage";
        case BufferUsage::eIndirect: return "Indirect";
        case BufferUsage::eQueryResolve: return "QueryResolve";
        default: return "Unknown";
    }
}
std::string_view to_string(ColorWriteMask value) {
    switch (value) {
        case ColorWriteMask::eNone: return "None";
        case ColorWriteMask::eRed: return "Red";
        case ColorWriteMask::eGreen: return "Green";
        case ColorWriteMask::eBlue: return "Blue";
        case ColorWriteMask::eAlpha: return "Alpha";
        case ColorWriteMask::eAll: return "All";
        default: return "Unknown";
    }
}
std::string_view to_string(MapMode value) {
    switch (value) {
        case MapMode::eNone: return "None";
        case MapMode::eRead: return "Read";
        case MapMode::eWrite: return "Write";
        default: return "Unknown";
    }
}
std::string_view to_string(ShaderStage value) {
    switch (value) {
        case ShaderStage::eNone: return "None";
        case ShaderStage::eVertex: return "Vertex";
        case ShaderStage::eFragment: return "Fragment";
        case ShaderStage::eCompute: return "Compute";
        default: return "Unknown";
    }
}
std::string_view to_string(TextureUsage value) {
    switch (value) {
        case TextureUsage::eNone: return "None";
        case TextureUsage::eCopySrc: return "CopySrc";
        case TextureUsage::eCopyDst: return "CopyDst";
        case TextureUsage::eTextureBinding: return "TextureBinding";
        case TextureUsage::eStorageBinding: return "StorageBinding";
        case TextureUsage::eRenderAttachment: return "RenderAttachment";
        case TextureUsage::eTransientAttachment: return "TransientAttachment";
        default: return "Unknown";
    }
}
BufferUsage operator|(BufferUsage lhs, BufferUsage rhs) {
    using T = std::underlying_type_t<BufferUsage>;
    return static_cast<BufferUsage>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
ColorWriteMask operator|(ColorWriteMask lhs, ColorWriteMask rhs) {
    using T = std::underlying_type_t<ColorWriteMask>;
    return static_cast<ColorWriteMask>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
MapMode operator|(MapMode lhs, MapMode rhs) {
    using T = std::underlying_type_t<MapMode>;
    return static_cast<MapMode>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
ShaderStage operator|(ShaderStage lhs, ShaderStage rhs) {
    using T = std::underlying_type_t<ShaderStage>;
    return static_cast<ShaderStage>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
TextureUsage operator|(TextureUsage lhs, TextureUsage rhs) {
    using T = std::underlying_type_t<TextureUsage>;
    return static_cast<TextureUsage>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
}
namespace wgpu {
StringView::StringView(const WGPUStringView& native) {
    this->data = static_cast<char const*>(native.data);
    this->length = static_cast<size_t>(native.length);
}
StringView::CStruct StringView::to_cstruct() const {
    CStruct cstruct;
    cstruct.data = static_cast<const char*>(this->data);
    cstruct.length = static_cast<size_t>(this->length);
    return cstruct;
}
StringView& StringView::setData(char const* value) & {
    this->data = value;
    return *this;
}
StringView&& StringView::setData(char const* value) && {
    this->data = value;
    return std::move(*this);
}
StringView& StringView::setLength(size_t value) & {
    this->length = value;
    return *this;
}
StringView&& StringView::setLength(size_t value) && {
    this->length = value;
    return std::move(*this);
}
ChainedStruct::ChainedStruct(const WGPUChainedStruct& native) {
    this->next.setNext(native.next);
    this->sType = static_cast<wgpu::SType>(native.sType);
}
ChainedStruct::CStruct ChainedStruct::to_cstruct() const {
    CStruct cstruct;
    cstruct.next = this->next.getNext();
    cstruct.sType = static_cast<WGPUSType>(this->sType);
    return cstruct;
}
ChainedStruct& ChainedStruct::setSType(wgpu::SType value) & {
    this->sType = value;
    return *this;
}
ChainedStruct&& ChainedStruct::setSType(wgpu::SType value) && {
    this->sType = value;
    return std::move(*this);
}
BufferMapCallbackInfo::BufferMapCallbackInfo(const WGPUBufferMapCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::BufferMapCallback(native.callback, native.userdata1, native.userdata2);
    }
}
BufferMapCallbackInfo::CStruct BufferMapCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPUMapAsyncStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::BufferMapCallback*>(userdata1));
            callback(status, message);
        };
        new (cstruct.userdata1) wgpu::BufferMapCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
BufferMapCallbackInfo& BufferMapCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
BufferMapCallbackInfo&& BufferMapCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
BufferMapCallbackInfo& BufferMapCallbackInfo::setCallback(wgpu::BufferMapCallback value) & {
    this->callback = std::move(value);
    return *this;
}
BufferMapCallbackInfo&& BufferMapCallbackInfo::setCallback(wgpu::BufferMapCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
CompilationInfoCallbackInfo::CompilationInfoCallbackInfo(const WGPUCompilationInfoCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::CompilationInfoCallback(native.callback, native.userdata1, native.userdata2);
    }
}
CompilationInfoCallbackInfo::CStruct CompilationInfoCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const* compilationInfo, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::CompilationInfoCallback*>(userdata1));
            callback(status, compilationInfo);
        };
        new (cstruct.userdata1) wgpu::CompilationInfoCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
CompilationInfoCallbackInfo& CompilationInfoCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
CompilationInfoCallbackInfo&& CompilationInfoCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
CompilationInfoCallbackInfo& CompilationInfoCallbackInfo::setCallback(wgpu::CompilationInfoCallback value) & {
    this->callback = std::move(value);
    return *this;
}
CompilationInfoCallbackInfo&& CompilationInfoCallbackInfo::setCallback(wgpu::CompilationInfoCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
CreateComputePipelineAsyncCallbackInfo::CreateComputePipelineAsyncCallbackInfo(const WGPUCreateComputePipelineAsyncCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::CreateComputePipelineAsyncCallback(native.callback, native.userdata1, native.userdata2);
    }
}
CreateComputePipelineAsyncCallbackInfo::CStruct CreateComputePipelineAsyncCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::CreateComputePipelineAsyncCallback*>(userdata1));
            callback(status, pipeline, message);
        };
        new (cstruct.userdata1) wgpu::CreateComputePipelineAsyncCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
CreateComputePipelineAsyncCallbackInfo& CreateComputePipelineAsyncCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
CreateComputePipelineAsyncCallbackInfo&& CreateComputePipelineAsyncCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
CreateComputePipelineAsyncCallbackInfo& CreateComputePipelineAsyncCallbackInfo::setCallback(wgpu::CreateComputePipelineAsyncCallback value) & {
    this->callback = std::move(value);
    return *this;
}
CreateComputePipelineAsyncCallbackInfo&& CreateComputePipelineAsyncCallbackInfo::setCallback(wgpu::CreateComputePipelineAsyncCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
CreateRenderPipelineAsyncCallbackInfo::CreateRenderPipelineAsyncCallbackInfo(const WGPUCreateRenderPipelineAsyncCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::CreateRenderPipelineAsyncCallback(native.callback, native.userdata1, native.userdata2);
    }
}
CreateRenderPipelineAsyncCallbackInfo::CStruct CreateRenderPipelineAsyncCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::CreateRenderPipelineAsyncCallback*>(userdata1));
            callback(status, pipeline, message);
        };
        new (cstruct.userdata1) wgpu::CreateRenderPipelineAsyncCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
CreateRenderPipelineAsyncCallbackInfo& CreateRenderPipelineAsyncCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
CreateRenderPipelineAsyncCallbackInfo&& CreateRenderPipelineAsyncCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
CreateRenderPipelineAsyncCallbackInfo& CreateRenderPipelineAsyncCallbackInfo::setCallback(wgpu::CreateRenderPipelineAsyncCallback value) & {
    this->callback = std::move(value);
    return *this;
}
CreateRenderPipelineAsyncCallbackInfo&& CreateRenderPipelineAsyncCallbackInfo::setCallback(wgpu::CreateRenderPipelineAsyncCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
DeviceLostCallbackInfo::DeviceLostCallbackInfo(const WGPUDeviceLostCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::DeviceLostCallback(native.callback, native.userdata1, native.userdata2);
    }
}
DeviceLostCallbackInfo::CStruct DeviceLostCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::DeviceLostCallback*>(userdata1));
            callback(device, reason, message);
        };
        new (cstruct.userdata1) wgpu::DeviceLostCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
DeviceLostCallbackInfo& DeviceLostCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
DeviceLostCallbackInfo&& DeviceLostCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
DeviceLostCallbackInfo& DeviceLostCallbackInfo::setCallback(wgpu::DeviceLostCallback value) & {
    this->callback = std::move(value);
    return *this;
}
DeviceLostCallbackInfo&& DeviceLostCallbackInfo::setCallback(wgpu::DeviceLostCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
PopErrorScopeCallbackInfo::PopErrorScopeCallbackInfo(const WGPUPopErrorScopeCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::PopErrorScopeCallback(native.callback, native.userdata1, native.userdata2);
    }
}
PopErrorScopeCallbackInfo::CStruct PopErrorScopeCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPUPopErrorScopeStatus status, WGPUErrorType type, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::PopErrorScopeCallback*>(userdata1));
            callback(status, type, message);
        };
        new (cstruct.userdata1) wgpu::PopErrorScopeCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
PopErrorScopeCallbackInfo& PopErrorScopeCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
PopErrorScopeCallbackInfo&& PopErrorScopeCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
PopErrorScopeCallbackInfo& PopErrorScopeCallbackInfo::setCallback(wgpu::PopErrorScopeCallback value) & {
    this->callback = std::move(value);
    return *this;
}
PopErrorScopeCallbackInfo&& PopErrorScopeCallbackInfo::setCallback(wgpu::PopErrorScopeCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
QueueWorkDoneCallbackInfo::QueueWorkDoneCallbackInfo(const WGPUQueueWorkDoneCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::QueueWorkDoneCallback(native.callback, native.userdata1, native.userdata2);
    }
}
QueueWorkDoneCallbackInfo::CStruct QueueWorkDoneCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPUQueueWorkDoneStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::QueueWorkDoneCallback*>(userdata1));
            callback(status, message);
        };
        new (cstruct.userdata1) wgpu::QueueWorkDoneCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
QueueWorkDoneCallbackInfo& QueueWorkDoneCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
QueueWorkDoneCallbackInfo&& QueueWorkDoneCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
QueueWorkDoneCallbackInfo& QueueWorkDoneCallbackInfo::setCallback(wgpu::QueueWorkDoneCallback value) & {
    this->callback = std::move(value);
    return *this;
}
QueueWorkDoneCallbackInfo&& QueueWorkDoneCallbackInfo::setCallback(wgpu::QueueWorkDoneCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
RequestAdapterCallbackInfo::RequestAdapterCallbackInfo(const WGPURequestAdapterCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::RequestAdapterCallback(native.callback, native.userdata1, native.userdata2);
    }
}
RequestAdapterCallbackInfo::CStruct RequestAdapterCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::RequestAdapterCallback*>(userdata1));
            callback(status, adapter, message);
        };
        new (cstruct.userdata1) wgpu::RequestAdapterCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
RequestAdapterCallbackInfo& RequestAdapterCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
RequestAdapterCallbackInfo&& RequestAdapterCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
RequestAdapterCallbackInfo& RequestAdapterCallbackInfo::setCallback(wgpu::RequestAdapterCallback value) & {
    this->callback = std::move(value);
    return *this;
}
RequestAdapterCallbackInfo&& RequestAdapterCallbackInfo::setCallback(wgpu::RequestAdapterCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
RequestDeviceCallbackInfo::RequestDeviceCallbackInfo(const WGPURequestDeviceCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->mode = static_cast<wgpu::CallbackMode>(native.mode);
    if (native.callback != nullptr) {
        this->callback = wgpu::RequestDeviceCallback(native.callback, native.userdata1, native.userdata2);
    }
}
RequestDeviceCallbackInfo::CStruct RequestDeviceCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.mode = static_cast<WGPUCallbackMode>(this->mode);
    if (this->callback) {
        cstruct.callback = [](WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::RequestDeviceCallback*>(userdata1));
            callback(status, device, message);
        };
        new (cstruct.userdata1) wgpu::RequestDeviceCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
RequestDeviceCallbackInfo& RequestDeviceCallbackInfo::setMode(wgpu::CallbackMode value) & {
    this->mode = value;
    return *this;
}
RequestDeviceCallbackInfo&& RequestDeviceCallbackInfo::setMode(wgpu::CallbackMode value) && {
    this->mode = value;
    return std::move(*this);
}
RequestDeviceCallbackInfo& RequestDeviceCallbackInfo::setCallback(wgpu::RequestDeviceCallback value) & {
    this->callback = std::move(value);
    return *this;
}
RequestDeviceCallbackInfo&& RequestDeviceCallbackInfo::setCallback(wgpu::RequestDeviceCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
UncapturedErrorCallbackInfo::UncapturedErrorCallbackInfo(const WGPUUncapturedErrorCallbackInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    if (native.callback != nullptr) {
        this->callback = wgpu::UncapturedErrorCallback(native.callback, native.userdata1, native.userdata2);
    }
}
UncapturedErrorCallbackInfo::CStruct UncapturedErrorCallbackInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    if (this->callback) {
        cstruct.callback = [](WGPUDevice const* device, WGPUErrorType type, WGPUStringView message, void* userdata1, void* userdata2) {
            auto callback = std::move(*reinterpret_cast<wgpu::UncapturedErrorCallback*>(userdata1));
            callback(device, type, message);
        };
        new (cstruct.userdata1) wgpu::UncapturedErrorCallback(this->callback);
    } else {
        cstruct.callback = nullptr;
    }
    return cstruct;
}
UncapturedErrorCallbackInfo& UncapturedErrorCallbackInfo::setCallback(wgpu::UncapturedErrorCallback value) & {
    this->callback = std::move(value);
    return *this;
}
UncapturedErrorCallbackInfo&& UncapturedErrorCallbackInfo::setCallback(wgpu::UncapturedErrorCallback value) && {
    this->callback = std::move(value);
    return std::move(*this);
}
AdapterInfo::AdapterInfo(const WGPUAdapterInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->vendor = static_cast<wgpu::StringView>(native.vendor);
    this->architecture = static_cast<wgpu::StringView>(native.architecture);
    this->device = static_cast<wgpu::StringView>(native.device);
    this->description = static_cast<wgpu::StringView>(native.description);
    this->backendType = static_cast<wgpu::BackendType>(native.backendType);
    this->adapterType = static_cast<wgpu::AdapterType>(native.adapterType);
    this->vendorID = static_cast<uint32_t>(native.vendorID);
    this->deviceID = static_cast<uint32_t>(native.deviceID);
    this->subgroupMinSize = static_cast<uint32_t>(native.subgroupMinSize);
    this->subgroupMaxSize = static_cast<uint32_t>(native.subgroupMaxSize);
}
AdapterInfo::CStruct AdapterInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.vendor = static_cast<WGPUStringView>(this->vendor.to_cstruct());
    cstruct.architecture = static_cast<WGPUStringView>(this->architecture.to_cstruct());
    cstruct.device = static_cast<WGPUStringView>(this->device.to_cstruct());
    cstruct.description = static_cast<WGPUStringView>(this->description.to_cstruct());
    cstruct.backendType = static_cast<WGPUBackendType>(this->backendType);
    cstruct.adapterType = static_cast<WGPUAdapterType>(this->adapterType);
    cstruct.vendorID = static_cast<uint32_t>(this->vendorID);
    cstruct.deviceID = static_cast<uint32_t>(this->deviceID);
    cstruct.subgroupMinSize = static_cast<uint32_t>(this->subgroupMinSize);
    cstruct.subgroupMaxSize = static_cast<uint32_t>(this->subgroupMaxSize);
    return cstruct;
}
AdapterInfo& AdapterInfo::setVendor(const wgpu::StringView& value) & {
    this->vendor = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setVendor(const wgpu::StringView& value) && {
    this->vendor = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setVendor(wgpu::StringView&& value) & {
    this->vendor = std::move(value);
    return *this;
}
AdapterInfo&& AdapterInfo::setVendor(wgpu::StringView&& value) && {
    this->vendor = std::move(value);
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setArchitecture(const wgpu::StringView& value) & {
    this->architecture = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setArchitecture(const wgpu::StringView& value) && {
    this->architecture = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setArchitecture(wgpu::StringView&& value) & {
    this->architecture = std::move(value);
    return *this;
}
AdapterInfo&& AdapterInfo::setArchitecture(wgpu::StringView&& value) && {
    this->architecture = std::move(value);
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setDevice(const wgpu::StringView& value) & {
    this->device = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setDevice(const wgpu::StringView& value) && {
    this->device = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setDevice(wgpu::StringView&& value) & {
    this->device = std::move(value);
    return *this;
}
AdapterInfo&& AdapterInfo::setDevice(wgpu::StringView&& value) && {
    this->device = std::move(value);
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setDescription(const wgpu::StringView& value) & {
    this->description = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setDescription(const wgpu::StringView& value) && {
    this->description = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setDescription(wgpu::StringView&& value) & {
    this->description = std::move(value);
    return *this;
}
AdapterInfo&& AdapterInfo::setDescription(wgpu::StringView&& value) && {
    this->description = std::move(value);
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setBackendType(wgpu::BackendType value) & {
    this->backendType = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setBackendType(wgpu::BackendType value) && {
    this->backendType = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setAdapterType(wgpu::AdapterType value) & {
    this->adapterType = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setAdapterType(wgpu::AdapterType value) && {
    this->adapterType = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setVendorID(uint32_t value) & {
    this->vendorID = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setVendorID(uint32_t value) && {
    this->vendorID = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setDeviceID(uint32_t value) & {
    this->deviceID = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setDeviceID(uint32_t value) && {
    this->deviceID = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setSubgroupMinSize(uint32_t value) & {
    this->subgroupMinSize = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setSubgroupMinSize(uint32_t value) && {
    this->subgroupMinSize = value;
    return std::move(*this);
}
AdapterInfo& AdapterInfo::setSubgroupMaxSize(uint32_t value) & {
    this->subgroupMaxSize = value;
    return *this;
}
AdapterInfo&& AdapterInfo::setSubgroupMaxSize(uint32_t value) && {
    this->subgroupMaxSize = value;
    return std::move(*this);
}
BlendComponent::BlendComponent(const WGPUBlendComponent& native) {
    this->operation = static_cast<wgpu::BlendOperation>(native.operation);
    this->srcFactor = static_cast<wgpu::BlendFactor>(native.srcFactor);
    this->dstFactor = static_cast<wgpu::BlendFactor>(native.dstFactor);
}
BlendComponent::CStruct BlendComponent::to_cstruct() const {
    CStruct cstruct;
    cstruct.operation = static_cast<WGPUBlendOperation>(this->operation);
    cstruct.srcFactor = static_cast<WGPUBlendFactor>(this->srcFactor);
    cstruct.dstFactor = static_cast<WGPUBlendFactor>(this->dstFactor);
    return cstruct;
}
BlendComponent& BlendComponent::setOperation(wgpu::BlendOperation value) & {
    this->operation = value;
    return *this;
}
BlendComponent&& BlendComponent::setOperation(wgpu::BlendOperation value) && {
    this->operation = value;
    return std::move(*this);
}
BlendComponent& BlendComponent::setSrcFactor(wgpu::BlendFactor value) & {
    this->srcFactor = value;
    return *this;
}
BlendComponent&& BlendComponent::setSrcFactor(wgpu::BlendFactor value) && {
    this->srcFactor = value;
    return std::move(*this);
}
BlendComponent& BlendComponent::setDstFactor(wgpu::BlendFactor value) & {
    this->dstFactor = value;
    return *this;
}
BlendComponent&& BlendComponent::setDstFactor(wgpu::BlendFactor value) && {
    this->dstFactor = value;
    return std::move(*this);
}
BufferBindingLayout::BufferBindingLayout(const WGPUBufferBindingLayout& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->type = static_cast<wgpu::BufferBindingType>(native.type);
    this->hasDynamicOffset = static_cast<wgpu::Bool>(native.hasDynamicOffset);
    this->minBindingSize = static_cast<uint64_t>(native.minBindingSize);
}
BufferBindingLayout::CStruct BufferBindingLayout::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.type = static_cast<WGPUBufferBindingType>(this->type);
    cstruct.hasDynamicOffset = static_cast<WGPUBool>(this->hasDynamicOffset);
    cstruct.minBindingSize = static_cast<uint64_t>(this->minBindingSize);
    return cstruct;
}
BufferBindingLayout& BufferBindingLayout::setType(wgpu::BufferBindingType value) & {
    this->type = value;
    return *this;
}
BufferBindingLayout&& BufferBindingLayout::setType(wgpu::BufferBindingType value) && {
    this->type = value;
    return std::move(*this);
}
BufferBindingLayout& BufferBindingLayout::setHasDynamicOffset(wgpu::Bool value) & {
    this->hasDynamicOffset = value;
    return *this;
}
BufferBindingLayout&& BufferBindingLayout::setHasDynamicOffset(wgpu::Bool value) && {
    this->hasDynamicOffset = value;
    return std::move(*this);
}
BufferBindingLayout& BufferBindingLayout::setMinBindingSize(uint64_t value) & {
    this->minBindingSize = value;
    return *this;
}
BufferBindingLayout&& BufferBindingLayout::setMinBindingSize(uint64_t value) && {
    this->minBindingSize = value;
    return std::move(*this);
}
BufferDescriptor::BufferDescriptor(const WGPUBufferDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->usage = static_cast<wgpu::BufferUsage>(native.usage);
    this->size = static_cast<uint64_t>(native.size);
    this->mappedAtCreation = static_cast<wgpu::Bool>(native.mappedAtCreation);
}
BufferDescriptor::CStruct BufferDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.usage = static_cast<WGPUBufferUsage>(this->usage);
    cstruct.size = static_cast<uint64_t>(this->size);
    cstruct.mappedAtCreation = static_cast<WGPUBool>(this->mappedAtCreation);
    return cstruct;
}
BufferDescriptor& BufferDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
BufferDescriptor&& BufferDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
BufferDescriptor& BufferDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
BufferDescriptor&& BufferDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
BufferDescriptor& BufferDescriptor::setUsage(wgpu::BufferUsage value) & {
    this->usage = value;
    return *this;
}
BufferDescriptor&& BufferDescriptor::setUsage(wgpu::BufferUsage value) && {
    this->usage = value;
    return std::move(*this);
}
BufferDescriptor& BufferDescriptor::setSize(uint64_t value) & {
    this->size = value;
    return *this;
}
BufferDescriptor&& BufferDescriptor::setSize(uint64_t value) && {
    this->size = value;
    return std::move(*this);
}
BufferDescriptor& BufferDescriptor::setMappedAtCreation(wgpu::Bool value) & {
    this->mappedAtCreation = value;
    return *this;
}
BufferDescriptor&& BufferDescriptor::setMappedAtCreation(wgpu::Bool value) && {
    this->mappedAtCreation = value;
    return std::move(*this);
}
Color::Color(const WGPUColor& native) {
    this->r = static_cast<double>(native.r);
    this->g = static_cast<double>(native.g);
    this->b = static_cast<double>(native.b);
    this->a = static_cast<double>(native.a);
}
Color::CStruct Color::to_cstruct() const {
    CStruct cstruct;
    cstruct.r = static_cast<double>(this->r);
    cstruct.g = static_cast<double>(this->g);
    cstruct.b = static_cast<double>(this->b);
    cstruct.a = static_cast<double>(this->a);
    return cstruct;
}
Color& Color::setR(double value) & {
    this->r = value;
    return *this;
}
Color&& Color::setR(double value) && {
    this->r = value;
    return std::move(*this);
}
Color& Color::setG(double value) & {
    this->g = value;
    return *this;
}
Color&& Color::setG(double value) && {
    this->g = value;
    return std::move(*this);
}
Color& Color::setB(double value) & {
    this->b = value;
    return *this;
}
Color&& Color::setB(double value) && {
    this->b = value;
    return std::move(*this);
}
Color& Color::setA(double value) & {
    this->a = value;
    return *this;
}
Color&& Color::setA(double value) && {
    this->a = value;
    return std::move(*this);
}
CommandBufferDescriptor::CommandBufferDescriptor(const WGPUCommandBufferDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
}
CommandBufferDescriptor::CStruct CommandBufferDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    return cstruct;
}
CommandBufferDescriptor& CommandBufferDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
CommandBufferDescriptor&& CommandBufferDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
CommandBufferDescriptor& CommandBufferDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
CommandBufferDescriptor&& CommandBufferDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
CommandEncoderDescriptor::CommandEncoderDescriptor(const WGPUCommandEncoderDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
}
CommandEncoderDescriptor::CStruct CommandEncoderDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    return cstruct;
}
CommandEncoderDescriptor& CommandEncoderDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
CommandEncoderDescriptor&& CommandEncoderDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
CommandEncoderDescriptor& CommandEncoderDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
CommandEncoderDescriptor&& CommandEncoderDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
CompilationMessage::CompilationMessage(const WGPUCompilationMessage& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->message = static_cast<wgpu::StringView>(native.message);
    this->type = static_cast<wgpu::CompilationMessageType>(native.type);
    this->lineNum = static_cast<uint64_t>(native.lineNum);
    this->linePos = static_cast<uint64_t>(native.linePos);
    this->offset = static_cast<uint64_t>(native.offset);
    this->length = static_cast<uint64_t>(native.length);
}
CompilationMessage::CStruct CompilationMessage::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.message = static_cast<WGPUStringView>(this->message.to_cstruct());
    cstruct.type = static_cast<WGPUCompilationMessageType>(this->type);
    cstruct.lineNum = static_cast<uint64_t>(this->lineNum);
    cstruct.linePos = static_cast<uint64_t>(this->linePos);
    cstruct.offset = static_cast<uint64_t>(this->offset);
    cstruct.length = static_cast<uint64_t>(this->length);
    return cstruct;
}
CompilationMessage& CompilationMessage::setMessage(const wgpu::StringView& value) & {
    this->message = value;
    return *this;
}
CompilationMessage&& CompilationMessage::setMessage(const wgpu::StringView& value) && {
    this->message = value;
    return std::move(*this);
}
CompilationMessage& CompilationMessage::setMessage(wgpu::StringView&& value) & {
    this->message = std::move(value);
    return *this;
}
CompilationMessage&& CompilationMessage::setMessage(wgpu::StringView&& value) && {
    this->message = std::move(value);
    return std::move(*this);
}
CompilationMessage& CompilationMessage::setType(wgpu::CompilationMessageType value) & {
    this->type = value;
    return *this;
}
CompilationMessage&& CompilationMessage::setType(wgpu::CompilationMessageType value) && {
    this->type = value;
    return std::move(*this);
}
CompilationMessage& CompilationMessage::setLineNum(uint64_t value) & {
    this->lineNum = value;
    return *this;
}
CompilationMessage&& CompilationMessage::setLineNum(uint64_t value) && {
    this->lineNum = value;
    return std::move(*this);
}
CompilationMessage& CompilationMessage::setLinePos(uint64_t value) & {
    this->linePos = value;
    return *this;
}
CompilationMessage&& CompilationMessage::setLinePos(uint64_t value) && {
    this->linePos = value;
    return std::move(*this);
}
CompilationMessage& CompilationMessage::setOffset(uint64_t value) & {
    this->offset = value;
    return *this;
}
CompilationMessage&& CompilationMessage::setOffset(uint64_t value) && {
    this->offset = value;
    return std::move(*this);
}
CompilationMessage& CompilationMessage::setLength(uint64_t value) & {
    this->length = value;
    return *this;
}
CompilationMessage&& CompilationMessage::setLength(uint64_t value) && {
    this->length = value;
    return std::move(*this);
}
ConstantEntry::ConstantEntry(const WGPUConstantEntry& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->key = static_cast<wgpu::StringView>(native.key);
    this->value = static_cast<double>(native.value);
}
ConstantEntry::CStruct ConstantEntry::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.key = static_cast<WGPUStringView>(this->key.to_cstruct());
    cstruct.value = static_cast<double>(this->value);
    return cstruct;
}
ConstantEntry& ConstantEntry::setKey(const wgpu::StringView& value) & {
    this->key = value;
    return *this;
}
ConstantEntry&& ConstantEntry::setKey(const wgpu::StringView& value) && {
    this->key = value;
    return std::move(*this);
}
ConstantEntry& ConstantEntry::setKey(wgpu::StringView&& value) & {
    this->key = std::move(value);
    return *this;
}
ConstantEntry&& ConstantEntry::setKey(wgpu::StringView&& value) && {
    this->key = std::move(value);
    return std::move(*this);
}
ConstantEntry& ConstantEntry::setValue(double value) & {
    this->value = value;
    return *this;
}
ConstantEntry&& ConstantEntry::setValue(double value) && {
    this->value = value;
    return std::move(*this);
}
Extent3D::Extent3D(const WGPUExtent3D& native) {
    this->width = static_cast<uint32_t>(native.width);
    this->height = static_cast<uint32_t>(native.height);
    this->depthOrArrayLayers = static_cast<uint32_t>(native.depthOrArrayLayers);
}
Extent3D::CStruct Extent3D::to_cstruct() const {
    CStruct cstruct;
    cstruct.width = static_cast<uint32_t>(this->width);
    cstruct.height = static_cast<uint32_t>(this->height);
    cstruct.depthOrArrayLayers = static_cast<uint32_t>(this->depthOrArrayLayers);
    return cstruct;
}
Extent3D& Extent3D::setWidth(uint32_t value) & {
    this->width = value;
    return *this;
}
Extent3D&& Extent3D::setWidth(uint32_t value) && {
    this->width = value;
    return std::move(*this);
}
Extent3D& Extent3D::setHeight(uint32_t value) & {
    this->height = value;
    return *this;
}
Extent3D&& Extent3D::setHeight(uint32_t value) && {
    this->height = value;
    return std::move(*this);
}
Extent3D& Extent3D::setDepthOrArrayLayers(uint32_t value) & {
    this->depthOrArrayLayers = value;
    return *this;
}
Extent3D&& Extent3D::setDepthOrArrayLayers(uint32_t value) && {
    this->depthOrArrayLayers = value;
    return std::move(*this);
}
ExternalTextureBindingEntry::ExternalTextureBindingEntry(const WGPUExternalTextureBindingEntry& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->externalTexture = static_cast<wgpu::raw::ExternalTexture>(native.externalTexture);
}
ExternalTextureBindingEntry::CStruct ExternalTextureBindingEntry::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.externalTexture = static_cast<WGPUExternalTexture>(this->externalTexture);
    return cstruct;
}
ExternalTextureBindingEntry& ExternalTextureBindingEntry::setExternalTexture(wgpu::raw::ExternalTexture value) & {
    this->externalTexture = value;
    return *this;
}
ExternalTextureBindingEntry&& ExternalTextureBindingEntry::setExternalTexture(wgpu::raw::ExternalTexture value) && {
    this->externalTexture = value;
    return std::move(*this);
}
ExternalTextureBindingLayout::ExternalTextureBindingLayout(const WGPUExternalTextureBindingLayout& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
}
ExternalTextureBindingLayout::CStruct ExternalTextureBindingLayout::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    return cstruct;
}
Future::Future(const WGPUFuture& native) {
    this->id = static_cast<uint64_t>(native.id);
}
Future::CStruct Future::to_cstruct() const {
    CStruct cstruct;
    cstruct.id = static_cast<uint64_t>(this->id);
    return cstruct;
}
Future& Future::setId(uint64_t value) & {
    this->id = value;
    return *this;
}
Future&& Future::setId(uint64_t value) && {
    this->id = value;
    return std::move(*this);
}
InstanceLimits::InstanceLimits(const WGPUInstanceLimits& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->timedWaitAnyMaxCount = static_cast<size_t>(native.timedWaitAnyMaxCount);
}
InstanceLimits::CStruct InstanceLimits::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.timedWaitAnyMaxCount = static_cast<size_t>(this->timedWaitAnyMaxCount);
    return cstruct;
}
InstanceLimits& InstanceLimits::setTimedWaitAnyMaxCount(size_t value) & {
    this->timedWaitAnyMaxCount = value;
    return *this;
}
InstanceLimits&& InstanceLimits::setTimedWaitAnyMaxCount(size_t value) && {
    this->timedWaitAnyMaxCount = value;
    return std::move(*this);
}
Limits::Limits(const WGPULimits& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->maxTextureDimension1D = static_cast<uint32_t>(native.maxTextureDimension1D);
    this->maxTextureDimension2D = static_cast<uint32_t>(native.maxTextureDimension2D);
    this->maxTextureDimension3D = static_cast<uint32_t>(native.maxTextureDimension3D);
    this->maxTextureArrayLayers = static_cast<uint32_t>(native.maxTextureArrayLayers);
    this->maxBindGroups = static_cast<uint32_t>(native.maxBindGroups);
    this->maxBindGroupsPlusVertexBuffers = static_cast<uint32_t>(native.maxBindGroupsPlusVertexBuffers);
    this->maxBindingsPerBindGroup = static_cast<uint32_t>(native.maxBindingsPerBindGroup);
    this->maxDynamicUniformBuffersPerPipelineLayout = static_cast<uint32_t>(native.maxDynamicUniformBuffersPerPipelineLayout);
    this->maxDynamicStorageBuffersPerPipelineLayout = static_cast<uint32_t>(native.maxDynamicStorageBuffersPerPipelineLayout);
    this->maxSampledTexturesPerShaderStage = static_cast<uint32_t>(native.maxSampledTexturesPerShaderStage);
    this->maxSamplersPerShaderStage = static_cast<uint32_t>(native.maxSamplersPerShaderStage);
    this->maxStorageBuffersPerShaderStage = static_cast<uint32_t>(native.maxStorageBuffersPerShaderStage);
    this->maxStorageTexturesPerShaderStage = static_cast<uint32_t>(native.maxStorageTexturesPerShaderStage);
    this->maxUniformBuffersPerShaderStage = static_cast<uint32_t>(native.maxUniformBuffersPerShaderStage);
    this->maxUniformBufferBindingSize = static_cast<uint64_t>(native.maxUniformBufferBindingSize);
    this->maxStorageBufferBindingSize = static_cast<uint64_t>(native.maxStorageBufferBindingSize);
    this->minUniformBufferOffsetAlignment = static_cast<uint32_t>(native.minUniformBufferOffsetAlignment);
    this->minStorageBufferOffsetAlignment = static_cast<uint32_t>(native.minStorageBufferOffsetAlignment);
    this->maxVertexBuffers = static_cast<uint32_t>(native.maxVertexBuffers);
    this->maxBufferSize = static_cast<uint64_t>(native.maxBufferSize);
    this->maxVertexAttributes = static_cast<uint32_t>(native.maxVertexAttributes);
    this->maxVertexBufferArrayStride = static_cast<uint32_t>(native.maxVertexBufferArrayStride);
    this->maxInterStageShaderVariables = static_cast<uint32_t>(native.maxInterStageShaderVariables);
    this->maxColorAttachments = static_cast<uint32_t>(native.maxColorAttachments);
    this->maxColorAttachmentBytesPerSample = static_cast<uint32_t>(native.maxColorAttachmentBytesPerSample);
    this->maxComputeWorkgroupStorageSize = static_cast<uint32_t>(native.maxComputeWorkgroupStorageSize);
    this->maxComputeInvocationsPerWorkgroup = static_cast<uint32_t>(native.maxComputeInvocationsPerWorkgroup);
    this->maxComputeWorkgroupSizeX = static_cast<uint32_t>(native.maxComputeWorkgroupSizeX);
    this->maxComputeWorkgroupSizeY = static_cast<uint32_t>(native.maxComputeWorkgroupSizeY);
    this->maxComputeWorkgroupSizeZ = static_cast<uint32_t>(native.maxComputeWorkgroupSizeZ);
    this->maxComputeWorkgroupsPerDimension = static_cast<uint32_t>(native.maxComputeWorkgroupsPerDimension);
    this->maxImmediateSize = static_cast<uint32_t>(native.maxImmediateSize);
}
Limits::CStruct Limits::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.maxTextureDimension1D = static_cast<uint32_t>(this->maxTextureDimension1D);
    cstruct.maxTextureDimension2D = static_cast<uint32_t>(this->maxTextureDimension2D);
    cstruct.maxTextureDimension3D = static_cast<uint32_t>(this->maxTextureDimension3D);
    cstruct.maxTextureArrayLayers = static_cast<uint32_t>(this->maxTextureArrayLayers);
    cstruct.maxBindGroups = static_cast<uint32_t>(this->maxBindGroups);
    cstruct.maxBindGroupsPlusVertexBuffers = static_cast<uint32_t>(this->maxBindGroupsPlusVertexBuffers);
    cstruct.maxBindingsPerBindGroup = static_cast<uint32_t>(this->maxBindingsPerBindGroup);
    cstruct.maxDynamicUniformBuffersPerPipelineLayout = static_cast<uint32_t>(this->maxDynamicUniformBuffersPerPipelineLayout);
    cstruct.maxDynamicStorageBuffersPerPipelineLayout = static_cast<uint32_t>(this->maxDynamicStorageBuffersPerPipelineLayout);
    cstruct.maxSampledTexturesPerShaderStage = static_cast<uint32_t>(this->maxSampledTexturesPerShaderStage);
    cstruct.maxSamplersPerShaderStage = static_cast<uint32_t>(this->maxSamplersPerShaderStage);
    cstruct.maxStorageBuffersPerShaderStage = static_cast<uint32_t>(this->maxStorageBuffersPerShaderStage);
    cstruct.maxStorageTexturesPerShaderStage = static_cast<uint32_t>(this->maxStorageTexturesPerShaderStage);
    cstruct.maxUniformBuffersPerShaderStage = static_cast<uint32_t>(this->maxUniformBuffersPerShaderStage);
    cstruct.maxUniformBufferBindingSize = static_cast<uint64_t>(this->maxUniformBufferBindingSize);
    cstruct.maxStorageBufferBindingSize = static_cast<uint64_t>(this->maxStorageBufferBindingSize);
    cstruct.minUniformBufferOffsetAlignment = static_cast<uint32_t>(this->minUniformBufferOffsetAlignment);
    cstruct.minStorageBufferOffsetAlignment = static_cast<uint32_t>(this->minStorageBufferOffsetAlignment);
    cstruct.maxVertexBuffers = static_cast<uint32_t>(this->maxVertexBuffers);
    cstruct.maxBufferSize = static_cast<uint64_t>(this->maxBufferSize);
    cstruct.maxVertexAttributes = static_cast<uint32_t>(this->maxVertexAttributes);
    cstruct.maxVertexBufferArrayStride = static_cast<uint32_t>(this->maxVertexBufferArrayStride);
    cstruct.maxInterStageShaderVariables = static_cast<uint32_t>(this->maxInterStageShaderVariables);
    cstruct.maxColorAttachments = static_cast<uint32_t>(this->maxColorAttachments);
    cstruct.maxColorAttachmentBytesPerSample = static_cast<uint32_t>(this->maxColorAttachmentBytesPerSample);
    cstruct.maxComputeWorkgroupStorageSize = static_cast<uint32_t>(this->maxComputeWorkgroupStorageSize);
    cstruct.maxComputeInvocationsPerWorkgroup = static_cast<uint32_t>(this->maxComputeInvocationsPerWorkgroup);
    cstruct.maxComputeWorkgroupSizeX = static_cast<uint32_t>(this->maxComputeWorkgroupSizeX);
    cstruct.maxComputeWorkgroupSizeY = static_cast<uint32_t>(this->maxComputeWorkgroupSizeY);
    cstruct.maxComputeWorkgroupSizeZ = static_cast<uint32_t>(this->maxComputeWorkgroupSizeZ);
    cstruct.maxComputeWorkgroupsPerDimension = static_cast<uint32_t>(this->maxComputeWorkgroupsPerDimension);
    cstruct.maxImmediateSize = static_cast<uint32_t>(this->maxImmediateSize);
    return cstruct;
}
Limits& Limits::setMaxTextureDimension1D(uint32_t value) & {
    this->maxTextureDimension1D = value;
    return *this;
}
Limits&& Limits::setMaxTextureDimension1D(uint32_t value) && {
    this->maxTextureDimension1D = value;
    return std::move(*this);
}
Limits& Limits::setMaxTextureDimension2D(uint32_t value) & {
    this->maxTextureDimension2D = value;
    return *this;
}
Limits&& Limits::setMaxTextureDimension2D(uint32_t value) && {
    this->maxTextureDimension2D = value;
    return std::move(*this);
}
Limits& Limits::setMaxTextureDimension3D(uint32_t value) & {
    this->maxTextureDimension3D = value;
    return *this;
}
Limits&& Limits::setMaxTextureDimension3D(uint32_t value) && {
    this->maxTextureDimension3D = value;
    return std::move(*this);
}
Limits& Limits::setMaxTextureArrayLayers(uint32_t value) & {
    this->maxTextureArrayLayers = value;
    return *this;
}
Limits&& Limits::setMaxTextureArrayLayers(uint32_t value) && {
    this->maxTextureArrayLayers = value;
    return std::move(*this);
}
Limits& Limits::setMaxBindGroups(uint32_t value) & {
    this->maxBindGroups = value;
    return *this;
}
Limits&& Limits::setMaxBindGroups(uint32_t value) && {
    this->maxBindGroups = value;
    return std::move(*this);
}
Limits& Limits::setMaxBindGroupsPlusVertexBuffers(uint32_t value) & {
    this->maxBindGroupsPlusVertexBuffers = value;
    return *this;
}
Limits&& Limits::setMaxBindGroupsPlusVertexBuffers(uint32_t value) && {
    this->maxBindGroupsPlusVertexBuffers = value;
    return std::move(*this);
}
Limits& Limits::setMaxBindingsPerBindGroup(uint32_t value) & {
    this->maxBindingsPerBindGroup = value;
    return *this;
}
Limits&& Limits::setMaxBindingsPerBindGroup(uint32_t value) && {
    this->maxBindingsPerBindGroup = value;
    return std::move(*this);
}
Limits& Limits::setMaxDynamicUniformBuffersPerPipelineLayout(uint32_t value) & {
    this->maxDynamicUniformBuffersPerPipelineLayout = value;
    return *this;
}
Limits&& Limits::setMaxDynamicUniformBuffersPerPipelineLayout(uint32_t value) && {
    this->maxDynamicUniformBuffersPerPipelineLayout = value;
    return std::move(*this);
}
Limits& Limits::setMaxDynamicStorageBuffersPerPipelineLayout(uint32_t value) & {
    this->maxDynamicStorageBuffersPerPipelineLayout = value;
    return *this;
}
Limits&& Limits::setMaxDynamicStorageBuffersPerPipelineLayout(uint32_t value) && {
    this->maxDynamicStorageBuffersPerPipelineLayout = value;
    return std::move(*this);
}
Limits& Limits::setMaxSampledTexturesPerShaderStage(uint32_t value) & {
    this->maxSampledTexturesPerShaderStage = value;
    return *this;
}
Limits&& Limits::setMaxSampledTexturesPerShaderStage(uint32_t value) && {
    this->maxSampledTexturesPerShaderStage = value;
    return std::move(*this);
}
Limits& Limits::setMaxSamplersPerShaderStage(uint32_t value) & {
    this->maxSamplersPerShaderStage = value;
    return *this;
}
Limits&& Limits::setMaxSamplersPerShaderStage(uint32_t value) && {
    this->maxSamplersPerShaderStage = value;
    return std::move(*this);
}
Limits& Limits::setMaxStorageBuffersPerShaderStage(uint32_t value) & {
    this->maxStorageBuffersPerShaderStage = value;
    return *this;
}
Limits&& Limits::setMaxStorageBuffersPerShaderStage(uint32_t value) && {
    this->maxStorageBuffersPerShaderStage = value;
    return std::move(*this);
}
Limits& Limits::setMaxStorageTexturesPerShaderStage(uint32_t value) & {
    this->maxStorageTexturesPerShaderStage = value;
    return *this;
}
Limits&& Limits::setMaxStorageTexturesPerShaderStage(uint32_t value) && {
    this->maxStorageTexturesPerShaderStage = value;
    return std::move(*this);
}
Limits& Limits::setMaxUniformBuffersPerShaderStage(uint32_t value) & {
    this->maxUniformBuffersPerShaderStage = value;
    return *this;
}
Limits&& Limits::setMaxUniformBuffersPerShaderStage(uint32_t value) && {
    this->maxUniformBuffersPerShaderStage = value;
    return std::move(*this);
}
Limits& Limits::setMaxUniformBufferBindingSize(uint64_t value) & {
    this->maxUniformBufferBindingSize = value;
    return *this;
}
Limits&& Limits::setMaxUniformBufferBindingSize(uint64_t value) && {
    this->maxUniformBufferBindingSize = value;
    return std::move(*this);
}
Limits& Limits::setMaxStorageBufferBindingSize(uint64_t value) & {
    this->maxStorageBufferBindingSize = value;
    return *this;
}
Limits&& Limits::setMaxStorageBufferBindingSize(uint64_t value) && {
    this->maxStorageBufferBindingSize = value;
    return std::move(*this);
}
Limits& Limits::setMinUniformBufferOffsetAlignment(uint32_t value) & {
    this->minUniformBufferOffsetAlignment = value;
    return *this;
}
Limits&& Limits::setMinUniformBufferOffsetAlignment(uint32_t value) && {
    this->minUniformBufferOffsetAlignment = value;
    return std::move(*this);
}
Limits& Limits::setMinStorageBufferOffsetAlignment(uint32_t value) & {
    this->minStorageBufferOffsetAlignment = value;
    return *this;
}
Limits&& Limits::setMinStorageBufferOffsetAlignment(uint32_t value) && {
    this->minStorageBufferOffsetAlignment = value;
    return std::move(*this);
}
Limits& Limits::setMaxVertexBuffers(uint32_t value) & {
    this->maxVertexBuffers = value;
    return *this;
}
Limits&& Limits::setMaxVertexBuffers(uint32_t value) && {
    this->maxVertexBuffers = value;
    return std::move(*this);
}
Limits& Limits::setMaxBufferSize(uint64_t value) & {
    this->maxBufferSize = value;
    return *this;
}
Limits&& Limits::setMaxBufferSize(uint64_t value) && {
    this->maxBufferSize = value;
    return std::move(*this);
}
Limits& Limits::setMaxVertexAttributes(uint32_t value) & {
    this->maxVertexAttributes = value;
    return *this;
}
Limits&& Limits::setMaxVertexAttributes(uint32_t value) && {
    this->maxVertexAttributes = value;
    return std::move(*this);
}
Limits& Limits::setMaxVertexBufferArrayStride(uint32_t value) & {
    this->maxVertexBufferArrayStride = value;
    return *this;
}
Limits&& Limits::setMaxVertexBufferArrayStride(uint32_t value) && {
    this->maxVertexBufferArrayStride = value;
    return std::move(*this);
}
Limits& Limits::setMaxInterStageShaderVariables(uint32_t value) & {
    this->maxInterStageShaderVariables = value;
    return *this;
}
Limits&& Limits::setMaxInterStageShaderVariables(uint32_t value) && {
    this->maxInterStageShaderVariables = value;
    return std::move(*this);
}
Limits& Limits::setMaxColorAttachments(uint32_t value) & {
    this->maxColorAttachments = value;
    return *this;
}
Limits&& Limits::setMaxColorAttachments(uint32_t value) && {
    this->maxColorAttachments = value;
    return std::move(*this);
}
Limits& Limits::setMaxColorAttachmentBytesPerSample(uint32_t value) & {
    this->maxColorAttachmentBytesPerSample = value;
    return *this;
}
Limits&& Limits::setMaxColorAttachmentBytesPerSample(uint32_t value) && {
    this->maxColorAttachmentBytesPerSample = value;
    return std::move(*this);
}
Limits& Limits::setMaxComputeWorkgroupStorageSize(uint32_t value) & {
    this->maxComputeWorkgroupStorageSize = value;
    return *this;
}
Limits&& Limits::setMaxComputeWorkgroupStorageSize(uint32_t value) && {
    this->maxComputeWorkgroupStorageSize = value;
    return std::move(*this);
}
Limits& Limits::setMaxComputeInvocationsPerWorkgroup(uint32_t value) & {
    this->maxComputeInvocationsPerWorkgroup = value;
    return *this;
}
Limits&& Limits::setMaxComputeInvocationsPerWorkgroup(uint32_t value) && {
    this->maxComputeInvocationsPerWorkgroup = value;
    return std::move(*this);
}
Limits& Limits::setMaxComputeWorkgroupSizeX(uint32_t value) & {
    this->maxComputeWorkgroupSizeX = value;
    return *this;
}
Limits&& Limits::setMaxComputeWorkgroupSizeX(uint32_t value) && {
    this->maxComputeWorkgroupSizeX = value;
    return std::move(*this);
}
Limits& Limits::setMaxComputeWorkgroupSizeY(uint32_t value) & {
    this->maxComputeWorkgroupSizeY = value;
    return *this;
}
Limits&& Limits::setMaxComputeWorkgroupSizeY(uint32_t value) && {
    this->maxComputeWorkgroupSizeY = value;
    return std::move(*this);
}
Limits& Limits::setMaxComputeWorkgroupSizeZ(uint32_t value) & {
    this->maxComputeWorkgroupSizeZ = value;
    return *this;
}
Limits&& Limits::setMaxComputeWorkgroupSizeZ(uint32_t value) && {
    this->maxComputeWorkgroupSizeZ = value;
    return std::move(*this);
}
Limits& Limits::setMaxComputeWorkgroupsPerDimension(uint32_t value) & {
    this->maxComputeWorkgroupsPerDimension = value;
    return *this;
}
Limits&& Limits::setMaxComputeWorkgroupsPerDimension(uint32_t value) && {
    this->maxComputeWorkgroupsPerDimension = value;
    return std::move(*this);
}
Limits& Limits::setMaxImmediateSize(uint32_t value) & {
    this->maxImmediateSize = value;
    return *this;
}
Limits&& Limits::setMaxImmediateSize(uint32_t value) && {
    this->maxImmediateSize = value;
    return std::move(*this);
}
MultisampleState::MultisampleState(const WGPUMultisampleState& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->count = static_cast<uint32_t>(native.count);
    this->mask = static_cast<uint32_t>(native.mask);
    this->alphaToCoverageEnabled = static_cast<wgpu::Bool>(native.alphaToCoverageEnabled);
}
MultisampleState::CStruct MultisampleState::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.count = static_cast<uint32_t>(this->count);
    cstruct.mask = static_cast<uint32_t>(this->mask);
    cstruct.alphaToCoverageEnabled = static_cast<WGPUBool>(this->alphaToCoverageEnabled);
    return cstruct;
}
MultisampleState& MultisampleState::setCount(uint32_t value) & {
    this->count = value;
    return *this;
}
MultisampleState&& MultisampleState::setCount(uint32_t value) && {
    this->count = value;
    return std::move(*this);
}
MultisampleState& MultisampleState::setMask(uint32_t value) & {
    this->mask = value;
    return *this;
}
MultisampleState&& MultisampleState::setMask(uint32_t value) && {
    this->mask = value;
    return std::move(*this);
}
MultisampleState& MultisampleState::setAlphaToCoverageEnabled(wgpu::Bool value) & {
    this->alphaToCoverageEnabled = value;
    return *this;
}
MultisampleState&& MultisampleState::setAlphaToCoverageEnabled(wgpu::Bool value) && {
    this->alphaToCoverageEnabled = value;
    return std::move(*this);
}
Origin3D::Origin3D(const WGPUOrigin3D& native) {
    this->x = static_cast<uint32_t>(native.x);
    this->y = static_cast<uint32_t>(native.y);
    this->z = static_cast<uint32_t>(native.z);
}
Origin3D::CStruct Origin3D::to_cstruct() const {
    CStruct cstruct;
    cstruct.x = static_cast<uint32_t>(this->x);
    cstruct.y = static_cast<uint32_t>(this->y);
    cstruct.z = static_cast<uint32_t>(this->z);
    return cstruct;
}
Origin3D& Origin3D::setX(uint32_t value) & {
    this->x = value;
    return *this;
}
Origin3D&& Origin3D::setX(uint32_t value) && {
    this->x = value;
    return std::move(*this);
}
Origin3D& Origin3D::setY(uint32_t value) & {
    this->y = value;
    return *this;
}
Origin3D&& Origin3D::setY(uint32_t value) && {
    this->y = value;
    return std::move(*this);
}
Origin3D& Origin3D::setZ(uint32_t value) & {
    this->z = value;
    return *this;
}
Origin3D&& Origin3D::setZ(uint32_t value) && {
    this->z = value;
    return std::move(*this);
}
PassTimestampWrites::PassTimestampWrites(const WGPUPassTimestampWrites& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->querySet = static_cast<wgpu::raw::QuerySet>(native.querySet);
    this->beginningOfPassWriteIndex = static_cast<uint32_t>(native.beginningOfPassWriteIndex);
    this->endOfPassWriteIndex = static_cast<uint32_t>(native.endOfPassWriteIndex);
}
PassTimestampWrites::CStruct PassTimestampWrites::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.querySet = static_cast<WGPUQuerySet>(this->querySet);
    cstruct.beginningOfPassWriteIndex = static_cast<uint32_t>(this->beginningOfPassWriteIndex);
    cstruct.endOfPassWriteIndex = static_cast<uint32_t>(this->endOfPassWriteIndex);
    return cstruct;
}
PassTimestampWrites& PassTimestampWrites::setQuerySet(wgpu::raw::QuerySet value) & {
    this->querySet = value;
    return *this;
}
PassTimestampWrites&& PassTimestampWrites::setQuerySet(wgpu::raw::QuerySet value) && {
    this->querySet = value;
    return std::move(*this);
}
PassTimestampWrites& PassTimestampWrites::setBeginningOfPassWriteIndex(uint32_t value) & {
    this->beginningOfPassWriteIndex = value;
    return *this;
}
PassTimestampWrites&& PassTimestampWrites::setBeginningOfPassWriteIndex(uint32_t value) && {
    this->beginningOfPassWriteIndex = value;
    return std::move(*this);
}
PassTimestampWrites& PassTimestampWrites::setEndOfPassWriteIndex(uint32_t value) & {
    this->endOfPassWriteIndex = value;
    return *this;
}
PassTimestampWrites&& PassTimestampWrites::setEndOfPassWriteIndex(uint32_t value) && {
    this->endOfPassWriteIndex = value;
    return std::move(*this);
}
PipelineLayoutDescriptor::PipelineLayoutDescriptor(const WGPUPipelineLayoutDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->bindGroupLayouts = std::span(native.bindGroupLayouts, native.bindGroupLayoutCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::raw::BindGroupLayout>(e); }) | std::ranges::to<std::vector<wgpu::raw::BindGroupLayout>>();
    this->immediateSize = static_cast<uint32_t>(native.immediateSize);
}
PipelineLayoutDescriptor::CStruct PipelineLayoutDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.bindGroupLayouts = reinterpret_cast<const WGPUBindGroupLayout*>(this->bindGroupLayouts.data());
    cstruct.bindGroupLayoutCount = static_cast<size_t>(this->bindGroupLayouts.size());
    cstruct.immediateSize = static_cast<uint32_t>(this->immediateSize);
    return cstruct;
}
PipelineLayoutDescriptor& PipelineLayoutDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
PipelineLayoutDescriptor&& PipelineLayoutDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
PipelineLayoutDescriptor& PipelineLayoutDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
PipelineLayoutDescriptor&& PipelineLayoutDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
PipelineLayoutDescriptor& PipelineLayoutDescriptor::setImmediateSize(uint32_t value) & {
    this->immediateSize = value;
    return *this;
}
PipelineLayoutDescriptor&& PipelineLayoutDescriptor::setImmediateSize(uint32_t value) && {
    this->immediateSize = value;
    return std::move(*this);
}
PrimitiveState::PrimitiveState(const WGPUPrimitiveState& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->topology = static_cast<wgpu::PrimitiveTopology>(native.topology);
    this->stripIndexFormat = static_cast<wgpu::IndexFormat>(native.stripIndexFormat);
    this->frontFace = static_cast<wgpu::FrontFace>(native.frontFace);
    this->cullMode = static_cast<wgpu::CullMode>(native.cullMode);
    this->unclippedDepth = static_cast<wgpu::Bool>(native.unclippedDepth);
}
PrimitiveState::CStruct PrimitiveState::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.topology = static_cast<WGPUPrimitiveTopology>(this->topology);
    cstruct.stripIndexFormat = static_cast<WGPUIndexFormat>(this->stripIndexFormat);
    cstruct.frontFace = static_cast<WGPUFrontFace>(this->frontFace);
    cstruct.cullMode = static_cast<WGPUCullMode>(this->cullMode);
    cstruct.unclippedDepth = static_cast<WGPUBool>(this->unclippedDepth);
    return cstruct;
}
PrimitiveState& PrimitiveState::setTopology(wgpu::PrimitiveTopology value) & {
    this->topology = value;
    return *this;
}
PrimitiveState&& PrimitiveState::setTopology(wgpu::PrimitiveTopology value) && {
    this->topology = value;
    return std::move(*this);
}
PrimitiveState& PrimitiveState::setStripIndexFormat(wgpu::IndexFormat value) & {
    this->stripIndexFormat = value;
    return *this;
}
PrimitiveState&& PrimitiveState::setStripIndexFormat(wgpu::IndexFormat value) && {
    this->stripIndexFormat = value;
    return std::move(*this);
}
PrimitiveState& PrimitiveState::setFrontFace(wgpu::FrontFace value) & {
    this->frontFace = value;
    return *this;
}
PrimitiveState&& PrimitiveState::setFrontFace(wgpu::FrontFace value) && {
    this->frontFace = value;
    return std::move(*this);
}
PrimitiveState& PrimitiveState::setCullMode(wgpu::CullMode value) & {
    this->cullMode = value;
    return *this;
}
PrimitiveState&& PrimitiveState::setCullMode(wgpu::CullMode value) && {
    this->cullMode = value;
    return std::move(*this);
}
PrimitiveState& PrimitiveState::setUnclippedDepth(wgpu::Bool value) & {
    this->unclippedDepth = value;
    return *this;
}
PrimitiveState&& PrimitiveState::setUnclippedDepth(wgpu::Bool value) && {
    this->unclippedDepth = value;
    return std::move(*this);
}
QuerySetDescriptor::QuerySetDescriptor(const WGPUQuerySetDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->type = static_cast<wgpu::QueryType>(native.type);
    this->count = static_cast<uint32_t>(native.count);
}
QuerySetDescriptor::CStruct QuerySetDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.type = static_cast<WGPUQueryType>(this->type);
    cstruct.count = static_cast<uint32_t>(this->count);
    return cstruct;
}
QuerySetDescriptor& QuerySetDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
QuerySetDescriptor&& QuerySetDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
QuerySetDescriptor& QuerySetDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
QuerySetDescriptor&& QuerySetDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
QuerySetDescriptor& QuerySetDescriptor::setType(wgpu::QueryType value) & {
    this->type = value;
    return *this;
}
QuerySetDescriptor&& QuerySetDescriptor::setType(wgpu::QueryType value) && {
    this->type = value;
    return std::move(*this);
}
QuerySetDescriptor& QuerySetDescriptor::setCount(uint32_t value) & {
    this->count = value;
    return *this;
}
QuerySetDescriptor&& QuerySetDescriptor::setCount(uint32_t value) && {
    this->count = value;
    return std::move(*this);
}
QueueDescriptor::QueueDescriptor(const WGPUQueueDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
}
QueueDescriptor::CStruct QueueDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    return cstruct;
}
QueueDescriptor& QueueDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
QueueDescriptor&& QueueDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
QueueDescriptor& QueueDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
QueueDescriptor&& QueueDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
RenderBundleDescriptor::RenderBundleDescriptor(const WGPURenderBundleDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
}
RenderBundleDescriptor::CStruct RenderBundleDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    return cstruct;
}
RenderBundleDescriptor& RenderBundleDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
RenderBundleDescriptor&& RenderBundleDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
RenderBundleDescriptor& RenderBundleDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
RenderBundleDescriptor&& RenderBundleDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
RenderBundleEncoderDescriptor::RenderBundleEncoderDescriptor(const WGPURenderBundleEncoderDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->colorFormats = std::span(native.colorFormats, native.colorFormatCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    this->depthStencilFormat = static_cast<wgpu::TextureFormat>(native.depthStencilFormat);
    this->sampleCount = static_cast<uint32_t>(native.sampleCount);
    this->depthReadOnly = static_cast<wgpu::Bool>(native.depthReadOnly);
    this->stencilReadOnly = static_cast<wgpu::Bool>(native.stencilReadOnly);
}
RenderBundleEncoderDescriptor::CStruct RenderBundleEncoderDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.colorFormats = reinterpret_cast<const WGPUTextureFormat*>(this->colorFormats.data());
    cstruct.colorFormatCount = static_cast<size_t>(this->colorFormats.size());
    cstruct.depthStencilFormat = static_cast<WGPUTextureFormat>(this->depthStencilFormat);
    cstruct.sampleCount = static_cast<uint32_t>(this->sampleCount);
    cstruct.depthReadOnly = static_cast<WGPUBool>(this->depthReadOnly);
    cstruct.stencilReadOnly = static_cast<WGPUBool>(this->stencilReadOnly);
    return cstruct;
}
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setDepthStencilFormat(wgpu::TextureFormat value) & {
    this->depthStencilFormat = value;
    return *this;
}
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setDepthStencilFormat(wgpu::TextureFormat value) && {
    this->depthStencilFormat = value;
    return std::move(*this);
}
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setSampleCount(uint32_t value) & {
    this->sampleCount = value;
    return *this;
}
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setSampleCount(uint32_t value) && {
    this->sampleCount = value;
    return std::move(*this);
}
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setDepthReadOnly(wgpu::Bool value) & {
    this->depthReadOnly = value;
    return *this;
}
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setDepthReadOnly(wgpu::Bool value) && {
    this->depthReadOnly = value;
    return std::move(*this);
}
RenderBundleEncoderDescriptor& RenderBundleEncoderDescriptor::setStencilReadOnly(wgpu::Bool value) & {
    this->stencilReadOnly = value;
    return *this;
}
RenderBundleEncoderDescriptor&& RenderBundleEncoderDescriptor::setStencilReadOnly(wgpu::Bool value) && {
    this->stencilReadOnly = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment::RenderPassDepthStencilAttachment(const WGPURenderPassDepthStencilAttachment& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->view = static_cast<wgpu::raw::TextureView>(native.view);
    this->depthLoadOp = static_cast<wgpu::LoadOp>(native.depthLoadOp);
    this->depthStoreOp = static_cast<wgpu::StoreOp>(native.depthStoreOp);
    this->depthClearValue = static_cast<float>(native.depthClearValue);
    this->depthReadOnly = static_cast<wgpu::Bool>(native.depthReadOnly);
    this->stencilLoadOp = static_cast<wgpu::LoadOp>(native.stencilLoadOp);
    this->stencilStoreOp = static_cast<wgpu::StoreOp>(native.stencilStoreOp);
    this->stencilClearValue = static_cast<uint32_t>(native.stencilClearValue);
    this->stencilReadOnly = static_cast<wgpu::Bool>(native.stencilReadOnly);
}
RenderPassDepthStencilAttachment::CStruct RenderPassDepthStencilAttachment::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.view = static_cast<WGPUTextureView>(this->view);
    cstruct.depthLoadOp = static_cast<WGPULoadOp>(this->depthLoadOp);
    cstruct.depthStoreOp = static_cast<WGPUStoreOp>(this->depthStoreOp);
    cstruct.depthClearValue = static_cast<float>(this->depthClearValue);
    cstruct.depthReadOnly = static_cast<WGPUBool>(this->depthReadOnly);
    cstruct.stencilLoadOp = static_cast<WGPULoadOp>(this->stencilLoadOp);
    cstruct.stencilStoreOp = static_cast<WGPUStoreOp>(this->stencilStoreOp);
    cstruct.stencilClearValue = static_cast<uint32_t>(this->stencilClearValue);
    cstruct.stencilReadOnly = static_cast<WGPUBool>(this->stencilReadOnly);
    return cstruct;
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setView(wgpu::raw::TextureView value) & {
    this->view = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setView(wgpu::raw::TextureView value) && {
    this->view = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setDepthLoadOp(wgpu::LoadOp value) & {
    this->depthLoadOp = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setDepthLoadOp(wgpu::LoadOp value) && {
    this->depthLoadOp = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setDepthStoreOp(wgpu::StoreOp value) & {
    this->depthStoreOp = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setDepthStoreOp(wgpu::StoreOp value) && {
    this->depthStoreOp = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setDepthClearValue(float value) & {
    this->depthClearValue = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setDepthClearValue(float value) && {
    this->depthClearValue = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setDepthReadOnly(wgpu::Bool value) & {
    this->depthReadOnly = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setDepthReadOnly(wgpu::Bool value) && {
    this->depthReadOnly = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setStencilLoadOp(wgpu::LoadOp value) & {
    this->stencilLoadOp = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setStencilLoadOp(wgpu::LoadOp value) && {
    this->stencilLoadOp = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setStencilStoreOp(wgpu::StoreOp value) & {
    this->stencilStoreOp = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setStencilStoreOp(wgpu::StoreOp value) && {
    this->stencilStoreOp = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setStencilClearValue(uint32_t value) & {
    this->stencilClearValue = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setStencilClearValue(uint32_t value) && {
    this->stencilClearValue = value;
    return std::move(*this);
}
RenderPassDepthStencilAttachment& RenderPassDepthStencilAttachment::setStencilReadOnly(wgpu::Bool value) & {
    this->stencilReadOnly = value;
    return *this;
}
RenderPassDepthStencilAttachment&& RenderPassDepthStencilAttachment::setStencilReadOnly(wgpu::Bool value) && {
    this->stencilReadOnly = value;
    return std::move(*this);
}
RenderPassMaxDrawCount::RenderPassMaxDrawCount(const WGPURenderPassMaxDrawCount& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->maxDrawCount = static_cast<uint64_t>(native.maxDrawCount);
}
RenderPassMaxDrawCount::CStruct RenderPassMaxDrawCount::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.maxDrawCount = static_cast<uint64_t>(this->maxDrawCount);
    return cstruct;
}
RenderPassMaxDrawCount& RenderPassMaxDrawCount::setMaxDrawCount(uint64_t value) & {
    this->maxDrawCount = value;
    return *this;
}
RenderPassMaxDrawCount&& RenderPassMaxDrawCount::setMaxDrawCount(uint64_t value) && {
    this->maxDrawCount = value;
    return std::move(*this);
}
RequestAdapterWebXROptions::RequestAdapterWebXROptions(const WGPURequestAdapterWebXROptions& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->xrCompatible = static_cast<wgpu::Bool>(native.xrCompatible);
}
RequestAdapterWebXROptions::CStruct RequestAdapterWebXROptions::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.xrCompatible = static_cast<WGPUBool>(this->xrCompatible);
    return cstruct;
}
RequestAdapterWebXROptions& RequestAdapterWebXROptions::setXrCompatible(wgpu::Bool value) & {
    this->xrCompatible = value;
    return *this;
}
RequestAdapterWebXROptions&& RequestAdapterWebXROptions::setXrCompatible(wgpu::Bool value) && {
    this->xrCompatible = value;
    return std::move(*this);
}
SamplerBindingLayout::SamplerBindingLayout(const WGPUSamplerBindingLayout& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->type = static_cast<wgpu::SamplerBindingType>(native.type);
}
SamplerBindingLayout::CStruct SamplerBindingLayout::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.type = static_cast<WGPUSamplerBindingType>(this->type);
    return cstruct;
}
SamplerBindingLayout& SamplerBindingLayout::setType(wgpu::SamplerBindingType value) & {
    this->type = value;
    return *this;
}
SamplerBindingLayout&& SamplerBindingLayout::setType(wgpu::SamplerBindingType value) && {
    this->type = value;
    return std::move(*this);
}
SamplerDescriptor::SamplerDescriptor(const WGPUSamplerDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->addressModeU = static_cast<wgpu::AddressMode>(native.addressModeU);
    this->addressModeV = static_cast<wgpu::AddressMode>(native.addressModeV);
    this->addressModeW = static_cast<wgpu::AddressMode>(native.addressModeW);
    this->magFilter = static_cast<wgpu::FilterMode>(native.magFilter);
    this->minFilter = static_cast<wgpu::FilterMode>(native.minFilter);
    this->mipmapFilter = static_cast<wgpu::MipmapFilterMode>(native.mipmapFilter);
    this->lodMinClamp = static_cast<float>(native.lodMinClamp);
    this->lodMaxClamp = static_cast<float>(native.lodMaxClamp);
    this->compare = static_cast<wgpu::CompareFunction>(native.compare);
    this->maxAnisotropy = static_cast<uint16_t>(native.maxAnisotropy);
}
SamplerDescriptor::CStruct SamplerDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.addressModeU = static_cast<WGPUAddressMode>(this->addressModeU);
    cstruct.addressModeV = static_cast<WGPUAddressMode>(this->addressModeV);
    cstruct.addressModeW = static_cast<WGPUAddressMode>(this->addressModeW);
    cstruct.magFilter = static_cast<WGPUFilterMode>(this->magFilter);
    cstruct.minFilter = static_cast<WGPUFilterMode>(this->minFilter);
    cstruct.mipmapFilter = static_cast<WGPUMipmapFilterMode>(this->mipmapFilter);
    cstruct.lodMinClamp = static_cast<float>(this->lodMinClamp);
    cstruct.lodMaxClamp = static_cast<float>(this->lodMaxClamp);
    cstruct.compare = static_cast<WGPUCompareFunction>(this->compare);
    cstruct.maxAnisotropy = static_cast<uint16_t>(this->maxAnisotropy);
    return cstruct;
}
SamplerDescriptor& SamplerDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setAddressModeU(wgpu::AddressMode value) & {
    this->addressModeU = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setAddressModeU(wgpu::AddressMode value) && {
    this->addressModeU = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setAddressModeV(wgpu::AddressMode value) & {
    this->addressModeV = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setAddressModeV(wgpu::AddressMode value) && {
    this->addressModeV = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setAddressModeW(wgpu::AddressMode value) & {
    this->addressModeW = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setAddressModeW(wgpu::AddressMode value) && {
    this->addressModeW = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setMagFilter(wgpu::FilterMode value) & {
    this->magFilter = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setMagFilter(wgpu::FilterMode value) && {
    this->magFilter = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setMinFilter(wgpu::FilterMode value) & {
    this->minFilter = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setMinFilter(wgpu::FilterMode value) && {
    this->minFilter = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setMipmapFilter(wgpu::MipmapFilterMode value) & {
    this->mipmapFilter = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setMipmapFilter(wgpu::MipmapFilterMode value) && {
    this->mipmapFilter = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setLodMinClamp(float value) & {
    this->lodMinClamp = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setLodMinClamp(float value) && {
    this->lodMinClamp = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setLodMaxClamp(float value) & {
    this->lodMaxClamp = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setLodMaxClamp(float value) && {
    this->lodMaxClamp = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setCompare(wgpu::CompareFunction value) & {
    this->compare = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setCompare(wgpu::CompareFunction value) && {
    this->compare = value;
    return std::move(*this);
}
SamplerDescriptor& SamplerDescriptor::setMaxAnisotropy(uint16_t value) & {
    this->maxAnisotropy = value;
    return *this;
}
SamplerDescriptor&& SamplerDescriptor::setMaxAnisotropy(uint16_t value) && {
    this->maxAnisotropy = value;
    return std::move(*this);
}
ShaderSourceSPIRV::ShaderSourceSPIRV(const WGPUShaderSourceSPIRV& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->codeSize = static_cast<uint32_t>(native.codeSize);
    this->code = static_cast<uint32_t const*>(native.code);
}
ShaderSourceSPIRV::CStruct ShaderSourceSPIRV::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.codeSize = static_cast<uint32_t>(this->codeSize);
    cstruct.code = static_cast<const uint32_t*>(this->code);
    return cstruct;
}
ShaderSourceSPIRV& ShaderSourceSPIRV::setCodeSize(uint32_t value) & {
    this->codeSize = value;
    return *this;
}
ShaderSourceSPIRV&& ShaderSourceSPIRV::setCodeSize(uint32_t value) && {
    this->codeSize = value;
    return std::move(*this);
}
ShaderSourceSPIRV& ShaderSourceSPIRV::setCode(uint32_t const* value) & {
    this->code = value;
    return *this;
}
ShaderSourceSPIRV&& ShaderSourceSPIRV::setCode(uint32_t const* value) && {
    this->code = value;
    return std::move(*this);
}
ShaderSourceWGSL::ShaderSourceWGSL(const WGPUShaderSourceWGSL& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->code = static_cast<wgpu::StringView>(native.code);
}
ShaderSourceWGSL::CStruct ShaderSourceWGSL::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.code = static_cast<WGPUStringView>(this->code.to_cstruct());
    return cstruct;
}
ShaderSourceWGSL& ShaderSourceWGSL::setCode(const wgpu::StringView& value) & {
    this->code = value;
    return *this;
}
ShaderSourceWGSL&& ShaderSourceWGSL::setCode(const wgpu::StringView& value) && {
    this->code = value;
    return std::move(*this);
}
ShaderSourceWGSL& ShaderSourceWGSL::setCode(wgpu::StringView&& value) & {
    this->code = std::move(value);
    return *this;
}
ShaderSourceWGSL&& ShaderSourceWGSL::setCode(wgpu::StringView&& value) && {
    this->code = std::move(value);
    return std::move(*this);
}
StencilFaceState::StencilFaceState(const WGPUStencilFaceState& native) {
    this->compare = static_cast<wgpu::CompareFunction>(native.compare);
    this->failOp = static_cast<wgpu::StencilOperation>(native.failOp);
    this->depthFailOp = static_cast<wgpu::StencilOperation>(native.depthFailOp);
    this->passOp = static_cast<wgpu::StencilOperation>(native.passOp);
}
StencilFaceState::CStruct StencilFaceState::to_cstruct() const {
    CStruct cstruct;
    cstruct.compare = static_cast<WGPUCompareFunction>(this->compare);
    cstruct.failOp = static_cast<WGPUStencilOperation>(this->failOp);
    cstruct.depthFailOp = static_cast<WGPUStencilOperation>(this->depthFailOp);
    cstruct.passOp = static_cast<WGPUStencilOperation>(this->passOp);
    return cstruct;
}
StencilFaceState& StencilFaceState::setCompare(wgpu::CompareFunction value) & {
    this->compare = value;
    return *this;
}
StencilFaceState&& StencilFaceState::setCompare(wgpu::CompareFunction value) && {
    this->compare = value;
    return std::move(*this);
}
StencilFaceState& StencilFaceState::setFailOp(wgpu::StencilOperation value) & {
    this->failOp = value;
    return *this;
}
StencilFaceState&& StencilFaceState::setFailOp(wgpu::StencilOperation value) && {
    this->failOp = value;
    return std::move(*this);
}
StencilFaceState& StencilFaceState::setDepthFailOp(wgpu::StencilOperation value) & {
    this->depthFailOp = value;
    return *this;
}
StencilFaceState&& StencilFaceState::setDepthFailOp(wgpu::StencilOperation value) && {
    this->depthFailOp = value;
    return std::move(*this);
}
StencilFaceState& StencilFaceState::setPassOp(wgpu::StencilOperation value) & {
    this->passOp = value;
    return *this;
}
StencilFaceState&& StencilFaceState::setPassOp(wgpu::StencilOperation value) && {
    this->passOp = value;
    return std::move(*this);
}
StorageTextureBindingLayout::StorageTextureBindingLayout(const WGPUStorageTextureBindingLayout& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->access = static_cast<wgpu::StorageTextureAccess>(native.access);
    this->format = static_cast<wgpu::TextureFormat>(native.format);
    this->viewDimension = static_cast<wgpu::TextureViewDimension>(native.viewDimension);
}
StorageTextureBindingLayout::CStruct StorageTextureBindingLayout::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.access = static_cast<WGPUStorageTextureAccess>(this->access);
    cstruct.format = static_cast<WGPUTextureFormat>(this->format);
    cstruct.viewDimension = static_cast<WGPUTextureViewDimension>(this->viewDimension);
    return cstruct;
}
StorageTextureBindingLayout& StorageTextureBindingLayout::setAccess(wgpu::StorageTextureAccess value) & {
    this->access = value;
    return *this;
}
StorageTextureBindingLayout&& StorageTextureBindingLayout::setAccess(wgpu::StorageTextureAccess value) && {
    this->access = value;
    return std::move(*this);
}
StorageTextureBindingLayout& StorageTextureBindingLayout::setFormat(wgpu::TextureFormat value) & {
    this->format = value;
    return *this;
}
StorageTextureBindingLayout&& StorageTextureBindingLayout::setFormat(wgpu::TextureFormat value) && {
    this->format = value;
    return std::move(*this);
}
StorageTextureBindingLayout& StorageTextureBindingLayout::setViewDimension(wgpu::TextureViewDimension value) & {
    this->viewDimension = value;
    return *this;
}
StorageTextureBindingLayout&& StorageTextureBindingLayout::setViewDimension(wgpu::TextureViewDimension value) && {
    this->viewDimension = value;
    return std::move(*this);
}
SupportedFeatures::SupportedFeatures(const WGPUSupportedFeatures& native) {
    this->features = std::span(native.features, native.featureCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::FeatureName>(e); }) | std::ranges::to<std::vector<wgpu::FeatureName>>();
}
SupportedFeatures::CStruct SupportedFeatures::to_cstruct() const {
    CStruct cstruct;
    cstruct.features = reinterpret_cast<const WGPUFeatureName*>(this->features.data());
    cstruct.featureCount = static_cast<size_t>(this->features.size());
    return cstruct;
}
SupportedInstanceFeatures::SupportedInstanceFeatures(const WGPUSupportedInstanceFeatures& native) {
    this->features = std::span(native.features, native.featureCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::InstanceFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::InstanceFeatureName>>();
}
SupportedInstanceFeatures::CStruct SupportedInstanceFeatures::to_cstruct() const {
    CStruct cstruct;
    cstruct.features = reinterpret_cast<const WGPUInstanceFeatureName*>(this->features.data());
    cstruct.featureCount = static_cast<size_t>(this->features.size());
    return cstruct;
}
SupportedWGSLLanguageFeatures::SupportedWGSLLanguageFeatures(const WGPUSupportedWGSLLanguageFeatures& native) {
    this->features = std::span(native.features, native.featureCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::WGSLLanguageFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::WGSLLanguageFeatureName>>();
}
SupportedWGSLLanguageFeatures::CStruct SupportedWGSLLanguageFeatures::to_cstruct() const {
    CStruct cstruct;
    cstruct.features = reinterpret_cast<const WGPUWGSLLanguageFeatureName*>(this->features.data());
    cstruct.featureCount = static_cast<size_t>(this->features.size());
    return cstruct;
}
SurfaceCapabilities::SurfaceCapabilities(const WGPUSurfaceCapabilities& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->usages = static_cast<wgpu::TextureUsage>(native.usages);
    this->formats = std::span(native.formats, native.formatCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    this->presentModes = std::span(native.presentModes, native.presentModeCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::PresentMode>(e); }) | std::ranges::to<std::vector<wgpu::PresentMode>>();
    this->alphaModes = std::span(native.alphaModes, native.alphaModeCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::CompositeAlphaMode>(e); }) | std::ranges::to<std::vector<wgpu::CompositeAlphaMode>>();
}
SurfaceCapabilities::CStruct SurfaceCapabilities::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.usages = static_cast<WGPUTextureUsage>(this->usages);
    cstruct.formats = reinterpret_cast<const WGPUTextureFormat*>(this->formats.data());
    cstruct.formatCount = static_cast<size_t>(this->formats.size());
    cstruct.presentModes = reinterpret_cast<const WGPUPresentMode*>(this->presentModes.data());
    cstruct.presentModeCount = static_cast<size_t>(this->presentModes.size());
    cstruct.alphaModes = reinterpret_cast<const WGPUCompositeAlphaMode*>(this->alphaModes.data());
    cstruct.alphaModeCount = static_cast<size_t>(this->alphaModes.size());
    return cstruct;
}
SurfaceCapabilities& SurfaceCapabilities::setUsages(wgpu::TextureUsage value) & {
    this->usages = value;
    return *this;
}
SurfaceCapabilities&& SurfaceCapabilities::setUsages(wgpu::TextureUsage value) && {
    this->usages = value;
    return std::move(*this);
}
SurfaceColorManagement::SurfaceColorManagement(const WGPUSurfaceColorManagement& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->colorSpace = static_cast<wgpu::PredefinedColorSpace>(native.colorSpace);
    this->toneMappingMode = static_cast<wgpu::ToneMappingMode>(native.toneMappingMode);
}
SurfaceColorManagement::CStruct SurfaceColorManagement::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.colorSpace = static_cast<WGPUPredefinedColorSpace>(this->colorSpace);
    cstruct.toneMappingMode = static_cast<WGPUToneMappingMode>(this->toneMappingMode);
    return cstruct;
}
SurfaceColorManagement& SurfaceColorManagement::setColorSpace(wgpu::PredefinedColorSpace value) & {
    this->colorSpace = value;
    return *this;
}
SurfaceColorManagement&& SurfaceColorManagement::setColorSpace(wgpu::PredefinedColorSpace value) && {
    this->colorSpace = value;
    return std::move(*this);
}
SurfaceColorManagement& SurfaceColorManagement::setToneMappingMode(wgpu::ToneMappingMode value) & {
    this->toneMappingMode = value;
    return *this;
}
SurfaceColorManagement&& SurfaceColorManagement::setToneMappingMode(wgpu::ToneMappingMode value) && {
    this->toneMappingMode = value;
    return std::move(*this);
}
SurfaceConfiguration::SurfaceConfiguration(const WGPUSurfaceConfiguration& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->device = static_cast<wgpu::raw::Device>(native.device);
    this->format = static_cast<wgpu::TextureFormat>(native.format);
    this->usage = static_cast<wgpu::TextureUsage>(native.usage);
    this->width = static_cast<uint32_t>(native.width);
    this->height = static_cast<uint32_t>(native.height);
    this->viewFormats = std::span(native.viewFormats, native.viewFormatCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
    this->alphaMode = static_cast<wgpu::CompositeAlphaMode>(native.alphaMode);
    this->presentMode = static_cast<wgpu::PresentMode>(native.presentMode);
}
SurfaceConfiguration::CStruct SurfaceConfiguration::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.device = static_cast<WGPUDevice>(this->device);
    cstruct.format = static_cast<WGPUTextureFormat>(this->format);
    cstruct.usage = static_cast<WGPUTextureUsage>(this->usage);
    cstruct.width = static_cast<uint32_t>(this->width);
    cstruct.height = static_cast<uint32_t>(this->height);
    cstruct.viewFormats = reinterpret_cast<const WGPUTextureFormat*>(this->viewFormats.data());
    cstruct.viewFormatCount = static_cast<size_t>(this->viewFormats.size());
    cstruct.alphaMode = static_cast<WGPUCompositeAlphaMode>(this->alphaMode);
    cstruct.presentMode = static_cast<WGPUPresentMode>(this->presentMode);
    return cstruct;
}
SurfaceConfiguration& SurfaceConfiguration::setDevice(wgpu::raw::Device value) & {
    this->device = value;
    return *this;
}
SurfaceConfiguration&& SurfaceConfiguration::setDevice(wgpu::raw::Device value) && {
    this->device = value;
    return std::move(*this);
}
SurfaceConfiguration& SurfaceConfiguration::setFormat(wgpu::TextureFormat value) & {
    this->format = value;
    return *this;
}
SurfaceConfiguration&& SurfaceConfiguration::setFormat(wgpu::TextureFormat value) && {
    this->format = value;
    return std::move(*this);
}
SurfaceConfiguration& SurfaceConfiguration::setUsage(wgpu::TextureUsage value) & {
    this->usage = value;
    return *this;
}
SurfaceConfiguration&& SurfaceConfiguration::setUsage(wgpu::TextureUsage value) && {
    this->usage = value;
    return std::move(*this);
}
SurfaceConfiguration& SurfaceConfiguration::setWidth(uint32_t value) & {
    this->width = value;
    return *this;
}
SurfaceConfiguration&& SurfaceConfiguration::setWidth(uint32_t value) && {
    this->width = value;
    return std::move(*this);
}
SurfaceConfiguration& SurfaceConfiguration::setHeight(uint32_t value) & {
    this->height = value;
    return *this;
}
SurfaceConfiguration&& SurfaceConfiguration::setHeight(uint32_t value) && {
    this->height = value;
    return std::move(*this);
}
SurfaceConfiguration& SurfaceConfiguration::setAlphaMode(wgpu::CompositeAlphaMode value) & {
    this->alphaMode = value;
    return *this;
}
SurfaceConfiguration&& SurfaceConfiguration::setAlphaMode(wgpu::CompositeAlphaMode value) && {
    this->alphaMode = value;
    return std::move(*this);
}
SurfaceConfiguration& SurfaceConfiguration::setPresentMode(wgpu::PresentMode value) & {
    this->presentMode = value;
    return *this;
}
SurfaceConfiguration&& SurfaceConfiguration::setPresentMode(wgpu::PresentMode value) && {
    this->presentMode = value;
    return std::move(*this);
}
SurfaceSourceAndroidNativeWindow::SurfaceSourceAndroidNativeWindow(const WGPUSurfaceSourceAndroidNativeWindow& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->window = static_cast<void*>(native.window);
}
SurfaceSourceAndroidNativeWindow::CStruct SurfaceSourceAndroidNativeWindow::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.window = static_cast<void*>(this->window);
    return cstruct;
}
SurfaceSourceAndroidNativeWindow& SurfaceSourceAndroidNativeWindow::setWindow(void* value) & {
    this->window = value;
    return *this;
}
SurfaceSourceAndroidNativeWindow&& SurfaceSourceAndroidNativeWindow::setWindow(void* value) && {
    this->window = value;
    return std::move(*this);
}
SurfaceSourceMetalLayer::SurfaceSourceMetalLayer(const WGPUSurfaceSourceMetalLayer& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->layer = static_cast<void*>(native.layer);
}
SurfaceSourceMetalLayer::CStruct SurfaceSourceMetalLayer::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.layer = static_cast<void*>(this->layer);
    return cstruct;
}
SurfaceSourceMetalLayer& SurfaceSourceMetalLayer::setLayer(void* value) & {
    this->layer = value;
    return *this;
}
SurfaceSourceMetalLayer&& SurfaceSourceMetalLayer::setLayer(void* value) && {
    this->layer = value;
    return std::move(*this);
}
SurfaceSourceWaylandSurface::SurfaceSourceWaylandSurface(const WGPUSurfaceSourceWaylandSurface& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->display = static_cast<void*>(native.display);
    this->surface = static_cast<void*>(native.surface);
}
SurfaceSourceWaylandSurface::CStruct SurfaceSourceWaylandSurface::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.display = static_cast<void*>(this->display);
    cstruct.surface = static_cast<void*>(this->surface);
    return cstruct;
}
SurfaceSourceWaylandSurface& SurfaceSourceWaylandSurface::setDisplay(void* value) & {
    this->display = value;
    return *this;
}
SurfaceSourceWaylandSurface&& SurfaceSourceWaylandSurface::setDisplay(void* value) && {
    this->display = value;
    return std::move(*this);
}
SurfaceSourceWaylandSurface& SurfaceSourceWaylandSurface::setSurface(void* value) & {
    this->surface = value;
    return *this;
}
SurfaceSourceWaylandSurface&& SurfaceSourceWaylandSurface::setSurface(void* value) && {
    this->surface = value;
    return std::move(*this);
}
SurfaceSourceWindowsHWND::SurfaceSourceWindowsHWND(const WGPUSurfaceSourceWindowsHWND& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->hinstance = static_cast<void*>(native.hinstance);
    this->hwnd = static_cast<void*>(native.hwnd);
}
SurfaceSourceWindowsHWND::CStruct SurfaceSourceWindowsHWND::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.hinstance = static_cast<void*>(this->hinstance);
    cstruct.hwnd = static_cast<void*>(this->hwnd);
    return cstruct;
}
SurfaceSourceWindowsHWND& SurfaceSourceWindowsHWND::setHinstance(void* value) & {
    this->hinstance = value;
    return *this;
}
SurfaceSourceWindowsHWND&& SurfaceSourceWindowsHWND::setHinstance(void* value) && {
    this->hinstance = value;
    return std::move(*this);
}
SurfaceSourceWindowsHWND& SurfaceSourceWindowsHWND::setHwnd(void* value) & {
    this->hwnd = value;
    return *this;
}
SurfaceSourceWindowsHWND&& SurfaceSourceWindowsHWND::setHwnd(void* value) && {
    this->hwnd = value;
    return std::move(*this);
}
SurfaceSourceXCBWindow::SurfaceSourceXCBWindow(const WGPUSurfaceSourceXCBWindow& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->connection = static_cast<void*>(native.connection);
    this->window = static_cast<uint32_t>(native.window);
}
SurfaceSourceXCBWindow::CStruct SurfaceSourceXCBWindow::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.connection = static_cast<void*>(this->connection);
    cstruct.window = static_cast<uint32_t>(this->window);
    return cstruct;
}
SurfaceSourceXCBWindow& SurfaceSourceXCBWindow::setConnection(void* value) & {
    this->connection = value;
    return *this;
}
SurfaceSourceXCBWindow&& SurfaceSourceXCBWindow::setConnection(void* value) && {
    this->connection = value;
    return std::move(*this);
}
SurfaceSourceXCBWindow& SurfaceSourceXCBWindow::setWindow(uint32_t value) & {
    this->window = value;
    return *this;
}
SurfaceSourceXCBWindow&& SurfaceSourceXCBWindow::setWindow(uint32_t value) && {
    this->window = value;
    return std::move(*this);
}
SurfaceSourceXlibWindow::SurfaceSourceXlibWindow(const WGPUSurfaceSourceXlibWindow& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->display = static_cast<void*>(native.display);
    this->window = static_cast<uint64_t>(native.window);
}
SurfaceSourceXlibWindow::CStruct SurfaceSourceXlibWindow::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.display = static_cast<void*>(this->display);
    cstruct.window = static_cast<uint64_t>(this->window);
    return cstruct;
}
SurfaceSourceXlibWindow& SurfaceSourceXlibWindow::setDisplay(void* value) & {
    this->display = value;
    return *this;
}
SurfaceSourceXlibWindow&& SurfaceSourceXlibWindow::setDisplay(void* value) && {
    this->display = value;
    return std::move(*this);
}
SurfaceSourceXlibWindow& SurfaceSourceXlibWindow::setWindow(uint64_t value) & {
    this->window = value;
    return *this;
}
SurfaceSourceXlibWindow&& SurfaceSourceXlibWindow::setWindow(uint64_t value) && {
    this->window = value;
    return std::move(*this);
}
SurfaceTexture::SurfaceTexture(const WGPUSurfaceTexture& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->texture = static_cast<wgpu::Texture>(native.texture);
    this->status = static_cast<wgpu::SurfaceGetCurrentTextureStatus>(native.status);
}
SurfaceTexture::CStruct SurfaceTexture::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.texture = static_cast<WGPUTexture>(this->texture);
    cstruct.status = static_cast<WGPUSurfaceGetCurrentTextureStatus>(this->status);
    return cstruct;
}
SurfaceTexture& SurfaceTexture::setTexture(wgpu::Texture value) & {
    this->texture = value;
    return *this;
}
SurfaceTexture&& SurfaceTexture::setTexture(wgpu::Texture value) && {
    this->texture = value;
    return std::move(*this);
}
SurfaceTexture& SurfaceTexture::setStatus(wgpu::SurfaceGetCurrentTextureStatus value) & {
    this->status = value;
    return *this;
}
SurfaceTexture&& SurfaceTexture::setStatus(wgpu::SurfaceGetCurrentTextureStatus value) && {
    this->status = value;
    return std::move(*this);
}
TexelCopyBufferLayout::TexelCopyBufferLayout(const WGPUTexelCopyBufferLayout& native) {
    this->offset = static_cast<uint64_t>(native.offset);
    this->bytesPerRow = static_cast<uint32_t>(native.bytesPerRow);
    this->rowsPerImage = static_cast<uint32_t>(native.rowsPerImage);
}
TexelCopyBufferLayout::CStruct TexelCopyBufferLayout::to_cstruct() const {
    CStruct cstruct;
    cstruct.offset = static_cast<uint64_t>(this->offset);
    cstruct.bytesPerRow = static_cast<uint32_t>(this->bytesPerRow);
    cstruct.rowsPerImage = static_cast<uint32_t>(this->rowsPerImage);
    return cstruct;
}
TexelCopyBufferLayout& TexelCopyBufferLayout::setOffset(uint64_t value) & {
    this->offset = value;
    return *this;
}
TexelCopyBufferLayout&& TexelCopyBufferLayout::setOffset(uint64_t value) && {
    this->offset = value;
    return std::move(*this);
}
TexelCopyBufferLayout& TexelCopyBufferLayout::setBytesPerRow(uint32_t value) & {
    this->bytesPerRow = value;
    return *this;
}
TexelCopyBufferLayout&& TexelCopyBufferLayout::setBytesPerRow(uint32_t value) && {
    this->bytesPerRow = value;
    return std::move(*this);
}
TexelCopyBufferLayout& TexelCopyBufferLayout::setRowsPerImage(uint32_t value) & {
    this->rowsPerImage = value;
    return *this;
}
TexelCopyBufferLayout&& TexelCopyBufferLayout::setRowsPerImage(uint32_t value) && {
    this->rowsPerImage = value;
    return std::move(*this);
}
TextureBindingLayout::TextureBindingLayout(const WGPUTextureBindingLayout& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->sampleType = static_cast<wgpu::TextureSampleType>(native.sampleType);
    this->viewDimension = static_cast<wgpu::TextureViewDimension>(native.viewDimension);
    this->multisampled = static_cast<wgpu::Bool>(native.multisampled);
}
TextureBindingLayout::CStruct TextureBindingLayout::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.sampleType = static_cast<WGPUTextureSampleType>(this->sampleType);
    cstruct.viewDimension = static_cast<WGPUTextureViewDimension>(this->viewDimension);
    cstruct.multisampled = static_cast<WGPUBool>(this->multisampled);
    return cstruct;
}
TextureBindingLayout& TextureBindingLayout::setSampleType(wgpu::TextureSampleType value) & {
    this->sampleType = value;
    return *this;
}
TextureBindingLayout&& TextureBindingLayout::setSampleType(wgpu::TextureSampleType value) && {
    this->sampleType = value;
    return std::move(*this);
}
TextureBindingLayout& TextureBindingLayout::setViewDimension(wgpu::TextureViewDimension value) & {
    this->viewDimension = value;
    return *this;
}
TextureBindingLayout&& TextureBindingLayout::setViewDimension(wgpu::TextureViewDimension value) && {
    this->viewDimension = value;
    return std::move(*this);
}
TextureBindingLayout& TextureBindingLayout::setMultisampled(wgpu::Bool value) & {
    this->multisampled = value;
    return *this;
}
TextureBindingLayout&& TextureBindingLayout::setMultisampled(wgpu::Bool value) && {
    this->multisampled = value;
    return std::move(*this);
}
TextureComponentSwizzle::TextureComponentSwizzle(const WGPUTextureComponentSwizzle& native) {
    this->r = static_cast<wgpu::ComponentSwizzle>(native.r);
    this->g = static_cast<wgpu::ComponentSwizzle>(native.g);
    this->b = static_cast<wgpu::ComponentSwizzle>(native.b);
    this->a = static_cast<wgpu::ComponentSwizzle>(native.a);
}
TextureComponentSwizzle::CStruct TextureComponentSwizzle::to_cstruct() const {
    CStruct cstruct;
    cstruct.r = static_cast<WGPUComponentSwizzle>(this->r);
    cstruct.g = static_cast<WGPUComponentSwizzle>(this->g);
    cstruct.b = static_cast<WGPUComponentSwizzle>(this->b);
    cstruct.a = static_cast<WGPUComponentSwizzle>(this->a);
    return cstruct;
}
TextureComponentSwizzle& TextureComponentSwizzle::setR(wgpu::ComponentSwizzle value) & {
    this->r = value;
    return *this;
}
TextureComponentSwizzle&& TextureComponentSwizzle::setR(wgpu::ComponentSwizzle value) && {
    this->r = value;
    return std::move(*this);
}
TextureComponentSwizzle& TextureComponentSwizzle::setG(wgpu::ComponentSwizzle value) & {
    this->g = value;
    return *this;
}
TextureComponentSwizzle&& TextureComponentSwizzle::setG(wgpu::ComponentSwizzle value) && {
    this->g = value;
    return std::move(*this);
}
TextureComponentSwizzle& TextureComponentSwizzle::setB(wgpu::ComponentSwizzle value) & {
    this->b = value;
    return *this;
}
TextureComponentSwizzle&& TextureComponentSwizzle::setB(wgpu::ComponentSwizzle value) && {
    this->b = value;
    return std::move(*this);
}
TextureComponentSwizzle& TextureComponentSwizzle::setA(wgpu::ComponentSwizzle value) & {
    this->a = value;
    return *this;
}
TextureComponentSwizzle&& TextureComponentSwizzle::setA(wgpu::ComponentSwizzle value) && {
    this->a = value;
    return std::move(*this);
}
TextureViewDescriptor::TextureViewDescriptor(const WGPUTextureViewDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->format = static_cast<wgpu::TextureFormat>(native.format);
    this->dimension = static_cast<wgpu::TextureViewDimension>(native.dimension);
    this->baseMipLevel = static_cast<uint32_t>(native.baseMipLevel);
    this->mipLevelCount = static_cast<uint32_t>(native.mipLevelCount);
    this->baseArrayLayer = static_cast<uint32_t>(native.baseArrayLayer);
    this->arrayLayerCount = static_cast<uint32_t>(native.arrayLayerCount);
    this->aspect = static_cast<wgpu::TextureAspect>(native.aspect);
    this->usage = static_cast<wgpu::TextureUsage>(native.usage);
}
TextureViewDescriptor::CStruct TextureViewDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.format = static_cast<WGPUTextureFormat>(this->format);
    cstruct.dimension = static_cast<WGPUTextureViewDimension>(this->dimension);
    cstruct.baseMipLevel = static_cast<uint32_t>(this->baseMipLevel);
    cstruct.mipLevelCount = static_cast<uint32_t>(this->mipLevelCount);
    cstruct.baseArrayLayer = static_cast<uint32_t>(this->baseArrayLayer);
    cstruct.arrayLayerCount = static_cast<uint32_t>(this->arrayLayerCount);
    cstruct.aspect = static_cast<WGPUTextureAspect>(this->aspect);
    cstruct.usage = static_cast<WGPUTextureUsage>(this->usage);
    return cstruct;
}
TextureViewDescriptor& TextureViewDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setFormat(wgpu::TextureFormat value) & {
    this->format = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setFormat(wgpu::TextureFormat value) && {
    this->format = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setDimension(wgpu::TextureViewDimension value) & {
    this->dimension = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setDimension(wgpu::TextureViewDimension value) && {
    this->dimension = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setBaseMipLevel(uint32_t value) & {
    this->baseMipLevel = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setBaseMipLevel(uint32_t value) && {
    this->baseMipLevel = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setMipLevelCount(uint32_t value) & {
    this->mipLevelCount = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setMipLevelCount(uint32_t value) && {
    this->mipLevelCount = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setBaseArrayLayer(uint32_t value) & {
    this->baseArrayLayer = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setBaseArrayLayer(uint32_t value) && {
    this->baseArrayLayer = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setArrayLayerCount(uint32_t value) & {
    this->arrayLayerCount = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setArrayLayerCount(uint32_t value) && {
    this->arrayLayerCount = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setAspect(wgpu::TextureAspect value) & {
    this->aspect = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setAspect(wgpu::TextureAspect value) && {
    this->aspect = value;
    return std::move(*this);
}
TextureViewDescriptor& TextureViewDescriptor::setUsage(wgpu::TextureUsage value) & {
    this->usage = value;
    return *this;
}
TextureViewDescriptor&& TextureViewDescriptor::setUsage(wgpu::TextureUsage value) && {
    this->usage = value;
    return std::move(*this);
}
VertexAttribute::VertexAttribute(const WGPUVertexAttribute& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->format = static_cast<wgpu::VertexFormat>(native.format);
    this->offset = static_cast<uint64_t>(native.offset);
    this->shaderLocation = static_cast<uint32_t>(native.shaderLocation);
}
VertexAttribute::CStruct VertexAttribute::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.format = static_cast<WGPUVertexFormat>(this->format);
    cstruct.offset = static_cast<uint64_t>(this->offset);
    cstruct.shaderLocation = static_cast<uint32_t>(this->shaderLocation);
    return cstruct;
}
VertexAttribute& VertexAttribute::setFormat(wgpu::VertexFormat value) & {
    this->format = value;
    return *this;
}
VertexAttribute&& VertexAttribute::setFormat(wgpu::VertexFormat value) && {
    this->format = value;
    return std::move(*this);
}
VertexAttribute& VertexAttribute::setOffset(uint64_t value) & {
    this->offset = value;
    return *this;
}
VertexAttribute&& VertexAttribute::setOffset(uint64_t value) && {
    this->offset = value;
    return std::move(*this);
}
VertexAttribute& VertexAttribute::setShaderLocation(uint32_t value) & {
    this->shaderLocation = value;
    return *this;
}
VertexAttribute&& VertexAttribute::setShaderLocation(uint32_t value) && {
    this->shaderLocation = value;
    return std::move(*this);
}
BindGroupEntry::BindGroupEntry(const WGPUBindGroupEntry& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->binding = static_cast<uint32_t>(native.binding);
    this->buffer = static_cast<wgpu::raw::Buffer>(native.buffer);
    this->offset = static_cast<uint64_t>(native.offset);
    this->size = static_cast<uint64_t>(native.size);
    this->sampler = static_cast<wgpu::raw::Sampler>(native.sampler);
    this->textureView = static_cast<wgpu::raw::TextureView>(native.textureView);
}
BindGroupEntry::CStruct BindGroupEntry::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.binding = static_cast<uint32_t>(this->binding);
    cstruct.buffer = static_cast<WGPUBuffer>(this->buffer);
    cstruct.offset = static_cast<uint64_t>(this->offset);
    cstruct.size = static_cast<uint64_t>(this->size);
    cstruct.sampler = static_cast<WGPUSampler>(this->sampler);
    cstruct.textureView = static_cast<WGPUTextureView>(this->textureView);
    return cstruct;
}
BindGroupEntry& BindGroupEntry::setBinding(uint32_t value) & {
    this->binding = value;
    return *this;
}
BindGroupEntry&& BindGroupEntry::setBinding(uint32_t value) && {
    this->binding = value;
    return std::move(*this);
}
BindGroupEntry& BindGroupEntry::setBuffer(wgpu::raw::Buffer value) & {
    this->buffer = value;
    return *this;
}
BindGroupEntry&& BindGroupEntry::setBuffer(wgpu::raw::Buffer value) && {
    this->buffer = value;
    return std::move(*this);
}
BindGroupEntry& BindGroupEntry::setOffset(uint64_t value) & {
    this->offset = value;
    return *this;
}
BindGroupEntry&& BindGroupEntry::setOffset(uint64_t value) && {
    this->offset = value;
    return std::move(*this);
}
BindGroupEntry& BindGroupEntry::setSize(uint64_t value) & {
    this->size = value;
    return *this;
}
BindGroupEntry&& BindGroupEntry::setSize(uint64_t value) && {
    this->size = value;
    return std::move(*this);
}
BindGroupEntry& BindGroupEntry::setSampler(wgpu::raw::Sampler value) & {
    this->sampler = value;
    return *this;
}
BindGroupEntry&& BindGroupEntry::setSampler(wgpu::raw::Sampler value) && {
    this->sampler = value;
    return std::move(*this);
}
BindGroupEntry& BindGroupEntry::setTextureView(wgpu::raw::TextureView value) & {
    this->textureView = value;
    return *this;
}
BindGroupEntry&& BindGroupEntry::setTextureView(wgpu::raw::TextureView value) && {
    this->textureView = value;
    return std::move(*this);
}
BindGroupLayoutEntry::BindGroupLayoutEntry(const WGPUBindGroupLayoutEntry& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->binding = static_cast<uint32_t>(native.binding);
    this->visibility = static_cast<wgpu::ShaderStage>(native.visibility);
    this->bindingArraySize = static_cast<uint32_t>(native.bindingArraySize);
    this->buffer = static_cast<wgpu::BufferBindingLayout>(native.buffer);
    this->sampler = static_cast<wgpu::SamplerBindingLayout>(native.sampler);
    this->texture = static_cast<wgpu::TextureBindingLayout>(native.texture);
    this->storageTexture = static_cast<wgpu::StorageTextureBindingLayout>(native.storageTexture);
}
BindGroupLayoutEntry::CStruct BindGroupLayoutEntry::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.binding = static_cast<uint32_t>(this->binding);
    cstruct.visibility = static_cast<WGPUShaderStage>(this->visibility);
    cstruct.bindingArraySize = static_cast<uint32_t>(this->bindingArraySize);
    cstruct.buffer = static_cast<WGPUBufferBindingLayout>(this->buffer.to_cstruct());
    cstruct.sampler = static_cast<WGPUSamplerBindingLayout>(this->sampler.to_cstruct());
    cstruct.texture = static_cast<WGPUTextureBindingLayout>(this->texture.to_cstruct());
    cstruct.storageTexture = static_cast<WGPUStorageTextureBindingLayout>(this->storageTexture.to_cstruct());
    return cstruct;
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setBinding(uint32_t value) & {
    this->binding = value;
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setBinding(uint32_t value) && {
    this->binding = value;
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setVisibility(wgpu::ShaderStage value) & {
    this->visibility = value;
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setVisibility(wgpu::ShaderStage value) && {
    this->visibility = value;
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setBindingArraySize(uint32_t value) & {
    this->bindingArraySize = value;
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setBindingArraySize(uint32_t value) && {
    this->bindingArraySize = value;
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setBuffer(const wgpu::BufferBindingLayout& value) & {
    this->buffer = value;
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setBuffer(const wgpu::BufferBindingLayout& value) && {
    this->buffer = value;
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setBuffer(wgpu::BufferBindingLayout&& value) & {
    this->buffer = std::move(value);
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setBuffer(wgpu::BufferBindingLayout&& value) && {
    this->buffer = std::move(value);
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setSampler(const wgpu::SamplerBindingLayout& value) & {
    this->sampler = value;
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setSampler(const wgpu::SamplerBindingLayout& value) && {
    this->sampler = value;
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setSampler(wgpu::SamplerBindingLayout&& value) & {
    this->sampler = std::move(value);
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setSampler(wgpu::SamplerBindingLayout&& value) && {
    this->sampler = std::move(value);
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setTexture(const wgpu::TextureBindingLayout& value) & {
    this->texture = value;
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setTexture(const wgpu::TextureBindingLayout& value) && {
    this->texture = value;
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setTexture(wgpu::TextureBindingLayout&& value) & {
    this->texture = std::move(value);
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setTexture(wgpu::TextureBindingLayout&& value) && {
    this->texture = std::move(value);
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setStorageTexture(const wgpu::StorageTextureBindingLayout& value) & {
    this->storageTexture = value;
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setStorageTexture(const wgpu::StorageTextureBindingLayout& value) && {
    this->storageTexture = value;
    return std::move(*this);
}
BindGroupLayoutEntry& BindGroupLayoutEntry::setStorageTexture(wgpu::StorageTextureBindingLayout&& value) & {
    this->storageTexture = std::move(value);
    return *this;
}
BindGroupLayoutEntry&& BindGroupLayoutEntry::setStorageTexture(wgpu::StorageTextureBindingLayout&& value) && {
    this->storageTexture = std::move(value);
    return std::move(*this);
}
BlendState::BlendState(const WGPUBlendState& native) {
    this->color = static_cast<wgpu::BlendComponent>(native.color);
    this->alpha = static_cast<wgpu::BlendComponent>(native.alpha);
}
BlendState::CStruct BlendState::to_cstruct() const {
    CStruct cstruct;
    cstruct.color = static_cast<WGPUBlendComponent>(this->color.to_cstruct());
    cstruct.alpha = static_cast<WGPUBlendComponent>(this->alpha.to_cstruct());
    return cstruct;
}
BlendState& BlendState::setColor(const wgpu::BlendComponent& value) & {
    this->color = value;
    return *this;
}
BlendState&& BlendState::setColor(const wgpu::BlendComponent& value) && {
    this->color = value;
    return std::move(*this);
}
BlendState& BlendState::setColor(wgpu::BlendComponent&& value) & {
    this->color = std::move(value);
    return *this;
}
BlendState&& BlendState::setColor(wgpu::BlendComponent&& value) && {
    this->color = std::move(value);
    return std::move(*this);
}
BlendState& BlendState::setAlpha(const wgpu::BlendComponent& value) & {
    this->alpha = value;
    return *this;
}
BlendState&& BlendState::setAlpha(const wgpu::BlendComponent& value) && {
    this->alpha = value;
    return std::move(*this);
}
BlendState& BlendState::setAlpha(wgpu::BlendComponent&& value) & {
    this->alpha = std::move(value);
    return *this;
}
BlendState&& BlendState::setAlpha(wgpu::BlendComponent&& value) && {
    this->alpha = std::move(value);
    return std::move(*this);
}
CompilationInfo::CompilationInfo(const WGPUCompilationInfo& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->messages = std::span(native.messages, native.messageCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::CompilationMessage>(e); }) | std::ranges::to<std::vector<wgpu::CompilationMessage>>();
}
CompilationInfo::CStruct CompilationInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.messages_vec = this->messages | std::views::transform([](auto&& e) { return static_cast<WGPUCompilationMessage>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUCompilationMessage>>();
    cstruct.messages = cstruct.messages_vec.data();
    cstruct.messageCount = static_cast<size_t>(cstruct.messages_vec.size());
    return cstruct;
}
ComputePassDescriptor::ComputePassDescriptor(const WGPUComputePassDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    if (native.timestampWrites != nullptr) {
        this->timestampWrites = static_cast<wgpu::PassTimestampWrites>(*(native.timestampWrites));
    } else {
        this->timestampWrites.reset();
    }
}
ComputePassDescriptor::CStruct ComputePassDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    if (this->timestampWrites.has_value()) {
        cstruct.timestampWrites_cstruct = this->timestampWrites->to_cstruct();
        cstruct.timestampWrites = &(cstruct.timestampWrites_cstruct);
    } else {
        cstruct.timestampWrites = nullptr;
    }
    return cstruct;
}
ComputePassDescriptor& ComputePassDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
ComputePassDescriptor&& ComputePassDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
ComputePassDescriptor& ComputePassDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
ComputePassDescriptor&& ComputePassDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
ComputePassDescriptor& ComputePassDescriptor::setTimestampWrites(const wgpu::PassTimestampWrites& value) & {
    this->timestampWrites.emplace(value);
    return *this;
}
ComputePassDescriptor&& ComputePassDescriptor::setTimestampWrites(const wgpu::PassTimestampWrites& value) && {
    this->timestampWrites.emplace(value);
    return std::move(*this);
}
ComputePassDescriptor& ComputePassDescriptor::setTimestampWrites(wgpu::PassTimestampWrites&& value) & {
    this->timestampWrites.emplace(std::move(value));
    return *this;
}
ComputePassDescriptor&& ComputePassDescriptor::setTimestampWrites(wgpu::PassTimestampWrites&& value) && {
    this->timestampWrites.emplace(std::move(value));
    return std::move(*this);
}
ComputeState::ComputeState(const WGPUComputeState& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->module = static_cast<wgpu::raw::ShaderModule>(native.module);
    this->entryPoint = static_cast<wgpu::StringView>(native.entryPoint);
    this->constants = std::span(native.constants, native.constantCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
}
ComputeState::CStruct ComputeState::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.module = static_cast<WGPUShaderModule>(this->module);
    cstruct.entryPoint = static_cast<WGPUStringView>(this->entryPoint.to_cstruct());
    cstruct.constants_vec = this->constants | std::views::transform([](auto&& e) { return static_cast<WGPUConstantEntry>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUConstantEntry>>();
    cstruct.constants = cstruct.constants_vec.data();
    cstruct.constantCount = static_cast<size_t>(cstruct.constants_vec.size());
    return cstruct;
}
ComputeState& ComputeState::setModule(wgpu::raw::ShaderModule value) & {
    this->module = value;
    return *this;
}
ComputeState&& ComputeState::setModule(wgpu::raw::ShaderModule value) && {
    this->module = value;
    return std::move(*this);
}
ComputeState& ComputeState::setEntryPoint(const wgpu::StringView& value) & {
    this->entryPoint = value;
    return *this;
}
ComputeState&& ComputeState::setEntryPoint(const wgpu::StringView& value) && {
    this->entryPoint = value;
    return std::move(*this);
}
ComputeState& ComputeState::setEntryPoint(wgpu::StringView&& value) & {
    this->entryPoint = std::move(value);
    return *this;
}
ComputeState&& ComputeState::setEntryPoint(wgpu::StringView&& value) && {
    this->entryPoint = std::move(value);
    return std::move(*this);
}
DepthStencilState::DepthStencilState(const WGPUDepthStencilState& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->format = static_cast<wgpu::TextureFormat>(native.format);
    this->depthWriteEnabled = static_cast<wgpu::OptionalBool>(native.depthWriteEnabled);
    this->depthCompare = static_cast<wgpu::CompareFunction>(native.depthCompare);
    this->stencilFront = static_cast<wgpu::StencilFaceState>(native.stencilFront);
    this->stencilBack = static_cast<wgpu::StencilFaceState>(native.stencilBack);
    this->stencilReadMask = static_cast<uint32_t>(native.stencilReadMask);
    this->stencilWriteMask = static_cast<uint32_t>(native.stencilWriteMask);
    this->depthBias = static_cast<int32_t>(native.depthBias);
    this->depthBiasSlopeScale = static_cast<float>(native.depthBiasSlopeScale);
    this->depthBiasClamp = static_cast<float>(native.depthBiasClamp);
}
DepthStencilState::CStruct DepthStencilState::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.format = static_cast<WGPUTextureFormat>(this->format);
    cstruct.depthWriteEnabled = static_cast<WGPUOptionalBool>(this->depthWriteEnabled);
    cstruct.depthCompare = static_cast<WGPUCompareFunction>(this->depthCompare);
    cstruct.stencilFront = static_cast<WGPUStencilFaceState>(this->stencilFront.to_cstruct());
    cstruct.stencilBack = static_cast<WGPUStencilFaceState>(this->stencilBack.to_cstruct());
    cstruct.stencilReadMask = static_cast<uint32_t>(this->stencilReadMask);
    cstruct.stencilWriteMask = static_cast<uint32_t>(this->stencilWriteMask);
    cstruct.depthBias = static_cast<int32_t>(this->depthBias);
    cstruct.depthBiasSlopeScale = static_cast<float>(this->depthBiasSlopeScale);
    cstruct.depthBiasClamp = static_cast<float>(this->depthBiasClamp);
    return cstruct;
}
DepthStencilState& DepthStencilState::setFormat(wgpu::TextureFormat value) & {
    this->format = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setFormat(wgpu::TextureFormat value) && {
    this->format = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setDepthWriteEnabled(wgpu::OptionalBool value) & {
    this->depthWriteEnabled = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setDepthWriteEnabled(wgpu::OptionalBool value) && {
    this->depthWriteEnabled = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setDepthCompare(wgpu::CompareFunction value) & {
    this->depthCompare = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setDepthCompare(wgpu::CompareFunction value) && {
    this->depthCompare = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setStencilFront(const wgpu::StencilFaceState& value) & {
    this->stencilFront = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setStencilFront(const wgpu::StencilFaceState& value) && {
    this->stencilFront = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setStencilFront(wgpu::StencilFaceState&& value) & {
    this->stencilFront = std::move(value);
    return *this;
}
DepthStencilState&& DepthStencilState::setStencilFront(wgpu::StencilFaceState&& value) && {
    this->stencilFront = std::move(value);
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setStencilBack(const wgpu::StencilFaceState& value) & {
    this->stencilBack = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setStencilBack(const wgpu::StencilFaceState& value) && {
    this->stencilBack = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setStencilBack(wgpu::StencilFaceState&& value) & {
    this->stencilBack = std::move(value);
    return *this;
}
DepthStencilState&& DepthStencilState::setStencilBack(wgpu::StencilFaceState&& value) && {
    this->stencilBack = std::move(value);
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setStencilReadMask(uint32_t value) & {
    this->stencilReadMask = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setStencilReadMask(uint32_t value) && {
    this->stencilReadMask = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setStencilWriteMask(uint32_t value) & {
    this->stencilWriteMask = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setStencilWriteMask(uint32_t value) && {
    this->stencilWriteMask = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setDepthBias(int32_t value) & {
    this->depthBias = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setDepthBias(int32_t value) && {
    this->depthBias = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setDepthBiasSlopeScale(float value) & {
    this->depthBiasSlopeScale = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setDepthBiasSlopeScale(float value) && {
    this->depthBiasSlopeScale = value;
    return std::move(*this);
}
DepthStencilState& DepthStencilState::setDepthBiasClamp(float value) & {
    this->depthBiasClamp = value;
    return *this;
}
DepthStencilState&& DepthStencilState::setDepthBiasClamp(float value) && {
    this->depthBiasClamp = value;
    return std::move(*this);
}
DeviceDescriptor::DeviceDescriptor(const WGPUDeviceDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->requiredFeatures = std::span(native.requiredFeatures, native.requiredFeatureCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::FeatureName>(e); }) | std::ranges::to<std::vector<wgpu::FeatureName>>();
    if (native.requiredLimits != nullptr) {
        this->requiredLimits = static_cast<wgpu::Limits>(*(native.requiredLimits));
    } else {
        this->requiredLimits.reset();
    }
    this->defaultQueue = static_cast<wgpu::QueueDescriptor>(native.defaultQueue);
    this->deviceLostCallbackInfo = static_cast<wgpu::DeviceLostCallbackInfo>(native.deviceLostCallbackInfo);
    this->uncapturedErrorCallbackInfo = static_cast<wgpu::UncapturedErrorCallbackInfo>(native.uncapturedErrorCallbackInfo);
}
DeviceDescriptor::CStruct DeviceDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.requiredFeatures = reinterpret_cast<const WGPUFeatureName*>(this->requiredFeatures.data());
    cstruct.requiredFeatureCount = static_cast<size_t>(this->requiredFeatures.size());
    if (this->requiredLimits.has_value()) {
        cstruct.requiredLimits_cstruct = this->requiredLimits->to_cstruct();
        cstruct.requiredLimits = &(cstruct.requiredLimits_cstruct);
    } else {
        cstruct.requiredLimits = nullptr;
    }
    cstruct.defaultQueue = static_cast<WGPUQueueDescriptor>(this->defaultQueue.to_cstruct());
    cstruct.deviceLostCallbackInfo = static_cast<WGPUDeviceLostCallbackInfo>(this->deviceLostCallbackInfo.to_cstruct());
    cstruct.uncapturedErrorCallbackInfo = static_cast<WGPUUncapturedErrorCallbackInfo>(this->uncapturedErrorCallbackInfo.to_cstruct());
    return cstruct;
}
DeviceDescriptor& DeviceDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setRequiredLimits(const wgpu::Limits& value) & {
    this->requiredLimits.emplace(value);
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setRequiredLimits(const wgpu::Limits& value) && {
    this->requiredLimits.emplace(value);
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setRequiredLimits(wgpu::Limits&& value) & {
    this->requiredLimits.emplace(std::move(value));
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setRequiredLimits(wgpu::Limits&& value) && {
    this->requiredLimits.emplace(std::move(value));
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setDefaultQueue(const wgpu::QueueDescriptor& value) & {
    this->defaultQueue = value;
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setDefaultQueue(const wgpu::QueueDescriptor& value) && {
    this->defaultQueue = value;
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setDefaultQueue(wgpu::QueueDescriptor&& value) & {
    this->defaultQueue = std::move(value);
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setDefaultQueue(wgpu::QueueDescriptor&& value) && {
    this->defaultQueue = std::move(value);
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setDeviceLostCallbackInfo(const wgpu::DeviceLostCallbackInfo& value) & {
    this->deviceLostCallbackInfo = value;
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setDeviceLostCallbackInfo(const wgpu::DeviceLostCallbackInfo& value) && {
    this->deviceLostCallbackInfo = value;
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setDeviceLostCallbackInfo(wgpu::DeviceLostCallbackInfo&& value) & {
    this->deviceLostCallbackInfo = std::move(value);
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setDeviceLostCallbackInfo(wgpu::DeviceLostCallbackInfo&& value) && {
    this->deviceLostCallbackInfo = std::move(value);
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setUncapturedErrorCallbackInfo(const wgpu::UncapturedErrorCallbackInfo& value) & {
    this->uncapturedErrorCallbackInfo = value;
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setUncapturedErrorCallbackInfo(const wgpu::UncapturedErrorCallbackInfo& value) && {
    this->uncapturedErrorCallbackInfo = value;
    return std::move(*this);
}
DeviceDescriptor& DeviceDescriptor::setUncapturedErrorCallbackInfo(wgpu::UncapturedErrorCallbackInfo&& value) & {
    this->uncapturedErrorCallbackInfo = std::move(value);
    return *this;
}
DeviceDescriptor&& DeviceDescriptor::setUncapturedErrorCallbackInfo(wgpu::UncapturedErrorCallbackInfo&& value) && {
    this->uncapturedErrorCallbackInfo = std::move(value);
    return std::move(*this);
}
FutureWaitInfo::FutureWaitInfo(const WGPUFutureWaitInfo& native) {
    this->future = static_cast<wgpu::Future>(native.future);
    this->completed = static_cast<wgpu::Bool>(native.completed);
}
FutureWaitInfo::CStruct FutureWaitInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.future = static_cast<WGPUFuture>(this->future.to_cstruct());
    cstruct.completed = static_cast<WGPUBool>(this->completed);
    return cstruct;
}
FutureWaitInfo& FutureWaitInfo::setFuture(const wgpu::Future& value) & {
    this->future = value;
    return *this;
}
FutureWaitInfo&& FutureWaitInfo::setFuture(const wgpu::Future& value) && {
    this->future = value;
    return std::move(*this);
}
FutureWaitInfo& FutureWaitInfo::setFuture(wgpu::Future&& value) & {
    this->future = std::move(value);
    return *this;
}
FutureWaitInfo&& FutureWaitInfo::setFuture(wgpu::Future&& value) && {
    this->future = std::move(value);
    return std::move(*this);
}
FutureWaitInfo& FutureWaitInfo::setCompleted(wgpu::Bool value) & {
    this->completed = value;
    return *this;
}
FutureWaitInfo&& FutureWaitInfo::setCompleted(wgpu::Bool value) && {
    this->completed = value;
    return std::move(*this);
}
InstanceDescriptor::InstanceDescriptor(const WGPUInstanceDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->requiredFeatures = std::span(native.requiredFeatures, native.requiredFeatureCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::InstanceFeatureName>(e); }) | std::ranges::to<std::vector<wgpu::InstanceFeatureName>>();
    if (native.requiredLimits != nullptr) {
        this->requiredLimits = static_cast<wgpu::InstanceLimits>(*(native.requiredLimits));
    } else {
        this->requiredLimits.reset();
    }
}
InstanceDescriptor::CStruct InstanceDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.requiredFeatures = reinterpret_cast<const WGPUInstanceFeatureName*>(this->requiredFeatures.data());
    cstruct.requiredFeatureCount = static_cast<size_t>(this->requiredFeatures.size());
    if (this->requiredLimits.has_value()) {
        cstruct.requiredLimits_cstruct = this->requiredLimits->to_cstruct();
        cstruct.requiredLimits = &(cstruct.requiredLimits_cstruct);
    } else {
        cstruct.requiredLimits = nullptr;
    }
    return cstruct;
}
InstanceDescriptor& InstanceDescriptor::setRequiredLimits(const wgpu::InstanceLimits& value) & {
    this->requiredLimits.emplace(value);
    return *this;
}
InstanceDescriptor&& InstanceDescriptor::setRequiredLimits(const wgpu::InstanceLimits& value) && {
    this->requiredLimits.emplace(value);
    return std::move(*this);
}
InstanceDescriptor& InstanceDescriptor::setRequiredLimits(wgpu::InstanceLimits&& value) & {
    this->requiredLimits.emplace(std::move(value));
    return *this;
}
InstanceDescriptor&& InstanceDescriptor::setRequiredLimits(wgpu::InstanceLimits&& value) && {
    this->requiredLimits.emplace(std::move(value));
    return std::move(*this);
}
RenderPassColorAttachment::RenderPassColorAttachment(const WGPURenderPassColorAttachment& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->view = static_cast<wgpu::raw::TextureView>(native.view);
    this->depthSlice = static_cast<uint32_t>(native.depthSlice);
    this->resolveTarget = static_cast<wgpu::raw::TextureView>(native.resolveTarget);
    this->loadOp = static_cast<wgpu::LoadOp>(native.loadOp);
    this->storeOp = static_cast<wgpu::StoreOp>(native.storeOp);
    this->clearValue = static_cast<wgpu::Color>(native.clearValue);
}
RenderPassColorAttachment::CStruct RenderPassColorAttachment::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.view = static_cast<WGPUTextureView>(this->view);
    cstruct.depthSlice = static_cast<uint32_t>(this->depthSlice);
    cstruct.resolveTarget = static_cast<WGPUTextureView>(this->resolveTarget);
    cstruct.loadOp = static_cast<WGPULoadOp>(this->loadOp);
    cstruct.storeOp = static_cast<WGPUStoreOp>(this->storeOp);
    cstruct.clearValue = static_cast<WGPUColor>(this->clearValue.to_cstruct());
    return cstruct;
}
RenderPassColorAttachment& RenderPassColorAttachment::setView(wgpu::raw::TextureView value) & {
    this->view = value;
    return *this;
}
RenderPassColorAttachment&& RenderPassColorAttachment::setView(wgpu::raw::TextureView value) && {
    this->view = value;
    return std::move(*this);
}
RenderPassColorAttachment& RenderPassColorAttachment::setDepthSlice(uint32_t value) & {
    this->depthSlice = value;
    return *this;
}
RenderPassColorAttachment&& RenderPassColorAttachment::setDepthSlice(uint32_t value) && {
    this->depthSlice = value;
    return std::move(*this);
}
RenderPassColorAttachment& RenderPassColorAttachment::setResolveTarget(wgpu::raw::TextureView value) & {
    this->resolveTarget = value;
    return *this;
}
RenderPassColorAttachment&& RenderPassColorAttachment::setResolveTarget(wgpu::raw::TextureView value) && {
    this->resolveTarget = value;
    return std::move(*this);
}
RenderPassColorAttachment& RenderPassColorAttachment::setLoadOp(wgpu::LoadOp value) & {
    this->loadOp = value;
    return *this;
}
RenderPassColorAttachment&& RenderPassColorAttachment::setLoadOp(wgpu::LoadOp value) && {
    this->loadOp = value;
    return std::move(*this);
}
RenderPassColorAttachment& RenderPassColorAttachment::setStoreOp(wgpu::StoreOp value) & {
    this->storeOp = value;
    return *this;
}
RenderPassColorAttachment&& RenderPassColorAttachment::setStoreOp(wgpu::StoreOp value) && {
    this->storeOp = value;
    return std::move(*this);
}
RenderPassColorAttachment& RenderPassColorAttachment::setClearValue(const wgpu::Color& value) & {
    this->clearValue = value;
    return *this;
}
RenderPassColorAttachment&& RenderPassColorAttachment::setClearValue(const wgpu::Color& value) && {
    this->clearValue = value;
    return std::move(*this);
}
RenderPassColorAttachment& RenderPassColorAttachment::setClearValue(wgpu::Color&& value) & {
    this->clearValue = std::move(value);
    return *this;
}
RenderPassColorAttachment&& RenderPassColorAttachment::setClearValue(wgpu::Color&& value) && {
    this->clearValue = std::move(value);
    return std::move(*this);
}
RequestAdapterOptions::RequestAdapterOptions(const WGPURequestAdapterOptions& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->featureLevel = static_cast<wgpu::FeatureLevel>(native.featureLevel);
    this->powerPreference = static_cast<wgpu::PowerPreference>(native.powerPreference);
    this->forceFallbackAdapter = static_cast<wgpu::Bool>(native.forceFallbackAdapter);
    this->backendType = static_cast<wgpu::BackendType>(native.backendType);
    this->compatibleSurface = static_cast<wgpu::raw::Surface>(native.compatibleSurface);
}
RequestAdapterOptions::CStruct RequestAdapterOptions::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.featureLevel = static_cast<WGPUFeatureLevel>(this->featureLevel);
    cstruct.powerPreference = static_cast<WGPUPowerPreference>(this->powerPreference);
    cstruct.forceFallbackAdapter = static_cast<WGPUBool>(this->forceFallbackAdapter);
    cstruct.backendType = static_cast<WGPUBackendType>(this->backendType);
    cstruct.compatibleSurface = static_cast<WGPUSurface>(this->compatibleSurface);
    return cstruct;
}
RequestAdapterOptions& RequestAdapterOptions::setFeatureLevel(wgpu::FeatureLevel value) & {
    this->featureLevel = value;
    return *this;
}
RequestAdapterOptions&& RequestAdapterOptions::setFeatureLevel(wgpu::FeatureLevel value) && {
    this->featureLevel = value;
    return std::move(*this);
}
RequestAdapterOptions& RequestAdapterOptions::setPowerPreference(wgpu::PowerPreference value) & {
    this->powerPreference = value;
    return *this;
}
RequestAdapterOptions&& RequestAdapterOptions::setPowerPreference(wgpu::PowerPreference value) && {
    this->powerPreference = value;
    return std::move(*this);
}
RequestAdapterOptions& RequestAdapterOptions::setForceFallbackAdapter(wgpu::Bool value) & {
    this->forceFallbackAdapter = value;
    return *this;
}
RequestAdapterOptions&& RequestAdapterOptions::setForceFallbackAdapter(wgpu::Bool value) && {
    this->forceFallbackAdapter = value;
    return std::move(*this);
}
RequestAdapterOptions& RequestAdapterOptions::setBackendType(wgpu::BackendType value) & {
    this->backendType = value;
    return *this;
}
RequestAdapterOptions&& RequestAdapterOptions::setBackendType(wgpu::BackendType value) && {
    this->backendType = value;
    return std::move(*this);
}
RequestAdapterOptions& RequestAdapterOptions::setCompatibleSurface(wgpu::raw::Surface value) & {
    this->compatibleSurface = value;
    return *this;
}
RequestAdapterOptions&& RequestAdapterOptions::setCompatibleSurface(wgpu::raw::Surface value) && {
    this->compatibleSurface = value;
    return std::move(*this);
}
ShaderModuleDescriptor::ShaderModuleDescriptor(const WGPUShaderModuleDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
}
ShaderModuleDescriptor::CStruct ShaderModuleDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    return cstruct;
}
ShaderModuleDescriptor& ShaderModuleDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
ShaderModuleDescriptor&& ShaderModuleDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
ShaderModuleDescriptor& ShaderModuleDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
ShaderModuleDescriptor&& ShaderModuleDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
SurfaceDescriptor::SurfaceDescriptor(const WGPUSurfaceDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
}
SurfaceDescriptor::CStruct SurfaceDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    return cstruct;
}
SurfaceDescriptor& SurfaceDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
SurfaceDescriptor&& SurfaceDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
SurfaceDescriptor& SurfaceDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
SurfaceDescriptor&& SurfaceDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
TexelCopyBufferInfo::TexelCopyBufferInfo(const WGPUTexelCopyBufferInfo& native) {
    this->layout = static_cast<wgpu::TexelCopyBufferLayout>(native.layout);
    this->buffer = static_cast<wgpu::raw::Buffer>(native.buffer);
}
TexelCopyBufferInfo::CStruct TexelCopyBufferInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.layout = static_cast<WGPUTexelCopyBufferLayout>(this->layout.to_cstruct());
    cstruct.buffer = static_cast<WGPUBuffer>(this->buffer);
    return cstruct;
}
TexelCopyBufferInfo& TexelCopyBufferInfo::setLayout(const wgpu::TexelCopyBufferLayout& value) & {
    this->layout = value;
    return *this;
}
TexelCopyBufferInfo&& TexelCopyBufferInfo::setLayout(const wgpu::TexelCopyBufferLayout& value) && {
    this->layout = value;
    return std::move(*this);
}
TexelCopyBufferInfo& TexelCopyBufferInfo::setLayout(wgpu::TexelCopyBufferLayout&& value) & {
    this->layout = std::move(value);
    return *this;
}
TexelCopyBufferInfo&& TexelCopyBufferInfo::setLayout(wgpu::TexelCopyBufferLayout&& value) && {
    this->layout = std::move(value);
    return std::move(*this);
}
TexelCopyBufferInfo& TexelCopyBufferInfo::setBuffer(wgpu::raw::Buffer value) & {
    this->buffer = value;
    return *this;
}
TexelCopyBufferInfo&& TexelCopyBufferInfo::setBuffer(wgpu::raw::Buffer value) && {
    this->buffer = value;
    return std::move(*this);
}
TexelCopyTextureInfo::TexelCopyTextureInfo(const WGPUTexelCopyTextureInfo& native) {
    this->texture = static_cast<wgpu::raw::Texture>(native.texture);
    this->mipLevel = static_cast<uint32_t>(native.mipLevel);
    this->origin = static_cast<wgpu::Origin3D>(native.origin);
    this->aspect = static_cast<wgpu::TextureAspect>(native.aspect);
}
TexelCopyTextureInfo::CStruct TexelCopyTextureInfo::to_cstruct() const {
    CStruct cstruct;
    cstruct.texture = static_cast<WGPUTexture>(this->texture);
    cstruct.mipLevel = static_cast<uint32_t>(this->mipLevel);
    cstruct.origin = static_cast<WGPUOrigin3D>(this->origin.to_cstruct());
    cstruct.aspect = static_cast<WGPUTextureAspect>(this->aspect);
    return cstruct;
}
TexelCopyTextureInfo& TexelCopyTextureInfo::setTexture(wgpu::raw::Texture value) & {
    this->texture = value;
    return *this;
}
TexelCopyTextureInfo&& TexelCopyTextureInfo::setTexture(wgpu::raw::Texture value) && {
    this->texture = value;
    return std::move(*this);
}
TexelCopyTextureInfo& TexelCopyTextureInfo::setMipLevel(uint32_t value) & {
    this->mipLevel = value;
    return *this;
}
TexelCopyTextureInfo&& TexelCopyTextureInfo::setMipLevel(uint32_t value) && {
    this->mipLevel = value;
    return std::move(*this);
}
TexelCopyTextureInfo& TexelCopyTextureInfo::setOrigin(const wgpu::Origin3D& value) & {
    this->origin = value;
    return *this;
}
TexelCopyTextureInfo&& TexelCopyTextureInfo::setOrigin(const wgpu::Origin3D& value) && {
    this->origin = value;
    return std::move(*this);
}
TexelCopyTextureInfo& TexelCopyTextureInfo::setOrigin(wgpu::Origin3D&& value) & {
    this->origin = std::move(value);
    return *this;
}
TexelCopyTextureInfo&& TexelCopyTextureInfo::setOrigin(wgpu::Origin3D&& value) && {
    this->origin = std::move(value);
    return std::move(*this);
}
TexelCopyTextureInfo& TexelCopyTextureInfo::setAspect(wgpu::TextureAspect value) & {
    this->aspect = value;
    return *this;
}
TexelCopyTextureInfo&& TexelCopyTextureInfo::setAspect(wgpu::TextureAspect value) && {
    this->aspect = value;
    return std::move(*this);
}
TextureComponentSwizzleDescriptor::TextureComponentSwizzleDescriptor(const WGPUTextureComponentSwizzleDescriptor& native) {
    this->chain = static_cast<wgpu::ChainedStruct>(native.chain);
    this->swizzle = static_cast<wgpu::TextureComponentSwizzle>(native.swizzle);
}
TextureComponentSwizzleDescriptor::CStruct TextureComponentSwizzleDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.chain = static_cast<WGPUChainedStruct>(this->chain.to_cstruct());
    cstruct.swizzle = static_cast<WGPUTextureComponentSwizzle>(this->swizzle.to_cstruct());
    return cstruct;
}
TextureComponentSwizzleDescriptor& TextureComponentSwizzleDescriptor::setSwizzle(const wgpu::TextureComponentSwizzle& value) & {
    this->swizzle = value;
    return *this;
}
TextureComponentSwizzleDescriptor&& TextureComponentSwizzleDescriptor::setSwizzle(const wgpu::TextureComponentSwizzle& value) && {
    this->swizzle = value;
    return std::move(*this);
}
TextureComponentSwizzleDescriptor& TextureComponentSwizzleDescriptor::setSwizzle(wgpu::TextureComponentSwizzle&& value) & {
    this->swizzle = std::move(value);
    return *this;
}
TextureComponentSwizzleDescriptor&& TextureComponentSwizzleDescriptor::setSwizzle(wgpu::TextureComponentSwizzle&& value) && {
    this->swizzle = std::move(value);
    return std::move(*this);
}
TextureDescriptor::TextureDescriptor(const WGPUTextureDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->usage = static_cast<wgpu::TextureUsage>(native.usage);
    this->dimension = static_cast<wgpu::TextureDimension>(native.dimension);
    this->size = static_cast<wgpu::Extent3D>(native.size);
    this->format = static_cast<wgpu::TextureFormat>(native.format);
    this->mipLevelCount = static_cast<uint32_t>(native.mipLevelCount);
    this->sampleCount = static_cast<uint32_t>(native.sampleCount);
    this->viewFormats = std::span(native.viewFormats, native.viewFormatCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::TextureFormat>(e); }) | std::ranges::to<std::vector<wgpu::TextureFormat>>();
}
TextureDescriptor::CStruct TextureDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.usage = static_cast<WGPUTextureUsage>(this->usage);
    cstruct.dimension = static_cast<WGPUTextureDimension>(this->dimension);
    cstruct.size = static_cast<WGPUExtent3D>(this->size.to_cstruct());
    cstruct.format = static_cast<WGPUTextureFormat>(this->format);
    cstruct.mipLevelCount = static_cast<uint32_t>(this->mipLevelCount);
    cstruct.sampleCount = static_cast<uint32_t>(this->sampleCount);
    cstruct.viewFormats = reinterpret_cast<const WGPUTextureFormat*>(this->viewFormats.data());
    cstruct.viewFormatCount = static_cast<size_t>(this->viewFormats.size());
    return cstruct;
}
TextureDescriptor& TextureDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
TextureDescriptor&& TextureDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
TextureDescriptor&& TextureDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setUsage(wgpu::TextureUsage value) & {
    this->usage = value;
    return *this;
}
TextureDescriptor&& TextureDescriptor::setUsage(wgpu::TextureUsage value) && {
    this->usage = value;
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setDimension(wgpu::TextureDimension value) & {
    this->dimension = value;
    return *this;
}
TextureDescriptor&& TextureDescriptor::setDimension(wgpu::TextureDimension value) && {
    this->dimension = value;
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setSize(const wgpu::Extent3D& value) & {
    this->size = value;
    return *this;
}
TextureDescriptor&& TextureDescriptor::setSize(const wgpu::Extent3D& value) && {
    this->size = value;
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setSize(wgpu::Extent3D&& value) & {
    this->size = std::move(value);
    return *this;
}
TextureDescriptor&& TextureDescriptor::setSize(wgpu::Extent3D&& value) && {
    this->size = std::move(value);
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setFormat(wgpu::TextureFormat value) & {
    this->format = value;
    return *this;
}
TextureDescriptor&& TextureDescriptor::setFormat(wgpu::TextureFormat value) && {
    this->format = value;
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setMipLevelCount(uint32_t value) & {
    this->mipLevelCount = value;
    return *this;
}
TextureDescriptor&& TextureDescriptor::setMipLevelCount(uint32_t value) && {
    this->mipLevelCount = value;
    return std::move(*this);
}
TextureDescriptor& TextureDescriptor::setSampleCount(uint32_t value) & {
    this->sampleCount = value;
    return *this;
}
TextureDescriptor&& TextureDescriptor::setSampleCount(uint32_t value) && {
    this->sampleCount = value;
    return std::move(*this);
}
VertexBufferLayout::VertexBufferLayout(const WGPUVertexBufferLayout& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->stepMode = static_cast<wgpu::VertexStepMode>(native.stepMode);
    this->arrayStride = static_cast<uint64_t>(native.arrayStride);
    this->attributes = std::span(native.attributes, native.attributeCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::VertexAttribute>(e); }) | std::ranges::to<std::vector<wgpu::VertexAttribute>>();
}
VertexBufferLayout::CStruct VertexBufferLayout::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.stepMode = static_cast<WGPUVertexStepMode>(this->stepMode);
    cstruct.arrayStride = static_cast<uint64_t>(this->arrayStride);
    cstruct.attributes_vec = this->attributes | std::views::transform([](auto&& e) { return static_cast<WGPUVertexAttribute>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUVertexAttribute>>();
    cstruct.attributes = cstruct.attributes_vec.data();
    cstruct.attributeCount = static_cast<size_t>(cstruct.attributes_vec.size());
    return cstruct;
}
VertexBufferLayout& VertexBufferLayout::setStepMode(wgpu::VertexStepMode value) & {
    this->stepMode = value;
    return *this;
}
VertexBufferLayout&& VertexBufferLayout::setStepMode(wgpu::VertexStepMode value) && {
    this->stepMode = value;
    return std::move(*this);
}
VertexBufferLayout& VertexBufferLayout::setArrayStride(uint64_t value) & {
    this->arrayStride = value;
    return *this;
}
VertexBufferLayout&& VertexBufferLayout::setArrayStride(uint64_t value) && {
    this->arrayStride = value;
    return std::move(*this);
}
BindGroupDescriptor::BindGroupDescriptor(const WGPUBindGroupDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->layout = static_cast<wgpu::raw::BindGroupLayout>(native.layout);
    this->entries = std::span(native.entries, native.entryCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::BindGroupEntry>(e); }) | std::ranges::to<std::vector<wgpu::BindGroupEntry>>();
}
BindGroupDescriptor::CStruct BindGroupDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.layout = static_cast<WGPUBindGroupLayout>(this->layout);
    cstruct.entries_vec = this->entries | std::views::transform([](auto&& e) { return static_cast<WGPUBindGroupEntry>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUBindGroupEntry>>();
    cstruct.entries = cstruct.entries_vec.data();
    cstruct.entryCount = static_cast<size_t>(cstruct.entries_vec.size());
    return cstruct;
}
BindGroupDescriptor& BindGroupDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
BindGroupDescriptor&& BindGroupDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
BindGroupDescriptor& BindGroupDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
BindGroupDescriptor&& BindGroupDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
BindGroupDescriptor& BindGroupDescriptor::setLayout(wgpu::raw::BindGroupLayout value) & {
    this->layout = value;
    return *this;
}
BindGroupDescriptor&& BindGroupDescriptor::setLayout(wgpu::raw::BindGroupLayout value) && {
    this->layout = value;
    return std::move(*this);
}
BindGroupLayoutDescriptor::BindGroupLayoutDescriptor(const WGPUBindGroupLayoutDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->entries = std::span(native.entries, native.entryCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::BindGroupLayoutEntry>(e); }) | std::ranges::to<std::vector<wgpu::BindGroupLayoutEntry>>();
}
BindGroupLayoutDescriptor::CStruct BindGroupLayoutDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.entries_vec = this->entries | std::views::transform([](auto&& e) { return static_cast<WGPUBindGroupLayoutEntry>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUBindGroupLayoutEntry>>();
    cstruct.entries = cstruct.entries_vec.data();
    cstruct.entryCount = static_cast<size_t>(cstruct.entries_vec.size());
    return cstruct;
}
BindGroupLayoutDescriptor& BindGroupLayoutDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
BindGroupLayoutDescriptor&& BindGroupLayoutDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
BindGroupLayoutDescriptor& BindGroupLayoutDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
BindGroupLayoutDescriptor&& BindGroupLayoutDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
ColorTargetState::ColorTargetState(const WGPUColorTargetState& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->format = static_cast<wgpu::TextureFormat>(native.format);
    if (native.blend != nullptr) {
        this->blend = static_cast<wgpu::BlendState>(*(native.blend));
    } else {
        this->blend.reset();
    }
    this->writeMask = static_cast<wgpu::ColorWriteMask>(native.writeMask);
}
ColorTargetState::CStruct ColorTargetState::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.format = static_cast<WGPUTextureFormat>(this->format);
    if (this->blend.has_value()) {
        cstruct.blend = reinterpret_cast<const WGPUBlendState*>(&(*(this->blend)));
    } else {
        cstruct.blend = nullptr;
    }
    cstruct.writeMask = static_cast<WGPUColorWriteMask>(this->writeMask);
    return cstruct;
}
ColorTargetState& ColorTargetState::setFormat(wgpu::TextureFormat value) & {
    this->format = value;
    return *this;
}
ColorTargetState&& ColorTargetState::setFormat(wgpu::TextureFormat value) && {
    this->format = value;
    return std::move(*this);
}
ColorTargetState& ColorTargetState::setBlend(const wgpu::BlendState& value) & {
    this->blend.emplace(value);
    return *this;
}
ColorTargetState&& ColorTargetState::setBlend(const wgpu::BlendState& value) && {
    this->blend.emplace(value);
    return std::move(*this);
}
ColorTargetState& ColorTargetState::setBlend(wgpu::BlendState&& value) & {
    this->blend.emplace(std::move(value));
    return *this;
}
ColorTargetState&& ColorTargetState::setBlend(wgpu::BlendState&& value) && {
    this->blend.emplace(std::move(value));
    return std::move(*this);
}
ColorTargetState& ColorTargetState::setWriteMask(wgpu::ColorWriteMask value) & {
    this->writeMask = value;
    return *this;
}
ColorTargetState&& ColorTargetState::setWriteMask(wgpu::ColorWriteMask value) && {
    this->writeMask = value;
    return std::move(*this);
}
ComputePipelineDescriptor::ComputePipelineDescriptor(const WGPUComputePipelineDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->layout = static_cast<wgpu::raw::PipelineLayout>(native.layout);
    this->compute = static_cast<wgpu::ComputeState>(native.compute);
}
ComputePipelineDescriptor::CStruct ComputePipelineDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.layout = static_cast<WGPUPipelineLayout>(this->layout);
    cstruct.compute_cstruct = this->compute.to_cstruct();
    cstruct.compute = static_cast<WGPUComputeState>(cstruct.compute_cstruct);
    return cstruct;
}
ComputePipelineDescriptor& ComputePipelineDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
ComputePipelineDescriptor&& ComputePipelineDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
ComputePipelineDescriptor& ComputePipelineDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
ComputePipelineDescriptor&& ComputePipelineDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
ComputePipelineDescriptor& ComputePipelineDescriptor::setLayout(wgpu::raw::PipelineLayout value) & {
    this->layout = value;
    return *this;
}
ComputePipelineDescriptor&& ComputePipelineDescriptor::setLayout(wgpu::raw::PipelineLayout value) && {
    this->layout = value;
    return std::move(*this);
}
ComputePipelineDescriptor& ComputePipelineDescriptor::setCompute(const wgpu::ComputeState& value) & {
    this->compute = value;
    return *this;
}
ComputePipelineDescriptor&& ComputePipelineDescriptor::setCompute(const wgpu::ComputeState& value) && {
    this->compute = value;
    return std::move(*this);
}
ComputePipelineDescriptor& ComputePipelineDescriptor::setCompute(wgpu::ComputeState&& value) & {
    this->compute = std::move(value);
    return *this;
}
ComputePipelineDescriptor&& ComputePipelineDescriptor::setCompute(wgpu::ComputeState&& value) && {
    this->compute = std::move(value);
    return std::move(*this);
}
RenderPassDescriptor::RenderPassDescriptor(const WGPURenderPassDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->colorAttachments = std::span(native.colorAttachments, native.colorAttachmentCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::RenderPassColorAttachment>(e); }) | std::ranges::to<std::vector<wgpu::RenderPassColorAttachment>>();
    if (native.depthStencilAttachment != nullptr) {
        this->depthStencilAttachment = static_cast<wgpu::RenderPassDepthStencilAttachment>(*(native.depthStencilAttachment));
    } else {
        this->depthStencilAttachment.reset();
    }
    this->occlusionQuerySet = static_cast<wgpu::raw::QuerySet>(native.occlusionQuerySet);
    if (native.timestampWrites != nullptr) {
        this->timestampWrites = static_cast<wgpu::PassTimestampWrites>(*(native.timestampWrites));
    } else {
        this->timestampWrites.reset();
    }
}
RenderPassDescriptor::CStruct RenderPassDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.colorAttachments_vec = this->colorAttachments | std::views::transform([](auto&& e) { return static_cast<WGPURenderPassColorAttachment>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPURenderPassColorAttachment>>();
    cstruct.colorAttachments = cstruct.colorAttachments_vec.data();
    cstruct.colorAttachmentCount = static_cast<size_t>(cstruct.colorAttachments_vec.size());
    if (this->depthStencilAttachment.has_value()) {
        cstruct.depthStencilAttachment_cstruct = this->depthStencilAttachment->to_cstruct();
        cstruct.depthStencilAttachment = &(cstruct.depthStencilAttachment_cstruct);
    } else {
        cstruct.depthStencilAttachment = nullptr;
    }
    cstruct.occlusionQuerySet = static_cast<WGPUQuerySet>(this->occlusionQuerySet);
    if (this->timestampWrites.has_value()) {
        cstruct.timestampWrites_cstruct = this->timestampWrites->to_cstruct();
        cstruct.timestampWrites = &(cstruct.timestampWrites_cstruct);
    } else {
        cstruct.timestampWrites = nullptr;
    }
    return cstruct;
}
RenderPassDescriptor& RenderPassDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
RenderPassDescriptor&& RenderPassDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
RenderPassDescriptor& RenderPassDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
RenderPassDescriptor&& RenderPassDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
RenderPassDescriptor& RenderPassDescriptor::setDepthStencilAttachment(const wgpu::RenderPassDepthStencilAttachment& value) & {
    this->depthStencilAttachment.emplace(value);
    return *this;
}
RenderPassDescriptor&& RenderPassDescriptor::setDepthStencilAttachment(const wgpu::RenderPassDepthStencilAttachment& value) && {
    this->depthStencilAttachment.emplace(value);
    return std::move(*this);
}
RenderPassDescriptor& RenderPassDescriptor::setDepthStencilAttachment(wgpu::RenderPassDepthStencilAttachment&& value) & {
    this->depthStencilAttachment.emplace(std::move(value));
    return *this;
}
RenderPassDescriptor&& RenderPassDescriptor::setDepthStencilAttachment(wgpu::RenderPassDepthStencilAttachment&& value) && {
    this->depthStencilAttachment.emplace(std::move(value));
    return std::move(*this);
}
RenderPassDescriptor& RenderPassDescriptor::setOcclusionQuerySet(wgpu::raw::QuerySet value) & {
    this->occlusionQuerySet = value;
    return *this;
}
RenderPassDescriptor&& RenderPassDescriptor::setOcclusionQuerySet(wgpu::raw::QuerySet value) && {
    this->occlusionQuerySet = value;
    return std::move(*this);
}
RenderPassDescriptor& RenderPassDescriptor::setTimestampWrites(const wgpu::PassTimestampWrites& value) & {
    this->timestampWrites.emplace(value);
    return *this;
}
RenderPassDescriptor&& RenderPassDescriptor::setTimestampWrites(const wgpu::PassTimestampWrites& value) && {
    this->timestampWrites.emplace(value);
    return std::move(*this);
}
RenderPassDescriptor& RenderPassDescriptor::setTimestampWrites(wgpu::PassTimestampWrites&& value) & {
    this->timestampWrites.emplace(std::move(value));
    return *this;
}
RenderPassDescriptor&& RenderPassDescriptor::setTimestampWrites(wgpu::PassTimestampWrites&& value) && {
    this->timestampWrites.emplace(std::move(value));
    return std::move(*this);
}
VertexState::VertexState(const WGPUVertexState& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->module = static_cast<wgpu::raw::ShaderModule>(native.module);
    this->entryPoint = static_cast<wgpu::StringView>(native.entryPoint);
    this->constants = std::span(native.constants, native.constantCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    this->buffers = std::span(native.buffers, native.bufferCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::VertexBufferLayout>(e); }) | std::ranges::to<std::vector<wgpu::VertexBufferLayout>>();
}
VertexState::CStruct VertexState::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.module = static_cast<WGPUShaderModule>(this->module);
    cstruct.entryPoint = static_cast<WGPUStringView>(this->entryPoint.to_cstruct());
    cstruct.constants_vec = this->constants | std::views::transform([](auto&& e) { return static_cast<WGPUConstantEntry>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUConstantEntry>>();
    cstruct.constants = cstruct.constants_vec.data();
    cstruct.constantCount = static_cast<size_t>(cstruct.constants_vec.size());
    cstruct.buffers_cstruct_vec = this->buffers | std::views::transform([](auto&& e) { return e.to_cstruct(); }) | std::ranges::to<std::vector<wgpu::VertexBufferLayout::CStruct>>();
    cstruct.buffers_vec = cstruct.buffers_cstruct_vec | std::views::transform([](auto&& e) { return static_cast<WGPUVertexBufferLayout>(e); }) | std::ranges::to<std::vector<WGPUVertexBufferLayout>>();
    cstruct.buffers = cstruct.buffers_vec.data();
    cstruct.bufferCount = static_cast<size_t>(cstruct.buffers_vec.size());
    return cstruct;
}
VertexState& VertexState::setModule(wgpu::raw::ShaderModule value) & {
    this->module = value;
    return *this;
}
VertexState&& VertexState::setModule(wgpu::raw::ShaderModule value) && {
    this->module = value;
    return std::move(*this);
}
VertexState& VertexState::setEntryPoint(const wgpu::StringView& value) & {
    this->entryPoint = value;
    return *this;
}
VertexState&& VertexState::setEntryPoint(const wgpu::StringView& value) && {
    this->entryPoint = value;
    return std::move(*this);
}
VertexState& VertexState::setEntryPoint(wgpu::StringView&& value) & {
    this->entryPoint = std::move(value);
    return *this;
}
VertexState&& VertexState::setEntryPoint(wgpu::StringView&& value) && {
    this->entryPoint = std::move(value);
    return std::move(*this);
}
FragmentState::FragmentState(const WGPUFragmentState& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->module = static_cast<wgpu::raw::ShaderModule>(native.module);
    this->entryPoint = static_cast<wgpu::StringView>(native.entryPoint);
    this->constants = std::span(native.constants, native.constantCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::ConstantEntry>(e); }) | std::ranges::to<std::vector<wgpu::ConstantEntry>>();
    this->targets = std::span(native.targets, native.targetCount) | std::views::transform([](auto&& e) { return static_cast<wgpu::ColorTargetState>(e); }) | std::ranges::to<std::vector<wgpu::ColorTargetState>>();
}
FragmentState::CStruct FragmentState::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.module = static_cast<WGPUShaderModule>(this->module);
    cstruct.entryPoint = static_cast<WGPUStringView>(this->entryPoint.to_cstruct());
    cstruct.constants_vec = this->constants | std::views::transform([](auto&& e) { return static_cast<WGPUConstantEntry>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUConstantEntry>>();
    cstruct.constants = cstruct.constants_vec.data();
    cstruct.constantCount = static_cast<size_t>(cstruct.constants_vec.size());
    cstruct.targets_vec = this->targets | std::views::transform([](auto&& e) { return static_cast<WGPUColorTargetState>(e.to_cstruct()); }) | std::ranges::to<std::vector<WGPUColorTargetState>>();
    cstruct.targets = cstruct.targets_vec.data();
    cstruct.targetCount = static_cast<size_t>(cstruct.targets_vec.size());
    return cstruct;
}
FragmentState& FragmentState::setModule(wgpu::raw::ShaderModule value) & {
    this->module = value;
    return *this;
}
FragmentState&& FragmentState::setModule(wgpu::raw::ShaderModule value) && {
    this->module = value;
    return std::move(*this);
}
FragmentState& FragmentState::setEntryPoint(const wgpu::StringView& value) & {
    this->entryPoint = value;
    return *this;
}
FragmentState&& FragmentState::setEntryPoint(const wgpu::StringView& value) && {
    this->entryPoint = value;
    return std::move(*this);
}
FragmentState& FragmentState::setEntryPoint(wgpu::StringView&& value) & {
    this->entryPoint = std::move(value);
    return *this;
}
FragmentState&& FragmentState::setEntryPoint(wgpu::StringView&& value) && {
    this->entryPoint = std::move(value);
    return std::move(*this);
}
RenderPipelineDescriptor::RenderPipelineDescriptor(const WGPURenderPipelineDescriptor& native) {
    this->nextInChain.setNext(native.nextInChain);
    this->label = static_cast<wgpu::StringView>(native.label);
    this->layout = static_cast<wgpu::raw::PipelineLayout>(native.layout);
    this->vertex = static_cast<wgpu::VertexState>(native.vertex);
    this->primitive = static_cast<wgpu::PrimitiveState>(native.primitive);
    if (native.depthStencil != nullptr) {
        this->depthStencil = static_cast<wgpu::DepthStencilState>(*(native.depthStencil));
    } else {
        this->depthStencil.reset();
    }
    this->multisample = static_cast<wgpu::MultisampleState>(native.multisample);
    if (native.fragment != nullptr) {
        this->fragment = static_cast<wgpu::FragmentState>(*(native.fragment));
    } else {
        this->fragment.reset();
    }
}
RenderPipelineDescriptor::CStruct RenderPipelineDescriptor::to_cstruct() const {
    CStruct cstruct;
    cstruct.nextInChain = this->nextInChain.getNext();
    cstruct.label = static_cast<WGPUStringView>(this->label.to_cstruct());
    cstruct.layout = static_cast<WGPUPipelineLayout>(this->layout);
    cstruct.vertex_cstruct = this->vertex.to_cstruct();
    cstruct.vertex = static_cast<WGPUVertexState>(cstruct.vertex_cstruct);
    cstruct.primitive = static_cast<WGPUPrimitiveState>(this->primitive.to_cstruct());
    if (this->depthStencil.has_value()) {
        cstruct.depthStencil_cstruct = this->depthStencil->to_cstruct();
        cstruct.depthStencil = &(cstruct.depthStencil_cstruct);
    } else {
        cstruct.depthStencil = nullptr;
    }
    cstruct.multisample = static_cast<WGPUMultisampleState>(this->multisample.to_cstruct());
    if (this->fragment.has_value()) {
        cstruct.fragment_cstruct = this->fragment->to_cstruct();
        cstruct.fragment = &(cstruct.fragment_cstruct);
    } else {
        cstruct.fragment = nullptr;
    }
    return cstruct;
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setLabel(const wgpu::StringView& value) & {
    this->label = value;
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setLabel(const wgpu::StringView& value) && {
    this->label = value;
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setLabel(wgpu::StringView&& value) & {
    this->label = std::move(value);
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setLabel(wgpu::StringView&& value) && {
    this->label = std::move(value);
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setLayout(wgpu::raw::PipelineLayout value) & {
    this->layout = value;
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setLayout(wgpu::raw::PipelineLayout value) && {
    this->layout = value;
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setVertex(const wgpu::VertexState& value) & {
    this->vertex = value;
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setVertex(const wgpu::VertexState& value) && {
    this->vertex = value;
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setVertex(wgpu::VertexState&& value) & {
    this->vertex = std::move(value);
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setVertex(wgpu::VertexState&& value) && {
    this->vertex = std::move(value);
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setPrimitive(const wgpu::PrimitiveState& value) & {
    this->primitive = value;
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setPrimitive(const wgpu::PrimitiveState& value) && {
    this->primitive = value;
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setPrimitive(wgpu::PrimitiveState&& value) & {
    this->primitive = std::move(value);
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setPrimitive(wgpu::PrimitiveState&& value) && {
    this->primitive = std::move(value);
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setDepthStencil(const wgpu::DepthStencilState& value) & {
    this->depthStencil.emplace(value);
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setDepthStencil(const wgpu::DepthStencilState& value) && {
    this->depthStencil.emplace(value);
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setDepthStencil(wgpu::DepthStencilState&& value) & {
    this->depthStencil.emplace(std::move(value));
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setDepthStencil(wgpu::DepthStencilState&& value) && {
    this->depthStencil.emplace(std::move(value));
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setMultisample(const wgpu::MultisampleState& value) & {
    this->multisample = value;
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setMultisample(const wgpu::MultisampleState& value) && {
    this->multisample = value;
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setMultisample(wgpu::MultisampleState&& value) & {
    this->multisample = std::move(value);
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setMultisample(wgpu::MultisampleState&& value) && {
    this->multisample = std::move(value);
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setFragment(const wgpu::FragmentState& value) & {
    this->fragment.emplace(value);
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setFragment(const wgpu::FragmentState& value) && {
    this->fragment.emplace(value);
    return std::move(*this);
}
RenderPipelineDescriptor& RenderPipelineDescriptor::setFragment(wgpu::FragmentState&& value) & {
    this->fragment.emplace(std::move(value));
    return *this;
}
RenderPipelineDescriptor&& RenderPipelineDescriptor::setFragment(wgpu::FragmentState&& value) && {
    this->fragment.emplace(std::move(value));
    return std::move(*this);
}
}
namespace wgpu::raw {
void Adapter::getFeatures(wgpu::SupportedFeatures& features) const {
    WGPUSupportedFeatures features_native;
    wgpuAdapterGetFeatures(m_raw, &features_native);
    features = static_cast<wgpu::SupportedFeatures>(features_native);
    wgpuSupportedFeaturesFreeMembers(features_native);
}
wgpu::Status Adapter::getInfo(wgpu::AdapterInfo& info) const {
    WGPUAdapterInfo info_native;
    wgpu::Status res = static_cast<wgpu::Status>(wgpuAdapterGetInfo(m_raw, &info_native));
    info = static_cast<wgpu::AdapterInfo>(info_native);
    wgpuAdapterInfoFreeMembers(info_native);
    return res;
}
wgpu::Status Adapter::getLimits(wgpu::Limits& limits) const {
    WGPULimits limits_native;
    wgpu::Status res = static_cast<wgpu::Status>(wgpuAdapterGetLimits(m_raw, &limits_native));
    limits = static_cast<wgpu::Limits>(limits_native);
    return res;
}
wgpu::Bool Adapter::hasFeature(wgpu::FeatureName feature) const {
    wgpu::Bool res = static_cast<wgpu::Bool>(wgpuAdapterHasFeature(m_raw, static_cast<WGPUFeatureName>(feature)));
    return res;
}
wgpu::Future Adapter::requestDevice(wgpu::DeviceDescriptor const* descriptor, wgpu::RequestDeviceCallbackInfo callbackInfo) const {
    wgpu::DeviceDescriptor::CStruct descriptor_cstruct;
    if (descriptor) descriptor_cstruct = descriptor->to_cstruct();
    wgpu::RequestDeviceCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuAdapterRequestDevice(m_raw, descriptor? &descriptor_cstruct : nullptr, callbackInfo_cstruct));
    return res;
}
void Adapter::addRef() const {
    wgpuAdapterAddRef(m_raw);
}
void Adapter::release() const {
    wgpuAdapterRelease(m_raw);
}
void BindGroup::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuBindGroupSetLabel(m_raw, label_cstruct);
}
void BindGroup::addRef() const {
    wgpuBindGroupAddRef(m_raw);
}
void BindGroup::release() const {
    wgpuBindGroupRelease(m_raw);
}
void BindGroupLayout::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuBindGroupLayoutSetLabel(m_raw, label_cstruct);
}
void BindGroupLayout::addRef() const {
    wgpuBindGroupLayoutAddRef(m_raw);
}
void BindGroupLayout::release() const {
    wgpuBindGroupLayoutRelease(m_raw);
}
void Buffer::destroy() const {
    wgpuBufferDestroy(m_raw);
}
void const * Buffer::getConstMappedRange(size_t offset, size_t size) const {
    void const * res = static_cast<void const *>(wgpuBufferGetConstMappedRange(m_raw, offset, size));
    return res;
}
void * Buffer::getMappedRange(size_t offset, size_t size) const {
    void * res = static_cast<void *>(wgpuBufferGetMappedRange(m_raw, offset, size));
    return res;
}
wgpu::BufferMapState Buffer::getMapState() const {
    wgpu::BufferMapState res = static_cast<wgpu::BufferMapState>(wgpuBufferGetMapState(m_raw));
    return res;
}
uint64_t Buffer::getSize() const {
    uint64_t res = static_cast<uint64_t>(wgpuBufferGetSize(m_raw));
    return res;
}
wgpu::BufferUsage Buffer::getUsage() const {
    wgpu::BufferUsage res = static_cast<wgpu::BufferUsage>(wgpuBufferGetUsage(m_raw));
    return res;
}
wgpu::Future Buffer::mapAsync(wgpu::MapMode mode, size_t offset, size_t size, wgpu::BufferMapCallbackInfo callbackInfo) const {
    wgpu::BufferMapCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuBufferMapAsync(m_raw, static_cast<WGPUMapMode>(mode), offset, size, callbackInfo_cstruct));
    return res;
}
wgpu::Status Buffer::readMappedRange(size_t offset, void* data, size_t size) const {
    wgpu::Status res = static_cast<wgpu::Status>(wgpuBufferReadMappedRange(m_raw, offset, data, size));
    return res;
}
void Buffer::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuBufferSetLabel(m_raw, label_cstruct);
}
void Buffer::unmap() const {
    wgpuBufferUnmap(m_raw);
}
wgpu::Status Buffer::writeMappedRange(size_t offset, void const* data, size_t size) const {
    wgpu::Status res = static_cast<wgpu::Status>(wgpuBufferWriteMappedRange(m_raw, offset, data, size));
    return res;
}
void Buffer::addRef() const {
    wgpuBufferAddRef(m_raw);
}
void Buffer::release() const {
    wgpuBufferRelease(m_raw);
}
void CommandBuffer::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuCommandBufferSetLabel(m_raw, label_cstruct);
}
void CommandBuffer::addRef() const {
    wgpuCommandBufferAddRef(m_raw);
}
void CommandBuffer::release() const {
    wgpuCommandBufferRelease(m_raw);
}
wgpu::ComputePassEncoder CommandEncoder::beginComputePass(wgpu::ComputePassDescriptor const& descriptor) const {
    wgpu::ComputePassDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::ComputePassEncoder res = static_cast<wgpu::ComputePassEncoder>(wgpuCommandEncoderBeginComputePass(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::ComputePassEncoder CommandEncoder::beginComputePass() const {
    wgpu::ComputePassEncoder res = static_cast<wgpu::ComputePassEncoder>(wgpuCommandEncoderBeginComputePass(m_raw, nullptr));
    return res;
}
wgpu::RenderPassEncoder CommandEncoder::beginRenderPass(wgpu::RenderPassDescriptor const& descriptor) const {
    wgpu::RenderPassDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::RenderPassEncoder res = static_cast<wgpu::RenderPassEncoder>(wgpuCommandEncoderBeginRenderPass(m_raw, &descriptor_cstruct));
    return res;
}
void CommandEncoder::clearBuffer(wgpu::Buffer const& buffer, uint64_t offset, uint64_t size) const {
    wgpuCommandEncoderClearBuffer(m_raw, *reinterpret_cast<WGPUBuffer const*>(&buffer), offset, size);
}
void CommandEncoder::copyBufferToBuffer(wgpu::Buffer const& source, uint64_t sourceOffset, wgpu::Buffer const& destination, uint64_t destinationOffset, uint64_t size) const {
    wgpuCommandEncoderCopyBufferToBuffer(m_raw, *reinterpret_cast<WGPUBuffer const*>(&source), sourceOffset, *reinterpret_cast<WGPUBuffer const*>(&destination), destinationOffset, size);
}
void CommandEncoder::copyBufferToTexture(wgpu::TexelCopyBufferInfo const& source, wgpu::TexelCopyTextureInfo const& destination, wgpu::Extent3D const& copySize) const {
    wgpu::TexelCopyBufferInfo::CStruct source_cstruct = source.to_cstruct();
    wgpu::TexelCopyTextureInfo::CStruct destination_cstruct = destination.to_cstruct();
    wgpu::Extent3D::CStruct copySize_cstruct = copySize.to_cstruct();
    wgpuCommandEncoderCopyBufferToTexture(m_raw, &source_cstruct, &destination_cstruct, &copySize_cstruct);
}
void CommandEncoder::copyTextureToBuffer(wgpu::TexelCopyTextureInfo const& source, wgpu::TexelCopyBufferInfo const& destination, wgpu::Extent3D const& copySize) const {
    wgpu::TexelCopyTextureInfo::CStruct source_cstruct = source.to_cstruct();
    wgpu::TexelCopyBufferInfo::CStruct destination_cstruct = destination.to_cstruct();
    wgpu::Extent3D::CStruct copySize_cstruct = copySize.to_cstruct();
    wgpuCommandEncoderCopyTextureToBuffer(m_raw, &source_cstruct, &destination_cstruct, &copySize_cstruct);
}
void CommandEncoder::copyTextureToTexture(wgpu::TexelCopyTextureInfo const& source, wgpu::TexelCopyTextureInfo const& destination, wgpu::Extent3D const& copySize) const {
    wgpu::TexelCopyTextureInfo::CStruct source_cstruct = source.to_cstruct();
    wgpu::TexelCopyTextureInfo::CStruct destination_cstruct = destination.to_cstruct();
    wgpu::Extent3D::CStruct copySize_cstruct = copySize.to_cstruct();
    wgpuCommandEncoderCopyTextureToTexture(m_raw, &source_cstruct, &destination_cstruct, &copySize_cstruct);
}
wgpu::CommandBuffer CommandEncoder::finish(wgpu::CommandBufferDescriptor const& descriptor) const {
    wgpu::CommandBufferDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::CommandBuffer res = static_cast<wgpu::CommandBuffer>(wgpuCommandEncoderFinish(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::CommandBuffer CommandEncoder::finish() const {
    wgpu::CommandBuffer res = static_cast<wgpu::CommandBuffer>(wgpuCommandEncoderFinish(m_raw, nullptr));
    return res;
}
void CommandEncoder::insertDebugMarker(wgpu::StringView markerLabel) const {
    wgpu::StringView::CStruct markerLabel_cstruct = markerLabel.to_cstruct();
    wgpuCommandEncoderInsertDebugMarker(m_raw, markerLabel_cstruct);
}
void CommandEncoder::popDebugGroup() const {
    wgpuCommandEncoderPopDebugGroup(m_raw);
}
void CommandEncoder::pushDebugGroup(wgpu::StringView groupLabel) const {
    wgpu::StringView::CStruct groupLabel_cstruct = groupLabel.to_cstruct();
    wgpuCommandEncoderPushDebugGroup(m_raw, groupLabel_cstruct);
}
void CommandEncoder::resolveQuerySet(wgpu::QuerySet const& querySet, uint32_t firstQuery, uint32_t queryCount, wgpu::Buffer const& destination, uint64_t destinationOffset) const {
    wgpuCommandEncoderResolveQuerySet(m_raw, *reinterpret_cast<WGPUQuerySet const*>(&querySet), firstQuery, queryCount, *reinterpret_cast<WGPUBuffer const*>(&destination), destinationOffset);
}
void CommandEncoder::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuCommandEncoderSetLabel(m_raw, label_cstruct);
}
void CommandEncoder::writeTimestamp(wgpu::QuerySet const& querySet, uint32_t queryIndex) const {
    wgpuCommandEncoderWriteTimestamp(m_raw, *reinterpret_cast<WGPUQuerySet const*>(&querySet), queryIndex);
}
void CommandEncoder::addRef() const {
    wgpuCommandEncoderAddRef(m_raw);
}
void CommandEncoder::release() const {
    wgpuCommandEncoderRelease(m_raw);
}
void ComputePassEncoder::dispatchWorkgroups(uint32_t workgroupCountX, uint32_t workgroupCountY, uint32_t workgroupCountZ) const {
    wgpuComputePassEncoderDispatchWorkgroups(m_raw, workgroupCountX, workgroupCountY, workgroupCountZ);
}
void ComputePassEncoder::dispatchWorkgroupsIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const {
    wgpuComputePassEncoderDispatchWorkgroupsIndirect(m_raw, *reinterpret_cast<WGPUBuffer const*>(&indirectBuffer), indirectOffset);
}
void ComputePassEncoder::end() const {
    wgpuComputePassEncoderEnd(m_raw);
}
void ComputePassEncoder::insertDebugMarker(wgpu::StringView markerLabel) const {
    wgpu::StringView::CStruct markerLabel_cstruct = markerLabel.to_cstruct();
    wgpuComputePassEncoderInsertDebugMarker(m_raw, markerLabel_cstruct);
}
void ComputePassEncoder::popDebugGroup() const {
    wgpuComputePassEncoderPopDebugGroup(m_raw);
}
void ComputePassEncoder::pushDebugGroup(wgpu::StringView groupLabel) const {
    wgpu::StringView::CStruct groupLabel_cstruct = groupLabel.to_cstruct();
    wgpuComputePassEncoderPushDebugGroup(m_raw, groupLabel_cstruct);
}
void ComputePassEncoder::setBindGroup(uint32_t groupIndex, wgpu::BindGroup const& group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets) const {
    wgpuComputePassEncoderSetBindGroup(m_raw, groupIndex, *reinterpret_cast<WGPUBindGroup const*>(&group), dynamicOffsetCount, dynamicOffsets);
}
void ComputePassEncoder::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuComputePassEncoderSetLabel(m_raw, label_cstruct);
}
void ComputePassEncoder::setPipeline(wgpu::ComputePipeline const& pipeline) const {
    wgpuComputePassEncoderSetPipeline(m_raw, *reinterpret_cast<WGPUComputePipeline const*>(&pipeline));
}
void ComputePassEncoder::addRef() const {
    wgpuComputePassEncoderAddRef(m_raw);
}
void ComputePassEncoder::release() const {
    wgpuComputePassEncoderRelease(m_raw);
}
wgpu::BindGroupLayout ComputePipeline::getBindGroupLayout(uint32_t groupIndex) const {
    wgpu::BindGroupLayout res = static_cast<wgpu::BindGroupLayout>(wgpuComputePipelineGetBindGroupLayout(m_raw, groupIndex));
    return res;
}
void ComputePipeline::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuComputePipelineSetLabel(m_raw, label_cstruct);
}
void ComputePipeline::addRef() const {
    wgpuComputePipelineAddRef(m_raw);
}
void ComputePipeline::release() const {
    wgpuComputePipelineRelease(m_raw);
}
wgpu::BindGroup Device::createBindGroup(wgpu::BindGroupDescriptor const& descriptor) const {
    wgpu::BindGroupDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::BindGroup res = static_cast<wgpu::BindGroup>(wgpuDeviceCreateBindGroup(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::BindGroupLayout Device::createBindGroupLayout(wgpu::BindGroupLayoutDescriptor const& descriptor) const {
    wgpu::BindGroupLayoutDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::BindGroupLayout res = static_cast<wgpu::BindGroupLayout>(wgpuDeviceCreateBindGroupLayout(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::Buffer Device::createBuffer(wgpu::BufferDescriptor const& descriptor) const {
    wgpu::BufferDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::Buffer res = static_cast<wgpu::Buffer>(wgpuDeviceCreateBuffer(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::CommandEncoder Device::createCommandEncoder(wgpu::CommandEncoderDescriptor const& descriptor) const {
    wgpu::CommandEncoderDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::CommandEncoder res = static_cast<wgpu::CommandEncoder>(wgpuDeviceCreateCommandEncoder(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::CommandEncoder Device::createCommandEncoder() const {
    wgpu::CommandEncoder res = static_cast<wgpu::CommandEncoder>(wgpuDeviceCreateCommandEncoder(m_raw, nullptr));
    return res;
}
wgpu::ComputePipeline Device::createComputePipeline(wgpu::ComputePipelineDescriptor const& descriptor) const {
    wgpu::ComputePipelineDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::ComputePipeline res = static_cast<wgpu::ComputePipeline>(wgpuDeviceCreateComputePipeline(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::Future Device::createComputePipelineAsync(wgpu::ComputePipelineDescriptor const& descriptor, wgpu::CreateComputePipelineAsyncCallbackInfo callbackInfo) const {
    wgpu::ComputePipelineDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::CreateComputePipelineAsyncCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuDeviceCreateComputePipelineAsync(m_raw, &descriptor_cstruct, callbackInfo_cstruct));
    return res;
}
wgpu::PipelineLayout Device::createPipelineLayout(wgpu::PipelineLayoutDescriptor const& descriptor) const {
    wgpu::PipelineLayoutDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::PipelineLayout res = static_cast<wgpu::PipelineLayout>(wgpuDeviceCreatePipelineLayout(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::QuerySet Device::createQuerySet(wgpu::QuerySetDescriptor const& descriptor) const {
    wgpu::QuerySetDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::QuerySet res = static_cast<wgpu::QuerySet>(wgpuDeviceCreateQuerySet(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::RenderBundleEncoder Device::createRenderBundleEncoder(wgpu::RenderBundleEncoderDescriptor const& descriptor) const {
    wgpu::RenderBundleEncoderDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::RenderBundleEncoder res = static_cast<wgpu::RenderBundleEncoder>(wgpuDeviceCreateRenderBundleEncoder(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::RenderPipeline Device::createRenderPipeline(wgpu::RenderPipelineDescriptor const& descriptor) const {
    wgpu::RenderPipelineDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::RenderPipeline res = static_cast<wgpu::RenderPipeline>(wgpuDeviceCreateRenderPipeline(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::Future Device::createRenderPipelineAsync(wgpu::RenderPipelineDescriptor const& descriptor, wgpu::CreateRenderPipelineAsyncCallbackInfo callbackInfo) const {
    wgpu::RenderPipelineDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::CreateRenderPipelineAsyncCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuDeviceCreateRenderPipelineAsync(m_raw, &descriptor_cstruct, callbackInfo_cstruct));
    return res;
}
wgpu::Sampler Device::createSampler(wgpu::SamplerDescriptor const& descriptor) const {
    wgpu::SamplerDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::Sampler res = static_cast<wgpu::Sampler>(wgpuDeviceCreateSampler(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::Sampler Device::createSampler() const {
    wgpu::Sampler res = static_cast<wgpu::Sampler>(wgpuDeviceCreateSampler(m_raw, nullptr));
    return res;
}
wgpu::ShaderModule Device::createShaderModule(wgpu::ShaderModuleDescriptor const& descriptor) const {
    wgpu::ShaderModuleDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::ShaderModule res = static_cast<wgpu::ShaderModule>(wgpuDeviceCreateShaderModule(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::Texture Device::createTexture(wgpu::TextureDescriptor const& descriptor) const {
    wgpu::TextureDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::Texture res = static_cast<wgpu::Texture>(wgpuDeviceCreateTexture(m_raw, &descriptor_cstruct));
    return res;
}
void Device::destroy() const {
    wgpuDeviceDestroy(m_raw);
}
wgpu::Status Device::getAdapterInfo(wgpu::AdapterInfo& adapterInfo) const {
    WGPUAdapterInfo adapterInfo_native;
    wgpu::Status res = static_cast<wgpu::Status>(wgpuDeviceGetAdapterInfo(m_raw, &adapterInfo_native));
    adapterInfo = static_cast<wgpu::AdapterInfo>(adapterInfo_native);
    wgpuAdapterInfoFreeMembers(adapterInfo_native);
    return res;
}
void Device::getFeatures(wgpu::SupportedFeatures& features) const {
    WGPUSupportedFeatures features_native;
    wgpuDeviceGetFeatures(m_raw, &features_native);
    features = static_cast<wgpu::SupportedFeatures>(features_native);
    wgpuSupportedFeaturesFreeMembers(features_native);
}
wgpu::Status Device::getLimits(wgpu::Limits& limits) const {
    WGPULimits limits_native;
    wgpu::Status res = static_cast<wgpu::Status>(wgpuDeviceGetLimits(m_raw, &limits_native));
    limits = static_cast<wgpu::Limits>(limits_native);
    return res;
}
wgpu::Future Device::getLostFuture() const {
    wgpu::Future res = static_cast<wgpu::Future>(wgpuDeviceGetLostFuture(m_raw));
    return res;
}
wgpu::Queue Device::getQueue() const {
    wgpu::Queue res = static_cast<wgpu::Queue>(wgpuDeviceGetQueue(m_raw));
    return res;
}
wgpu::Bool Device::hasFeature(wgpu::FeatureName feature) const {
    wgpu::Bool res = static_cast<wgpu::Bool>(wgpuDeviceHasFeature(m_raw, static_cast<WGPUFeatureName>(feature)));
    return res;
}
wgpu::Future Device::popErrorScope(wgpu::PopErrorScopeCallbackInfo callbackInfo) const {
    wgpu::PopErrorScopeCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuDevicePopErrorScope(m_raw, callbackInfo_cstruct));
    return res;
}
void Device::pushErrorScope(wgpu::ErrorFilter filter) const {
    wgpuDevicePushErrorScope(m_raw, static_cast<WGPUErrorFilter>(filter));
}
void Device::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuDeviceSetLabel(m_raw, label_cstruct);
}
void Device::addRef() const {
    wgpuDeviceAddRef(m_raw);
}
void Device::release() const {
    wgpuDeviceRelease(m_raw);
}
void ExternalTexture::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuExternalTextureSetLabel(m_raw, label_cstruct);
}
void ExternalTexture::addRef() const {
    wgpuExternalTextureAddRef(m_raw);
}
void ExternalTexture::release() const {
    wgpuExternalTextureRelease(m_raw);
}
wgpu::Surface Instance::createSurface(wgpu::SurfaceDescriptor const& descriptor) const {
    wgpu::SurfaceDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::Surface res = static_cast<wgpu::Surface>(wgpuInstanceCreateSurface(m_raw, &descriptor_cstruct));
    return res;
}
void Instance::getWGSLLanguageFeatures(wgpu::SupportedWGSLLanguageFeatures& features) const {
    WGPUSupportedWGSLLanguageFeatures features_native;
    wgpuInstanceGetWGSLLanguageFeatures(m_raw, &features_native);
    features = static_cast<wgpu::SupportedWGSLLanguageFeatures>(features_native);
    wgpuSupportedWGSLLanguageFeaturesFreeMembers(features_native);
}
wgpu::Bool Instance::hasWGSLLanguageFeature(wgpu::WGSLLanguageFeatureName feature) const {
    wgpu::Bool res = static_cast<wgpu::Bool>(wgpuInstanceHasWGSLLanguageFeature(m_raw, static_cast<WGPUWGSLLanguageFeatureName>(feature)));
    return res;
}
void Instance::processEvents() const {
    wgpuInstanceProcessEvents(m_raw);
}
wgpu::Future Instance::requestAdapter(wgpu::RequestAdapterOptions const* options, wgpu::RequestAdapterCallbackInfo callbackInfo) const {
    wgpu::RequestAdapterOptions::CStruct options_cstruct;
    if (options) options_cstruct = options->to_cstruct();
    wgpu::RequestAdapterCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuInstanceRequestAdapter(m_raw, options? &options_cstruct : nullptr, callbackInfo_cstruct));
    return res;
}
wgpu::WaitStatus Instance::waitAny(size_t futureCount, wgpu::FutureWaitInfo* futures, uint64_t timeoutNS) const {
    WGPUFutureWaitInfo futures_native;
    wgpu::WaitStatus res = static_cast<wgpu::WaitStatus>(wgpuInstanceWaitAny(m_raw, futureCount, futures? &futures_native : nullptr, timeoutNS));
    if (futures) *futures = static_cast<wgpu::FutureWaitInfo>(futures_native);
    return res;
}
void Instance::addRef() const {
    wgpuInstanceAddRef(m_raw);
}
void Instance::release() const {
    wgpuInstanceRelease(m_raw);
}
void PipelineLayout::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuPipelineLayoutSetLabel(m_raw, label_cstruct);
}
void PipelineLayout::addRef() const {
    wgpuPipelineLayoutAddRef(m_raw);
}
void PipelineLayout::release() const {
    wgpuPipelineLayoutRelease(m_raw);
}
void QuerySet::destroy() const {
    wgpuQuerySetDestroy(m_raw);
}
uint32_t QuerySet::getCount() const {
    uint32_t res = static_cast<uint32_t>(wgpuQuerySetGetCount(m_raw));
    return res;
}
wgpu::QueryType QuerySet::getType() const {
    wgpu::QueryType res = static_cast<wgpu::QueryType>(wgpuQuerySetGetType(m_raw));
    return res;
}
void QuerySet::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuQuerySetSetLabel(m_raw, label_cstruct);
}
void QuerySet::addRef() const {
    wgpuQuerySetAddRef(m_raw);
}
void QuerySet::release() const {
    wgpuQuerySetRelease(m_raw);
}
wgpu::Future Queue::onSubmittedWorkDone(wgpu::QueueWorkDoneCallbackInfo callbackInfo) const {
    wgpu::QueueWorkDoneCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuQueueOnSubmittedWorkDone(m_raw, callbackInfo_cstruct));
    return res;
}
void Queue::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuQueueSetLabel(m_raw, label_cstruct);
}
void Queue::submit(size_t commandCount, wgpu::CommandBuffer const& commands) const {
    wgpuQueueSubmit(m_raw, commandCount, reinterpret_cast<WGPUCommandBuffer const*>(&commands));
}
void Queue::writeBuffer(wgpu::Buffer const& buffer, uint64_t bufferOffset, void const* data, size_t size) const {
    wgpuQueueWriteBuffer(m_raw, *reinterpret_cast<WGPUBuffer const*>(&buffer), bufferOffset, data, size);
}
void Queue::writeTexture(wgpu::TexelCopyTextureInfo const& destination, void const* data, size_t dataSize, wgpu::TexelCopyBufferLayout const& dataLayout, wgpu::Extent3D const& writeSize) const {
    wgpu::TexelCopyTextureInfo::CStruct destination_cstruct = destination.to_cstruct();
    wgpu::TexelCopyBufferLayout::CStruct dataLayout_cstruct = dataLayout.to_cstruct();
    wgpu::Extent3D::CStruct writeSize_cstruct = writeSize.to_cstruct();
    wgpuQueueWriteTexture(m_raw, &destination_cstruct, data, dataSize, &dataLayout_cstruct, &writeSize_cstruct);
}
void Queue::addRef() const {
    wgpuQueueAddRef(m_raw);
}
void Queue::release() const {
    wgpuQueueRelease(m_raw);
}
void RenderBundle::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuRenderBundleSetLabel(m_raw, label_cstruct);
}
void RenderBundle::addRef() const {
    wgpuRenderBundleAddRef(m_raw);
}
void RenderBundle::release() const {
    wgpuRenderBundleRelease(m_raw);
}
void RenderBundleEncoder::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const {
    wgpuRenderBundleEncoderDraw(m_raw, vertexCount, instanceCount, firstVertex, firstInstance);
}
void RenderBundleEncoder::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) const {
    wgpuRenderBundleEncoderDrawIndexed(m_raw, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}
void RenderBundleEncoder::drawIndexedIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const {
    wgpuRenderBundleEncoderDrawIndexedIndirect(m_raw, *reinterpret_cast<WGPUBuffer const*>(&indirectBuffer), indirectOffset);
}
void RenderBundleEncoder::drawIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const {
    wgpuRenderBundleEncoderDrawIndirect(m_raw, *reinterpret_cast<WGPUBuffer const*>(&indirectBuffer), indirectOffset);
}
wgpu::RenderBundle RenderBundleEncoder::finish(wgpu::RenderBundleDescriptor const& descriptor) const {
    wgpu::RenderBundleDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::RenderBundle res = static_cast<wgpu::RenderBundle>(wgpuRenderBundleEncoderFinish(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::RenderBundle RenderBundleEncoder::finish() const {
    wgpu::RenderBundle res = static_cast<wgpu::RenderBundle>(wgpuRenderBundleEncoderFinish(m_raw, nullptr));
    return res;
}
void RenderBundleEncoder::insertDebugMarker(wgpu::StringView markerLabel) const {
    wgpu::StringView::CStruct markerLabel_cstruct = markerLabel.to_cstruct();
    wgpuRenderBundleEncoderInsertDebugMarker(m_raw, markerLabel_cstruct);
}
void RenderBundleEncoder::popDebugGroup() const {
    wgpuRenderBundleEncoderPopDebugGroup(m_raw);
}
void RenderBundleEncoder::pushDebugGroup(wgpu::StringView groupLabel) const {
    wgpu::StringView::CStruct groupLabel_cstruct = groupLabel.to_cstruct();
    wgpuRenderBundleEncoderPushDebugGroup(m_raw, groupLabel_cstruct);
}
void RenderBundleEncoder::setBindGroup(uint32_t groupIndex, wgpu::BindGroup const& group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets) const {
    wgpuRenderBundleEncoderSetBindGroup(m_raw, groupIndex, *reinterpret_cast<WGPUBindGroup const*>(&group), dynamicOffsetCount, dynamicOffsets);
}
void RenderBundleEncoder::setIndexBuffer(wgpu::Buffer const& buffer, wgpu::IndexFormat format, uint64_t offset, uint64_t size) const {
    wgpuRenderBundleEncoderSetIndexBuffer(m_raw, *reinterpret_cast<WGPUBuffer const*>(&buffer), static_cast<WGPUIndexFormat>(format), offset, size);
}
void RenderBundleEncoder::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuRenderBundleEncoderSetLabel(m_raw, label_cstruct);
}
void RenderBundleEncoder::setPipeline(wgpu::RenderPipeline const& pipeline) const {
    wgpuRenderBundleEncoderSetPipeline(m_raw, *reinterpret_cast<WGPURenderPipeline const*>(&pipeline));
}
void RenderBundleEncoder::setVertexBuffer(uint32_t slot, wgpu::Buffer const& buffer, uint64_t offset, uint64_t size) const {
    wgpuRenderBundleEncoderSetVertexBuffer(m_raw, slot, *reinterpret_cast<WGPUBuffer const*>(&buffer), offset, size);
}
void RenderBundleEncoder::addRef() const {
    wgpuRenderBundleEncoderAddRef(m_raw);
}
void RenderBundleEncoder::release() const {
    wgpuRenderBundleEncoderRelease(m_raw);
}
void RenderPassEncoder::beginOcclusionQuery(uint32_t queryIndex) const {
    wgpuRenderPassEncoderBeginOcclusionQuery(m_raw, queryIndex);
}
void RenderPassEncoder::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const {
    wgpuRenderPassEncoderDraw(m_raw, vertexCount, instanceCount, firstVertex, firstInstance);
}
void RenderPassEncoder::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) const {
    wgpuRenderPassEncoderDrawIndexed(m_raw, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}
void RenderPassEncoder::drawIndexedIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const {
    wgpuRenderPassEncoderDrawIndexedIndirect(m_raw, *reinterpret_cast<WGPUBuffer const*>(&indirectBuffer), indirectOffset);
}
void RenderPassEncoder::drawIndirect(wgpu::Buffer const& indirectBuffer, uint64_t indirectOffset) const {
    wgpuRenderPassEncoderDrawIndirect(m_raw, *reinterpret_cast<WGPUBuffer const*>(&indirectBuffer), indirectOffset);
}
void RenderPassEncoder::end() const {
    wgpuRenderPassEncoderEnd(m_raw);
}
void RenderPassEncoder::endOcclusionQuery() const {
    wgpuRenderPassEncoderEndOcclusionQuery(m_raw);
}
void RenderPassEncoder::executeBundles(size_t bundleCount, wgpu::RenderBundle const& bundles) const {
    wgpuRenderPassEncoderExecuteBundles(m_raw, bundleCount, reinterpret_cast<WGPURenderBundle const*>(&bundles));
}
void RenderPassEncoder::insertDebugMarker(wgpu::StringView markerLabel) const {
    wgpu::StringView::CStruct markerLabel_cstruct = markerLabel.to_cstruct();
    wgpuRenderPassEncoderInsertDebugMarker(m_raw, markerLabel_cstruct);
}
void RenderPassEncoder::popDebugGroup() const {
    wgpuRenderPassEncoderPopDebugGroup(m_raw);
}
void RenderPassEncoder::pushDebugGroup(wgpu::StringView groupLabel) const {
    wgpu::StringView::CStruct groupLabel_cstruct = groupLabel.to_cstruct();
    wgpuRenderPassEncoderPushDebugGroup(m_raw, groupLabel_cstruct);
}
void RenderPassEncoder::setBindGroup(uint32_t groupIndex, wgpu::BindGroup const& group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets) const {
    wgpuRenderPassEncoderSetBindGroup(m_raw, groupIndex, *reinterpret_cast<WGPUBindGroup const*>(&group), dynamicOffsetCount, dynamicOffsets);
}
void RenderPassEncoder::setBlendConstant(wgpu::Color const& color) const {
    wgpu::Color::CStruct color_cstruct = color.to_cstruct();
    wgpuRenderPassEncoderSetBlendConstant(m_raw, &color_cstruct);
}
void RenderPassEncoder::setIndexBuffer(wgpu::Buffer const& buffer, wgpu::IndexFormat format, uint64_t offset, uint64_t size) const {
    wgpuRenderPassEncoderSetIndexBuffer(m_raw, *reinterpret_cast<WGPUBuffer const*>(&buffer), static_cast<WGPUIndexFormat>(format), offset, size);
}
void RenderPassEncoder::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuRenderPassEncoderSetLabel(m_raw, label_cstruct);
}
void RenderPassEncoder::setPipeline(wgpu::RenderPipeline const& pipeline) const {
    wgpuRenderPassEncoderSetPipeline(m_raw, *reinterpret_cast<WGPURenderPipeline const*>(&pipeline));
}
void RenderPassEncoder::setScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const {
    wgpuRenderPassEncoderSetScissorRect(m_raw, x, y, width, height);
}
void RenderPassEncoder::setStencilReference(uint32_t reference) const {
    wgpuRenderPassEncoderSetStencilReference(m_raw, reference);
}
void RenderPassEncoder::setVertexBuffer(uint32_t slot, wgpu::Buffer const& buffer, uint64_t offset, uint64_t size) const {
    wgpuRenderPassEncoderSetVertexBuffer(m_raw, slot, *reinterpret_cast<WGPUBuffer const*>(&buffer), offset, size);
}
void RenderPassEncoder::setViewport(float x, float y, float width, float height, float minDepth, float maxDepth) const {
    wgpuRenderPassEncoderSetViewport(m_raw, x, y, width, height, minDepth, maxDepth);
}
void RenderPassEncoder::addRef() const {
    wgpuRenderPassEncoderAddRef(m_raw);
}
void RenderPassEncoder::release() const {
    wgpuRenderPassEncoderRelease(m_raw);
}
wgpu::BindGroupLayout RenderPipeline::getBindGroupLayout(uint32_t groupIndex) const {
    wgpu::BindGroupLayout res = static_cast<wgpu::BindGroupLayout>(wgpuRenderPipelineGetBindGroupLayout(m_raw, groupIndex));
    return res;
}
void RenderPipeline::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuRenderPipelineSetLabel(m_raw, label_cstruct);
}
void RenderPipeline::addRef() const {
    wgpuRenderPipelineAddRef(m_raw);
}
void RenderPipeline::release() const {
    wgpuRenderPipelineRelease(m_raw);
}
void Sampler::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuSamplerSetLabel(m_raw, label_cstruct);
}
void Sampler::addRef() const {
    wgpuSamplerAddRef(m_raw);
}
void Sampler::release() const {
    wgpuSamplerRelease(m_raw);
}
wgpu::Future ShaderModule::getCompilationInfo(wgpu::CompilationInfoCallbackInfo callbackInfo) const {
    wgpu::CompilationInfoCallbackInfo::CStruct callbackInfo_cstruct = callbackInfo.to_cstruct();
    wgpu::Future res = static_cast<wgpu::Future>(wgpuShaderModuleGetCompilationInfo(m_raw, callbackInfo_cstruct));
    return res;
}
void ShaderModule::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuShaderModuleSetLabel(m_raw, label_cstruct);
}
void ShaderModule::addRef() const {
    wgpuShaderModuleAddRef(m_raw);
}
void ShaderModule::release() const {
    wgpuShaderModuleRelease(m_raw);
}
void Surface::configure(wgpu::SurfaceConfiguration const& config) const {
    wgpu::SurfaceConfiguration::CStruct config_cstruct = config.to_cstruct();
    wgpuSurfaceConfigure(m_raw, &config_cstruct);
}
wgpu::Status Surface::getCapabilities(wgpu::Adapter const& adapter, wgpu::SurfaceCapabilities& capabilities) const {
    WGPUSurfaceCapabilities capabilities_native;
    wgpu::Status res = static_cast<wgpu::Status>(wgpuSurfaceGetCapabilities(m_raw, *reinterpret_cast<WGPUAdapter const*>(&adapter), &capabilities_native));
    capabilities = static_cast<wgpu::SurfaceCapabilities>(capabilities_native);
    wgpuSurfaceCapabilitiesFreeMembers(capabilities_native);
    return res;
}
void Surface::getCurrentTexture(wgpu::SurfaceTexture& surfaceTexture) const {
    WGPUSurfaceTexture surfaceTexture_native;
    wgpuSurfaceGetCurrentTexture(m_raw, &surfaceTexture_native);
    surfaceTexture = static_cast<wgpu::SurfaceTexture>(surfaceTexture_native);
}
wgpu::Status Surface::present() const {
    wgpu::Status res = static_cast<wgpu::Status>(wgpuSurfacePresent(m_raw));
    return res;
}
void Surface::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuSurfaceSetLabel(m_raw, label_cstruct);
}
void Surface::unconfigure() const {
    wgpuSurfaceUnconfigure(m_raw);
}
void Surface::addRef() const {
    wgpuSurfaceAddRef(m_raw);
}
void Surface::release() const {
    wgpuSurfaceRelease(m_raw);
}
wgpu::TextureView Texture::createView(wgpu::TextureViewDescriptor const& descriptor) const {
    wgpu::TextureViewDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::TextureView res = static_cast<wgpu::TextureView>(wgpuTextureCreateView(m_raw, &descriptor_cstruct));
    return res;
}
wgpu::TextureView Texture::createView() const {
    wgpu::TextureView res = static_cast<wgpu::TextureView>(wgpuTextureCreateView(m_raw, nullptr));
    return res;
}
void Texture::destroy() const {
    wgpuTextureDestroy(m_raw);
}
uint32_t Texture::getDepthOrArrayLayers() const {
    uint32_t res = static_cast<uint32_t>(wgpuTextureGetDepthOrArrayLayers(m_raw));
    return res;
}
wgpu::TextureDimension Texture::getDimension() const {
    wgpu::TextureDimension res = static_cast<wgpu::TextureDimension>(wgpuTextureGetDimension(m_raw));
    return res;
}
wgpu::TextureFormat Texture::getFormat() const {
    wgpu::TextureFormat res = static_cast<wgpu::TextureFormat>(wgpuTextureGetFormat(m_raw));
    return res;
}
uint32_t Texture::getHeight() const {
    uint32_t res = static_cast<uint32_t>(wgpuTextureGetHeight(m_raw));
    return res;
}
uint32_t Texture::getMipLevelCount() const {
    uint32_t res = static_cast<uint32_t>(wgpuTextureGetMipLevelCount(m_raw));
    return res;
}
uint32_t Texture::getSampleCount() const {
    uint32_t res = static_cast<uint32_t>(wgpuTextureGetSampleCount(m_raw));
    return res;
}
wgpu::TextureViewDimension Texture::getTextureBindingViewDimension() const {
    wgpu::TextureViewDimension res = static_cast<wgpu::TextureViewDimension>(wgpuTextureGetTextureBindingViewDimension(m_raw));
    return res;
}
wgpu::TextureUsage Texture::getUsage() const {
    wgpu::TextureUsage res = static_cast<wgpu::TextureUsage>(wgpuTextureGetUsage(m_raw));
    return res;
}
uint32_t Texture::getWidth() const {
    uint32_t res = static_cast<uint32_t>(wgpuTextureGetWidth(m_raw));
    return res;
}
void Texture::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuTextureSetLabel(m_raw, label_cstruct);
}
void Texture::addRef() const {
    wgpuTextureAddRef(m_raw);
}
void Texture::release() const {
    wgpuTextureRelease(m_raw);
}
void TextureView::setLabel(wgpu::StringView label) const {
    wgpu::StringView::CStruct label_cstruct = label.to_cstruct();
    wgpuTextureViewSetLabel(m_raw, label_cstruct);
}
void TextureView::addRef() const {
    wgpuTextureViewAddRef(m_raw);
}
void TextureView::release() const {
    wgpuTextureViewRelease(m_raw);
}
}
namespace wgpu {
struct BufferMapCallbackControlNative : BufferMapCallback::Control {
    WGPUBufferMapCallback native;
    void* userdata1;
    void* userdata2;
    BufferMapCallbackControlNative(WGPUBufferMapCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::MapAsyncStatus status, wgpu::StringView message) const override {}
    void invoke_c(WGPUMapAsyncStatus status, WGPUStringView message) const override;
};
void BufferMapCallbackControlNative::invoke_c(WGPUMapAsyncStatus status, WGPUStringView message) const {
    native(status, message, userdata1, userdata2);
}
BufferMapCallback::BufferMapCallback(WGPUBufferMapCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new BufferMapCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void BufferMapCallback::Control::invoke_c(WGPUMapAsyncStatus status, WGPUStringView message) const {
    invoke(static_cast<wgpu::MapAsyncStatus>(status), static_cast<wgpu::StringView>(message));
}
BufferMapCallback& BufferMapCallback::operator=(const BufferMapCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
BufferMapCallback& BufferMapCallback::operator=(BufferMapCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void BufferMapCallback::operator()(wgpu::MapAsyncStatus status, wgpu::StringView message) const { if (data) data->invoke(status, message); }
void BufferMapCallback::operator()(WGPUMapAsyncStatus status, WGPUStringView message) const { if (data) data->invoke_c(status, message); }
struct CompilationInfoCallbackControlNative : CompilationInfoCallback::Control {
    WGPUCompilationInfoCallback native;
    void* userdata1;
    void* userdata2;
    CompilationInfoCallbackControlNative(WGPUCompilationInfoCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::CompilationInfoRequestStatus status, wgpu::CompilationInfo const& compilationInfo) const override {}
    void invoke_c(WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const* compilationInfo) const override;
};
void CompilationInfoCallbackControlNative::invoke_c(WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const* compilationInfo) const {
    native(status, compilationInfo, userdata1, userdata2);
}
CompilationInfoCallback::CompilationInfoCallback(WGPUCompilationInfoCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new CompilationInfoCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void CompilationInfoCallback::Control::invoke_c(WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const* compilationInfo) const {
	wgpu::CompilationInfo compilationInfo_temp = static_cast<wgpu::CompilationInfo>(*compilationInfo);
    invoke(static_cast<wgpu::CompilationInfoRequestStatus>(status), compilationInfo_temp);
}
CompilationInfoCallback& CompilationInfoCallback::operator=(const CompilationInfoCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
CompilationInfoCallback& CompilationInfoCallback::operator=(CompilationInfoCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void CompilationInfoCallback::operator()(wgpu::CompilationInfoRequestStatus status, wgpu::CompilationInfo const& compilationInfo) const { if (data) data->invoke(status, compilationInfo); }
void CompilationInfoCallback::operator()(WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const* compilationInfo) const { if (data) data->invoke_c(status, compilationInfo); }
struct CreateComputePipelineAsyncCallbackControlNative : CreateComputePipelineAsyncCallback::Control {
    WGPUCreateComputePipelineAsyncCallback native;
    void* userdata1;
    void* userdata2;
    CreateComputePipelineAsyncCallbackControlNative(WGPUCreateComputePipelineAsyncCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::ComputePipeline const& pipeline, wgpu::StringView message) const override {}
    void invoke_c(WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, WGPUStringView message) const override;
};
void CreateComputePipelineAsyncCallbackControlNative::invoke_c(WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, WGPUStringView message) const {
    native(status, pipeline, message, userdata1, userdata2);
}
CreateComputePipelineAsyncCallback::CreateComputePipelineAsyncCallback(WGPUCreateComputePipelineAsyncCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new CreateComputePipelineAsyncCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void CreateComputePipelineAsyncCallback::Control::invoke_c(WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, WGPUStringView message) const {
    invoke(static_cast<wgpu::CreatePipelineAsyncStatus>(status), *reinterpret_cast<wgpu::ComputePipeline const*>(&pipeline), static_cast<wgpu::StringView>(message));
}
CreateComputePipelineAsyncCallback& CreateComputePipelineAsyncCallback::operator=(const CreateComputePipelineAsyncCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
CreateComputePipelineAsyncCallback& CreateComputePipelineAsyncCallback::operator=(CreateComputePipelineAsyncCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void CreateComputePipelineAsyncCallback::operator()(wgpu::CreatePipelineAsyncStatus status, wgpu::ComputePipeline const& pipeline, wgpu::StringView message) const { if (data) data->invoke(status, pipeline, message); }
void CreateComputePipelineAsyncCallback::operator()(WGPUCreatePipelineAsyncStatus status, WGPUComputePipeline pipeline, WGPUStringView message) const { if (data) data->invoke_c(status, pipeline, message); }
struct CreateRenderPipelineAsyncCallbackControlNative : CreateRenderPipelineAsyncCallback::Control {
    WGPUCreateRenderPipelineAsyncCallback native;
    void* userdata1;
    void* userdata2;
    CreateRenderPipelineAsyncCallbackControlNative(WGPUCreateRenderPipelineAsyncCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::CreatePipelineAsyncStatus status, wgpu::RenderPipeline const& pipeline, wgpu::StringView message) const override {}
    void invoke_c(WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, WGPUStringView message) const override;
};
void CreateRenderPipelineAsyncCallbackControlNative::invoke_c(WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, WGPUStringView message) const {
    native(status, pipeline, message, userdata1, userdata2);
}
CreateRenderPipelineAsyncCallback::CreateRenderPipelineAsyncCallback(WGPUCreateRenderPipelineAsyncCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new CreateRenderPipelineAsyncCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void CreateRenderPipelineAsyncCallback::Control::invoke_c(WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, WGPUStringView message) const {
    invoke(static_cast<wgpu::CreatePipelineAsyncStatus>(status), *reinterpret_cast<wgpu::RenderPipeline const*>(&pipeline), static_cast<wgpu::StringView>(message));
}
CreateRenderPipelineAsyncCallback& CreateRenderPipelineAsyncCallback::operator=(const CreateRenderPipelineAsyncCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
CreateRenderPipelineAsyncCallback& CreateRenderPipelineAsyncCallback::operator=(CreateRenderPipelineAsyncCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void CreateRenderPipelineAsyncCallback::operator()(wgpu::CreatePipelineAsyncStatus status, wgpu::RenderPipeline const& pipeline, wgpu::StringView message) const { if (data) data->invoke(status, pipeline, message); }
void CreateRenderPipelineAsyncCallback::operator()(WGPUCreatePipelineAsyncStatus status, WGPURenderPipeline pipeline, WGPUStringView message) const { if (data) data->invoke_c(status, pipeline, message); }
struct DeviceLostCallbackControlNative : DeviceLostCallback::Control {
    WGPUDeviceLostCallback native;
    void* userdata1;
    void* userdata2;
    DeviceLostCallbackControlNative(WGPUDeviceLostCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::Device const& device, wgpu::DeviceLostReason reason, wgpu::StringView message) const override {}
    void invoke_c(WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message) const override;
};
void DeviceLostCallbackControlNative::invoke_c(WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message) const {
    native(device, reason, message, userdata1, userdata2);
}
DeviceLostCallback::DeviceLostCallback(WGPUDeviceLostCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new DeviceLostCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void DeviceLostCallback::Control::invoke_c(WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message) const {
    invoke(*reinterpret_cast<wgpu::Device const*>(device), static_cast<wgpu::DeviceLostReason>(reason), static_cast<wgpu::StringView>(message));
}
DeviceLostCallback& DeviceLostCallback::operator=(const DeviceLostCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
DeviceLostCallback& DeviceLostCallback::operator=(DeviceLostCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void DeviceLostCallback::operator()(wgpu::Device const& device, wgpu::DeviceLostReason reason, wgpu::StringView message) const { if (data) data->invoke(device, reason, message); }
void DeviceLostCallback::operator()(WGPUDevice const* device, WGPUDeviceLostReason reason, WGPUStringView message) const { if (data) data->invoke_c(device, reason, message); }
struct PopErrorScopeCallbackControlNative : PopErrorScopeCallback::Control {
    WGPUPopErrorScopeCallback native;
    void* userdata1;
    void* userdata2;
    PopErrorScopeCallbackControlNative(WGPUPopErrorScopeCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::PopErrorScopeStatus status, wgpu::ErrorType type, wgpu::StringView message) const override {}
    void invoke_c(WGPUPopErrorScopeStatus status, WGPUErrorType type, WGPUStringView message) const override;
};
void PopErrorScopeCallbackControlNative::invoke_c(WGPUPopErrorScopeStatus status, WGPUErrorType type, WGPUStringView message) const {
    native(status, type, message, userdata1, userdata2);
}
PopErrorScopeCallback::PopErrorScopeCallback(WGPUPopErrorScopeCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new PopErrorScopeCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void PopErrorScopeCallback::Control::invoke_c(WGPUPopErrorScopeStatus status, WGPUErrorType type, WGPUStringView message) const {
    invoke(static_cast<wgpu::PopErrorScopeStatus>(status), static_cast<wgpu::ErrorType>(type), static_cast<wgpu::StringView>(message));
}
PopErrorScopeCallback& PopErrorScopeCallback::operator=(const PopErrorScopeCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
PopErrorScopeCallback& PopErrorScopeCallback::operator=(PopErrorScopeCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void PopErrorScopeCallback::operator()(wgpu::PopErrorScopeStatus status, wgpu::ErrorType type, wgpu::StringView message) const { if (data) data->invoke(status, type, message); }
void PopErrorScopeCallback::operator()(WGPUPopErrorScopeStatus status, WGPUErrorType type, WGPUStringView message) const { if (data) data->invoke_c(status, type, message); }
struct QueueWorkDoneCallbackControlNative : QueueWorkDoneCallback::Control {
    WGPUQueueWorkDoneCallback native;
    void* userdata1;
    void* userdata2;
    QueueWorkDoneCallbackControlNative(WGPUQueueWorkDoneCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::QueueWorkDoneStatus status, wgpu::StringView message) const override {}
    void invoke_c(WGPUQueueWorkDoneStatus status, WGPUStringView message) const override;
};
void QueueWorkDoneCallbackControlNative::invoke_c(WGPUQueueWorkDoneStatus status, WGPUStringView message) const {
    native(status, message, userdata1, userdata2);
}
QueueWorkDoneCallback::QueueWorkDoneCallback(WGPUQueueWorkDoneCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new QueueWorkDoneCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void QueueWorkDoneCallback::Control::invoke_c(WGPUQueueWorkDoneStatus status, WGPUStringView message) const {
    invoke(static_cast<wgpu::QueueWorkDoneStatus>(status), static_cast<wgpu::StringView>(message));
}
QueueWorkDoneCallback& QueueWorkDoneCallback::operator=(const QueueWorkDoneCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
QueueWorkDoneCallback& QueueWorkDoneCallback::operator=(QueueWorkDoneCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void QueueWorkDoneCallback::operator()(wgpu::QueueWorkDoneStatus status, wgpu::StringView message) const { if (data) data->invoke(status, message); }
void QueueWorkDoneCallback::operator()(WGPUQueueWorkDoneStatus status, WGPUStringView message) const { if (data) data->invoke_c(status, message); }
struct RequestAdapterCallbackControlNative : RequestAdapterCallback::Control {
    WGPURequestAdapterCallback native;
    void* userdata1;
    void* userdata2;
    RequestAdapterCallbackControlNative(WGPURequestAdapterCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::RequestAdapterStatus status, wgpu::Adapter const& adapter, wgpu::StringView message) const override {}
    void invoke_c(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message) const override;
};
void RequestAdapterCallbackControlNative::invoke_c(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message) const {
    native(status, adapter, message, userdata1, userdata2);
}
RequestAdapterCallback::RequestAdapterCallback(WGPURequestAdapterCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new RequestAdapterCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void RequestAdapterCallback::Control::invoke_c(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message) const {
    invoke(static_cast<wgpu::RequestAdapterStatus>(status), *reinterpret_cast<wgpu::Adapter const*>(&adapter), static_cast<wgpu::StringView>(message));
}
RequestAdapterCallback& RequestAdapterCallback::operator=(const RequestAdapterCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
RequestAdapterCallback& RequestAdapterCallback::operator=(RequestAdapterCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void RequestAdapterCallback::operator()(wgpu::RequestAdapterStatus status, wgpu::Adapter const& adapter, wgpu::StringView message) const { if (data) data->invoke(status, adapter, message); }
void RequestAdapterCallback::operator()(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message) const { if (data) data->invoke_c(status, adapter, message); }
struct RequestDeviceCallbackControlNative : RequestDeviceCallback::Control {
    WGPURequestDeviceCallback native;
    void* userdata1;
    void* userdata2;
    RequestDeviceCallbackControlNative(WGPURequestDeviceCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::RequestDeviceStatus status, wgpu::Device const& device, wgpu::StringView message) const override {}
    void invoke_c(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message) const override;
};
void RequestDeviceCallbackControlNative::invoke_c(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message) const {
    native(status, device, message, userdata1, userdata2);
}
RequestDeviceCallback::RequestDeviceCallback(WGPURequestDeviceCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new RequestDeviceCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void RequestDeviceCallback::Control::invoke_c(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message) const {
    invoke(static_cast<wgpu::RequestDeviceStatus>(status), *reinterpret_cast<wgpu::Device const*>(&device), static_cast<wgpu::StringView>(message));
}
RequestDeviceCallback& RequestDeviceCallback::operator=(const RequestDeviceCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
RequestDeviceCallback& RequestDeviceCallback::operator=(RequestDeviceCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void RequestDeviceCallback::operator()(wgpu::RequestDeviceStatus status, wgpu::Device const& device, wgpu::StringView message) const { if (data) data->invoke(status, device, message); }
void RequestDeviceCallback::operator()(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message) const { if (data) data->invoke_c(status, device, message); }
struct UncapturedErrorCallbackControlNative : UncapturedErrorCallback::Control {
    WGPUUncapturedErrorCallback native;
    void* userdata1;
    void* userdata2;
    UncapturedErrorCallbackControlNative(WGPUUncapturedErrorCallback n, void* userdata1, void* userdata2) : native(n), userdata1(userdata1), userdata2(userdata2) {}
    void invoke(wgpu::Device const& device, wgpu::ErrorType type, wgpu::StringView message) const override {}
    void invoke_c(WGPUDevice const* device, WGPUErrorType type, WGPUStringView message) const override;
};
void UncapturedErrorCallbackControlNative::invoke_c(WGPUDevice const* device, WGPUErrorType type, WGPUStringView message) const {
    native(device, type, message, userdata1, userdata2);
}
UncapturedErrorCallback::UncapturedErrorCallback(WGPUUncapturedErrorCallback native, void* userdata1, void* userdata2) {
    if (native) {
        data = new UncapturedErrorCallbackControlNative(native, userdata1, userdata2);
    } else {
        data = nullptr;
    }
}
void UncapturedErrorCallback::Control::invoke_c(WGPUDevice const* device, WGPUErrorType type, WGPUStringView message) const {
    invoke(*reinterpret_cast<wgpu::Device const*>(device), static_cast<wgpu::ErrorType>(type), static_cast<wgpu::StringView>(message));
}
UncapturedErrorCallback& UncapturedErrorCallback::operator=(const UncapturedErrorCallback& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        if (data) ++data->count;
    }
    return *this;
}
UncapturedErrorCallback& UncapturedErrorCallback::operator=(UncapturedErrorCallback&& other) {
    if (this != &other) {
        if (data && --data->count == 0) { delete data; }
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
void UncapturedErrorCallback::operator()(wgpu::Device const& device, wgpu::ErrorType type, wgpu::StringView message) const { if (data) data->invoke(device, type, message); }
void UncapturedErrorCallback::operator()(WGPUDevice const* device, WGPUErrorType type, WGPUStringView message) const { if (data) data->invoke_c(device, type, message); }
}
namespace wgpu {
wgpu::Instance createInstance(wgpu::InstanceDescriptor const& descriptor) {
    wgpu::InstanceDescriptor::CStruct descriptor_cstruct = descriptor.to_cstruct();
    wgpu::Instance res = static_cast<wgpu::Instance>(wgpuCreateInstance(&descriptor_cstruct));
    return res;
}
wgpu::Instance createInstance() {
    wgpu::Instance res = static_cast<wgpu::Instance>(wgpuCreateInstance(nullptr));
    return res;
}
void getInstanceFeatures(wgpu::SupportedInstanceFeatures& features) {
    WGPUSupportedInstanceFeatures features_native;
    wgpuGetInstanceFeatures(&features_native);
    features = static_cast<wgpu::SupportedInstanceFeatures>(features_native);
    wgpuSupportedInstanceFeaturesFreeMembers(features_native);
}
wgpu::Status getInstanceLimits(wgpu::InstanceLimits& limits) {
    WGPUInstanceLimits limits_native;
    wgpu::Status res = static_cast<wgpu::Status>(wgpuGetInstanceLimits(&limits_native));
    limits = static_cast<wgpu::InstanceLimits>(limits_native);
    return res;
}
wgpu::Bool hasInstanceFeature(wgpu::InstanceFeatureName feature) {
    wgpu::Bool res = static_cast<wgpu::Bool>(wgpuHasInstanceFeature(static_cast<WGPUInstanceFeatureName>(feature)));
    return res;
}
}
