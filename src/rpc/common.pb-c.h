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
typedef struct Rpc__NodeValue Rpc__NodeValue;
typedef struct Rpc__Filter Rpc__Filter;
typedef struct Rpc__FilterChain Rpc__FilterChain;
typedef struct Rpc__Nodes Rpc__Nodes;
typedef struct Rpc__Node Rpc__Node;
typedef struct Rpc__CreateNodeRequest Rpc__CreateNodeRequest;
typedef struct Rpc__UpdateNodeRequest Rpc__UpdateNodeRequest;
typedef struct Rpc__DeleteNodeRequest Rpc__DeleteNodeRequest;
typedef struct Rpc__DeletedNodes Rpc__DeletedNodes;


/* --- enums --- */

typedef enum _Rpc__NodeValue__Type {
  RPC__NODE_VALUE__TYPE__UNKNOWN = 0,
  RPC__NODE_VALUE__TYPE__INT_32 = 1,
  RPC__NODE_VALUE__TYPE__DOUBLE = 2,
  RPC__NODE_VALUE__TYPE__STRING = 3,
  RPC__NODE_VALUE__TYPE__BOOL = 4,
  RPC__NODE_VALUE__TYPE__FILE_INFO = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RPC__NODE_VALUE__TYPE)
} Rpc__NodeValue__Type;
typedef enum _Rpc__Filter__Type {
  RPC__FILTER__TYPE__UNKNOWN_TYPE = 0,
  RPC__FILTER__TYPE__GREATER = 1,
  RPC__FILTER__TYPE__LESS = 2,
  RPC__FILTER__TYPE__EQUAL = 3,
  RPC__FILTER__TYPE__NOT_EQUAL = 4,
  RPC__FILTER__TYPE__ALL = 5,
  RPC__FILTER__TYPE__FIELD_NAME = 6
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RPC__FILTER__TYPE)
} Rpc__Filter__Type;

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


typedef enum {
  RPC__NODE_VALUE__VALUE__NOT_SET = 0,
  RPC__NODE_VALUE__VALUE_INT_VALUE = 2,
  RPC__NODE_VALUE__VALUE_DOUBLE_VALUE = 3,
  RPC__NODE_VALUE__VALUE_STRING_VALUE = 4,
  RPC__NODE_VALUE__VALUE_BOOL_VALUE = 5,
  RPC__NODE_VALUE__VALUE_FILE_INFO_VALUE = 6
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RPC__NODE_VALUE__VALUE__CASE)
} Rpc__NodeValue__ValueCase;

struct  Rpc__NodeValue
{
  ProtobufCMessage base;
  Rpc__NodeValue__Type type;
  Rpc__NodeValue__ValueCase value_case;
  union {
    int32_t int_value;
    double double_value;
    char *string_value;
    protobuf_c_boolean bool_value;
    Rpc__FileInfo *file_info_value;
  };
};
#define RPC__NODE_VALUE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__node_value__descriptor) \
, RPC__NODE_VALUE__TYPE__UNKNOWN, RPC__NODE_VALUE__VALUE__NOT_SET, {0} }


typedef enum {
  RPC__FILTER__ARGUMENT__NOT_SET = 0,
  RPC__FILTER__ARGUMENT_INT_ARGUMENT = 2,
  RPC__FILTER__ARGUMENT_DOUBLE_ARGUMENT = 3,
  RPC__FILTER__ARGUMENT_STRING_ARGUMENT = 4,
  RPC__FILTER__ARGUMENT_BOOL_ARGUMENT = 5
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RPC__FILTER__ARGUMENT__CASE)
} Rpc__Filter__ArgumentCase;

typedef enum {
  RPC__FILTER__FIELD_NAME__NOT_SET = 0,
  RPC__FILTER__FIELD_NAME_NAME = 6
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RPC__FILTER__FIELD_NAME__CASE)
} Rpc__Filter__FieldNameCase;

struct  Rpc__Filter
{
  ProtobufCMessage base;
  Rpc__Filter__Type type;
  Rpc__Filter__ArgumentCase argument_case;
  union {
    int32_t int_argument;
    double double_argument;
    char *string_argument;
    protobuf_c_boolean bool_argument;
  };
  Rpc__Filter__FieldNameCase field_name_case;
  union {
    char *name;
  };
};
#define RPC__FILTER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__filter__descriptor) \
, RPC__FILTER__TYPE__UNKNOWN_TYPE, RPC__FILTER__ARGUMENT__NOT_SET, {0}, RPC__FILTER__FIELD_NAME__NOT_SET, {0} }


