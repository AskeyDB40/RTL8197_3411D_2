#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995, 1998, 2001, 2002 by Ralf Baechle
# Copyright (C) 2004  Maciej W. Rozycki
#

#
# Some DECstations need all possible sections of an ECOFF executable
#

# start for uImage
targets += vmlinux.bin.gz
$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

targets += vmlinux.bin.lzma
$(obj)/vmlinux.bin.lzma: $(obj)/vmlinux.bin FORCE
	$(call if_changed,lzma)

targets += vmlinux.bin.lzo
$(obj)/vmlinux.bin.lzo: $(obj)/vmlinux.bin
	$(call if_changed,lzo)

MKIMAGE = mkimage
targets += uImage
quiet_cmd_uImage = MKIMAGE $@
cmd_uImage = $(MKIMAGE) -A mips -O linux -T kernel -C lzma -a $(VMLINUX_LOAD_ADDRESS) \
			 -e 0x$(shell $(NM) $(VMLINUX) | grep ' kernel_entry' | cut -f1 -d ' ') \
			 -n MIPS -d $(obj)/vmlinux.bin.lzma $(obj)/uImage.lzma

$(obj)/uImage: $(obj)/vmlinux.bin.lzma FORCE
	$(call if_changed,uImage,none)

# end for uImage

ifdef CONFIG_MACH_DECSTATION
  e2eflag := -a
endif

#
# Drop some uninteresting sections in the kernel.
# This is only relevant for ELF kernels but doesn't hurt a.out
#
drop-sections := .reginfo .mdebug .comment .note .pdr .options .MIPS.options
strip-flags   := $(addprefix --remove-section=,$(drop-sections))

hostprogs-y := elf2ecoff

targets := vmlinux.ecoff
quiet_cmd_ecoff = ECOFF	  $@
      cmd_ecoff = $(obj)/elf2ecoff $(VMLINUX) $@ $(e2eflag)
$(obj)/vmlinux.ecoff: $(obj)/elf2ecoff $(VMLINUX) FORCE
	$(call if_changed,ecoff)

targets += vmlinux.bin
quiet_cmd_bin = OBJCOPY $@
      cmd_bin = $(OBJCOPY) -O binary $(strip-flags) $(VMLINUX) $@
$(obj)/vmlinux.bin: $(VMLINUX) FORCE
	$(call if_changed,bin)

targets += vmlinux.srec
quiet_cmd_srec = OBJCOPY $@
      cmd_srec = $(OBJCOPY) -S -O srec $(strip-flags) $(VMLINUX) $@
$(obj)/vmlinux.srec: $(VMLINUX) FORCE
	$(call if_changed,srec)
