// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: request.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_request_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_request_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3020000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3020003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_request_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_request_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_request_2eproto;
namespace bit {
class AddRequest;
struct AddRequestDefaultTypeInternal;
extern AddRequestDefaultTypeInternal _AddRequest_default_instance_;
class AddResponse;
struct AddResponseDefaultTypeInternal;
extern AddResponseDefaultTypeInternal _AddResponse_default_instance_;
class TranslateRequest;
struct TranslateRequestDefaultTypeInternal;
extern TranslateRequestDefaultTypeInternal _TranslateRequest_default_instance_;
class TranslateResponse;
struct TranslateResponseDefaultTypeInternal;
extern TranslateResponseDefaultTypeInternal _TranslateResponse_default_instance_;
}  // namespace bit
PROTOBUF_NAMESPACE_OPEN
template<> ::bit::AddRequest* Arena::CreateMaybeMessage<::bit::AddRequest>(Arena*);
template<> ::bit::AddResponse* Arena::CreateMaybeMessage<::bit::AddResponse>(Arena*);
template<> ::bit::TranslateRequest* Arena::CreateMaybeMessage<::bit::TranslateRequest>(Arena*);
template<> ::bit::TranslateResponse* Arena::CreateMaybeMessage<::bit::TranslateResponse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace bit {

// ===================================================================

class TranslateRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bit.TranslateRequest) */ {
 public:
  inline TranslateRequest() : TranslateRequest(nullptr) {}
  ~TranslateRequest() override;
  explicit PROTOBUF_CONSTEXPR TranslateRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  TranslateRequest(const TranslateRequest& from);
  TranslateRequest(TranslateRequest&& from) noexcept
    : TranslateRequest() {
    *this = ::std::move(from);
  }

