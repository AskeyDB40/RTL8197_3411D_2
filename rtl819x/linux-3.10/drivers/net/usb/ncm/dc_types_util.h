#ifndef _DC_TYPES_UTIL_H_
#define _DC_TYPES_UTIL_H_

#include "kstype.h"
#define CPU_BYTE_ORDER	CPU_LITTLE_ENDIAN


/*
 *      Generic comparison and types
 */

#ifndef MIN
# define MIN(a,b) ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
# define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif

#ifndef ABS
# define ABS(a)  ((a) < 0 ? (-(a)) : (a))
#endif

#define DC_OFFSET_OF(type, member)      ((size_t)(&((type *)0)->member))
#define DC_ARRAY_SIZE(a)                (sizeof(a) / sizeof((a)[0]))

#ifndef CPU_BYTE_ORDER
# error "CPU_BYTE_ORDER is not specified !"
#endif

#define CPU_LITTLE_ENDIAN       1234 /* LSB first: x86 */
#define CPU_BIG_ENDIAN          4321 /* MSB first: ppc, sparc */

#define DC_TIMEVAL_GT(a, b)                                             \
    ((a)->tv_sec > (b)->tv_sec ||                                       \
        ((a)->tv_sec == (b)->tv_sec && (a)->tv_usec > (b)->tv_usec))

#define DC_TIMEVAL_LT(a, b)                                             \
    ((a)->tv_sec < (b)->tv_sec ||                                       \
        ((a)->tv_sec == (b)->tv_sec && (a)->tv_usec < (b)->tv_usec))

#define DC_TIMEVAL_ADD_MSEC(a, msec) do {                               \
    (a)->tv_sec += (msec) / 1000;                                       \
    (a)->tv_usec += ((msec) % 1000) * 1000;                             \
    if ((a)->tv_usec >= 1000000L)                                       \
    {                                                                   \
        (a)->tv_sec++;                                                  \
        (a)->tv_usec -= 1000000L;                                       \
    }                                                                   \
} while(0)

#define DC_TIMEVAL_DIFF_MSEC(a, b)                                      \
    (((a)->tv_sec - (b)->tv_sec) * 1000 + ((a)->tv_usec - (b)->tv_usec) / 1000)

/*
 * Endiannes conversion of unaligned types
 */

#define ua_get_le16(ptr)        DC_CONVERT_LE(get16, ptr, 0)
#define ua_get_le32(ptr)        DC_CONVERT_LE(get32, ptr, 0)
#define ua_set_le16(ptr, val)   DC_CONVERT_LE(set16, ptr, val)
#define ua_set_le32(ptr, val)   DC_CONVERT_LE(set32, ptr, val)

#define ua_get_be16(ptr)        DC_CONVERT_BE(get16, ptr, 0)
#define ua_get_be32(ptr)        DC_CONVERT_BE(get32, ptr, 0)
#define ua_set_be16(ptr, val)   DC_CONVERT_BE(set16, ptr, val)
#define ua_set_be32(ptr, val)   DC_CONVERT_BE(set32, ptr, val)

/*
 * Endianess conversion of basic types to/from host byte order
 */

#define dc_letoh16(val)         DC_SWAP_LE(((uint16_t)val), 16)
#define dc_letoh32(val)         DC_SWAP_LE(((uint32_t)val), 32)
#define dc_htole16(val)         DC_SWAP_LE(((uint16_t)val), 16)
#define dc_htole32(val)         DC_SWAP_LE(((uint32_t)val), 32)

#define dc_betoh16(val)         DC_SWAP_BE(((uint16_t)val), 16)
#define dc_betoh32(val)         DC_SWAP_BE(((uint32_t)val), 32)
#define dc_htobe16(val)         DC_SWAP_BE(((uint16_t)val), 16)
#define dc_htobe32(val)         DC_SWAP_BE(((uint32_t)val), 32)

/*
 * Endianess conversion of basic types between host and network byte order
 */
#define dc_hton32(val)          dc_htobe32(val)
#define dc_hton16(val)          dc_htobe16(val)
#define dc_ntoh32(val)          dc_betoh32(val)
#define dc_ntoh16(val)          dc_betoh16(val)

/*
 * Unaligned types macros implementation
 */

#define DC_CONVERT_BE(func, ptr, val)  \
    dc_packed_##func(ptr, (val), 1, 0, 3, 2, 1, 0)

#define DC_CONVERT_LE(func, ptr, val)  \
    dc_packed_##func(ptr, (val), 0, 1, 0, 1, 2, 3)

#define DC_PGET(ptr, byte)        (((uint8_t *)(ptr))[byte])
#define DC_PSET(ptr, byte, val)    ((uint8_t *)(ptr))[byte] = (uint8_t)(val)

#define dc_packed_get16(ptr, val, a0, a1, b0, b1, b2, b3)               \
    ((((uint16_t)DC_PGET(ptr, a0)) <<  0) |                             \
        (((uint16_t)DC_PGET(ptr, a1)) <<  8))

#define dc_packed_get32(ptr, val, a0, a1, b0, b1, b2, b3)               \
    ((((uint32_t)DC_PGET(ptr, b0)) <<  0) |                             \
        (((uint32_t)DC_PGET(ptr, b1)) <<  8) |                          \
        (((uint32_t)DC_PGET(ptr, b2)) << 16) |                          \
        (((uint32_t)DC_PGET(ptr, b3)) << 24))

#define dc_packed_set16(ptr, val, a0, a1, b0, b1, b2, b3)               \
    do {                                                                \
        DC_PSET(ptr, a0, (val) >>  0);                                  \
        DC_PSET(ptr, a1, (val) >>  8);                                  \
    } while (0)

#define dc_packed_set32(ptr, val, a0, a1, b0, b1, b2, b3)               \
    do {                                                                \
        DC_PSET(ptr, b0, (val) >>  0);                                  \
        DC_PSET(ptr, b1, (val) >>  8);                                  \
        DC_PSET(ptr, b2, (val) >> 16);                                  \
        DC_PSET(ptr, b3, (val) >> 24);                                  \
    } while (0)

/*
 * Basic types macros implementation
 */
#if CPU_BYTE_ORDER == CPU_BIG_ENDIAN

# define DC_SWAP_BE     DC_NO_SWAP
# define DC_SWAP_LE     DC_SWAP

#elif CPU_BYTE_ORDER == CPU_LITTLE_ENDIAN

# define DC_SWAP_BE     DC_SWAP
# define DC_SWAP_LE     DC_NO_SWAP

#else

# error "Unknown byte order"

#endif

#define DC_SWAP(val, bits)         dc_bswap##bits(val)
#define DC_NO_SWAP(val, bits)      (val)

#define dc_bswap16(val)                                                 \
    (((val) >> 8) | (((val) << 8) & 0xff00))

#define dc_bswap32(val)                                                 \
    (((val) >> 24) | (((val) >> 8) & 0x0000ff00) |                      \
        (((val) << 8) & 0x00ff0000) | ((val) << 24))

#endif

