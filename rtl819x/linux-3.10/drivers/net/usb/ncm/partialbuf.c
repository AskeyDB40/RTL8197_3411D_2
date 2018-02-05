#include "partialbuf.h"

void
pb_init(
	struct partial_buf* pb,
	unsigned char* buffer,
	unsigned int length
	)
{
	pb->total_len = length;
	pb->buffer = buffer;
	pb->offset = 0;
}


u8
pb_get_byte(
	struct partial_buf* pb
	)
{
	unsigned char c;
	c = pb->buffer[pb->offset];
	pb->offset += 1;
	return c;
}

unsigned int
pb_get_bytes(
	struct partial_buf* pb,
	unsigned char* buffer,
	unsigned int length
	)
{
	unsigned int ret = length;

	for (; length; --length)
		*buffer++ = pb_get_byte(pb);

	return ret;
}



u16
pb_get_word(
	struct partial_buf* pb
	)
{
	u16 s;
	s = *(u16*)(&pb->buffer[pb->offset]);
	pb->offset += sizeof(u16);
	return s;
}


u32
pb_get_dword(
	struct partial_buf* pb
	)
{
	unsigned int d;
	d = *(u32*)(&pb->buffer[pb->offset]);
	pb->offset += sizeof(u32);
	return d;

}


void
pb_put_byte(
	struct partial_buf* pb,
	unsigned char c
	)
{
	pb->buffer[pb->offset] = c;
	pb->offset += 1;
}


void
pb_put_bytes(
	struct partial_buf* pb,
	unsigned char* buffer,
	unsigned int length
	)
{
	for (; length; --length)
		pb_put_byte(pb, *buffer++);
}


void
pb_fill_bytes(
	struct partial_buf* pb,
	unsigned char c,
	unsigned int count
	)
{
	for (; count; --count)
		pb_put_byte(pb, c);
}


void
pb_put_word(
	struct partial_buf* pb,
	u16 s
	)
{
	*(u16*)&pb->buffer[pb->offset] = s;
	pb->offset += sizeof(u16);
}


void
pb_put_dword(
	struct partial_buf* pb,
	u32 d
	)
{
	*(u32*)&pb->buffer[pb->offset] = d;
	pb->offset += sizeof(u32);
}

size_t
pb_length(
	struct partial_buf* pb
	)
{
	return pb->total_len - pb->offset;
}

unsigned int
pb_offset(
	struct partial_buf* pb
	)
{
	return pb->offset;
}


void
pb_skip(
	struct partial_buf* pb,
	unsigned int count
	)
{
	pb->offset += count;
}


unsigned char*
pb_get_current_address(
	struct partial_buf* pb
	)
{
	return &pb->buffer[pb->offset];
}