  inline TranslateRequest& operator=(const TranslateRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline TranslateRequest& operator=(TranslateRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const TranslateRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const TranslateRequest* internal_default_instance() {
    return reinterpret_cast<const TranslateRequest*>(
               &_TranslateRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(TranslateRequest& a, TranslateRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(TranslateRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(TranslateRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  TranslateRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<TranslateRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const TranslateRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const TranslateRequest& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(TranslateRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bit.TranslateRequest";
  }
  protected:
  explicit TranslateRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMsgFieldNumber = 1,
  };
  // string msg = 1;
  void clear_msg();
  const std::string& msg() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_msg(ArgT0&& arg0, ArgT... args);
  std::string* mutable_msg();
  PROTOBUF_NODISCARD std::string* release_msg();
  void set_allocated_msg(std::string* msg);
  private:
  const std::string& _internal_msg() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_msg(const std::string& value);
  std::string* _internal_mutable_msg();
  public:

  // @@protoc_insertion_point(class_scope:bit.TranslateRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr msg_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_request_2eproto;
};
// -------------------------------------------------------------------

class TranslateResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bit.TranslateResponse) */ {
 public:
  inline TranslateResponse() : TranslateResponse(nullptr) {}
  ~TranslateResponse() override;
  explicit PROTOBUF_CONSTEXPR TranslateResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  TranslateResponse(const TranslateResponse& from);
  TranslateResponse(TranslateResponse&& from) noexcept
    : TranslateResponse() {
    *this = ::std::move(from);
  }

  inline TranslateResponse& operator=(const TranslateResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline TranslateResponse& operator=(TranslateResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const TranslateResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const TranslateResponse* internal_default_instance() {
    return reinterpret_cast<const TranslateResponse*>(
               &_TranslateResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(TranslateResponse& a, TranslateResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(TranslateResponse* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(TranslateResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  TranslateResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<TranslateResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const TranslateResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const TranslateResponse& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(TranslateResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bit.TranslateResponse";
  }
  protected:
  explicit TranslateResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMsgFieldNumber = 1,
  };
  // string msg = 1;
  void clear_msg();
  const std::string& msg() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_msg(ArgT0&& arg0, ArgT... args);
  std::string* mutable_msg();
  PROTOBUF_NODISCARD std::string* release_msg();
  void set_allocated_msg(std::string* msg);
  private:
  const std::string& _internal_msg() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_msg(const std::string& value);
  std::string* _internal_mutable_msg();
  public:

  // @@protoc_insertion_point(class_scope:bit.TranslateResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr msg_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_request_2eproto;
};
// -------------------------------------------------------------------

class AddRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bit.AddRequest) */ {
 public:
  inline AddRequest() : AddRequest(nullptr) {}
  ~AddRequest() override;
  explicit PROTOBUF_CONSTEXPR AddRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  AddRequest(const AddRequest& from);
  AddRequest(AddRequest&& from) noexcept
    : AddRequest() {
    *this = ::std::move(from);
  }

  inline AddRequest& operator=(const AddRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline AddRequest& operator=(AddRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const AddRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const AddRequest* internal_default_instance() {
    return reinterpret_cast<const AddRequest*>(
               &_AddRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(AddRequest& a, AddRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(AddRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(AddRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  AddRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<AddRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const AddRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const AddRequest& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(AddRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bit.AddRequest";
  }
  protected:
  explicit AddRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNum1FieldNumber = 1,
    kNum2FieldNumber = 2,
  };
  // int32 num1 = 1;
  void clear_num1();
  int32_t num1() const;
  void set_num1(int32_t value);
  private:
  int32_t _internal_num1() const;
  void _internal_set_num1(int32_t value);
  public:

  // int32 num2 = 2;
  void clear_num2();
  int32_t num2() const;
  void set_num2(int32_t value);
  private:
  int32_t _internal_num2() const;
  void _internal_set_num2(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:bit.AddRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t num1_;
  int32_t num2_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_request_2eproto;
};
// -------------------------------------------------------------------

class AddResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:bit.AddResponse) */ {
 public:
  inline AddResponse() : AddResponse(nullptr) {}
  ~AddResponse() override;
  explicit PROTOBUF_CONSTEXPR AddResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  AddResponse(const AddResponse& from);
  AddResponse(AddResponse&& from) noexcept
    : AddResponse() {
    *this = ::std::move(from);
  }

  inline AddResponse& operator=(const AddResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline AddResponse& operator=(AddResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const AddResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const AddResponse* internal_default_instance() {
    return reinterpret_cast<const AddResponse*>(
               &_AddResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(AddResponse& a, AddResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(AddResponse* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(AddResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  AddResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<AddResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const AddResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const AddResponse& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(AddResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "bit.AddResponse";
  }
  protected:
  explicit AddResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kResultFieldNumber = 1,
  };
  // int32 result = 1;
  void clear_result();
  int32_t result() const;
  void set_result(int32_t value);
  private:
  int32_t _internal_result() const;
  void _internal_set_result(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:bit.AddResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t result_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_request_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// TranslateRequest

// string msg = 1;
inline void TranslateRequest::clear_msg() {
  msg_.ClearToEmpty();
}
inline const std::string& TranslateRequest::msg() const {
  // @@protoc_insertion_point(field_get:bit.TranslateRequest.msg)
  return _internal_msg();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void TranslateRequest::set_msg(ArgT0&& arg0, ArgT... args) {
 
 msg_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:bit.TranslateRequest.msg)
}
inline std::string* TranslateRequest::mutable_msg() {
  std::string* _s = _internal_mutable_msg();
  // @@protoc_insertion_point(field_mutable:bit.TranslateRequest.msg)
  return _s;
}
inline const std::string& TranslateRequest::_internal_msg() const {
  return msg_.Get();
}
inline void TranslateRequest::_internal_set_msg(const std::string& value) {
  
  msg_.Set(value, GetArenaForAllocation());
}
inline std::string* TranslateRequest::_internal_mutable_msg() {
  
  return msg_.Mutable(GetArenaForAllocation());
}
inline std::string* TranslateRequest::release_msg() {
  // @@protoc_insertion_point(field_release:bit.TranslateRequest.msg)
  return msg_.Release();
}
inline void TranslateRequest::set_allocated_msg(std::string* msg) {
  if (msg != nullptr) {
    
  } else {
    
  }
  msg_.SetAllocated(msg, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (msg_.IsDefault()) {
    msg_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:bit.TranslateRequest.msg)
}

// -------------------------------------------------------------------

// TranslateResponse

// string msg = 1;
inline void TranslateResponse::clear_msg() {
  msg_.ClearToEmpty();
}
inline const std::string& TranslateResponse::msg() const {
  // @@protoc_insertion_point(field_get:bit.TranslateResponse.msg)
  return _internal_msg();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void TranslateResponse::set_msg(ArgT0&& arg0, ArgT... args) {
 
 msg_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:bit.TranslateResponse.msg)
}
inline std::string* TranslateResponse::mutable_msg() {
  std::string* _s = _internal_mutable_msg();
  // @@protoc_insertion_point(field_mutable:bit.TranslateResponse.msg)
  return _s;
}
inline const std::string& TranslateResponse::_internal_msg() const {
  return msg_.Get();
}
inline void TranslateResponse::_internal_set_msg(const std::string& value) {
  
  msg_.Set(value, GetArenaForAllocation());
}
inline std::string* TranslateResponse::_internal_mutable_msg() {
  
  return msg_.Mutable(GetArenaForAllocation());
}
inline std::string* TranslateResponse::release_msg() {
  // @@protoc_insertion_point(field_release:bit.TranslateResponse.msg)
  return msg_.Release();
}
inline void TranslateResponse::set_allocated_msg(std::string* msg) {
  if (msg != nullptr) {
    
  } else {
    
  }
  msg_.SetAllocated(msg, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (msg_.IsDefault()) {
    msg_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:bit.TranslateResponse.msg)
}

// -------------------------------------------------------------------

// AddRequest

// int32 num1 = 1;
inline void AddRequest::clear_num1() {
  num1_ = 0;
}
inline int32_t AddRequest::_internal_num1() const {
  return num1_;
}
inline int32_t AddRequest::num1() const {
  // @@protoc_insertion_point(field_get:bit.AddRequest.num1)
  return _internal_num1();
}
inline void AddRequest::_internal_set_num1(int32_t value) {
  
  num1_ = value;
}
inline void AddRequest::set_num1(int32_t value) {
  _internal_set_num1(value);
  // @@protoc_insertion_point(field_set:bit.AddRequest.num1)
}

// int32 num2 = 2;
inline void AddRequest::clear_num2() {
  num2_ = 0;
}
inline int32_t AddRequest::_internal_num2() const {
  return num2_;
}
inline int32_t AddRequest::num2() const {
  // @@protoc_insertion_point(field_get:bit.AddRequest.num2)
  return _internal_num2();
}
inline void AddRequest::_internal_set_num2(int32_t value) {
  
  num2_ = value;
}
inline void AddRequest::set_num2(int32_t value) {
  _internal_set_num2(value);
  // @@protoc_insertion_point(field_set:bit.AddRequest.num2)
}

// -------------------------------------------------------------------

// AddResponse

// int32 result = 1;
inline void AddResponse::clear_result() {
  result_ = 0;
}
inline int32_t AddResponse::_internal_result() const {
  return result_;
}
inline int32_t AddResponse::result() const {
  // @@protoc_insertion_point(field_get:bit.AddResponse.result)
  return _internal_result();
}
inline void AddResponse::_internal_set_result(int32_t value) {
  
  result_ = value;
}
inline void AddResponse::set_result(int32_t value) {
  _internal_set_result(value);
  // @@protoc_insertion_point(field_set:bit.AddResponse.result)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace bit

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_request_2eproto
