/******************************************************************************
 * z80-board-firmware
 * debug.h - debugging helperss
 *
 *****************************************************************************/

#ifndef Z80_BOARD_DEBUG_H
#define Z80_BOARD_DEBUG_H

#ifdef DEBUG_ENABLED
#include "stringtools.h"
#include "comms-protocol.h"
#include <avr/pgmspace.h>

#define STRINGIFY_X(X) #X
#define STRINGIFY(X) STRINGIFY_X(X)

#define DEBUG_LOG_STRING(MESSAGE) \
    {\
        static const char dbg_msg_static[] PROGMEM = \
            __FILE__ ":"  STRINGIFY(__LINE__)  " " MESSAGE "\n"; \
        comms_put_P((unsigned char*)dbg_msg_static, sizeof(dbg_msg_static));\
    } while(0)

#define DEBUG_LOG_INT(VAL)  \
    {\
        static const char dbg_msg_static[] PROGMEM =  __FILE__  ":" STRINGIFY(__LINE__) " "; \
        comms_put_P((unsigned char*)dbg_msg_static, sizeof(dbg_msg_static));\
        char dbg_msg_buffer[16]; \
        uinttohexstring((VAL), dbg_msg_buffer, 16); \
        comms_put((unsigned char*)dbg_msg_buffer, 16); \
        comms_putchar('\n'); \
    } while(0)

#define DEBUG_LOG_VAL(NAME, VAL)  \
    {\
        static const char dbg_msg_static[] PROGMEM =  __FILE__ ":" STRINGIFY(__LINE__) " " NAME ": "; \
        comms_put_P((unsigned char*)dbg_msg_static, sizeof(dbg_msg_static));\
        char dbg_msg_buffer[16]; \
        uinttohexstring((VAL), dbg_msg_buffer, 16); \
        comms_put((unsigned char*)dbg_msg_buffer, 16); \
        comms_putchar('\n'); \
    } while(0)

#else

#define DEBUG_LOG_STRING(MESSAGE)
#define DEBUG_LOG_INT(VAL)
#define DEBUG_LOG_VAL(NAME, VAL)

#endif

#ifdef EXTREME_DEBUG_ENABLED
#define EXTREME_DEBUG_LOG_STRING(MESSAGE) DEBUG_LOG_STRING(MESSAGE)
#define EXTREME_DEBUG_LOG_INT(VAL) DEBUG_LOG_INT(VAL)
#define EXTREME_DEBUG_LOG_VAL(NAME, VAL) DEBUG_LOG_VAL(NAME, VAL)
#else
#define EXTREME_DEBUG_LOG_STRING(MESSAGE)
#define EXTREME_DEBUG_LOG_INT(VAL)
#define EXTREME_DEBUG_LOG_VAL(NAME, VAL)
#endif

#endif
