#ifndef PARTIALBUF_INC
#define PARTIALBUF_INC

#include "lte-ncm.h"

struct partial_buf
{
	unsigned int total_len;
	unsigned char* buffer;
	unsigned int offset;
};

void
pb_init(
	struct partial_buf* pb,
	unsigned char* buffer,
	unsigned int length
	);

size_t
pb_length(
	struct partial_buf* pb
	);

u8
pb_get_byte(
	struct partial_buf* pb
	);

unsigned int
pb_get_bytes(
	struct partial_buf* pb,
	unsigned char* buffer,
	unsigned int length
	);

u16
pb_get_word(
	struct partial_buf* pb
	);

u32
pb_get_dword(
	struct partial_buf* pb
	);

void
pb_put_byte(
	struct partial_buf* pb,
	unsigned char c
	);

void
pb_put_bytes(
	struct partial_buf* pb,
	unsigned char* buffer,
	unsigned int length
	);

void
pb_fill_bytes(
	struct partial_buf* pb,
	unsigned char c,
	unsigned int count
	);


void
pb_put_word(
	struct partial_buf* pb,
	u16 w
	);

void
pb_put_dword(
	struct partial_buf* pb,
	u32 d
	);

unsigned int
pb_offset(
	struct partial_buf* pb
	);

void
pb_skip(
	struct partial_buf* pb,
	unsigned int count
	);

unsigned char*
pb_get_current_address(
	struct partial_buf* pb
	);

#endif // PARTIALBUF_INC
// end of file
