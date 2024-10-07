// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: contacts.proto

#include "contacts.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace contacts {
PROTOBUF_CONSTEXPR PeopleInfo::PeopleInfo(
    ::_pbi::ConstantInitialized)
  : name_(&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{})
  , age_(0){}
struct PeopleInfoDefaultTypeInternal {
  PROTOBUF_CONSTEXPR PeopleInfoDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~PeopleInfoDefaultTypeInternal() {}
  union {
    PeopleInfo _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 PeopleInfoDefaultTypeInternal _PeopleInfo_default_instance_;
}  // namespace contacts
static ::_pb::Metadata file_level_metadata_contacts_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_contacts_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_contacts_2eproto = nullptr;

const uint32_t TableStruct_contacts_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::contacts::PeopleInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::contacts::PeopleInfo, name_),
  PROTOBUF_FIELD_OFFSET(::contacts::PeopleInfo, age_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::contacts::PeopleInfo)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::contacts::_PeopleInfo_default_instance_._instance,
};

const char descriptor_table_protodef_contacts_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016contacts.proto\022\010contacts\"\'\n\nPeopleInfo"
  "\022\014\n\004name\030\001 \001(\t\022\013\n\003age\030\002 \001(\005b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_contacts_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_contacts_2eproto = {
    false, false, 75, descriptor_table_protodef_contacts_2eproto,
    "contacts.proto",
    &descriptor_table_contacts_2eproto_once, nullptr, 0, 1,
    schemas, file_default_instances, TableStruct_contacts_2eproto::offsets,
    file_level_metadata_contacts_2eproto, file_level_enum_descriptors_contacts_2eproto,
    file_level_service_descriptors_contacts_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_contacts_2eproto_getter() {
  return &descriptor_table_contacts_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_contacts_2eproto(&descriptor_table_contacts_2eproto);
namespace contacts {

// ===================================================================

class PeopleInfo::_Internal {
 public:
};

PeopleInfo::PeopleInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  // @@protoc_insertion_point(arena_constructor:contacts.PeopleInfo)
}
PeopleInfo::PeopleInfo(const PeopleInfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_name().empty()) {
    name_.Set(from._internal_name(), 
      GetArenaForAllocation());
  }
  age_ = from.age_;
  // @@protoc_insertion_point(copy_constructor:contacts.PeopleInfo)
}

inline void PeopleInfo::SharedCtor() {
name_.InitDefault();
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  name_.Set("", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
age_ = 0;
}

PeopleInfo::~PeopleInfo() {
  // @@protoc_insertion_point(destructor:contacts.PeopleInfo)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void PeopleInfo::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  name_.Destroy();
}

void PeopleInfo::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void PeopleInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:contacts.PeopleInfo)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  name_.ClearToEmpty();
  age_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* PeopleInfo::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string name = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_name();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "contacts.PeopleInfo.name"));
        } else
          goto handle_unusual;
        continue;
      // int32 age = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          age_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* PeopleInfo::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:contacts.PeopleInfo)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string name = 1;
  if (!this->_internal_name().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "contacts.PeopleInfo.name");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_name(), target);
  }

  // int32 age = 2;
  if (this->_internal_age() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(2, this->_internal_age(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:contacts.PeopleInfo)
  return target;
}

size_t PeopleInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:contacts.PeopleInfo)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string name = 1;
  if (!this->_internal_name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // int32 age = 2;
  if (this->_internal_age() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_age());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData PeopleInfo::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    PeopleInfo::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*PeopleInfo::GetClassData() const { return &_class_data_; }

void PeopleInfo::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<PeopleInfo *>(to)->MergeFrom(
      static_cast<const PeopleInfo &>(from));
}


void PeopleInfo::MergeFrom(const PeopleInfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:contacts.PeopleInfo)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_name().empty()) {
    _internal_set_name(from._internal_name());
  }
  if (from._internal_age() != 0) {
    _internal_set_age(from._internal_age());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void PeopleInfo::CopyFrom(const PeopleInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:contacts.PeopleInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PeopleInfo::IsInitialized() const {
  return true;
}

void PeopleInfo::InternalSwap(PeopleInfo* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &name_, lhs_arena,
      &other->name_, rhs_arena
  );
  swap(age_, other->age_);
}

::PROTOBUF_NAMESPACE_ID::Metadata PeopleInfo::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_contacts_2eproto_getter, &descriptor_table_contacts_2eproto_once,
      file_level_metadata_contacts_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace contacts
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::contacts::PeopleInfo*
Arena::CreateMaybeMessage< ::contacts::PeopleInfo >(Arena* arena) {
  return Arena::CreateMessageInternal< ::contacts::PeopleInfo >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
