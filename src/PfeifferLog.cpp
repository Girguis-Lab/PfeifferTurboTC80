#include "PfeifferLog.h"

namespace PfeifferVacProtocol
{
#if defined(PFTC80_LOG_TO_STREAM)

    namespace detail
    {
        // No sink until the sketch installs one, so a library built with
        // PFTC80_LOG_TO_STREAM stays silent rather than touching an
        // uninitialised Serial during static construction.
        Print *logSink = nullptr;
        uint8_t logLevel = PFTC80_LOG_LVL_INFO;
    }

    void setLogStream(Print &sink)
    {
        detail::logSink = &sink;
    }

    void setLogLevel(uint8_t level)
    {
        detail::logLevel = level;
    }

#else

    // DebugLog owns the sink and level in the PFTC80_USE_DEBUGLOG build, and
    // there is nothing to configure when logging is compiled out. Both are
    // still defined so a sketch can call them unconditionally.
    void setLogStream(Print &sink)
    {
        (void)sink;
    }

    void setLogLevel(uint8_t level)
    {
        (void)level;
    }

#endif
}
