// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: kvserver.proto
#ifndef GRPC_kvserver_2eproto__INCLUDED
#define GRPC_kvserver_2eproto__INCLUDED

#include "kvserver.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace kv {

class KVServer final {
 public:
  static constexpr char const* service_full_name() {
    return "kv.KVServer";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status SetKV(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::kv::SetKVResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::SetKVResponse>> AsyncSetKV(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::SetKVResponse>>(AsyncSetKVRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::SetKVResponse>> PrepareAsyncSetKV(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::SetKVResponse>>(PrepareAsyncSetKVRaw(context, request, cq));
    }
    virtual ::grpc::Status GetKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::kv::GetKResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::GetKResponse>> AsyncGetKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::GetKResponse>>(AsyncGetKVRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::GetKResponse>> PrepareAsyncGetKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::GetKResponse>>(PrepareAsyncGetKVRaw(context, request, cq));
    }
    virtual ::grpc::Status DelKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::kv::DelKVResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::DelKVResponse>> AsyncDelKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::DelKVResponse>>(AsyncDelKVRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::DelKVResponse>> PrepareAsyncDelKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::kv::DelKVResponse>>(PrepareAsyncDelKVRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void SetKV(::grpc::ClientContext* context, const ::kv::ReqKV* request, ::kv::SetKVResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void SetKV(::grpc::ClientContext* context, const ::kv::ReqKV* request, ::kv::SetKVResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void GetKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::GetKResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void GetKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::GetKResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      virtual void DelKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::DelKVResponse* response, std::function<void(::grpc::Status)>) = 0;
      virtual void DelKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::DelKVResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::kv::SetKVResponse>* AsyncSetKVRaw(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::kv::SetKVResponse>* PrepareAsyncSetKVRaw(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::kv::GetKResponse>* AsyncGetKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::kv::GetKResponse>* PrepareAsyncGetKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::kv::DelKVResponse>* AsyncDelKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::kv::DelKVResponse>* PrepareAsyncDelKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status SetKV(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::kv::SetKVResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::SetKVResponse>> AsyncSetKV(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::SetKVResponse>>(AsyncSetKVRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::SetKVResponse>> PrepareAsyncSetKV(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::SetKVResponse>>(PrepareAsyncSetKVRaw(context, request, cq));
    }
    ::grpc::Status GetKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::kv::GetKResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::GetKResponse>> AsyncGetKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::GetKResponse>>(AsyncGetKVRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::GetKResponse>> PrepareAsyncGetKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::GetKResponse>>(PrepareAsyncGetKVRaw(context, request, cq));
    }
    ::grpc::Status DelKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::kv::DelKVResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::DelKVResponse>> AsyncDelKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::DelKVResponse>>(AsyncDelKVRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::DelKVResponse>> PrepareAsyncDelKV(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::kv::DelKVResponse>>(PrepareAsyncDelKVRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void SetKV(::grpc::ClientContext* context, const ::kv::ReqKV* request, ::kv::SetKVResponse* response, std::function<void(::grpc::Status)>) override;
      void SetKV(::grpc::ClientContext* context, const ::kv::ReqKV* request, ::kv::SetKVResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void GetKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::GetKResponse* response, std::function<void(::grpc::Status)>) override;
      void GetKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::GetKResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      void DelKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::DelKVResponse* response, std::function<void(::grpc::Status)>) override;
      void DelKV(::grpc::ClientContext* context, const ::kv::ReqK* request, ::kv::DelKVResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::kv::SetKVResponse>* AsyncSetKVRaw(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::kv::SetKVResponse>* PrepareAsyncSetKVRaw(::grpc::ClientContext* context, const ::kv::ReqKV& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::kv::GetKResponse>* AsyncGetKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::kv::GetKResponse>* PrepareAsyncGetKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::kv::DelKVResponse>* AsyncDelKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::kv::DelKVResponse>* PrepareAsyncDelKVRaw(::grpc::ClientContext* context, const ::kv::ReqK& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_SetKV_;
    const ::grpc::internal::RpcMethod rpcmethod_GetKV_;
    const ::grpc::internal::RpcMethod rpcmethod_DelKV_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status SetKV(::grpc::ServerContext* context, const ::kv::ReqKV* request, ::kv::SetKVResponse* response);
    virtual ::grpc::Status GetKV(::grpc::ServerContext* context, const ::kv::ReqK* request, ::kv::GetKResponse* response);
    virtual ::grpc::Status DelKV(::grpc::ServerContext* context, const ::kv::ReqK* request, ::kv::DelKVResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_SetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_SetKV() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_SetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqKV* /*request*/, ::kv::SetKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSetKV(::grpc::ServerContext* context, ::kv::ReqKV* request, ::grpc::ServerAsyncResponseWriter< ::kv::SetKVResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_GetKV() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_GetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::GetKResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetKV(::grpc::ServerContext* context, ::kv::ReqK* request, ::grpc::ServerAsyncResponseWriter< ::kv::GetKResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_DelKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_DelKV() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_DelKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DelKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::DelKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDelKV(::grpc::ServerContext* context, ::kv::ReqK* request, ::grpc::ServerAsyncResponseWriter< ::kv::DelKVResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_SetKV<WithAsyncMethod_GetKV<WithAsyncMethod_DelKV<Service > > > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_SetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_SetKV() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::kv::ReqKV, ::kv::SetKVResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::kv::ReqKV* request, ::kv::SetKVResponse* response) { return this->SetKV(context, request, response); }));}
    void SetMessageAllocatorFor_SetKV(
        ::grpc::MessageAllocator< ::kv::ReqKV, ::kv::SetKVResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::kv::ReqKV, ::kv::SetKVResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_SetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqKV* /*request*/, ::kv::SetKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SetKV(
      ::grpc::CallbackServerContext* /*context*/, const ::kv::ReqKV* /*request*/, ::kv::SetKVResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_GetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_GetKV() {
      ::grpc::Service::MarkMethodCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::kv::ReqK, ::kv::GetKResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::kv::ReqK* request, ::kv::GetKResponse* response) { return this->GetKV(context, request, response); }));}
    void SetMessageAllocatorFor_GetKV(
        ::grpc::MessageAllocator< ::kv::ReqK, ::kv::GetKResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(1);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::kv::ReqK, ::kv::GetKResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_GetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::GetKResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* GetKV(
      ::grpc::CallbackServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::GetKResponse* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithCallbackMethod_DelKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_DelKV() {
      ::grpc::Service::MarkMethodCallback(2,
          new ::grpc::internal::CallbackUnaryHandler< ::kv::ReqK, ::kv::DelKVResponse>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::kv::ReqK* request, ::kv::DelKVResponse* response) { return this->DelKV(context, request, response); }));}
    void SetMessageAllocatorFor_DelKV(
        ::grpc::MessageAllocator< ::kv::ReqK, ::kv::DelKVResponse>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(2);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::kv::ReqK, ::kv::DelKVResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_DelKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DelKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::DelKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* DelKV(
      ::grpc::CallbackServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::DelKVResponse* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_SetKV<WithCallbackMethod_GetKV<WithCallbackMethod_DelKV<Service > > > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_SetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_SetKV() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_SetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqKV* /*request*/, ::kv::SetKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_GetKV() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_GetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::GetKResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_DelKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_DelKV() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_DelKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DelKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::DelKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_SetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_SetKV() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_SetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqKV* /*request*/, ::kv::SetKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestSetKV(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_GetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_GetKV() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_GetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::GetKResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetKV(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_DelKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_DelKV() {
      ::grpc::Service::MarkMethodRaw(2);
    }
    ~WithRawMethod_DelKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DelKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::DelKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDelKV(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_SetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_SetKV() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->SetKV(context, request, response); }));
    }
    ~WithRawCallbackMethod_SetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status SetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqKV* /*request*/, ::kv::SetKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* SetKV(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_GetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_GetKV() {
      ::grpc::Service::MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->GetKV(context, request, response); }));
    }
    ~WithRawCallbackMethod_GetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::GetKResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* GetKV(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_DelKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_DelKV() {
      ::grpc::Service::MarkMethodRawCallback(2,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->DelKV(context, request, response); }));
    }
    ~WithRawCallbackMethod_DelKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DelKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::DelKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* DelKV(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_SetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_SetKV() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::kv::ReqKV, ::kv::SetKVResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::kv::ReqKV, ::kv::SetKVResponse>* streamer) {
                       return this->StreamedSetKV(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_SetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status SetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqKV* /*request*/, ::kv::SetKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedSetKV(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::kv::ReqKV,::kv::SetKVResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_GetKV() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler<
          ::kv::ReqK, ::kv::GetKResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::kv::ReqK, ::kv::GetKResponse>* streamer) {
                       return this->StreamedGetKV(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_GetKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::GetKResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetKV(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::kv::ReqK,::kv::GetKResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_DelKV : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_DelKV() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::internal::StreamedUnaryHandler<
          ::kv::ReqK, ::kv::DelKVResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::kv::ReqK, ::kv::DelKVResponse>* streamer) {
                       return this->StreamedDelKV(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_DelKV() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status DelKV(::grpc::ServerContext* /*context*/, const ::kv::ReqK* /*request*/, ::kv::DelKVResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedDelKV(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::kv::ReqK,::kv::DelKVResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_SetKV<WithStreamedUnaryMethod_GetKV<WithStreamedUnaryMethod_DelKV<Service > > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_SetKV<WithStreamedUnaryMethod_GetKV<WithStreamedUnaryMethod_DelKV<Service > > > StreamedService;
};

}  // namespace kv


#endif  // GRPC_kvserver_2eproto__INCLUDED
