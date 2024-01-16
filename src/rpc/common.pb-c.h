/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: common.proto */

#ifndef PROTOBUF_C_common_2eproto__INCLUDED
#define PROTOBUF_C_common_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1005000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct Rpc__FileInfo Rpc__FileInfo;
typedef struct Rpc__NodeId Rpc__NodeId;
typedef struct Rpc__CreateFileNodeRequest Rpc__CreateFileNodeRequest;
typedef struct Rpc__CreateFileNodeResponse Rpc__CreateFileNodeResponse;


/* --- enums --- */


/* --- messages --- */

struct  Rpc__FileInfo
{
  ProtobufCMessage base;
  char *name;
  char *owner;
  uint64_t access_time;
  char *mime_type;
};
#define RPC__FILE_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__file_info__descriptor) \
, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string, 0, (char *)protobuf_c_empty_string }


struct  Rpc__NodeId
{
  ProtobufCMessage base;
  uint32_t page_id;
  uint32_t item_id;
};
#define RPC__NODE_ID__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__node_id__descriptor) \
, 0, 0 }


struct  Rpc__CreateFileNodeRequest
{
  ProtobufCMessage base;
  Rpc__FileInfo *file_info;
  Rpc__NodeId *parent_id;
};
#define RPC__CREATE_FILE_NODE_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__create_file_node_request__descriptor) \
, NULL, NULL }


struct  Rpc__CreateFileNodeResponse
{
  ProtobufCMessage base;
  Rpc__NodeId *node_id;
};
#define RPC__CREATE_FILE_NODE_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__create_file_node_response__descriptor) \
, NULL }


/* Rpc__FileInfo methods */
void   rpc__file_info__init
                     (Rpc__FileInfo         *message);
size_t rpc__file_info__get_packed_size
                     (const Rpc__FileInfo   *message);
size_t rpc__file_info__pack
                     (const Rpc__FileInfo   *message,
                      uint8_t             *out);
size_t rpc__file_info__pack_to_buffer
                     (const Rpc__FileInfo   *message,
                      ProtobufCBuffer     *buffer);
Rpc__FileInfo *
       rpc__file_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__file_info__free_unpacked
                     (Rpc__FileInfo *message,
                      ProtobufCAllocator *allocator);
/* Rpc__NodeId methods */
void   rpc__node_id__init
                     (Rpc__NodeId         *message);
size_t rpc__node_id__get_packed_size
                     (const Rpc__NodeId   *message);
size_t rpc__node_id__pack
                     (const Rpc__NodeId   *message,
                      uint8_t             *out);
size_t rpc__node_id__pack_to_buffer
                     (const Rpc__NodeId   *message,
                      ProtobufCBuffer     *buffer);
Rpc__NodeId *
       rpc__node_id__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__node_id__free_unpacked
                     (Rpc__NodeId *message,
                      ProtobufCAllocator *allocator);
/* Rpc__CreateFileNodeRequest methods */
void   rpc__create_file_node_request__init
                     (Rpc__CreateFileNodeRequest         *message);
size_t rpc__create_file_node_request__get_packed_size
                     (const Rpc__CreateFileNodeRequest   *message);
size_t rpc__create_file_node_request__pack
                     (const Rpc__CreateFileNodeRequest   *message,
                      uint8_t             *out);
size_t rpc__create_file_node_request__pack_to_buffer
                     (const Rpc__CreateFileNodeRequest   *message,
                      ProtobufCBuffer     *buffer);
Rpc__CreateFileNodeRequest *
       rpc__create_file_node_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__create_file_node_request__free_unpacked
                     (Rpc__CreateFileNodeRequest *message,
                      ProtobufCAllocator *allocator);
/* Rpc__CreateFileNodeResponse methods */
void   rpc__create_file_node_response__init
                     (Rpc__CreateFileNodeResponse         *message);
size_t rpc__create_file_node_response__get_packed_size
                     (const Rpc__CreateFileNodeResponse   *message);
size_t rpc__create_file_node_response__pack
                     (const Rpc__CreateFileNodeResponse   *message,
                      uint8_t             *out);
size_t rpc__create_file_node_response__pack_to_buffer
                     (const Rpc__CreateFileNodeResponse   *message,
                      ProtobufCBuffer     *buffer);
Rpc__CreateFileNodeResponse *
       rpc__create_file_node_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__create_file_node_response__free_unpacked
                     (Rpc__CreateFileNodeResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Rpc__FileInfo_Closure)
                 (const Rpc__FileInfo *message,
                  void *closure_data);
typedef void (*Rpc__NodeId_Closure)
                 (const Rpc__NodeId *message,
                  void *closure_data);
typedef void (*Rpc__CreateFileNodeRequest_Closure)
                 (const Rpc__CreateFileNodeRequest *message,
                  void *closure_data);
typedef void (*Rpc__CreateFileNodeResponse_Closure)
                 (const Rpc__CreateFileNodeResponse *message,
                  void *closure_data);

/* --- services --- */

typedef struct Rpc__Database_Service Rpc__Database_Service;
struct Rpc__Database_Service
{
  ProtobufCService base;
  void (*create_file_node)(Rpc__Database_Service *service,
                           const Rpc__CreateFileNodeRequest *input,
                           Rpc__CreateFileNodeResponse_Closure closure,
                           void *closure_data);
};
typedef void (*Rpc__Database_ServiceDestroy)(Rpc__Database_Service *);
void rpc__database__init (Rpc__Database_Service *service,
                          Rpc__Database_ServiceDestroy destroy);
#define RPC__DATABASE__BASE_INIT \
    { &rpc__database__descriptor, protobuf_c_service_invoke_internal, NULL }
#define RPC__DATABASE__INIT(function_prefix__) \
    { RPC__DATABASE__BASE_INIT,\
      function_prefix__ ## create_file_node  }
void rpc__database__create_file_node(ProtobufCService *service,
                                     const Rpc__CreateFileNodeRequest *input,
                                     Rpc__CreateFileNodeResponse_Closure closure,
                                     void *closure_data);

/* --- descriptors --- */

extern const ProtobufCMessageDescriptor rpc__file_info__descriptor;
extern const ProtobufCMessageDescriptor rpc__node_id__descriptor;
extern const ProtobufCMessageDescriptor rpc__create_file_node_request__descriptor;
extern const ProtobufCMessageDescriptor rpc__create_file_node_response__descriptor;
extern const ProtobufCServiceDescriptor rpc__database__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_common_2eproto__INCLUDED */
