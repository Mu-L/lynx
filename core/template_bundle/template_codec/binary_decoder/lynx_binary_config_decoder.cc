// Copyright 2022 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "core/template_bundle/template_codec/binary_decoder/lynx_binary_config_decoder.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "core/public/prop_bundle.h"
#include "core/renderer/utils/lynx_env.h"
#include "core/runtime/piper/js/runtime_constant.h"
#include "core/runtime/vm/lepus/json_parser.h"
#include "core/services/event_report/event_tracker.h"
#include "core/template_bundle/template_codec/binary_decoder/page_config_constants.h"

namespace lynx {
namespace tasm {

bool LynxBinaryConfigDecoder::DecodePageConfig(
    const std::string& config_str, std::shared_ptr<PageConfig>& page_config) {
  rapidjson::Document doc;
  if (doc.Parse(config_str.c_str()).HasParseError()) {
    LOGE("DecodePageConfig Error!");
    return false;
  }

  if (doc.HasMember(TEMPLATE_BUNDLE_MODULE_MODE) &&
      doc[TEMPLATE_BUNDLE_MODULE_MODE].IsInt()) {
    int bundleModuleModeInt = doc[TEMPLATE_BUNDLE_MODULE_MODE].GetInt();
    PackageInstanceBundleModuleMode bundleModuleMode =
        static_cast<PackageInstanceBundleModuleMode>(bundleModuleModeInt);
    if (bundleModuleMode ==
        PackageInstanceBundleModuleMode::RETURN_BY_FUNCTION_MODE) {
      page_config->SetBundleModuleMode(
          PackageInstanceBundleModuleMode::RETURN_BY_FUNCTION_MODE);
    } else {
      page_config->SetBundleModuleMode(
          PackageInstanceBundleModuleMode::EVAL_REQUIRE_MODE);
    }
  } else {
    page_config->SetBundleModuleMode(
        PackageInstanceBundleModuleMode::EVAL_REQUIRE_MODE);
  }

  if (doc.HasMember(config::kVersion) && doc[config::kVersion].IsString()) {
    page_config->SetVersion(doc[config::kVersion].GetString());
  }
  if (doc.HasMember(config::kFlatten) && doc[config::kFlatten].IsBool()) {
    page_config->SetGlobalFlattern(doc[config::kFlatten].GetBool());
  }

  if (doc.HasMember(config::kEnableA11yIDMutationObserver) &&
      doc[config::kEnableA11yIDMutationObserver].IsBool()) {
    page_config->SetEnableA11yIDMutationObserver(
        doc[config::kEnableA11yIDMutationObserver].GetBool());
  }

  if (doc.HasMember(config::kEnableA11y) && doc[config::kEnableA11y].IsBool()) {
    page_config->SetEnableA11y(doc[config::kEnableA11y].GetBool());
  }

  if (doc.HasMember(config::kEnableCascadePseudo) &&
      doc[config::kEnableCascadePseudo].IsBool()) {
    page_config->SetEnableCascadePseudo(
        doc[config::kEnableCascadePseudo].GetBool());
  }

  if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                          LYNX_VERSION_2_0)) {
    page_config->SetGlobalImplicit(false);
  }

  if (doc.HasMember(config::kImplicit) && doc[config::kImplicit].IsBool()) {
    page_config->SetGlobalImplicit(doc[config::kImplicit].GetBool());
  }

  if (doc.HasMember(config::kLepusCheck) && doc[config::kLepusCheck].IsBool()) {
    page_config->SetEnableLepusStrictCheck(doc[config::kLepusCheck].GetBool());
  }

  if (doc.HasMember(config::kLepusQuickjsStackSize) &&
      doc[config::kLepusQuickjsStackSize].IsUint()) {
    page_config->SetLepusQuickjsStackSize(
        doc[config::kLepusQuickjsStackSize].GetUint());
  }

  if (doc.HasMember(config::kNullPropAsUndef) &&
      doc[config::kNullPropAsUndef].IsBool()) {
    page_config->SetEnableLepusNullPropAsUndef(
        doc[config::kNullPropAsUndef].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_1_6)) {
    page_config->SetEnableLepusNullPropAsUndef(true);
  }

  if (doc.HasMember(TEMPLATE_BUNDLE_APP_DSL) &&
      doc[TEMPLATE_BUNDLE_APP_DSL].IsInt()) {
    page_config->SetDSL(
        static_cast<PackageInstanceDSL>(doc[TEMPLATE_BUNDLE_APP_DSL].GetInt()));
  }

  if (doc.HasMember(config::kAutoExpose) && doc[config::kAutoExpose].IsBool()) {
    page_config.get()->SetAutoExpose(doc[config::kAutoExpose].GetBool());
  }

  if (doc.HasMember(config::kDataStrictMode) &&
      doc[config::kDataStrictMode].IsBool()) {
    page_config.get()->SetDataStrictMode(
        doc[config::kDataStrictMode].GetBool());
  }

  if (doc.HasMember(config::kEnableFixedNew) &&
      doc[config::kEnableFixedNew].IsBool()) {
    page_config.get()->SetEnableFixedNew(
        doc[config::kEnableFixedNew].GetBool());
  } else {
    page_config.get()->SetEnableFixedNew(
        LynxEnv::GetInstance().EnableFixedNew());
  }

  if (doc.HasMember(config::kAbsoluteInContentBound) &&
      doc[config::kAbsoluteInContentBound].IsBool()) {
    page_config.get()->SetAbsoluteInContentBound(
        doc[config::kAbsoluteInContentBound].GetBool());
  }

