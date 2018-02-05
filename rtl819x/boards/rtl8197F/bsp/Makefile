# Realtek Semiconductor Corp.
#
# bsp/Makefile
#     bsp Makefile
#
# Copyright (C) 2006-2012 Tony Wu (tonywu@realtek.com)
#

CPPFLAGS_vmlinux.lds := $(KBUILD_CFLAGS)

# mips-ori
#core-y   += arch/mips/bsp/
#cflags-y += -Iinclude/asm-mips -Iarch/mips/bsp/
#load-y   += 0x80000000

extra-y  += vmlinux.lds

obj-y := prom.o setup.o irq.o timer.o serial.o platform.o clk.o
obj-$(CONFIG_PCI) += pci-fixup.o pci-ops.o
#obj-$(CONFIG_PCI) += pci.o
obj-$(CONFIG_RTL_819X) += pci.o
obj-$(CONFIG_USB) += dev_usb.o
obj-$(CONFIG_SPI_SHEIPA) += mtd.o
obj-$(CONFIG_RTL819X_GPIO) += gpio.o
obj-$(CONFIG_RTL819X_DW_SPI)  += spi.o
obj-$(CONFIG_SND_RTL819XD_SOC_I2S) += i2s.o
obj-$(CONFIG_I2C_DESIGNWARE_PLATFORM)  += i2c.o
