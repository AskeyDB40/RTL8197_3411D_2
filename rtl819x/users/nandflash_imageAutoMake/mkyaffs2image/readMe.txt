
how to ues

cd ./yaffs2/utils
make

mkyaffs2image dir bin convert(if platform is big endian)


for different page size
1. 2048
code:yaffs2/utils/mkyaffs2image.c
#define chunkSize (2048-16)

2. 4096
code:yaffs2/utils/mkyaffs2image.c
#define chunkSize (4096-16)
