#pragma once

/**
 * @file PfeifferLog.h
 * @brief Internal logging shim for PfeifferTurboTC80.
 *
 * This library used to include <DebugLog.h> directly, which forced that
 * dependency (and its unprefixed LOG_* macros) onto every consumer. The
 * PFTC80_LOG_* macros below decouple it. Pick a backend with a build flag:
 *
 *   -D PFTC80_USE_DEBUGLOG   Forward to the DebugLog library's LOG_* macros.
 *                            The runtime level comes from LOG_GET_LEVEL().
 *
 *   -D PFTC80_LOG_TO_STREAM  Print to a Print/Stream the sketch supplies via
 *                            PfeifferVacProtocol::setLogStream(). The level is
 *                            set at runtime with setLogLevel(); nothing is
 *                            printed until a sink is installed.
 *
 *   (neither)                All logging compiles out to nothing. The debug
 *                            print paths become dead code and the optimizer
 *                            removes them.
 *
 * The macros are variadic and take the same argument lists DebugLog's do:
 * PFTC80_LOG_DEBUG(F("addr "), addr, " bad\n").
 *
 * Do not confuse the runtime level checked here with the compile-time macro
 * PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG in PfeifferVacProtocolConsts.h. That one
 * decides whether the human-readable parameter/error lookup tables are built
 * into flash at all; this one decides whether to spend cycles rendering a
 * message right now. Both must be satisfied to get a decoded telegram dump.
 */

#include <Arduino.h>

// Severity levels, ordered. Used by the PFTC80_LOG_TO_STREAM backend and by
// setLogLevel(); they intentionally mirror DebugLog's ordering.
#define PFTC80_LOG_LVL_NONE 0
#define PFTC80_LOG_LVL_ERROR 1
#define PFTC80_LOG_LVL_WARN 2
#define PFTC80_LOG_LVL_INFO 3
#define PFTC80_LOG_LVL_DEBUG 4
#define PFTC80_LOG_LVL_TRACE 5

namespace PfeifferVacProtocol
{
    /**
     * @brief Install the destination for library log output.
     *
     * Only meaningful when built with -D PFTC80_LOG_TO_STREAM; a no-op
     * otherwise, so a sketch can call it unconditionally.
     */
    void setLogStream(Print &sink);

    /**
     * @brief Set the runtime verbosity, one of the PFTC80_LOG_LVL_* constants.
     *
     * Only meaningful when built with -D PFTC80_LOG_TO_STREAM; a no-op
     * otherwise. Defaults to PFTC80_LOG_LVL_INFO.
     */
    void setLogLevel(uint8_t level);
}

#if defined(PFTC80_USE_DEBUGLOG)

#include <DebugLog.h>

#define PFTC80_LOG_WARN(...) LOG_WARN(__VA_ARGS__)
#define PFTC80_LOG_INFO(...) LOG_INFO(__VA_ARGS__)
#define PFTC80_LOG_DEBUG(...) LOG_DEBUG(__VA_ARGS__)
#define PFTC80_LOG_TRACE(...) LOG_TRACE(__VA_ARGS__)

// Runtime test of the active verbosity against a PFTC80_LOG_LVL_* constant.
// Call sites use it to skip expensive work (decoding a telegram into a
// human-readable dump) that would only be thrown away at the current level.
#define PFTC80_LOG_LEVEL_AT_LEAST(level) \
    ((int)LOG_GET_LEVEL() >= (int)(level))

#elif defined(PFTC80_LOG_TO_STREAM)

namespace PfeifferVacProtocol
{
    namespace detail
    {
        extern Print *logSink;
        extern uint8_t logLevel;

        // Recursive variadic print; terminates on the empty argument list.
        inline void logPrint() {}

        template <typename T, typename... Rest>
        inline void logPrint(const T &value, const Rest &...rest)
        {
            logSink->print(value);
            logPrint(rest...);
        }

        template <typename... Args>
        inline void logAt(uint8_t level, const Args &...args)
        {
            if (logSink != nullptr && logLevel >= level)
                logPrint(args...);
        }
    }
}

#define PFTC80_LOG_WARN(...) PfeifferVacProtocol::detail::logAt(PFTC80_LOG_LVL_WARN, __VA_ARGS__)
#define PFTC80_LOG_INFO(...) PfeifferVacProtocol::detail::logAt(PFTC80_LOG_LVL_INFO, __VA_ARGS__)
#define PFTC80_LOG_DEBUG(...) PfeifferVacProtocol::detail::logAt(PFTC80_LOG_LVL_DEBUG, __VA_ARGS__)
#define PFTC80_LOG_TRACE(...) PfeifferVacProtocol::detail::logAt(PFTC80_LOG_LVL_TRACE, __VA_ARGS__)

#define PFTC80_LOG_LEVEL_AT_LEAST(level) \
    (PfeifferVacProtocol::detail::logSink != nullptr && PfeifferVacProtocol::detail::logLevel >= (level))

#else

// No backend selected: every call site collapses to nothing. The ((void)0)
// form keeps `if (x) PFTC80_LOG_INFO(...); else ...` well-formed.
#define PFTC80_LOG_WARN(...) ((void)0)
#define PFTC80_LOG_INFO(...) ((void)0)
#define PFTC80_LOG_DEBUG(...) ((void)0)
#define PFTC80_LOG_TRACE(...) ((void)0)

// Constant false, so the guarded blocks are dead code the optimizer drops.
#define PFTC80_LOG_LEVEL_AT_LEAST(level) (false)

#endif