  if (doc.HasMember(config::kQuirksMode) && doc[config::kQuirksMode].IsBool()) {
    if (!doc[config::kQuirksMode].GetBool()) {
      page_config.get()->SetQuirksModeByVersion(kQuirksModeDisableVersion);
    }
  } else if (doc.HasMember(config::kQuirksMode) &&
             doc[config::kQuirksMode].IsString()) {
    page_config.get()->SetQuirksModeByVersion(
        base::Version(doc[config::kQuirksMode].GetString()));
  } else if ((lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                  kQuirksModeDisableVersion))) {
    page_config.get()->SetQuirksModeByVersion(
        base::Version(target_sdk_version_));
  }

  if (doc.HasMember(config::kEnableAsyncDisplay) &&
      doc[config::kEnableAsyncDisplay].IsBool()) {
    page_config.get()->SetEnableAsyncDisplay(
        doc[config::kEnableAsyncDisplay].GetBool());
  }

  if (doc.HasMember(config::kEnableImageDownsampling) &&
      doc[config::kEnableImageDownsampling].IsBool()) {
    page_config.get()->SetEnableImageDownsampling(
        doc[config::kEnableImageDownsampling].GetBool());
  }

  if (doc.HasMember(config::kEnableNewImage) &&
      doc[config::kEnableNewImage].IsBool()) {
    page_config.get()->SetEnableNewImage(
        doc[config::kEnableNewImage].GetBool());
  }

  if (doc.HasMember(config::kEnableTextLanguageAlignment) &&
      doc[config::kEnableTextLanguageAlignment].IsBool()) {
    page_config.get()->SetEnableTextLanguageAlignment(
        doc[config::kEnableTextLanguageAlignment].GetBool());
  }
  if (doc.HasMember(config::kEnableXTextLayoutReused) &&
      doc[config::kEnableXTextLayoutReused].IsBool()) {
    page_config.get()->SetEnableXTextLayoutReused(
        doc[config::kEnableXTextLayoutReused].GetBool());
  }

  if (doc.HasMember(config::kLogBoxImageSizeWarningThreshold) &&
      doc[config::kLogBoxImageSizeWarningThreshold].IsInt()) {
    page_config.get()->SetLogBoxImageSizeWarningThreshold(
        doc[config::kLogBoxImageSizeWarningThreshold].GetInt());
  }

  if (doc.HasMember(config::kEnableTextNonContiguousLayout) &&
      doc[config::kEnableTextNonContiguousLayout].IsBool()) {
    page_config->SetEnableTextNonContiguousLayout(
        doc[config::kEnableTextNonContiguousLayout].GetBool());
  }

  if (doc.HasMember(config::kEnableViewReceiveTouch) &&
      doc[config::kEnableViewReceiveTouch].IsBool()) {
    page_config.get()->SetEnableViewReceiveTouch(
        doc[config::kEnableViewReceiveTouch].GetBool());
  }

  if (doc.HasMember(config::kEnableEventThrough) &&
      doc[config::kEnableEventThrough].IsBool()) {
    page_config->SetEnableEventThrough(
        doc[config::kEnableEventThrough].GetBool());
  }

  if (doc.HasMember(config::kRemoveComponentElement) &&
      doc[config::kRemoveComponentElement].IsBool()) {
    page_config->SetRemoveComponentElement(
        doc[config::kRemoveComponentElement].GetBool());
  }

  if (doc.HasMember(config::kStrictPropType) &&
      doc[config::kStrictPropType].IsBool()) {
    page_config->SetStrictPropType(doc[config::kStrictPropType].GetBool());
  }

  if (doc.HasMember(config::kEnableCSSInheritance) &&
      doc[config::kEnableCSSInheritance].IsBool()) {
    page_config->SetEnableCSSInheritance(
        doc[config::kEnableCSSInheritance].GetBool());
  }

  if (doc.HasMember(config::kCustomCSSInheritanceList) &&
      doc[config::kCustomCSSInheritanceList].IsArray()) {
    std::unordered_set<CSSPropertyID> inherit_list;
    const auto& names_array = doc[config::kCustomCSSInheritanceList].GetArray();
    inherit_list.reserve(names_array.Size());
    for (const auto& entry : names_array) {
      if (entry.IsString()) {
        inherit_list.insert(
            tasm::CSSProperty::GetPropertyID(entry.GetString()));
      }
    }
    page_config->SetCustomCSSInheritList(std::move(inherit_list));
  }

  if (doc.HasMember(config::kCSSAlignWithLegacyW3C) &&
      doc[config::kCSSAlignWithLegacyW3C].IsBool()) {
    page_config->SetCSSAlignWithLegacyW3C(
        doc[config::kCSSAlignWithLegacyW3C].GetBool());
  }

  if (doc.HasMember(config::kUseNewImage) &&
      doc[config::kUseNewImage].IsBool()) {
    page_config->SetUseNewImage(doc[config::kUseNewImage].GetBool()
                                    ? TernaryBool::TRUE_VALUE
                                    : TernaryBool::FALSE_VALUE);
  }

  if (doc.HasMember(config::kSyncImageAttach) &&
      doc[config::kSyncImageAttach].IsBool()) {
    page_config->SetSyncImageAttach(doc[config::kSyncImageAttach].GetBool());
  }

  if (doc.HasMember(config::kUseImagePostProcessor) &&
      doc[config::kUseImagePostProcessor].IsBool()) {
    page_config->SetUseImagePostProcessor(
        doc[config::kUseImagePostProcessor].GetBool());
  }

  if (doc.HasMember(config::kUseNewSwiper) &&
      doc[config::kUseNewSwiper].IsBool()) {
    page_config->SetUseNewSwiper(doc[config::kUseNewSwiper].GetBool());
  }

  if (doc.HasMember(config::kEnableAsyncInitVideoEngine) &&
      doc[config::kEnableAsyncInitVideoEngine].IsBool()) {
    page_config->SetEnableAsyncInitTTVideoEngine(
        doc[config::kEnableAsyncInitVideoEngine].GetBool());
  }

  if (doc.HasMember(config::kCliVersion) &&
      doc[config::kCliVersion].IsString()) {
    page_config->SetCliVersion(doc[config::kCliVersion].GetString());
  }

  if (doc.HasMember(config::kReactVersion) &&
      doc[config::kReactVersion].IsString()) {
    page_config->SetReactVersion(doc[config::kReactVersion].GetString());
  }

  if (doc.HasMember(config::kCustomData) &&
      doc[config::kCustomData].IsString()) {
    page_config->SetCustomData(doc[config::kCustomData].GetString());
  }

  if (doc.HasMember(config::kEnableComponentLifecycleAlignWebview) &&
      doc[config::kEnableComponentLifecycleAlignWebview].IsBool()) {
    page_config->SetEnableComponentLifecycleAlignWebview(
        doc[config::kEnableComponentLifecycleAlignWebview].GetBool());
  }

  if (doc.HasMember(config::kEnableListNewArchitecture) &&
      doc[config::kEnableListNewArchitecture].IsBool()) {
    page_config->SetListNewArchitecture(
        doc[config::kEnableListNewArchitecture].GetBool());
  }

  if (doc.HasMember(config::kEnableListNewContainer) &&
      doc[config::kEnableListNewContainer].IsBool()) {
    page_config->SetEnableNewListContainer(
        doc[config::kEnableListNewContainer].GetBool());
  }

  if (doc.HasMember(config::kEnableListPlug) &&
      doc[config::kEnableListPlug].IsBool()) {
    page_config->SetEnableListPlug(doc[config::kEnableListPlug].GetBool());
  }

  if (doc.HasMember(config::kEnableListMoveOperation) &&
      doc[config::kEnableListMoveOperation].IsBool()) {
    page_config->SetEnableListMoveOperation(
        doc[config::kEnableListMoveOperation].GetBool());
  }

  if (doc.HasMember(config::kEnableCSSStrictMode) &&
      doc[config::kEnableCSSStrictMode].IsBool()) {
    page_config->SetEnableCSSStrictMode(
        doc[config::kEnableCSSStrictMode].GetBool());
  }

  if (doc.HasMember(config::kTapSlop) && doc[config::kTapSlop].IsString()) {
    page_config->SetTapSlop(doc[config::kTapSlop].GetString());
  } else {
    page_config->SetTapSlop(config::kDefaultTapSlop);
  }

  if (doc.HasMember(config::kEnableCreateViewAsync) &&
      doc[config::kEnableCreateViewAsync].IsBool()) {
    page_config->SetEnableCreateViewAsync(
        doc[config::kEnableCreateViewAsync].GetBool());
  }

  if (doc.HasMember(config::kEnableVsyncAlignedFlush) &&
      doc[config::kEnableVsyncAlignedFlush].IsBool()) {
    page_config->SetEnableVsyncAlignedFlush(
        doc[config::kEnableVsyncAlignedFlush].GetBool());
  }

  if (doc.HasMember(config::kEnableAccessibilityElement) &&
      doc[config::kEnableAccessibilityElement].IsBool()) {
    page_config->SetEnableAccessibilityElement(
        doc[config::kEnableAccessibilityElement].GetBool());
  }

  if (doc.HasMember(config::kEnableOverlapForAccessibilityElement) &&
      doc[config::kEnableOverlapForAccessibilityElement].IsBool()) {
    page_config->SetEnableOverlapForAccessibilityElement(
        doc[config::kEnableOverlapForAccessibilityElement].GetBool());
  }

  if (doc.HasMember(config::kEnableNewAccessibility) &&
      doc[config::kEnableNewAccessibility].IsBool()) {
    page_config->SetEnableNewAccessibility(
        doc[config::kEnableNewAccessibility].GetBool());
  }
  if (doc.HasMember(config::kEnableNewLayoutOnly) &&
      doc[config::kEnableNewLayoutOnly].IsBool()) {
    page_config->SetEnableNewLayoutOnly(
        doc[config::kEnableNewLayoutOnly].GetBool());
  }

  if (doc.HasMember(config::kEnableReactOnlyPropsId) &&
      doc[config::kEnableReactOnlyPropsId].IsBool()) {
    page_config->SetEnableReactOnlyPropsId(
        doc[config::kEnableReactOnlyPropsId].GetBool());
  }

  if (doc.HasMember(config::kEnableGlobalComponentMap) &&
      doc[config::kEnableGlobalComponentMap].IsBool()) {
    page_config->SetEnableGlobalComponentMap(
        doc[config::kEnableGlobalComponentMap].GetBool());
  }

  if (doc.HasMember(config::kEnableRemoveComponentExtraData) &&
      doc[config::kEnableRemoveComponentExtraData].IsBool()) {
    page_config->SetEnableRemoveComponentExtraData(
        doc[config::kEnableRemoveComponentExtraData].GetBool());
  }

  if (doc.HasMember(config::kEnableTextRefactor) &&
      doc[config::kEnableTextRefactor].IsBool()) {
    page_config->SetEnableTextRefactor(
        doc[config::kEnableTextRefactor].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_2)) {
    page_config->SetEnableTextRefactor(true);
  }

  if (doc.HasMember(config::kEnableTextOverflow) &&
      doc[config::kEnableTextOverflow].IsBool()) {
    page_config->SetEnableTextOverflow(
        doc[config::kEnableTextOverflow].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_8)) {
    page_config->SetEnableTextOverflow(true);
  }

  if (doc.HasMember(config::kEnableTextBoringLayout) &&
      doc[config::kEnableTextBoringLayout].IsBool()) {
    page_config->SetEnableTextBoringLayout(
        doc[config::kEnableTextBoringLayout].GetBool()
            ? TernaryBool::TRUE_VALUE
            : TernaryBool::FALSE_VALUE);
  }

  if (doc.HasMember(config::kEnableNewClipMode) &&
      doc[config::kEnableNewClipMode].IsBool()) {
    page_config->SetEnableNewClipMode(
        doc[config::kEnableNewClipMode].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_10)) {
    page_config->SetEnableNewClipMode(true);
  }

  if (doc.HasMember(config::kAutoResumeAnimation) &&
      doc[config::kAutoResumeAnimation].IsBool()) {
    page_config->SetGlobalAutoResumeAnimation(
        doc[config::kAutoResumeAnimation].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_3)) {
    page_config->SetGlobalAutoResumeAnimation(true);
  }

  if (doc.HasMember(config::kEnableNewTransformOrigin) &&
      doc[config::kEnableNewTransformOrigin].IsBool()) {
    page_config->SetGlobalEnableNewTransformOrigin(
        doc[config::kEnableNewTransformOrigin].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_6)) {
    page_config->SetGlobalEnableNewTransformOrigin(true);
  }

  if (doc.HasMember(config::kEnableCircularDataCheck) &&
      doc[config::kEnableCircularDataCheck].IsBool()) {
    page_config->SetGlobalCircularDataCheck(
        doc[config::kEnableCircularDataCheck].GetBool());
  }

  if (doc.HasMember(config::kEnableTextLayerRender) &&
      doc[config::kEnableTextLayerRender].IsBool()) {
    page_config->SetEnableTextLayerRender(
        doc[config::kEnableTextLayerRender].GetBool()
            ? TernaryBool::TRUE_VALUE
            : TernaryBool::FALSE_VALUE);
  }

  if (doc.HasMember(config::kEnableReduceInitDataCopy) &&
      doc[config::kEnableReduceInitDataCopy].IsBool()) {
    page_config->SetEnableReduceInitDataCopy(
        doc[config::kEnableReduceInitDataCopy].GetBool());
  }

  if (doc.HasMember(config::kUnifyVWVHBehavior) &&
      doc[config::kUnifyVWVHBehavior].IsBool()) {
    page_config->SetUnifyVWVH(doc[config::kUnifyVWVHBehavior].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_3)) {
    page_config->SetUnifyVWVH(true);
  }

  if (doc.HasMember(config::kFontScaleEffectiveOnlyOnSp) &&
      doc[config::kFontScaleEffectiveOnlyOnSp].IsBool()) {
    page_config->SetFontScaleSpOnly(
        doc[config::kFontScaleEffectiveOnlyOnSp].GetBool());
  }

  if (doc.HasMember(config::kEnableSimultaneousTap) &&
      doc[config::kEnableSimultaneousTap].IsBool()) {
    page_config->SetEnableSimultaneousTap(
        doc[config::kEnableSimultaneousTap].GetBool());
  }

  if (doc.HasMember(config::kEnableComponentLayoutOnly) &&
      doc[config::kEnableComponentLayoutOnly].IsBool()) {
    page_config->SetEnableComponentLayoutOnly(
        doc[config::kEnableComponentLayoutOnly].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_6)) {
    page_config->SetEnableComponentLayoutOnly(true);
  }

  if (doc.HasMember(config::kExtendedLayoutOnlyOpt) &&
      doc[config::kExtendedLayoutOnlyOpt].IsBool()) {
    page_config->SetEnableExtendedLayoutOpt(
        doc[config::kExtendedLayoutOnlyOpt].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_13)) {
    page_config->SetEnableExtendedLayoutOpt(true);
  }

  if (doc.HasMember(config::kEnableTouchRefactor) &&
      doc[config::kEnableTouchRefactor].IsBool()) {
    page_config->SetEnableTouchRefactor(
        doc[config::kEnableTouchRefactor].GetBool());
  }

  if (doc.HasMember(config::kEnableEndGestureAtLastFingerUp) &&
      doc[config::kEnableEndGestureAtLastFingerUp].IsBool()) {
    page_config->SetEnableEndGestureAtLastFingerUp(
        doc[config::kEnableEndGestureAtLastFingerUp].GetBool());
  }

  if (doc.HasMember(config::kDisableLongpressAfterScroll) &&
      doc[config::kDisableLongpressAfterScroll].IsBool()) {
    page_config->SetDisableLongpressAfterScroll(
        doc[config::kDisableLongpressAfterScroll].GetBool());
  }

  if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                          LYNX_VERSION_2_0) &&
      compile_options_.default_overflow_visible_) {
    page_config->SetDefaultOverflowVisible(true);
  }

  if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                          LYNX_VERSION_2_2) &&
      compile_options_.default_display_linear_) {
    page_config->SetDefaultDisplayLinear(true);
  }

  if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                          LYNX_VERSION_2_3)) {
    page_config->SetEnableZIndex(true);
  }

  if (doc.HasMember(config::kKeyboardCallbackPassRelativeHeight) &&
      doc[config::kKeyboardCallbackPassRelativeHeight].IsBool()) {
    page_config->SetKeyboardCallbackUseRelativeHeight(
        doc[config::kKeyboardCallbackPassRelativeHeight].GetBool());
  } else if (lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                 LYNX_VERSION_2_2)) {
    page_config->SetKeyboardCallbackUseRelativeHeight(true);
  }

  if (doc.HasMember(config::kEnableNewIntersectionObserver) &&
      doc[config::kEnableNewIntersectionObserver].IsBool()) {
    page_config->SetEnableNewIntersectionObserver(
        doc[config::kEnableNewIntersectionObserver].GetBool());
  } else {
    page_config->SetEnableNewIntersectionObserver(
        LynxEnv::GetInstance().EnableNewIntersectionObserver());
  }

  if (doc.HasMember(config::kObserverFrameRate) &&
      doc[config::kObserverFrameRate].IsInt()) {
    page_config->SetObserverFrameRate(doc[config::kObserverFrameRate].GetInt());
  }

  if (doc.HasMember(config::kEnableCheckExposureOptimize) &&
      doc[config::kEnableCheckExposureOptimize].IsBool()) {
    page_config->SetEnableCheckExposureOptimize(
        doc[config::kEnableCheckExposureOptimize].GetBool());
  }

  if (doc.HasMember(config::kEnableDisexposureWhenLynxHidden) &&
      doc[config::kEnableDisexposureWhenLynxHidden].IsBool()) {
    page_config->SetEnableDisexposureWhenLynxHidden(
        doc[config::kEnableDisexposureWhenLynxHidden].GetBool());
  }

  if (doc.HasMember(config::kEnableExposureWhenLayout) &&
      doc[config::kEnableExposureWhenLayout].IsBool()) {
    page_config->SetEnableExposureWhenLayout(
        doc[config::kEnableExposureWhenLayout].GetBool());
  }

  if (doc.HasMember(config::kEnableAirDetectRemovedKeysWhenUpdateData) &&
      doc[config::kEnableAirDetectRemovedKeysWhenUpdateData].IsBool()) {
    page_config->SetEnableAirDetectRemovedKeysWhenUpdateData(
        doc[config::kEnableAirDetectRemovedKeysWhenUpdateData].GetBool());
  }

  if (doc.HasMember(config::kEnableExposureUIMargin) &&
      doc[config::kEnableExposureUIMargin].IsBool()) {
    page_config->SetEnableExposureUIMargin(
        doc[config::kEnableExposureUIMargin].GetBool());
  }

  // parse enable new gesture config
  if (doc.HasMember(config::kEnableNewGesture) &&
      doc[config::kEnableNewGesture].IsBool()) {
    page_config->SetEnableNewGesture(doc[config::kEnableNewGesture].GetBool());
  }

  if (doc.HasMember(config::kLongPressDuration) &&
      doc[config::kLongPressDuration].IsInt()) {
    page_config->SetLongPressDuration(doc[config::kLongPressDuration].GetInt());
  }

  if (doc.HasMember(config::kMapContainerType) &&
      doc[config::kMapContainerType].IsInt()) {
    page_config->SetMapContainerType(doc[config::kMapContainerType].GetInt());
  }

  if (doc.HasMember(config::kEnableCheckLocalImage) &&
      doc[config::kEnableCheckLocalImage].IsBool()) {
    page_config->SetEnableCheckLocalImage(
        doc[config::kEnableCheckLocalImage].GetBool());
  }

  if (doc.HasMember(config::kEnableAsyncRequestImage) &&
      doc[config::kEnableAsyncRequestImage].IsBool()) {
    page_config->SetEnableAsyncRequestImage(
        doc[config::kEnableAsyncRequestImage].GetBool());
  }

  if (doc.HasMember(config::kEnableCheckDataWhenUpdatePage) &&
      doc[config::kEnableCheckDataWhenUpdatePage].IsBool()) {
    page_config->SetEnableCheckDataWhenUpdatePage(
        doc[config::kEnableCheckDataWhenUpdatePage].GetBool());
  }

  if (doc.HasMember(config::kEnableJSDataProcessor) &&
      doc[config::kEnableJSDataProcessor].IsBool()) {
    page_config->SetEnableDataProcessorOnJs(
        doc[config::kEnableJSDataProcessor].GetBool());
  }

  if (doc.HasMember(config::kEnableMultiTouch) &&
      doc[config::kEnableMultiTouch].IsBool()) {
    page_config->SetEnableMultiTouch(doc[config::kEnableMultiTouch].GetBool());
  } else {
    page_config->SetEnableMultiTouch(LynxEnv::GetInstance().EnableMultiTouch());
  }

  if (doc.HasMember(config::kEnableNativeList) &&
      doc[config::kEnableNativeList].IsBool()) {
    page_config->SetEnableNativeList(doc[config::kEnableNativeList].GetBool());
  }

  page_config->SetTargetSDKVersion(target_sdk_version_);
  page_config->SetEnableLepusNG(is_lepusng_binary_);

  // engineVersion > 2.1 && enableKeepPageData ON.
  page_config->SetEnableSavePageData(
      Config::IsHigherOrEqual(compile_options_.target_sdk_version_,
                              FEATURE_NEW_RENDER_PAGE) &&
      compile_options_.enable_keep_page_data);

  page_config->SetEnableLynxAir(compile_options_.enable_lynx_air_);
  page_config->SetEnableFiberArch(compile_options_.enable_fiber_arch_);
  page_config->SetEnableCSSParser(enable_css_parser_);
  page_config->SetAbSettingDisableCSSLazyDecode(
      absetting_disable_css_lazy_decode_);

  // if enable_event_refactor == enabled, set page config's
  // enable_event_refactor_ as true.
  page_config->SetEnableEventRefactor(
      compile_options_.enable_event_refactor_ == FE_OPTION_ENABLE ||
      compile_options_.enable_event_refactor_ == FE_OPTION_UNDEFINED);

  page_config->SetEnableCSSInvalidation(
      compile_options_.enable_css_invalidation_);

  page_config->SetLynxAirMode(
      CompileOptionAirMode(compile_options_.lynx_air_mode_));

  if (compile_options_.force_calc_new_style_ != FE_OPTION_UNDEFINED) {
    page_config->SetForceCalcNewStyle(compile_options_.force_calc_new_style_ !=
                                      FE_OPTION_DISABLE);
  } else {
    if (doc.HasMember(config::kForceCalcNewStyleKey) &&
        doc[config::kForceCalcNewStyleKey].IsBool()) {
      page_config.get()->SetForceCalcNewStyle(
          doc[config::kForceCalcNewStyleKey].GetBool());
    }
  }

  // include font padding
  if (doc.HasMember(config::kIncludeFontPadding) &&
      doc[config::kIncludeFontPadding].IsBool()) {
    page_config->SetIncludeFontPadding(
        doc[config::kIncludeFontPadding].GetBool());
  }

  if (doc.HasMember(config::kEnableBackgroundShapeLayer) &&
      doc[config::kEnableBackgroundShapeLayer].IsBool()) {
    page_config->SetEnableBackgroundShapeLayer(
        doc[config::kEnableBackgroundShapeLayer].GetBool());
  }

  // compile render
  if (doc.HasMember(config::kCompileRender) &&
      doc[config::kCompileRender].IsBool()) {
    page_config.get()->SetCompileRender(doc[config::kCompileRender].GetBool());
  }

  /**
   * @name: enableLynxResourceServiceProvider
   * @description: Enable LynxResourceService to fetch external resource
   * @note: None
   * @platform: Both
   * @supportVersion: 2.8
   **/
  if (doc.HasMember(config::kEnableLynxResourceServiceProvider) &&
      doc[config::kEnableLynxResourceServiceProvider].IsBool()) {
    page_config->SetEnableLynxResourceServiceProvider(
        doc[config::kEnableLynxResourceServiceProvider].GetBool());
  }

  /**
   * @name: extraInfo
   * @description: user defined extra info.
   * @note: None
   * @platform: Both
   * @supportVersion: 2.9
   **/
  if (doc.HasMember(config::kUserDefinedExtraInfo) &&
      doc[config::kUserDefinedExtraInfo].IsObject()) {
    page_config->SetExtraInfo(
        lepus::jsonValueTolepusValue(doc[config::kUserDefinedExtraInfo]));
  }

  /**
   * @name: lepusGCThreshold
   * @description: the gc threshold of lepusNG.
   * @note: None
   * @platform: Both
   * @supportVersion: 2.9
   **/
  if (doc.HasMember(config::kLepusGCThreshold) &&
      doc[config::kLepusGCThreshold].IsInt()) {
    page_config->SetLepusGCThreshold(doc[config::kLepusGCThreshold].GetInt());
  }

  /**
   * @name: enableComponentNullProp
   * @description: lepus support use null as component prop
   * @note: None
   * @platform: Both
   * @supportVersion: 2.9
   **/
  if (doc.HasMember(config::kEnableComponentNullProp) &&
      doc[config::kEnableComponentNullProp].IsBool()) {
    page_config->SetEnableComponentNullProp(
        doc[config::kEnableComponentNullProp].GetBool());
  }

  if (doc.HasMember(config::kRemoveDescendantSelectorScope) &&
      doc[config::kRemoveDescendantSelectorScope].IsBool()) {
    page_config->SetRemoveDescendantSelectorScope(
        doc[config::kRemoveDescendantSelectorScope].GetBool());
  } else if (compile_options_.enable_fiber_arch_) {
    // Fiber arch, descendant selector only works in component scope by default
    page_config->SetRemoveDescendantSelectorScope(false);
  }

  page_config->SetEnableStandardCSSSelector(
      compile_options_.enable_css_selector_);

  // enableComponentAsyncDecode
  if (doc.HasMember(config::kEnableComponentAsyncDecode) &&
      doc[config::kEnableComponentAsyncDecode].IsBool()) {
    page_config->SetEnableComponentAsyncDecode(
        doc[config::kEnableComponentAsyncDecode].GetBool());
  }

  // enableUseContextPool
  if (doc.HasMember(config::kEnableUseContextPool) &&
      doc[config::kEnableUseContextPool].IsBool()) {
    page_config->SetEnableUseContextPool(
        doc[config::kEnableUseContextPool].GetBool());
  }

  if (doc.HasMember(config::kEnableUseMapBuffer) &&
      doc[config::kEnableUseMapBuffer].IsBool()) {
    page_config->SetEnableUseMapBuffer(
        doc[config::kEnableUseMapBuffer].GetBool() ? TernaryBool::TRUE_VALUE
                                                   : TernaryBool::FALSE_VALUE);
  }

  if (doc.HasMember(config::kEnableFiberElementForRadonDiff) &&
      doc[config::kEnableFiberElementForRadonDiff].IsBool()) {
    page_config->SetEnableFiberElementForRadonDiff(
        doc[config::kEnableFiberElementForRadonDiff].GetBool()
            ? TernaryBool::TRUE_VALUE
            : TernaryBool::FALSE_VALUE);
  }

  // enableLynxScrollFluency
  if (doc.HasMember(config::kEnableLynxScrollFluency)) {
    if (doc[config::kEnableLynxScrollFluency].IsBool()) {
      page_config->SetEnableScrollFluencyMonitor(
          doc[config::kEnableLynxScrollFluency].GetBool() ? 1 : 0);
    } else if (doc[config::kEnableLynxScrollFluency].IsDouble()) {
      page_config->SetEnableScrollFluencyMonitor(
          doc[config::kEnableLynxScrollFluency].GetDouble());
    } else if (doc[config::kEnableLynxScrollFluency].IsInt()) {
      page_config->SetEnableScrollFluencyMonitor(
          doc[config::kEnableLynxScrollFluency].GetInt());
    }
  }

  // preferred_fps
  if (doc.HasMember(config::kPreferredFps) &&
      doc[config::kPreferredFps].IsString()) {
    page_config->SetPreferredFps(doc[config::kPreferredFps].GetString());
  }

  // enableCSSLazyImport
  if (doc.HasMember(config::kEnableCSSLazyImport) &&
      doc[config::kEnableCSSLazyImport].IsBool()) {
    page_config->SetEnableCSSLazyImport(
        doc[config::kEnableCSSLazyImport].GetBool() ? TernaryBool::TRUE_VALUE
                                                    : TernaryBool::FALSE_VALUE);
  }

  // enableNewAnimator
  if (doc.HasMember(config::kEnableNewAnimator) &&
      doc[config::kEnableNewAnimator].IsBool()) {
    page_config->SetEnableNewAnimator(doc[config::kEnableNewAnimator].GetBool()
                                          ? TernaryBool::TRUE_VALUE
                                          : TernaryBool::FALSE_VALUE);
  } else {
    page_config->SetEnableNewAnimator(TernaryBool::UNDEFINE_VALUE);
  }

  // TODO(nihao.royal): removed in later versions
  if (doc.HasMember(config::kEnableReloadLifecycle) &&
      doc[config::kEnableReloadLifecycle].IsBool()) {
    page_config->SetEnableReloadLifecycle(
        doc[config::kEnableReloadLifecycle].GetBool());
  }

  // enableJsBindApiThrowException
  // user configuration takes priority, otherwise the default configuration is
  // set by target_sdk_version. Enable it if it is greater than version 2.14,
  // disable it if it is less than.
  page_config->SetEnableJsBindingApiThrowException(
      (doc.HasMember(config::kEnableJsBindingApiThrowException) &&
       doc[config::kEnableJsBindingApiThrowException].IsBool())
          ? doc[config::kEnableJsBindingApiThrowException].GetBool()
          : lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                                LYNX_VERSION_2_14));

  // Parse page config dynamically
  // TODO(nihao.royal) enumerate doc and parse page config one by one, and unify
  // parameters of different types.
  page_config->ForEachBoolConfig([&doc](const std::string& name) {
    const char* const key = name.c_str();
    if (doc.HasMember(key) && doc[key].IsBool()) {
      return doc[key].GetBool() ? TernaryBool::TRUE_VALUE
                                : TernaryBool::FALSE_VALUE;
    }
    return TernaryBool::UNDEFINE_VALUE;
  });

  page_config->ForEachUint64Config([&doc](const std::string& name) {
    const char* const key = name.c_str();
    if (doc.HasMember(key) && doc[key].IsUint64()) {
      return doc[key].GetUint64();
    }
    return static_cast<uint64_t>(0);
  });

  page_config->SetEnableElementAPITypeCheckThrowWarning(
      lynx::tasm::Config::IsHigherOrEqual(target_sdk_version_,
                                          LYNX_VERSION_2_16));

  UpdateCSSConfigs(page_config);

  // enableICU
  if (doc.HasMember(config::kEnableBindICU) &&
      doc[config::kEnableBindICU].IsBool()) {
    page_config->SetEnableBindICU(doc[config::kEnableBindICU].GetBool());
  }

  // enableQueryComponentSync
  if (doc.HasMember(config::kEnableQueryComponentSync) &&
      doc[config::kEnableQueryComponentSync].IsBool()) {
    page_config->SetEnableQueryComponentSync(
        doc[config::kEnableQueryComponentSync].GetBool());
  }

  // enableMicrotaskPromisePolyfill
  if (doc.HasMember(runtime::kEnableMicrotaskPromisePolyfill) &&
      doc[runtime::kEnableMicrotaskPromisePolyfill].IsBool()) {
    page_config->SetEnableMicrotaskPromisePolyfill(
        doc[runtime::kEnableMicrotaskPromisePolyfill].GetBool());
  }

  // disableQuickTracingGC
  if (doc.HasMember(config::kDisableQuickTracingGC) &&
      doc[config::kDisableQuickTracingGC].IsBool()) {
    page_config->SetDisableQuickTracingGC(
        doc[config::kDisableQuickTracingGC].GetBool());
  }

  // fix css import rule order issue
  if (doc.HasMember(config::kFixCSSImportRuleOrder) &&
      doc[config::kFixCSSImportRuleOrder].IsBool()) {
    page_config->SetFixCSSImportRuleOrder(
        doc[config::kFixCSSImportRuleOrder].GetBool());
  }

  // enableSignalAPI
  if (doc.HasMember(kEnableSignalAPI) && doc[kEnableSignalAPI].IsBool()) {
    page_config->SetEnableSignalAPI(doc[kEnableSignalAPI].GetBool()
                                        ? TernaryBool::TRUE_VALUE
                                        : TernaryBool::FALSE_VALUE);
  }

  config_helper_.HandlePageConfig(doc, page_config);

  ReportGlobalFeatureSwitch(page_config);
  return true;
}

