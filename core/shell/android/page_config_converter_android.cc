// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.
#include "core/shell/android/page_config_converter_android.h"

#include <memory>

#include "core/template_bundle/template_codec/binary_decoder/page_config_constants.h"
namespace lynx {
namespace shell {

base::android::JavaOnlyMap ConvertPageConfigToJavaOnlyMap(
    const std::shared_ptr<tasm::PageConfig>& config) {
  base::android::JavaOnlyMap map;
  // TODO(yangguangzhao.solace):: Generate push logic by script
  // Since code will be automatically generated, use config->GetXXX() !=
  // default_value instead of if (!config->GetXXX())
  if (config->GetAutoExpose() != true) {
    map.PushBoolean(tasm::config::kAutoExpose, config->GetAutoExpose());
  }
  if (config->GetVersion() != "") {
    map.PushString(tasm::config::kPageVersion, config->GetVersion());
  }
  if (config->GetEnableEventThrough() != false) {
    map.PushBoolean(tasm::config::kEnableEventThrough,
                    config->GetEnableEventThrough());
  }
  auto version = lynx::base::Version(config->GetTargetSDKVersion());
  if (config->GetAsyncRedirectUrl() != tasm::TernaryBool::UNDEFINE_VALUE) {
    map.PushBoolean(
        tasm::config::kAsyncRedirectUrl,
        (config->GetAsyncRedirectUrl() == tasm::TernaryBool::TRUE_VALUE));
  }
  if (config->GetSyncImageAttach() != true) {
    map.PushBoolean(tasm::config::kSyncImageAttach,
                    config->GetSyncImageAttach());
  }
  if (config->GetEnableCheckLocalImage() != true) {
    map.PushBoolean(tasm::config::kEnableCheckLocalImage,
                    config->GetEnableCheckLocalImage());
  }
  if (config->GetUseImagePostProcessor() != false) {
    map.PushBoolean(tasm::config::kUseImagePostProcessor,
                    config->GetUseImagePostProcessor());
  }
  if (config->GetDSL() != tasm::PackageInstanceDSL::TT) {
    map.PushString(tasm::config::kPageType, GetDSLName(config->GetDSL()));
  }
  if (config->GetUseNewSwiper() != true) {
    map.PushBoolean(tasm::config::kUseNewSwiper, config->GetUseNewSwiper());
  }
  if (config->GetEnableAsyncInitTTVideoEngine() != false) {
    map.PushBoolean(tasm::config::kEnableAsyncInitVideoEngine,
                    config->GetEnableAsyncInitTTVideoEngine());
  }
  if (config->GetTargetSDKVersion() != "") {
    map.PushString(tasm::config::kTargetSdkVersion,
                   config->GetTargetSDKVersion());
  }
  if (config->GetEnableNewGesture() != false) {
    map.PushBoolean(tasm::config::kEnableNewGesture,
                    config->GetEnableNewGesture());
  }
  if (config->GetEnableLepusNG() != true) {
    map.PushBoolean(tasm::config::kEnableLepusNg, config->GetEnableLepusNG());
  }
  if (config->GetTapSlop() != "") {
    map.PushString(tasm::config::kTapSlop, config->GetTapSlop());
  }
  if (config->GetDefaultOverflowVisible() != false) {
    map.PushBoolean(tasm::config::kDefaultOverflowVisible,
                    config->GetDefaultOverflowVisible());
  }
  if (config->GetEnableScrollFluencyMonitor() != -1) {
    map.PushDouble(tasm::config::kEnableLynxScrollFluency,
                   config->GetEnableScrollFluencyMonitor());
  }
  if (config->GetEnableCreateViewAsync() != true) {
    map.PushBoolean(tasm::config::kEnableCreateViewAsync,
                    config->GetEnableCreateViewAsync());
  }
  if (config->GetEnableVsyncAlignedFlush() != false) {
    map.PushBoolean(tasm::config::kEnableVsyncAlignedFlush,
                    config->GetEnableVsyncAlignedFlush());
  }
  if (config->GetCSSAlignWithLegacyW3C() != false) {
    map.PushBoolean(tasm::config::kCSSAlignWithLegacyW3C,
                    config->GetCSSAlignWithLegacyW3C());
  }
  if (config->GetEnableAccessibilityElement() != true) {
    map.PushBoolean(tasm::config::kEnableAccessibilityElement,
                    config->GetEnableAccessibilityElement());
  }
  if (config->GetEnableOverlapForAccessibilityElement() != true) {
    map.PushBoolean(tasm::config::kEnableOverlapForAccessibilityElement,
                    config->GetEnableOverlapForAccessibilityElement());
  }
  if (config->GetEnableNewAccessibility() != false) {
    map.PushBoolean(tasm::config::kEnableNewAccessibility,
                    config->GetEnableNewAccessibility());
  }
  if (config->GetEnableA11yIDMutationObserver() != false) {
    map.PushBoolean(tasm::config::kEnableA11yIDMutationObserver,
                    config->GetEnableA11yIDMutationObserver());
  }
  if (config->GetEnableA11y() != false) {
    map.PushBoolean(tasm::config::kEnableA11y, config->GetEnableA11y());
  }
  if (config->GetEnableTextRefactor() != false) {
    map.PushBoolean(tasm::config::kEnableTextRefactor,
                    config->GetEnableTextRefactor());
  }
  if (config->GetEnableTextOverflow() != false) {
    map.PushBoolean(tasm::config::kEnableTextOverflow,
                    config->GetEnableTextOverflow());
  }
  if (config->GetEnableTextBoringLayout() !=
      tasm::TernaryBool::UNDEFINE_VALUE) {
    map.PushBoolean(
        tasm::config::kEnableTextBoringLayout,
        (config->GetEnableTextBoringLayout() == tasm::TernaryBool::TRUE_VALUE));
  }
  if (config->GetEnableNewClipMode() != false) {
    map.PushBoolean(tasm::config::kEnableNewClipMode,
                    config->GetEnableNewClipMode());
  }
  if (config->GetKeyboardCallbackUseRelativeHeight() != false) {
    map.PushBoolean(tasm::config::kKeyboardCallbackPassRelativeHeight,
                    config->GetKeyboardCallbackUseRelativeHeight());
  }
  if (config->GetEnableEventRefactor() != true) {
    map.PushBoolean(tasm::config::kEnableEventRefactor,
                    config->GetEnableEventRefactor());
  }
  if (config->GetEnableDisexposureWhenLynxHidden() != true) {
    map.PushBoolean(tasm::config::kEnableDisexposureWhenLynxHidden,
                    config->GetEnableDisexposureWhenLynxHidden());
  }
  if (config->GetEnableExposureWhenLayout() != false) {
    map.PushBoolean(tasm::config::kEnableExposureWhenLayout,
                    config->GetEnableExposureWhenLayout());
  }
  if (config->GetEnableNewIntersectionObserver() != false) {
    map.PushBoolean(tasm::config::kEnableNewIntersectionObserver,
                    config->GetEnableNewIntersectionObserver());
  }
  if (config->GetObserverFrameRate() != 20) {
    map.PushInt(tasm::config::kObserverFrameRate,
                config->GetObserverFrameRate());
  }
  if (config->GetEnableExposureUIMargin() != false) {
    map.PushBoolean(tasm::config::kEnableExposureUIMargin,
                    config->GetEnableExposureUIMargin());
  }
  if (config->GetLongPressDuration() != -1) {
    map.PushInt(tasm::config::kLongPressDuration,
                config->GetLongPressDuration());
  }
  if (config->GetMapContainerType() != 0) {
    map.PushInt(tasm::config::kMapContainerType, config->GetMapContainerType());
  }
  if (config->GetIncludeFontPadding() != 0) {
    map.PushBoolean(tasm::config::kIncludeFontPadding,
                    config->GetIncludeFontPadding() == 1);
  } else if (version >= LYNX_VERSION_2_4 && version < LYNX_VERSION_2_9) {
    // for history reason
    map.PushBoolean(tasm::config::kIncludeFontPadding, true);
  }
  if (version < LYNX_VERSION_2_5) {
    map.PushBoolean(tasm::config::kEnableFlattenTranslateZ, false);
  }
  if (config->GetEnableFiberArch() != false) {
    map.PushBoolean(tasm::config::kEnableFiber, config->GetEnableFiberArch());
  }
  if (config->GetEnableMultiTouch() != false) {
    map.PushBoolean(tasm::config::kEnableMultiTouch,
                    config->GetEnableMultiTouch());
  }
  return map;
}

}  // namespace shell
}  // namespace lynx
