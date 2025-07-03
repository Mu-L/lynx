// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef CORE_RENDERER_DOM_FIBER_TEXT_PROPS_H_
#define CORE_RENDERER_DOM_FIBER_TEXT_PROPS_H_
#include <optional>

#include "base/include/value/base_string.h"

namespace lynx {
namespace tasm {
using starlight::VerticalAlignType;
constexpr const char* kInlinePlaceHolder = "I";
enum TextPropertyKeyID {
  kPropInlineStart = 0,
  kPropInlineEnd = 1,
  kPropTextString = 2,

  // styles
  kTextPropFontSize = 3,
  kTextPropColor = 4,
  kTextPropWhiteSpace = 5,
  kTextPropTextOverflow = 6,
  kTextPropFontWeight = 7,
  kTextPropFontStyle = 8,
  kTextPropFontFamily = 9,
  kTextPropLineHeight = 10,
  kTextPropLetterSpacing = 11,
  kTextPropLineSpacing = 12,
  kTextPropTextShadow = 13,
  kTextPropTextDecoration = 14,
  kTextPropTextAlign = 15,
  kTextPropVerticalAlign = 16,

  // attributes
  kTextPropTextMaxLine = 99,
  kTextPropBackGroundColor = 100,
  kPropImageSrc = 101,  // image
  kPropInlineView = 102,
  kPropRectSize = 103,
  kPropMargin = 104,

  kTextPropEnd = 0xFF,
};
struct TextProps {
  enum class WhiteSpace { NO_WRAP = 0, NORMAL = 1 };

  enum class TextOverflow { CLIP = 0, ELLIPSIS = 1 };

  enum class Typeface { NORMAL = 0, BOLD = 1, ITALIC = 2 };

  enum class TextAlign { LEFT = 0, CENTER = 1, RIGHT = 2 };

  // attributes
  std::optional<int> text_max_line;        //=-1
  std::optional<base::String> image_mode;  //""
};

}  // namespace tasm
}  // namespace lynx

#endif  // CORE_RENDERER_DOM_FIBER_TEXT_PROPS_H_