void LynxBinaryConfigDecoder::UpdateCSSConfigs(
    const std::shared_ptr<PageConfig>& page_config) {
  auto configs =
      CSSParserConfigs::GetCSSParserConfigsByComplierOptions(compile_options_);
  page_config->SetCSSParserConfigs(configs);
}

bool LynxBinaryConfigDecoder::DecodeComponentConfig(
    const std::string& config_str,
    std::shared_ptr<ComponentConfig>& component_config) {
  rapidjson::Document doc;
  if (doc.Parse(config_str.c_str()).HasParseError()) {
    LOGE("DecodeComponentConfig Error");
    return false;
  }

  if (doc.HasMember(config::kEnableRemoveComponentExtraData) &&
      doc[config::kEnableRemoveComponentExtraData].IsBool()) {
    // only set when has this member defaults to undefined
    component_config->SetEnableRemoveExtraData(
        doc[config::kEnableRemoveComponentExtraData].GetBool());
  }

  if (doc.HasMember(config::kRemoveComponentElement) &&
      doc[config::kRemoveComponentElement].IsBool()) {
    component_config->SetRemoveComponentElement(
        doc[config::kRemoveComponentElement].GetBool());
  }
  return true;
}

/// TODO(limeng.amer): move to report thread.
/// Upload global feature switches in PageConfig with common data about lynx
/// view. If you add a new  global feature switch, you should add it to report
/// event.
void LynxBinaryConfigDecoder::ReportGlobalFeatureSwitch(
    const std::shared_ptr<PageConfig>& page_config) {
  if (!tasm::LynxEnv::GetInstance().EnableGlobalFeatureSwitchStatistic()) {
    return;
  }
  report::EventTracker::OnEvent([page_config](report::MoveOnlyEvent& event) {
    event.SetName(config::kLynxSDKGlobalFeatureSwitchEvent);
    event.SetProps(config::kImplicit, page_config->GetGlobalImplicit());
    event.SetProps(config::kEnableAsyncDisplay,
                   page_config->GetEnableAsyncDisplay());
    event.SetProps(config::kEnableViewReceiveTouch,
                   page_config->GetEnableViewReceiveTouch());
    event.SetProps(config::kEnableEventThrough,
                   page_config->GetEnableEventThrough());
    event.SetProps(config::kRemoveComponentElement,
                   page_config->GetRemoveComponentElement());
    event.SetProps(config::kEnableCSSInheritance,
                   page_config->GetEnableCSSInheritance());
    event.SetProps(config::kEnableListNewArchitecture,
                   page_config->GetListNewArchitecture());
    event.SetProps(config::kEnableCSSStrictMode,
                   page_config->GetEnableCSSStrictMode());
    event.SetProps(config::kEnableReactOnlyPropsId,
                   page_config->GetEnableReactOnlyPropsId());
    event.SetProps(config::kEnableCircularDataCheck,
                   page_config->GetGlobalCircularDataCheck());
    event.SetProps(config::kEnableReduceInitDataCopy,
                   page_config->GetEnableReduceInitDataCopy());
    event.SetProps(config::kUnifyVWVHBehavior, page_config->GetUnifyVWVH());
    event.SetProps(config::kEnableComponentLayoutOnly,
                   page_config->GetEnableComponentLayoutOnly());
    event.SetProps(config::kAutoExpose, page_config->GetAutoExpose());
    event.SetProps(config::kAbsoluteInContentBound,
                   page_config->GetAbsoluteInContentBound());
    event.SetProps(config::kLongPressDuration,
                   page_config->GetLongPressDuration());
    event.SetProps(config::kObserverFrameRate,
                   page_config->GetObserverFrameRate());
    event.SetProps(config::kEnableExposureUIMargin,
                   page_config->GetEnableExposureUIMargin());
    event.SetProps(config::kFlatten, page_config->GetGlobalFlattern());
    event.SetProps(config::kForceCalcNewStyleKey,
                   page_config->GetForceCalcNewStyle());
    event.SetProps(config::kEnableLynxResourceServiceProvider,
                   page_config->GetEnableLynxResourceServiceProvider());
    event.SetProps(config::kEnableComponentNullProp,
                   page_config->GetEnableComponentNullProp());
    event.SetProps(config::kRemoveDescendantSelectorScope,
                   page_config->GetRemoveDescendantSelectorScope());
    event.SetProps(config::kEnableComponentAsyncDecode,
                   page_config->GetEnableComponentAsyncDecode());
  });
}

}  // namespace tasm
}  // namespace lynx
