usage:

for 98c/97f 
cp ecc/98c_ecc.cpp bch6.cpp

for 8881a/97d/97dn
cp ecc/8881a_ecc.cpp bch6.cpp

for 98c/8881a/97d/97dn 
page size 2k, bbi_dma_offset  2000 ,  bbi_swap_offset   23

prepare 3 file:   boot linux.bin squashfs.o
./eccenc --chunk-size 2048 --spare-size 64 --chunk-per-block 64 -bso 23 -bdo 2000 -ctype RTL8198C_NAND -edn BIG_ENDIAN boot 0 linux.bin 9437184 squashfs.o 14680064

 will create burn.bin.ecc file


for 97f
page size 2k, bbi_dma_offset  4294967295 ,  bbi_swap_offset   4294967295

prepare 3 file:   boot.bin linux.bin squashfs.o
for spi nand
./eccenc --chunk-size 2048 --spare-size 64 --chunk-per-block 64 -bso 4294967295 -bdo 4294967295 -ctype RTL8197F_SPI_NAND -edn LITTLE_ENDIAN boot.bin 0 linux.bin 9437184 squashfs.o 14680064
 will create burn.bin.ecc.convert file

for 97f parallel nand
./eccenc --chunk-size 2048 --spare-size 64 --chunk-per-block 64 -bso 4294967295 -bdo 4294967295 -ctype RTL8197F_PARALLEL_NAND -edn LITTLE_ENDIAN boot.bin 0 linux.bin 9437184 squashfs.o 14680064

 will create burn.bin.ecc file







