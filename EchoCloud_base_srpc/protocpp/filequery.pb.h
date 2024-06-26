// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: filequery.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_filequery_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_filequery_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3020000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3020001 < PROTOBUF_MIN_PROTOC_VERSION
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
#define PROTOBUF_INTERNAL_EXPORT_filequery_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_filequery_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_filequery_2eproto;
namespace filequery {
class ReqFilequery;
struct ReqFilequeryDefaultTypeInternal;
extern ReqFilequeryDefaultTypeInternal _ReqFilequery_default_instance_;
class RespFilequery;
struct RespFilequeryDefaultTypeInternal;
extern RespFilequeryDefaultTypeInternal _RespFilequery_default_instance_;
}  // namespace filequery
PROTOBUF_NAMESPACE_OPEN
template<> ::filequery::ReqFilequery* Arena::CreateMaybeMessage<::filequery::ReqFilequery>(Arena*);
template<> ::filequery::RespFilequery* Arena::CreateMaybeMessage<::filequery::RespFilequery>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace filequery {

// ===================================================================

class ReqFilequery final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:filequery.ReqFilequery) */ {
 public:
  inline ReqFilequery() : ReqFilequery(nullptr) {}
  ~ReqFilequery() override;
  explicit PROTOBUF_CONSTEXPR ReqFilequery(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ReqFilequery(const ReqFilequery& from);
  ReqFilequery(ReqFilequery&& from) noexcept
    : ReqFilequery() {
    *this = ::std::move(from);
  }

  inline ReqFilequery& operator=(const ReqFilequery& from) {
    CopyFrom(from);
    return *this;
  }
  inline ReqFilequery& operator=(ReqFilequery&& from) noexcept {
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
  static const ReqFilequery& default_instance() {
    return *internal_default_instance();
  }
  static inline const ReqFilequery* internal_default_instance() {
    return reinterpret_cast<const ReqFilequery*>(
               &_ReqFilequery_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ReqFilequery& a, ReqFilequery& b) {
    a.Swap(&b);
  }
  inline void Swap(ReqFilequery* other) {
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
  void UnsafeArenaSwap(ReqFilequery* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ReqFilequery* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ReqFilequery>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ReqFilequery& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const ReqFilequery& from);
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
  void InternalSwap(ReqFilequery* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "filequery.ReqFilequery";
  }
  protected:
  explicit ReqFilequery(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kUsernameFieldNumber = 1,
    kTokenstrFieldNumber = 2,
    kExtrMessageFieldNumber = 3,
    kExtrIntFieldNumber = 4,
  };
  // string username = 1;
  void clear_username();
  const std::string& username() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_username(ArgT0&& arg0, ArgT... args);
  std::string* mutable_username();
  PROTOBUF_NODISCARD std::string* release_username();
  void set_allocated_username(std::string* username);
  private:
  const std::string& _internal_username() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_username(const std::string& value);
  std::string* _internal_mutable_username();
  public:

  // string tokenstr = 2;
  void clear_tokenstr();
  const std::string& tokenstr() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_tokenstr(ArgT0&& arg0, ArgT... args);
  std::string* mutable_tokenstr();
  PROTOBUF_NODISCARD std::string* release_tokenstr();
  void set_allocated_tokenstr(std::string* tokenstr);
  private:
  const std::string& _internal_tokenstr() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_tokenstr(const std::string& value);
  std::string* _internal_mutable_tokenstr();
  public:

  // string extr_message = 3;
  void clear_extr_message();
  const std::string& extr_message() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_extr_message(ArgT0&& arg0, ArgT... args);
  std::string* mutable_extr_message();
  PROTOBUF_NODISCARD std::string* release_extr_message();
  void set_allocated_extr_message(std::string* extr_message);
  private:
  const std::string& _internal_extr_message() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_extr_message(const std::string& value);
  std::string* _internal_mutable_extr_message();
  public:

  // int32 extr_int = 4;
  void clear_extr_int();
  int32_t extr_int() const;
  void set_extr_int(int32_t value);
  private:
  int32_t _internal_extr_int() const;
  void _internal_set_extr_int(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:filequery.ReqFilequery)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr username_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr tokenstr_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr extr_message_;
  int32_t extr_int_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_filequery_2eproto;
};
// -------------------------------------------------------------------

class RespFilequery final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:filequery.RespFilequery) */ {
 public:
  inline RespFilequery() : RespFilequery(nullptr) {}
  ~RespFilequery() override;
  explicit PROTOBUF_CONSTEXPR RespFilequery(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  RespFilequery(const RespFilequery& from);
  RespFilequery(RespFilequery&& from) noexcept
    : RespFilequery() {
    *this = ::std::move(from);
  }

  inline RespFilequery& operator=(const RespFilequery& from) {
    CopyFrom(from);
    return *this;
  }
  inline RespFilequery& operator=(RespFilequery&& from) noexcept {
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
  static const RespFilequery& default_instance() {
    return *internal_default_instance();
  }
  static inline const RespFilequery* internal_default_instance() {
    return reinterpret_cast<const RespFilequery*>(
               &_RespFilequery_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(RespFilequery& a, RespFilequery& b) {
    a.Swap(&b);
  }
  inline void Swap(RespFilequery* other) {
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
  void UnsafeArenaSwap(RespFilequery* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  RespFilequery* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<RespFilequery>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const RespFilequery& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const RespFilequery& from);
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
  void InternalSwap(RespFilequery* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "filequery.RespFilequery";
  }
  protected:
  explicit RespFilequery(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kMessageFieldNumber = 2,
    kExtrMessageFieldNumber = 3,
    kCodeFieldNumber = 1,
  };
  // string message = 2;
  void clear_message();
  const std::string& message() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_message(ArgT0&& arg0, ArgT... args);
  std::string* mutable_message();
  PROTOBUF_NODISCARD std::string* release_message();
  void set_allocated_message(std::string* message);
  private:
  const std::string& _internal_message() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_message(const std::string& value);
  std::string* _internal_mutable_message();
  public:

  // string extr_message = 3;
  void clear_extr_message();
  const std::string& extr_message() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_extr_message(ArgT0&& arg0, ArgT... args);
  std::string* mutable_extr_message();
  PROTOBUF_NODISCARD std::string* release_extr_message();
  void set_allocated_extr_message(std::string* extr_message);
  private:
  const std::string& _internal_extr_message() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_extr_message(const std::string& value);
  std::string* _internal_mutable_extr_message();
  public:

  // int32 code = 1;
  void clear_code();
  int32_t code() const;
  void set_code(int32_t value);
  private:
  int32_t _internal_code() const;
  void _internal_set_code(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:filequery.RespFilequery)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr message_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr extr_message_;
  int32_t code_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_filequery_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ReqFilequery

// string username = 1;
inline void ReqFilequery::clear_username() {
  username_.ClearToEmpty();
}
inline const std::string& ReqFilequery::username() const {
  // @@protoc_insertion_point(field_get:filequery.ReqFilequery.username)
  return _internal_username();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ReqFilequery::set_username(ArgT0&& arg0, ArgT... args) {
 
 username_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:filequery.ReqFilequery.username)
}
inline std::string* ReqFilequery::mutable_username() {
  std::string* _s = _internal_mutable_username();
  // @@protoc_insertion_point(field_mutable:filequery.ReqFilequery.username)
  return _s;
}
inline const std::string& ReqFilequery::_internal_username() const {
  return username_.Get();
}
inline void ReqFilequery::_internal_set_username(const std::string& value) {
  
  username_.Set(value, GetArenaForAllocation());
}
inline std::string* ReqFilequery::_internal_mutable_username() {
  
  return username_.Mutable(GetArenaForAllocation());
}
inline std::string* ReqFilequery::release_username() {
  // @@protoc_insertion_point(field_release:filequery.ReqFilequery.username)
  return username_.Release();
}
inline void ReqFilequery::set_allocated_username(std::string* username) {
  if (username != nullptr) {
    
  } else {
    
  }
  username_.SetAllocated(username, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (username_.IsDefault()) {
    username_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:filequery.ReqFilequery.username)
}

// string tokenstr = 2;
inline void ReqFilequery::clear_tokenstr() {
  tokenstr_.ClearToEmpty();
}
inline const std::string& ReqFilequery::tokenstr() const {
  // @@protoc_insertion_point(field_get:filequery.ReqFilequery.tokenstr)
  return _internal_tokenstr();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ReqFilequery::set_tokenstr(ArgT0&& arg0, ArgT... args) {
 
 tokenstr_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:filequery.ReqFilequery.tokenstr)
}
inline std::string* ReqFilequery::mutable_tokenstr() {
  std::string* _s = _internal_mutable_tokenstr();
  // @@protoc_insertion_point(field_mutable:filequery.ReqFilequery.tokenstr)
  return _s;
}
inline const std::string& ReqFilequery::_internal_tokenstr() const {
  return tokenstr_.Get();
}
inline void ReqFilequery::_internal_set_tokenstr(const std::string& value) {
  
  tokenstr_.Set(value, GetArenaForAllocation());
}
inline std::string* ReqFilequery::_internal_mutable_tokenstr() {
  
  return tokenstr_.Mutable(GetArenaForAllocation());
}
inline std::string* ReqFilequery::release_tokenstr() {
  // @@protoc_insertion_point(field_release:filequery.ReqFilequery.tokenstr)
  return tokenstr_.Release();
}
inline void ReqFilequery::set_allocated_tokenstr(std::string* tokenstr) {
  if (tokenstr != nullptr) {
    
  } else {
    
  }
  tokenstr_.SetAllocated(tokenstr, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (tokenstr_.IsDefault()) {
    tokenstr_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:filequery.ReqFilequery.tokenstr)
}

// string extr_message = 3;
inline void ReqFilequery::clear_extr_message() {
  extr_message_.ClearToEmpty();
}
inline const std::string& ReqFilequery::extr_message() const {
  // @@protoc_insertion_point(field_get:filequery.ReqFilequery.extr_message)
  return _internal_extr_message();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ReqFilequery::set_extr_message(ArgT0&& arg0, ArgT... args) {
 
 extr_message_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:filequery.ReqFilequery.extr_message)
}
inline std::string* ReqFilequery::mutable_extr_message() {
  std::string* _s = _internal_mutable_extr_message();
  // @@protoc_insertion_point(field_mutable:filequery.ReqFilequery.extr_message)
  return _s;
}
inline const std::string& ReqFilequery::_internal_extr_message() const {
  return extr_message_.Get();
}
inline void ReqFilequery::_internal_set_extr_message(const std::string& value) {
  
  extr_message_.Set(value, GetArenaForAllocation());
}
inline std::string* ReqFilequery::_internal_mutable_extr_message() {
  
  return extr_message_.Mutable(GetArenaForAllocation());
}
inline std::string* ReqFilequery::release_extr_message() {
  // @@protoc_insertion_point(field_release:filequery.ReqFilequery.extr_message)
  return extr_message_.Release();
}
inline void ReqFilequery::set_allocated_extr_message(std::string* extr_message) {
  if (extr_message != nullptr) {
    
  } else {
    
  }
  extr_message_.SetAllocated(extr_message, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (extr_message_.IsDefault()) {
    extr_message_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:filequery.ReqFilequery.extr_message)
}

// int32 extr_int = 4;
inline void ReqFilequery::clear_extr_int() {
  extr_int_ = 0;
}
inline int32_t ReqFilequery::_internal_extr_int() const {
  return extr_int_;
}
inline int32_t ReqFilequery::extr_int() const {
  // @@protoc_insertion_point(field_get:filequery.ReqFilequery.extr_int)
  return _internal_extr_int();
}
inline void ReqFilequery::_internal_set_extr_int(int32_t value) {
  
  extr_int_ = value;
}
inline void ReqFilequery::set_extr_int(int32_t value) {
  _internal_set_extr_int(value);
  // @@protoc_insertion_point(field_set:filequery.ReqFilequery.extr_int)
}

// -------------------------------------------------------------------

// RespFilequery

// int32 code = 1;
inline void RespFilequery::clear_code() {
  code_ = 0;
}
inline int32_t RespFilequery::_internal_code() const {
  return code_;
}
inline int32_t RespFilequery::code() const {
  // @@protoc_insertion_point(field_get:filequery.RespFilequery.code)
  return _internal_code();
}
inline void RespFilequery::_internal_set_code(int32_t value) {
  
  code_ = value;
}
inline void RespFilequery::set_code(int32_t value) {
  _internal_set_code(value);
  // @@protoc_insertion_point(field_set:filequery.RespFilequery.code)
}

// string message = 2;
inline void RespFilequery::clear_message() {
  message_.ClearToEmpty();
}
inline const std::string& RespFilequery::message() const {
  // @@protoc_insertion_point(field_get:filequery.RespFilequery.message)
  return _internal_message();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void RespFilequery::set_message(ArgT0&& arg0, ArgT... args) {
 
 message_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:filequery.RespFilequery.message)
}
inline std::string* RespFilequery::mutable_message() {
  std::string* _s = _internal_mutable_message();
  // @@protoc_insertion_point(field_mutable:filequery.RespFilequery.message)
  return _s;
}
inline const std::string& RespFilequery::_internal_message() const {
  return message_.Get();
}
inline void RespFilequery::_internal_set_message(const std::string& value) {
  
  message_.Set(value, GetArenaForAllocation());
}
inline std::string* RespFilequery::_internal_mutable_message() {
  
  return message_.Mutable(GetArenaForAllocation());
}
inline std::string* RespFilequery::release_message() {
  // @@protoc_insertion_point(field_release:filequery.RespFilequery.message)
  return message_.Release();
}
inline void RespFilequery::set_allocated_message(std::string* message) {
  if (message != nullptr) {
    
  } else {
    
  }
  message_.SetAllocated(message, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (message_.IsDefault()) {
    message_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:filequery.RespFilequery.message)
}

// string extr_message = 3;
inline void RespFilequery::clear_extr_message() {
  extr_message_.ClearToEmpty();
}
inline const std::string& RespFilequery::extr_message() const {
  // @@protoc_insertion_point(field_get:filequery.RespFilequery.extr_message)
  return _internal_extr_message();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void RespFilequery::set_extr_message(ArgT0&& arg0, ArgT... args) {
 
 extr_message_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:filequery.RespFilequery.extr_message)
}
inline std::string* RespFilequery::mutable_extr_message() {
  std::string* _s = _internal_mutable_extr_message();
  // @@protoc_insertion_point(field_mutable:filequery.RespFilequery.extr_message)
  return _s;
}
inline const std::string& RespFilequery::_internal_extr_message() const {
  return extr_message_.Get();
}
inline void RespFilequery::_internal_set_extr_message(const std::string& value) {
  
  extr_message_.Set(value, GetArenaForAllocation());
}
inline std::string* RespFilequery::_internal_mutable_extr_message() {
  
  return extr_message_.Mutable(GetArenaForAllocation());
}
inline std::string* RespFilequery::release_extr_message() {
  // @@protoc_insertion_point(field_release:filequery.RespFilequery.extr_message)
  return extr_message_.Release();
}
inline void RespFilequery::set_allocated_extr_message(std::string* extr_message) {
  if (extr_message != nullptr) {
    
  } else {
    
  }
  extr_message_.SetAllocated(extr_message, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (extr_message_.IsDefault()) {
    extr_message_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:filequery.RespFilequery.extr_message)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace filequery

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_filequery_2eproto
