// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef CORE_RUNTIME_JSI_JSVM_JSVM_RUNTIME_H_
#define CORE_RUNTIME_JSI_JSVM_JSVM_RUNTIME_H_

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "core/runtime/jsi/jsi.h"

namespace lynx {
namespace piper {

class JSVMRuntime : public Runtime {
 public:
  JSVMRuntime() = default;
  ~JSVMRuntime() override = default;

  JSRuntimeType type() override { return JSRuntimeType::jsvm; };
  void InitRuntime(std::shared_ptr<JSIContext> sharedContext,
                   std::shared_ptr<JSIExceptionHandler> handler) override{};
  std::shared_ptr<VMInstance> createVM(const StartupData*) const override {
    return nullptr;
  };
  std::shared_ptr<VMInstance> getSharedVM() override { return nullptr; };
  std::shared_ptr<JSIContext> createContext(
      std::shared_ptr<VMInstance>) const override {
    return nullptr;
  };
  std::shared_ptr<JSIContext> getSharedContext() override { return nullptr; };

  std::shared_ptr<const PreparedJavaScript> prepareJavaScript(
      const std::shared_ptr<const Buffer>& buffer,
      std::string sourceURL) override {
    return nullptr;
  };

  base::expected<Value, JSINativeException> evaluatePreparedJavaScript(
      const std::shared_ptr<const PreparedJavaScript>& js) override {
    return Value();
  };

  base::expected<Value, JSINativeException> evaluateJavaScript(
      const std::shared_ptr<const Buffer>& buffer,
      const std::string& sourceURL) override {
    return Value();
  };

  base::expected<Value, JSINativeException> evaluateJavaScriptBytecode(
      const std::shared_ptr<const Buffer>& buffer,
      const std::string& source_url) override {
    return Value();
  };
  Object global() override { return Object(*this); };

  std::string description() override { return description_; };

  bool isInspectable() override { return true; };

  void setDescription(const std::string& desc) { description_ = desc; };

 protected:
  PointerValue* cloneSymbol(const Runtime::PointerValue* pv) override {
    return nullptr;
  };
  PointerValue* cloneString(const Runtime::PointerValue* pv) override {
    return nullptr;
  };
  PointerValue* cloneObject(const Runtime::PointerValue* pv) override {
    return nullptr;
  };
  PointerValue* clonePropNameID(const Runtime::PointerValue* pv) override {
    return nullptr;
  };

  piper::PropNameID createPropNameIDFromAscii(const char* str,
                                              size_t length) override {
    return piper::PropNameID::forAscii(*this, "");
  };
  piper::PropNameID createPropNameIDFromUtf8(const uint8_t* utf8,
                                             size_t length) override {
    return piper::PropNameID::forAscii(*this, "");
  };
  piper::PropNameID createPropNameIDFromString(
      const piper::String& str) override {
    return piper::PropNameID::forAscii(*this, "");
  };
  std::string utf8(const piper::PropNameID&) override { return std::string(); };
  bool compare(const piper::PropNameID&, const piper::PropNameID&) override {
    return false;
  };

  std::optional<std::string> symbolToString(const piper::Symbol&) override {
    return std::nullopt;
  };

  piper::String createStringFromAscii(const char* str, size_t length) override {
    return piper::String::createFromAscii(*this, "");
  };
  piper::String createStringFromUtf8(const uint8_t* utf8,
                                     size_t length) override {
    return piper::String::createFromAscii(*this, "");
  };
  std::string utf8(const piper::String&) override { return std::string(); };

  piper::Object createObject() override {
    return piper::Object::createFromHostObject(*this, nullptr);
  };
  piper::Object createObject(std::shared_ptr<piper::HostObject> ho) override {
    return piper::Object::createFromHostObject(*this, nullptr);
  };
  std::weak_ptr<piper::HostObject> getHostObject(
      const piper::Object&) override {
    return std::weak_ptr<piper::HostObject>();
  };

