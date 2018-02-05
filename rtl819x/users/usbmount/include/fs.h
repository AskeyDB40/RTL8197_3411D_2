#ifndef __FS_H
#define __FS_H

#define NONE		-1
#define MBR			2
#define EXT2		3
#define EXT3		4
#define FAT			5
#define HFSPLUS		6
#define EFI			7
#define NTFS		8
#define EXTENDED	9
#define EXT4		10
#define EXFAT		11

int detect_fs(const char *device);

#endif /* __FS_H */