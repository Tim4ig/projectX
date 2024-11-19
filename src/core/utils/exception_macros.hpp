
#pragma once

#define THROW_MESSAGE_DEBUG(e) \
do { \
    OutputDebugStringA(((e).what() + std::string("\n\n")).c_str()); \
    x::core::utils::logger.Exception(PREFIX, (e).what()); \
} while (0)

#define XTHROW(message) \
do { \
    const auto e = x::core::utils::Exception(message, __FUNCTION__, __LINE__); \
    THROW_MESSAGE_DEBUG(e); \
    throw(e); \
} while (0)

#define HTHROW(message) ;\
do { \
    if ((hr) < 0) { \
        const auto e = x::core::utils::Exception( \
            std::string(message) + ", HRESULT was: " + std::to_string(static_cast<unsigned long>(hr)), \
            __FUNCTION__, __LINE__); \
        THROW_MESSAGE_DEBUG(e); \
        throw(e); \
    } \
} while (0)