  piper::HostFunctionType f = [](Runtime& rt, const piper::Value& thisVal,
                                 const piper::Value* args, size_t count) {
    return piper::Value::undefined();
  };
  piper::HostFunctionType& getHostFunction(const piper::Function&) override {
    return f;
  }

  std::optional<Value> getProperty(const piper::Object&,
                                   const piper::String& name) override {
    return std::nullopt;
  };
  std::optional<Value> getProperty(const piper::Object&,
                                   const piper::PropNameID& name) override {
    return std::nullopt;
  };
  bool hasProperty(const piper::Object&, const piper::String& name) override {
    return false;
  };
  bool hasProperty(const piper::Object&,
                   const piper::PropNameID& name) override {
    return false;
  };
  bool setPropertyValue(piper::Object&, const piper::String& name,
                        const piper::Value& value) override {
    return false;
  };
  bool setPropertyValue(piper::Object&, const piper::PropNameID& name,
                        const piper::Value& value) override {
    return false;
  };
  bool isArray(const piper::Object&) const override { return false; };
  bool isArrayBuffer(const piper::Object&) const override { return false; };
  bool isFunction(const piper::Object&) const override { return false; };
  bool isHostObject(const piper::Object&) const override { return false; };
  bool isHostFunction(const piper::Function&) const override { return false; };
  std::optional<Array> getPropertyNames(const piper::Object&) override {
    return std::nullopt;
  };

  std::optional<BigInt> createBigInt(const std::string& value,
                                     Runtime& rt) override {
    return std::nullopt;
  };

  std::optional<Array> createArray(size_t length) override {
    return std::nullopt;
  };

  piper::ArrayBuffer createArrayBufferCopy(const uint8_t* bytes,
                                           size_t byte_length) override {
    return piper::ArrayBuffer(*this, nullptr, 0);
  };

  piper::ArrayBuffer createArrayBufferNoCopy(
      std::unique_ptr<const uint8_t[]> bytes, size_t byte_length) override {
    return piper::ArrayBuffer(*this, nullptr, 0);
  };
  std::optional<size_t> size(const piper::Array&) override {
    return std::nullopt;
  };
  size_t size(const piper::ArrayBuffer&) override { return 0; };
  uint8_t* data(const piper::ArrayBuffer&) override { return nullptr; };
  size_t copyData(const ArrayBuffer&, uint8_t*, size_t) override { return 0; };
  std::optional<Value> getValueAtIndex(const piper::Array&, size_t i) override {
    return std::nullopt;
  };
  bool setValueAtIndexImpl(piper::Array&, size_t i,
                           const piper::Value& value) override {
    return false;
  };

  piper::Function createFunctionFromHostFunction(
      const piper::PropNameID& name, unsigned int paramCount,
      piper::HostFunctionType func) override {
    return piper::Function::createFromHostFunction(*this, name, paramCount,
                                                   std::move(func));
  };
  std::optional<Value> call(const piper::Function&, const piper::Value& jsThis,
                            const piper::Value* args, size_t count) override {
    return std::nullopt;
  };
  std::optional<Value> callAsConstructor(const piper::Function&,
                                         const piper::Value* args,
                                         size_t count) override {
    return std::nullopt;
  };
  ScopeState* pushScope() override { return nullptr; };
  void popScope(ScopeState*) override{};
  bool strictEquals(const piper::Symbol& a,
                    const piper::Symbol& b) const override {
    return false;
  };
  bool strictEquals(const piper::String& a,
                    const piper::String& b) const override {
    return false;
  };
  bool strictEquals(const piper::Object& a,
                    const piper::Object& b) const override {
    return false;
  };
  bool instanceOf(const piper::Object& o, const piper::Function& f) override {
    return false;
  };

  void RequestGC() override{};

  void InitInspector(
      const std::shared_ptr<InspectorRuntimeObserverNG>& observer) override{};
  void DestroyInspector() override{};

 private:
  std::string description_;
};
}  // namespace piper
}  // namespace lynx

#endif  // CORE_RUNTIME_JSI_JSVM_JSVM_RUNTIME_H_
