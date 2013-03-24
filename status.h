// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// A Status encapsulates the result of an operation.  It may indicate success,
// or it may indicate an error with an associated error message.
//
// Multiple threads can invoke const methods on a Status without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same Status must use
// external synchronization.

// Modified by John R. Bandela

#ifndef JRB_MODIFIED_STORAGE_LEVELDB_INCLUDE_STATUS_H_
#define JRB_MODIFIED_STORAGE_LEVELDB_INCLUDE_STATUS_H_

#include <string>
#include "slice.h"
#include <cstdint>

namespace leveldb_cc{
// Simple status that just has code
struct status_cross_type;

class  Status {

	friend void modify_status(Status& s, const status_cross_type&);
	friend void status_to_cross(const Status& s, status_cross_type&);

 public:
  // Create a success status.
  Status() : state_(NULL) { }
  ~Status() { delete[] state_; }

  // Copy the specified status.
  Status(const Status& s);
  void operator=(const Status& s);

  // Return a success status.
  static Status OK() { return Status(); }

  // Return error status of an appropriate type.
  static Status NotFound(const Slice& msg, const Slice& msg2 = Slice()) {
	return Status(kNotFound, msg, msg2);
  }
  static Status Corruption(const Slice& msg, const Slice& msg2 = Slice()) {
	return Status(kCorruption, msg, msg2);
  }
  static Status NotSupported(const Slice& msg, const Slice& msg2 = Slice()) {
	return Status(kNotSupported, msg, msg2);
  }
  static Status InvalidArgument(const Slice& msg, const Slice& msg2 = Slice()) {
	return Status(kInvalidArgument, msg, msg2);
  }
  static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) {
	return Status(kIOError, msg, msg2);
  }

  // Returns true iff the status indicates success.
  bool ok() const { return (state_ == NULL); }

  // Returns true iff the status indicates a NotFound error.
  bool IsNotFound() const { return code() == kNotFound; }

  // Return a string representation of this status suitable for printing.
  // Returns the string "OK" for success.
  std::string ToString() const;

 private:
  // OK status has a NULL state_.  Otherwise, state_ is a new[] array
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4]    == code
  //    state_[5..]  == message
  const char* state_;

  enum Code {
	kOk = 0,
	kNotFound = 1,
	kCorruption = 2,
	kNotSupported = 3,
	kInvalidArgument = 4,
	kIOError = 5
  };

  Code code() const {
	return (state_ == NULL) ? kOk : static_cast<Code>(state_[4]);
  }

  Status(Code code, const Slice& msg, const Slice& msg2);
  static const char* CopyState(const char* s);
};

inline Status::Status(const Status& s) {
  state_ = (s.state_ == NULL) ? NULL : CopyState(s.state_);
}
inline void Status::operator=(const Status& s) {
  // The following condition catches both aliasing (when this == &s),
  // and the common case where both s and *this are ok.
  if (state_ != s.state_) {
	delete[] state_;
	state_ = (s.state_ == NULL) ? NULL : CopyState(s.state_);
  }
}

inline const char* Status::CopyState(const char* state) {
  std::uint32_t size;
  memcpy(&size, state, sizeof(size));
  char* result = new char[size + 5];
  memcpy(result, state, size + 5);
  return result;
}

inline Status::Status(Code code, const Slice& msg, const Slice& msg2) {
  assert(code != kOk);
  const uint32_t len1 = msg.size();
  const uint32_t len2 = msg2.size();
  const uint32_t size = len1 + (len2 ? (2 + len2) : 0);
  char* result = new char[size + 5];
  memcpy(result, &size, sizeof(size));
  result[4] = static_cast<char>(code);
  memcpy(result + 5, msg.data(), len1);
  if (len2) {
	result[5 + len1] = ':';
	result[6 + len1] = ' ';
	memcpy(result + 7 + len1, msg2.data(), len2);
  }
  state_ = result;
}

inline std::string Status::ToString() const {
	using std::uint32_t;
  if (state_ == NULL) {
	return "OK";
  } else {
	char tmp[30];
	const char* type;
	switch (code()) {
	  case kOk:
		type = "OK";
		break;
	  case kNotFound:
		type = "NotFound: ";
		break;
	  case kCorruption:
		type = "Corruption: ";
		break;
	  case kNotSupported:
		type = "Not implemented: ";
		break;
	  case kInvalidArgument:
		type = "Invalid argument: ";
		break;
	  case kIOError:
		type = "IO error: ";
		break;
	  default:
		// Got rid of snprintf call at cost of not knowing code
		type = "Unknown code";
		break;
	}
	std::string result(type);
	uint32_t length;
	memcpy(&length, state_, sizeof(length));
	result.append(state_ + 5, length);
	return result;
  }
}

#pragma pack(push,1)
	struct status_cross_type{
		const char* state;
		void  (CROSS_CALL_CALLING_CONVENTION *deleter)(const char*);

	};
#pragma pack(pop)
	inline void modify_status(Status& s, const status_cross_type& sc){
		s.state_ = nullptr;
		if(sc.state){
			s.state_ = Status::CopyState(sc.state);
		}

	}
	inline void CROSS_CALL_CALLING_CONVENTION delete_state(const char* s){
		delete [] s;
	}
	inline void status_to_cross(const Status& s, status_cross_type& sc){
		sc.state = nullptr;
		sc.deleter = &delete_state;
		if(s.state_){
			sc.state = Status::CopyState(s.state_);
		}

	}



}



namespace cross_compiler_interface{
	template<>
	struct cross_conversion<leveldb_cc::Status>{


		typedef leveldb_cc::Status original_type;
		typedef leveldb_cc::status_cross_type converted_type;
		static converted_type to_converted_type(const original_type& s){
			leveldb_cc::status_cross_type ret;
			leveldb_cc::status_to_cross(s,ret);
			return ret;
		}
		static  original_type to_original_type(leveldb_cc::status_cross_type& sc){
			leveldb_cc::Status ret;
			leveldb_cc::modify_status(ret,sc);
			if(sc.state){
				sc.deleter(sc.state);
			}

			return ret;
		}
	};

}


#endif  // JRB_MODIFIED_STORAGE_LEVELDB_INCLUDE_STATUS_H_
