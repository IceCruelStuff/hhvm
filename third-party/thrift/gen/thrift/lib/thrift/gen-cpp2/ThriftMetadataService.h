/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#pragma once

#include <thrift/lib/cpp2/gen/service_h.h>

#include "thrift/lib/thrift/gen-cpp2/ThriftMetadataServiceAsyncClient.h"
#include "thrift/lib/thrift/gen-cpp2/metadata_types.h"

namespace folly {
  class IOBuf;
  class IOBufQueue;
}
namespace apache { namespace thrift {
  class Cpp2RequestContext;
  class BinaryProtocolReader;
  class CompactProtocolReader;
  namespace transport { class THeader; }
}}

namespace apache { namespace thrift { namespace metadata {

class ThriftMetadataServiceSvAsyncIf {
 public:
  virtual ~ThriftMetadataServiceSvAsyncIf() {}
  virtual void async_tm_getThriftServiceMetadata(std::unique_ptr<apache::thrift::HandlerCallback<std::unique_ptr< ::apache::thrift::metadata::ThriftServiceMetadataResponse>>> callback) = 0;
  virtual folly::Future<std::unique_ptr< ::apache::thrift::metadata::ThriftServiceMetadataResponse>> future_getThriftServiceMetadata() = 0;
  virtual folly::SemiFuture<std::unique_ptr< ::apache::thrift::metadata::ThriftServiceMetadataResponse>> semifuture_getThriftServiceMetadata() = 0;
};

class ThriftMetadataServiceAsyncProcessor;

class ThriftMetadataServiceSvIf : public ThriftMetadataServiceSvAsyncIf, public apache::thrift::ServerInterface {
 public:
  typedef ThriftMetadataServiceAsyncProcessor ProcessorType;
  std::unique_ptr<apache::thrift::AsyncProcessor> getProcessor() override;
  virtual void getThriftServiceMetadata( ::apache::thrift::metadata::ThriftServiceMetadataResponse& /*_return*/);
  folly::Future<std::unique_ptr< ::apache::thrift::metadata::ThriftServiceMetadataResponse>> future_getThriftServiceMetadata() override;
  folly::SemiFuture<std::unique_ptr< ::apache::thrift::metadata::ThriftServiceMetadataResponse>> semifuture_getThriftServiceMetadata() override;
  void async_tm_getThriftServiceMetadata(std::unique_ptr<apache::thrift::HandlerCallback<std::unique_ptr< ::apache::thrift::metadata::ThriftServiceMetadataResponse>>> callback) override;
};

class ThriftMetadataServiceSvNull : public ThriftMetadataServiceSvIf {
 public:
  void getThriftServiceMetadata( ::apache::thrift::metadata::ThriftServiceMetadataResponse& /*_return*/) override;
};

class ThriftMetadataServiceAsyncProcessor : public ::apache::thrift::GeneratedAsyncProcessor {
 public:
  const char* getServiceName() override;
  void getServiceMetadata(apache::thrift::metadata::ThriftServiceMetadataResponse& response) override;
  using BaseAsyncProcessor = void;
 protected:
  ThriftMetadataServiceSvIf* iface_;
 public:
  void processSerializedRequest(apache::thrift::ResponseChannelRequest::UniquePtr req, apache::thrift::SerializedRequest&& serializedRequest, apache::thrift::protocol::PROTOCOL_TYPES protType, apache::thrift::Cpp2RequestContext* context, folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm) override;
 protected:
  std::shared_ptr<folly::RequestContext> getBaseContextForRequest() override;
 public:
  using ProcessFunc = GeneratedAsyncProcessor::ProcessFunc<ThriftMetadataServiceAsyncProcessor>;
  using ProcessMap = GeneratedAsyncProcessor::ProcessMap<ProcessFunc>;
  static const ThriftMetadataServiceAsyncProcessor::ProcessMap& getBinaryProtocolProcessMap();
  static const ThriftMetadataServiceAsyncProcessor::ProcessMap& getCompactProtocolProcessMap();
 private:
  static const ThriftMetadataServiceAsyncProcessor::ProcessMap binaryProcessMap_;
   static const ThriftMetadataServiceAsyncProcessor::ProcessMap compactProcessMap_;
 private:
  template <typename ProtocolIn_, typename ProtocolOut_>
  void _processInThread_getThriftServiceMetadata(apache::thrift::ResponseChannelRequest::UniquePtr req, apache::thrift::SerializedRequest&& serializedRequest, apache::thrift::Cpp2RequestContext* ctx, folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm);
  template <typename ProtocolIn_, typename ProtocolOut_>
  void process_getThriftServiceMetadata(apache::thrift::ResponseChannelRequest::UniquePtr req, apache::thrift::SerializedRequest&& serializedRequest, apache::thrift::Cpp2RequestContext* ctx,folly::EventBase* eb, apache::thrift::concurrency::ThreadManager* tm);
  template <class ProtocolIn_, class ProtocolOut_>
  static folly::IOBufQueue return_getThriftServiceMetadata(int32_t protoSeqId, apache::thrift::ContextStack* ctx,  ::apache::thrift::metadata::ThriftServiceMetadataResponse const& _return);
  template <class ProtocolIn_, class ProtocolOut_>
  static void throw_wrapped_getThriftServiceMetadata(apache::thrift::ResponseChannelRequest::UniquePtr req,int32_t protoSeqId,apache::thrift::ContextStack* ctx,folly::exception_wrapper ew,apache::thrift::Cpp2RequestContext* reqCtx);
 public:
  ThriftMetadataServiceAsyncProcessor(ThriftMetadataServiceSvIf* iface) :
      iface_(iface) {}

  virtual ~ThriftMetadataServiceAsyncProcessor() {}
};

}}} // apache::thrift::metadata