struct  Rpc__FilterChain
{
  ProtobufCMessage base;
  size_t n_filters;
  Rpc__Filter **filters;
};
#define RPC__FILTER_CHAIN__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__filter_chain__descriptor) \
, 0,NULL }


struct  Rpc__Nodes
{
  ProtobufCMessage base;
  size_t n_nodes;
  Rpc__Node **nodes;
};
#define RPC__NODES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__nodes__descriptor) \
, 0,NULL }


struct  Rpc__Node
{
  ProtobufCMessage base;
  Rpc__NodeId *id;
  Rpc__NodeId *parent_id;
  Rpc__NodeValue *value;
};
#define RPC__NODE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__node__descriptor) \
, NULL, NULL, NULL }


struct  Rpc__CreateNodeRequest
{
  ProtobufCMessage base;
  Rpc__NodeValue *value;
  Rpc__NodeId *parent_id;
};
#define RPC__CREATE_NODE_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__create_node_request__descriptor) \
, NULL, NULL }


struct  Rpc__UpdateNodeRequest
{
  ProtobufCMessage base;
  Rpc__NodeId *id;
  Rpc__NodeId *parent_id;
  Rpc__NodeValue *value;
};
#define RPC__UPDATE_NODE_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__update_node_request__descriptor) \
, NULL, NULL, NULL }


struct  Rpc__DeleteNodeRequest
{
  ProtobufCMessage base;
  Rpc__NodeId *id;
};
#define RPC__DELETE_NODE_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__delete_node_request__descriptor) \
, NULL }


struct  Rpc__DeletedNodes
{
  ProtobufCMessage base;
  int32_t count;
};
#define RPC__DELETED_NODES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rpc__deleted_nodes__descriptor) \
, 0 }


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
/* Rpc__NodeValue methods */
void   rpc__node_value__init
                     (Rpc__NodeValue         *message);
size_t rpc__node_value__get_packed_size
                     (const Rpc__NodeValue   *message);
size_t rpc__node_value__pack
                     (const Rpc__NodeValue   *message,
                      uint8_t             *out);
size_t rpc__node_value__pack_to_buffer
                     (const Rpc__NodeValue   *message,
                      ProtobufCBuffer     *buffer);
Rpc__NodeValue *
       rpc__node_value__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__node_value__free_unpacked
                     (Rpc__NodeValue *message,
                      ProtobufCAllocator *allocator);
/* Rpc__Filter methods */
void   rpc__filter__init
                     (Rpc__Filter         *message);
size_t rpc__filter__get_packed_size
                     (const Rpc__Filter   *message);
size_t rpc__filter__pack
                     (const Rpc__Filter   *message,
                      uint8_t             *out);
size_t rpc__filter__pack_to_buffer
                     (const Rpc__Filter   *message,
                      ProtobufCBuffer     *buffer);
Rpc__Filter *
       rpc__filter__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__filter__free_unpacked
                     (Rpc__Filter *message,
                      ProtobufCAllocator *allocator);
/* Rpc__FilterChain methods */
void   rpc__filter_chain__init
                     (Rpc__FilterChain         *message);
size_t rpc__filter_chain__get_packed_size
                     (const Rpc__FilterChain   *message);
size_t rpc__filter_chain__pack
                     (const Rpc__FilterChain   *message,
                      uint8_t             *out);
size_t rpc__filter_chain__pack_to_buffer
                     (const Rpc__FilterChain   *message,
                      ProtobufCBuffer     *buffer);
Rpc__FilterChain *
       rpc__filter_chain__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__filter_chain__free_unpacked
                     (Rpc__FilterChain *message,
                      ProtobufCAllocator *allocator);
/* Rpc__Nodes methods */
void   rpc__nodes__init
                     (Rpc__Nodes         *message);
size_t rpc__nodes__get_packed_size
                     (const Rpc__Nodes   *message);
size_t rpc__nodes__pack
                     (const Rpc__Nodes   *message,
                      uint8_t             *out);
size_t rpc__nodes__pack_to_buffer
                     (const Rpc__Nodes   *message,
                      ProtobufCBuffer     *buffer);
Rpc__Nodes *
       rpc__nodes__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__nodes__free_unpacked
                     (Rpc__Nodes *message,
                      ProtobufCAllocator *allocator);
/* Rpc__Node methods */
void   rpc__node__init
                     (Rpc__Node         *message);
size_t rpc__node__get_packed_size
                     (const Rpc__Node   *message);
