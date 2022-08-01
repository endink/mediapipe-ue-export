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

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "mediapipe/framework/calculator_framework.h"

//#define PROF_ENABLE
#include "ump_profiler.h"

// Macro

#define UMP_LOCK(_mux) auto _lock = std::lock_guard(_mux)

// Globals

extern IUmpLog* _ump_log;

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
