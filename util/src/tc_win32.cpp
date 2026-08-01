#include "util/tc_win32.h"

#if TARGET_PLATFORM_WINDOWS

#include <limits>

namespace tars
{
namespace detail
{

bool utf8ToWide(const std::string &input, std::wstring &output)
{
    output.clear();
    if (input.empty())
    {
        return true;
    }

    if (input.size() > static_cast<size_t>(std::numeric_limits<int>::max()))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    const int inputLength = static_cast<int>(input.size());
    const int outputLength = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, input.data(), inputLength, NULL, 0);
    if (outputLength <= 0)
    {
        return false;
    }

    output.resize(static_cast<size_t>(outputLength));
    return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, input.data(), inputLength, &output[0], outputLength) == outputLength;
}

bool wideToUtf8(const std::wstring &input, std::string &output)
{
    output.clear();
    if (input.empty())
    {
        return true;
    }

    if (input.size() > static_cast<size_t>(std::numeric_limits<int>::max()))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return false;
    }

    const int inputLength = static_cast<int>(input.size());
    const int outputLength = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, input.data(), inputLength, NULL, 0, NULL, NULL);
    if (outputLength <= 0)
    {
        return false;
    }

    output.resize(static_cast<size_t>(outputLength));
    return WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, input.data(), inputLength, &output[0], outputLength, NULL, NULL) == outputLength;
}

}
}

#endif
