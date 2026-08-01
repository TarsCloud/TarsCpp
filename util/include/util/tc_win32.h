#pragma once

#include "util/tc_platform.h"

#if TARGET_PLATFORM_WINDOWS

#include <string>

namespace tars
{
namespace detail
{

// All Windows-facing std::string values in tarscpp are UTF-8.
bool utf8ToWide(const std::string &input, std::wstring &output);
bool wideToUtf8(const std::wstring &input, std::string &output);

}
}

#endif
