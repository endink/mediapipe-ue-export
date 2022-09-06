#pragma once

#include "ump_api.h"

#include <cinttypes>
#include <cstdlib>
#include <cstdarg>

#include <algorithm>
#include <string>
#include <vector>

#include <thread>
#include <atomic>
#include <mutex>

#include <iostream>  
#include <exception>
#include <map>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "mediapipe/framework/calculator_framework.h"

#include "ump_commons.h"

//#define PROF_ENABLE
#include "ump_profiler.h"

// Macro

#define UMP_LOCK(_mux) auto _lock = std::lock_guard(_mux)

#define TRY             \
  try {

#define CATCH_EXCEPTION                            \
  return 0;            \
  }            \
  catch (std::exception & e) {            \
    log(EUmpVerbosity::Error,  e.what());            \
  }            \
  catch (...) {            \
    log(EUmpVerbosity::Error, "Unknown exception occured");            \
  }            \
return -1;

#define CATCH_RETURN(RET_VALUE, FAILED)                            \
return RET_VALUE;            \
}            \
catch (std::exception & e) {            \
log(EUmpVerbosity::Error,  e.what());            \
}            \
catch (...) {            \
log(EUmpVerbosity::Error, "Unknown exception occured");            \
}            \
return FAILED;

#define CATCH_RETURN_STATUS                        \
}            \
catch (std::exception & e) {            \
	return absl::Status(absl::StatusCode::kAborted, e.what()); \
}            \
catch (...) {            \
	return absl::Status(absl::StatusCode::kAborted,"Unknown exception occured"); \
}            


#define CATCH_ONLY                            \
  }            \
  catch (std::exception & e) {            \
    log(EUmpVerbosity::Error,  e.what());           \
  }            \
  catch (...) {            \
     log(EUmpVerbosity::Error, "Unknown exception occured");             \
  }            



 
// Globals
extern IUmpLog* _ump_log;

inline void log(EUmpVerbosity verbosity, const char* msg) { if (_ump_log) { _ump_log->Println(verbosity, msg); } }

// String

inline const char* operator*(const std::string& str) { return str.c_str(); }

inline std::string strf(const char* format, ...)
{
	const size_t buf_size = 2048;
	char buf[buf_size];
	va_list args;
	va_start(args, format);
	int n = vsnprintf_s(buf, buf_size - 1, _TRUNCATE, format, args);
	va_end(args);
	if (n >= 0)
		buf[n] = 0;
	else
		buf[0] = 0;
	return std::move(std::string(buf));
}


extern inline const char* strcpy_to_heap(const std::string& str) {
	if (str.empty()) {
		return nullptr;
	}

	auto str_ptr = new char[str.length() + 1];
	snprintf(str_ptr, str.length() + 1, str.c_str());

	return str_ptr;
}