size_t rpc__node__pack
                     (const Rpc__Node   *message,
                      uint8_t             *out);
size_t rpc__node__pack_to_buffer
                     (const Rpc__Node   *message,
                      ProtobufCBuffer     *buffer);
Rpc__Node *
       rpc__node__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__node__free_unpacked
                     (Rpc__Node *message,
                      ProtobufCAllocator *allocator);
/* Rpc__CreateNodeRequest methods */
void   rpc__create_node_request__init
                     (Rpc__CreateNodeRequest         *message);
size_t rpc__create_node_request__get_packed_size
                     (const Rpc__CreateNodeRequest   *message);
size_t rpc__create_node_request__pack
                     (const Rpc__CreateNodeRequest   *message,
                      uint8_t             *out);
size_t rpc__create_node_request__pack_to_buffer
                     (const Rpc__CreateNodeRequest   *message,
                      ProtobufCBuffer     *buffer);
Rpc__CreateNodeRequest *
       rpc__create_node_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__create_node_request__free_unpacked
                     (Rpc__CreateNodeRequest *message,
                      ProtobufCAllocator *allocator);
/* Rpc__UpdateNodeRequest methods */
void   rpc__update_node_request__init
                     (Rpc__UpdateNodeRequest         *message);
size_t rpc__update_node_request__get_packed_size
                     (const Rpc__UpdateNodeRequest   *message);
size_t rpc__update_node_request__pack
                     (const Rpc__UpdateNodeRequest   *message,
                      uint8_t             *out);
size_t rpc__update_node_request__pack_to_buffer
                     (const Rpc__UpdateNodeRequest   *message,
                      ProtobufCBuffer     *buffer);
Rpc__UpdateNodeRequest *
       rpc__update_node_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__update_node_request__free_unpacked
                     (Rpc__UpdateNodeRequest *message,
                      ProtobufCAllocator *allocator);
/* Rpc__DeleteNodeRequest methods */
void   rpc__delete_node_request__init
                     (Rpc__DeleteNodeRequest         *message);
size_t rpc__delete_node_request__get_packed_size
                     (const Rpc__DeleteNodeRequest   *message);
size_t rpc__delete_node_request__pack
                     (const Rpc__DeleteNodeRequest   *message,
                      uint8_t             *out);
size_t rpc__delete_node_request__pack_to_buffer
                     (const Rpc__DeleteNodeRequest   *message,
                      ProtobufCBuffer     *buffer);
Rpc__DeleteNodeRequest *
       rpc__delete_node_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__delete_node_request__free_unpacked
                     (Rpc__DeleteNodeRequest *message,
                      ProtobufCAllocator *allocator);
/* Rpc__DeletedNodes methods */
void   rpc__deleted_nodes__init
                     (Rpc__DeletedNodes         *message);
size_t rpc__deleted_nodes__get_packed_size
                     (const Rpc__DeletedNodes   *message);
size_t rpc__deleted_nodes__pack
                     (const Rpc__DeletedNodes   *message,
                      uint8_t             *out);
size_t rpc__deleted_nodes__pack_to_buffer
                     (const Rpc__DeletedNodes   *message,
                      ProtobufCBuffer     *buffer);
Rpc__DeletedNodes *
       rpc__deleted_nodes__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rpc__deleted_nodes__free_unpacked
                     (Rpc__DeletedNodes *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Rpc__FileInfo_Closure)
                 (const Rpc__FileInfo *message,
                  void *closure_data);
typedef void (*Rpc__NodeId_Closure)
                 (const Rpc__NodeId *message,
                  void *closure_data);
typedef void (*Rpc__NodeValue_Closure)
                 (const Rpc__NodeValue *message,
                  void *closure_data);
typedef void (*Rpc__Filter_Closure)
                 (const Rpc__Filter *message,
                  void *closure_data);
typedef void (*Rpc__FilterChain_Closure)
                 (const Rpc__FilterChain *message,
                  void *closure_data);
typedef void (*Rpc__Nodes_Closure)
                 (const Rpc__Nodes *message,
                  void *closure_data);
typedef void (*Rpc__Node_Closure)
                 (const Rpc__Node *message,
                  void *closure_data);
typedef void (*Rpc__CreateNodeRequest_Closure)
                 (const Rpc__CreateNodeRequest *message,
                  void *closure_data);
typedef void (*Rpc__UpdateNodeRequest_Closure)
                 (const Rpc__UpdateNodeRequest *message,
                  void *closure_data);
