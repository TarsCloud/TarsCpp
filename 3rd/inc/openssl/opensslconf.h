/* opensslconf.h */
/* Add by Bob.Liu for multiple platform definition. */

/******************* for Windows ********************/
/* 32 bits */
#if defined(OPENSSL_CONF_FILE_WIN_X86)
#include <openssl/conf_win_x86.h>

/* 64 bits */
#elif defined(OPENSSL_CONF_FILE_WIN_X86_64)
#include <openssl/conf_win_x86_64.h>

/******************* for iOS ********************/
/* simulator 32 bits */
#elif defined(OPENSSL_CONF_FILE_IOS_X86)
#include <openssl/conf_ios_i386.h>

/* simulator 64 bits */
#elif defined(OPENSSL_CONF_FILE_IOS_X86_64)
#include <openssl/conf_ios_x86_64.h>

/* iPhone OS armv7a */
#elif defined(OPENSSL_CONF_FILE_IOS_ARMV7)
#include <openssl/conf_ios_armv7.h>

/* iPhone OS arm64 */
#elif defined(OPENSSL_CONF_FILE_IOS_ARM64)
#include <openssl/conf_ios_arm64.h>

/******************* for Android ********************/
/* simulator */
#elif defined(OPENSSL_CONF_FILE_ANDROID_X86)
#include <openssl/conf_android_x86.h>

#elif defined(OPENSSL_CONF_FILE_ANDROID_X86_64)
#include <openssl/conf_android_x86_64.h>

/* Android armv7a */
#elif defined(OPENSSL_CONF_FILE_ANDROID_ARMV7)
#include <openssl/conf_android_armv7.h>

/* Android armv8 */
#elif defined(OPENSSL_CONF_FILE_ANDROID_ARM64)
#include <openssl/conf_android_arm64.h>

/******************* for Linux ********************/
#elif defined(OPENSSL_CONF_FILE_LINUX_X86_64)
#include <openssl/conf_linux_x86_64.h>

#elif defined(OPENSSL_CONF_FILE_LINUX_ARM64)
#include <openssl/conf_linux_aarch64.h>

/******************* for MACOS ********************/
#elif defined(OPENSSL_CONF_FILE_MACOS_X86_64)
#include <openssl/conf_macos_x86_64.h>

#else
#error Please define config file for OpenSSL.
#endif