typedef void (*Rpc__DeleteNodeRequest_Closure)
                 (const Rpc__DeleteNodeRequest *message,
                  void *closure_data);
typedef void (*Rpc__DeletedNodes_Closure)
                 (const Rpc__DeletedNodes *message,
                  void *closure_data);

/* --- services --- */

typedef struct Rpc__Database_Service Rpc__Database_Service;
struct Rpc__Database_Service
{
  ProtobufCService base;
  void (*create_node)(Rpc__Database_Service *service,
                      const Rpc__CreateNodeRequest *input,
                      Rpc__Node_Closure closure,
                      void *closure_data);
  void (*update_node)(Rpc__Database_Service *service,
                      const Rpc__UpdateNodeRequest *input,
                      Rpc__Node_Closure closure,
                      void *closure_data);
  void (*delete_node)(Rpc__Database_Service *service,
                      const Rpc__DeleteNodeRequest *input,
                      Rpc__Node_Closure closure,
                      void *closure_data);
  void (*get_node)(Rpc__Database_Service *service,
                   const Rpc__NodeId *input,
                   Rpc__Node_Closure closure,
                   void *closure_data);
  void (*get_nodes_by_filter)(Rpc__Database_Service *service,
                              const Rpc__FilterChain *input,
                              Rpc__Nodes_Closure closure,
                              void *closure_data);
  void (*delete_nodes_by_filter)(Rpc__Database_Service *service,
                                 const Rpc__FilterChain *input,
                                 Rpc__DeletedNodes_Closure closure,
                                 void *closure_data);
};
typedef void (*Rpc__Database_ServiceDestroy)(Rpc__Database_Service *);
void rpc__database__init (Rpc__Database_Service *service,
                          Rpc__Database_ServiceDestroy destroy);
#define RPC__DATABASE__BASE_INIT \
    { &rpc__database__descriptor, protobuf_c_service_invoke_internal, NULL }
#define RPC__DATABASE__INIT(function_prefix__) \
    { RPC__DATABASE__BASE_INIT,\
      function_prefix__ ## create_node,\
      function_prefix__ ## update_node,\
      function_prefix__ ## delete_node,\
      function_prefix__ ## get_node,\
      function_prefix__ ## get_nodes_by_filter,\
      function_prefix__ ## delete_nodes_by_filter  }
void rpc__database__create_node(ProtobufCService *service,
                                const Rpc__CreateNodeRequest *input,
                                Rpc__Node_Closure closure,
                                void *closure_data);
void rpc__database__update_node(ProtobufCService *service,
                                const Rpc__UpdateNodeRequest *input,
                                Rpc__Node_Closure closure,
                                void *closure_data);
void rpc__database__delete_node(ProtobufCService *service,
                                const Rpc__DeleteNodeRequest *input,
                                Rpc__Node_Closure closure,
                                void *closure_data);
void rpc__database__get_node(ProtobufCService *service,
                             const Rpc__NodeId *input,
                             Rpc__Node_Closure closure,
                             void *closure_data);
void rpc__database__get_nodes_by_filter(ProtobufCService *service,
                                        const Rpc__FilterChain *input,
                                        Rpc__Nodes_Closure closure,
                                        void *closure_data);
void rpc__database__delete_nodes_by_filter(ProtobufCService *service,
                                           const Rpc__FilterChain *input,
                                           Rpc__DeletedNodes_Closure closure,
                                           void *closure_data);

/* --- descriptors --- */

extern const ProtobufCMessageDescriptor rpc__file_info__descriptor;
extern const ProtobufCMessageDescriptor rpc__node_id__descriptor;
extern const ProtobufCMessageDescriptor rpc__node_value__descriptor;
extern const ProtobufCEnumDescriptor    rpc__node_value__type__descriptor;
extern const ProtobufCMessageDescriptor rpc__filter__descriptor;
extern const ProtobufCEnumDescriptor    rpc__filter__type__descriptor;
extern const ProtobufCMessageDescriptor rpc__filter_chain__descriptor;
extern const ProtobufCMessageDescriptor rpc__nodes__descriptor;
extern const ProtobufCMessageDescriptor rpc__node__descriptor;
extern const ProtobufCMessageDescriptor rpc__create_node_request__descriptor;
extern const ProtobufCMessageDescriptor rpc__update_node_request__descriptor;
extern const ProtobufCMessageDescriptor rpc__delete_node_request__descriptor;
extern const ProtobufCMessageDescriptor rpc__deleted_nodes__descriptor;
extern const ProtobufCServiceDescriptor rpc__database__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_common_2eproto__INCLUDED */
