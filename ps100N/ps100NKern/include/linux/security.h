AREA
	select FB_CFB_IMAGEBLIT
	select FB_MACMODES
	help
	  This driver supports a frame buffer for the "valkyrie" graphics
	  adapter in some Power Macintoshes.

config FB_CT65550
	bool "Chips 65550 display support"
	depends on (FB = y) && PPC32 && PCI
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the Chips & Technologies
	  65550 graphics chip in PowerBooks.

config FB_ASILIANT
	bool "Asiliant (Chips) 69000 display support"
	depends on (FB = y) && PCI
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the Asiliant 69030 chipset

config FB_IMSTT
	bool "IMS Twin Turbo display support"
	depends on (FB = y) && PCI
	select FB_CFB_IMAGEBLIT
	select FB_MACMODES if PPC
	help
	  The IMS Twin Turbo is a PCI-based frame buffer card bundled with
	  many Macintosh and compatible computers.

config FB_VGA16
	tristate "VGA 16-color graphics support"
	depends on FB && (X86 || PPC)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select VGASTATE
	select FONT_8x16 if FRAMEBUFFER_CONSOLE
	help
	  This is the frame buffer device driver for VGA 16 color graphic
	  cards. Say Y if you have such a card.

	  To compile this driver as a module, choose M here: the
	  module will be called vga16fb.

config FB_BF54X_LQ043
	tristate "SHARP LQ043 TFT LCD (BF548 EZKIT)"
	depends on FB && (BF54x) && !BF542
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	 This is the framebuffer device driver for a SHARP LQ043T1DG01 TFT LCD

config FB_BFIN_T350MCQB
	tristate "Varitronix COG-T350MCQB TFT LCD display (BF527 EZKIT)"
	depends on FB && BLACKFIN
	select BFIN_GPTIMERS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	 This is the framebuffer device driver for a Varitronix VL-PS-COG-T350MCQB-01 display TFT LCD
	 This display is a QVGA 320x240 24-bit RGB display interfaced by an 8-bit wide PPI
	 It uses PPI[0..7] PPI_FS1, PPI_FS2 and PPI_CLK.


config FB_STI
	tristate "HP STI frame buffer device support"
	depends on FB && PARISC
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select STI_CONSOLE
	select VT
	default y
	---help---
	  STI refers to the HP "Standard Text Interface" which is a set of
	  BIOS routines contained in a ROM chip in HP PA-RISC based machines.
	  Enabling this option will implement the linux framebuffer device
	  using calls to the STI BIOS routines for initialisation.
	
	  If you enable this option, you will get a planar framebuffer device
	  /dev/fb which will work on the most common HP graphic cards of the
	  NGLE family, including the artist chips (in the 7xx and Bxxx series),
	  HCRX, HCRX24, CRX, CRX24 and VisEG series.

	  It is safe to enable this option, so you should probably say "Y".

config FB_MAC
	bool "Generic Macintosh display support"
	depends on (FB = y) && MAC
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_MACMODES

config FB_HP300
	bool
	depends on (FB = y) && DIO
	select FB_CFB_IMAGEBLIT
	default y

config FB_TGA
	tristate "TGA/SFB+ framebuffer support"
	depends on FB && (ALPHA || TC)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select BITREVERSE
	---help---
	  This is the frame buffer device driver for generic TGA and SFB+
	  graphic cards.  These include DEC ZLXp-E1, -E2 and -E3 PCI cards,
	  also known as PBXGA-A, -B and -C, and DEC ZLX-E1, -E2 and -E3
	  TURBOchannel cards, also known as PMAGD-A, -B and -C.

	  Due to hardware limitations ZLX-E2 and E3 cards are not supported
	  for DECstation 5000/200 systems.  Additionally due to firmware
	  limitations these cards may cause troubles with booting DECstation
	  5000/240 and /260 systems, but are fully supported under Linux if
	  you manage to get it going. ;-)

	  Say Y if you have one of those.

config FB_UVESA
	tristate "Userspace VESA VGA graphics support"
	depends on FB && CONNECTOR
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_MODE_HELPERS
	help
	  This is the frame buffer driver for generic VBE 2.0 compliant
	  graphic cards. It can also take advantage of VBE 3.0 features,
	  such as refresh rate adjustment.

	  This driver generally provides more features than vesafb but
	  requires a userspace helper application called 'v86d'. See
	  <file:Documentation/fb/uvesafb.txt> for more information.

	  If unsure, say N.

config FB_VESA
	bool "VESA VGA graphics support"
	depends on (FB = y) && X86
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_BOOT_VESA_SUPPORT
	help
	  This is the frame buffer device driver for generic VESA 2.0
	  compliant graphic cards. The older VESA 1.2 cards are not supported.
	  You will get a boot time penguin logo at no additional cost. Please
	  read <file:Documentation/fb/vesafb.txt>. If unsure, say Y.

config FB_EFI
	bool "EFI-based Framebuffer Support"
	depends on (FB = y) && X86 && EFI
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the EFI frame buffer device driver. If the firmware on
	  your platform is EFI 1.10 or UEFI 2.0, select Y to add support for
	  using the EFI framebuffer as your console.

config FB_N411
       tristate "N411 Apollo/Hecuba devkit support"
       depends on FB && X86 && MMU
       select FB_SYS_FILLRECT
       select FB_SYS_COPYAREA
       select FB_SYS_IMAGEBLIT
       select FB_SYS_FOPS
       select FB_DEFERRED_IO
       select FB_HECUBA
       help
         This enables support for the Apollo display controller in its
         Hecuba form using the n411 devkit.

config FB_HGA
	tristate "Hercules mono graphics support"
	depends on FB && X86
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  Say Y here if you have a Hercules mono graphics card.

	  To compile this driver as a module, choose M here: the
	  module will be called hgafb.

	  As this card technology is 15 years old, most people will answer N
	  here.

config FB_HGA_ACCEL
	bool "Hercules mono Acceleration functions (EXPERIMENTAL)"
	depends on FB_HGA && EXPERIMENTAL
	---help---
	This will compile the Hercules mono graphics with
	acceleration functions.

config FB_SGIVW
	tristate "SGI Visual Workstation framebuffer support"
	depends on FB && X86_VISWS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  SGI Visual Workstation support for framebuffer graphics.

config FB_GBE
	bool "SGI Graphics Backend frame buffer support"
	depends on (FB = y) && (SGI_IP32 || X86_VISWS)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
 	help
	  This is the frame buffer device driver for SGI Graphics Backend.
	  This chip is used in SGI O2 and Visual Workstation 320/540.

config FB_GBE_MEM
	int "Video memory size in MB"
	depends on FB_GBE
	default 4
	help
	  This is the amount of memory reserved for the framebuffer,
	  which can be any value between 1MB and 8MB.

config FB_SBUS
	bool "SBUS and UPA framebuffers"
	depends on (FB = y) && SPARC
	help
	  Say Y if you want support for SBUS or UPA based frame buffer device.

config FB_BW2
	bool "BWtwo support"
	depends on (FB = y) && (SPARC && FB_SBUS)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the BWtwo frame buffer.

config FB_CG3
	bool "CGthree support"
	depends on (FB = y) && (SPARC && FB_SBUS)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the CGthree frame buffer.

config FB_CG6
	bool "CGsix (GX,TurboGX) support"
	depends on (FB = y) && (SPARC && FB_SBUS)
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the CGsix (GX, TurboGX)
	  frame buffer.

config FB_FFB
	bool "Creator/Creator3D/Elite3D support"
	depends on FB_SBUS && SPARC64
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the Creator, Creator3D,
	  and Elite3D graphics boards.

config FB_TCX
	bool "TCX (SS4/SS5 only) support"
	depends on FB_SBUS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the TCX 24/8bit frame
	  buffer.

config FB_CG14
	bool "CGfourteen (SX) support"
	depends on FB_SBUS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the CGfourteen frame
	  buffer on Desktop SPARCsystems with the SX graphics option.

config FB_P9100
	bool "P9100 (Sparcbook 3 only) support"
	depends on FB_SBUS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the P9100 card
	  supported on Sparcbook 3 machines.

config FB_LEO
	bool "Leo (ZX) support"
	depends on FB_SBUS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the frame buffer device driver for the SBUS-based Sun ZX
	  (leo) frame buffer cards.

config FB_IGA
	bool "IGA 168x display support"
	depends on (FB = y) && SPARC32
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the framebuffer device for the INTERGRAPHICS 1680 and
	  successor frame buffer cards.

config FB_XVR500
	bool "Sun XVR-500 3DLABS Wildcat support"
	depends on (FB = y) && PCI && SPARC64
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the framebuffer device for the Sun XVR-500 and similar
	  graphics cards based upon the 3DLABS Wildcat chipset.  The driver
	  only works on sparc64 systems where the system firmware has
	  mostly initialized the card already.  It is treated as a
	  completely dumb framebuffer device.

config FB_XVR2500
	bool "Sun XVR-2500 3DLABS Wildcat support"
	depends on (FB = y) && PCI && SPARC64
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This is the framebuffer device for the Sun XVR-2500 and similar
	  graphics cards based upon the 3DLABS Wildcat chipset.  The driver
	  only works on sparc64 systems where the system firmware has
	  mostly initialized the card already.  It is treated as a
	  completely dumb framebuffer device.

config FB_PVR2
	tristate "NEC PowerVR 2 display support"
	depends on FB && SH_DREAMCAST
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Say Y here if you have a PowerVR 2 card in your box.  If you plan to
	  run linux on your Dreamcast, you will have to say Y here.
	  This driver may or may not work on other PowerVR 2 cards, but is
	  totally untested.  Use at your own risk.  If unsure, say N.

	  To compile this driver as a module, choose M here: the
	  module will be called pvr2fb.

	  You can pass several parameters to the driver at boot time or at
	  module load time.  The parameters look like "video=pvr2:XXX", where
	  the meaning of XXX can be found at the end of the main source file
	  (<file:drivers/video/pvr2fb.c>). Please see the file
	  <file:Documentation/fb/pvr2fb.txt>.

config FB_EPSON1355
	bool "Epson 1355 framebuffer support"
	depends on (FB = y) && ARCH_CEIVA
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  Build in support for the SED1355 Epson Research Embedded RAMDAC
	  LCD/CRT Controller (since redesignated as the S1D13505) as a
	  framebuffer.  Product specs at
	  <http://www.erd.epson.com/vdc/html/products.htm>.

config FB_S1D13XXX
	tristate "Epson S1D13XXX framebuffer support"
	depends on FB
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  Support for S1D13XXX framebuffer device family (currently only
	  working with S1D13806). Product specs at
	  <http://www.erd.epson.com/vdc/html/legacy_13xxx.htm>

config SPLC501C
	tristate "LCD_SPLC501C support"
	select LCD_SPLC501C
	help
	  Support for LCD_SPLC501C
	  
config FB_ATMEL
	tristate "AT91/AT32 LCD Controller support"
	depends on FB && (ARCH_AT91SAM9261 || ARCH_AT91SAM9263 || ARCH_AT91SAM9RL || ARCH_AT91CAP9 || AVR32)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This enables support for the AT91/AT32 LCD Controller.

config FB_INTSRAM
	bool "Frame Buffer in internal SRAM"
	depends on FB_ATMEL && ARCH_AT91SAM9261
	help
	  Say Y if you want to map Frame Buffer in internal SRAM. Say N if you want
	  to let frame buffer in external SDRAM.

config FB_ATMEL_STN
	bool "Use a STN display with AT91/AT32 LCD Controller"
	depends on FB_ATMEL && MACH_AT91SAM9261EK
	default n
	help
	  Say Y if you want to connect a STN LCD display to the AT91/AT32 LCD
	  Controller. Say N if you want to connect a TFT.

	  If unsure, say N.

config FB_NVIDIA
	tristate "nVidia Framebuffer Support"
	depends on FB && PCI
	select FB_BACKLIGHT if FB_NVIDIA_BACKLIGHT
	select FB_MODE_HELPERS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select BITREVERSE
	select VGASTATE
	help
	  This driver supports graphics boards with the nVidia chips, TNT
	  and newer. For very old chipsets, such as the RIVA128, then use
	  the rivafb.
	  Say Y if you have such a graphics board.

	  To compile this driver as a module, choose M here: the
	  module will be called nvidiafb.

config FB_NVIDIA_I2C
       bool "Enable DDC Support"
       depends on FB_NVIDIA
       select FB_DDC
       help
	  This enables I2C support for nVidia Chipsets.  This is used
	  only for getting EDID information from the attached display
	  allowing for robust video mode handling and switching.

	  Because fbdev-2.6 requires that drivers must be able to
	  independently validate video mode parameters, you should say Y
	  here.

config FB_NVIDIA_DEBUG
	bool "Lots of debug output"
	depends on FB_NVIDIA
	default n
	help
	  Say Y here if you want the nVidia driver to output all sorts
	  of debugging information to provide to the maintainer when
	  something goes wrong.

config FB_NVIDIA_BACKLIGHT
	bool "Support for backlight control"
	depends on FB_NVIDIA
	default y
	help
	  Say Y here if you want to control the backlight of your display.

config FB_RIVA
	tristate "nVidia Riva support"
	depends on FB && PCI
	select FB_BACKLIGHT if FB_RIVA_BACKLIGHT
	select FB_MODE_HELPERS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select BITREVERSE
	select VGASTATE
	help
	  This driver supports graphics boards with the nVidia Riva/Geforce
	  chips.
	  Say Y if you have such a graphics board.

	  To compile this driver as a module, choose M here: the
	  module will be called rivafb.

config FB_RIVA_I2C
       bool "Enable DDC Support"
       depends on FB_RIVA
       select FB_DDC
       help
	  This enables I2C support for nVidia Chipsets.  This is used
	  only for getting EDID information from the attached display
	  allowing for robust video mode handling and switching.

	  Because fbdev-2.6 requires that drivers must be able to
	  independently validate video mode parameters, you should say Y
	  here.

config FB_RIVA_DEBUG
	bool "Lots of debug output"
	depends on FB_RIVA
	default n
	help
	  Say Y here if you want the Riva driver to output all sorts
	  of debugging information to provide to the maintainer when
	  something goes wrong.

config FB_RIVA_BACKLIGHT
	bool "Support for backlight control"
	depends on FB_RIVA
	default y
	help
	  Say Y here if you want to control the backlight of your display.

config FB_I810
	tristate "Intel 810/815 support (EXPERIMENTAL)"
	depends on EXPERIMENTAL && FB && PCI && X86_32 && AGP_INTEL
	select FB_MODE_HELPERS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select VGASTATE
	help
	  This driver supports the on-board graphics built in to the Intel 810 
          and 815 chipsets.  Say Y if you have and plan to use such a board.

          To compile this driver as a module, choose M here: the
	  module will be called i810fb.

          For more information, please read 
	  <file:Documentation/fb/intel810.txt>

config FB_I810_GTF
	bool "use VESA Generalized Timing Formula"
	depends on FB_I810
	help
	  If you say Y, then the VESA standard, Generalized Timing Formula 
          or GTF, will be used to calculate the required video timing values
	  per video mode.  Since the GTF allows nondiscrete timings 
          (nondiscrete being a range of values as opposed to discrete being a
          set of values), you'll be able to use any combination of horizontal 
	  and vertical resolutions, and vertical refresh rates without having
	  to specify your own timing parameters.  This is especially useful
	  to maximize the performance of an aging display, or if you just 
          have a display with nonstandard dimensions. A VESA compliant 
	  monitor is recommended, but can still work with non-compliant ones.
	  If you need or want this, then select this option. The timings may 
	  not be compliant with Intel's recommended values. Use at your own 
	  risk.

          If you say N, the driver will revert to discrete video timings 
	  using a set recommended by Intel in their documentation.
  
          If unsure, say N.

config FB_I810_I2C
	bool "Enable DDC Support"
	depends on FB_I810 && FB_I810_GTF
	select FB_DDC
	help

config FB_LE80578
	tristate "Intel LE80578 (Vermilion) support"
	depends on FB && PCI && X86
	select FB_MODE_HELPERS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	help
	  This driver supports the LE80578 (Vermilion Range) chipset

config FB_CARILLO_RANCH
	tristate "Intel Carillo Ranch support"
	depends on FB_LE80578 && FB && PCI && X86
	help
	  This driver supports the LE80578 (Carillo Ranch) board

config FB_INTEL
	tristate "Intel 830M/845G/852GM/855GM/865G/915G/945G/945GM/965G/965GM support (EXPERIMENTAL)"
	depends on EXPERIMENTAL && FB && PCI && X86 && AGP_INTEL && EMBEDDED
	select FB_MODE_HELPERS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_BOOT_VESA_SUPPORT if FB_INTEL = y
	depends on !DRM_I915
	help
	  This driver supports the on-board graphics built in to the Intel
          830M/845G/852GM/855GM/865G/915G/915GM/945G/945GM/965G/965GM chipsets.
          Say Y if you have and plan to use such a board.

	  To make FB_INTELFB=Y work you need to say AGP_INTEL=y too.

	  To compile this driver as a module, choose M here: the
	  module will be called intelfb.

	  For more information, please read <file:Documentation/fb/intelfb.txt>

config FB_INTEL_DEBUG
	bool "Intel driver Debug Messages"
	depends on FB_INTEL
	---help---
	  Say Y here if you want the Intel driver to output all sorts
	  of debugging information to provide to the maintainer when
	  something goes wrong.

config FB_INTEL_I2C
	bool "DDC/I2C for Intel framebuffer support"
	depends on FB_INTEL
	select FB_DDC
	default y
	help
	  Say Y here if you want DDC/I2C support for your on-board Intel graphics.

config FB_MATROX
	tristate "Matrox acceleration"
	depends on FB && PCI
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_TILEBLITTING
	select FB_MACMODES if PPC_PMAC
	---help---
	  Say Y here if you have a Matrox Millennium, Matrox Millennium II,
	  Matrox Mystique, Matrox Mystique 220, Matrox Productiva G100, Matrox
	  Mystique G200, Matrox Millennium G200, Matrox Marvel G200 video,
	  Matrox G400, G450 or G550 card in your box.

	  To compile this driver as a module, choose M here: the
	  module will be called matroxfb.

	  You can pass several parameters to the driver at boot time or at
	  module load time. The parameters look like "video=matrox:XXX", and
	  are described in <file:Documentation/fb/matroxfb.txt>.

config FB_MATROX_MILLENIUM
	bool "Millennium I/II support"
	depends on FB_MATROX
	help
	  Say Y here if you have a Matrox Millennium or Matrox Millennium II
	  video card. If you select "Advanced lowlevel driver options" below,
	  you should check 4 bpp packed pixel, 8 bpp packed pixel, 16 bpp
	  packed pixel, 24 bpp packed pixel and 32 bpp packed pixel. You can
	  also use font widths different from 8.

config FB_MATROX_MYSTIQUE
	bool "Mystique support"
	depends on FB_MATROX
	help
	  Say Y here if you have a Matrox Mystique or Matrox Mystique 220
	  video card. If you select "Advanced lowlevel driver options" below,
	  you should check 8 bpp packed pixel, 16 bpp packed pixel, 24 bpp
	  packed pixel and 32 bpp packed pixel. You can also use font widths
	  different from 8.

config FB_MATROX_G
	bool "G100/G200/G400/G450/G550 support"
	depends on FB_MATROX
	---help---
	  Say Y here if you have a Matrox G100, G200, G400, G450 or G550 based
	  video card. If you select "Advanced lowlevel driver options", you
	  should check 8 bpp packed pixel, 16 bpp packed pixel, 24 bpp packed
	  pixel and 32 bpp packed pixel. You can also use font widths
	  different from 8.

	  If you need support for G400 secondary head, you must say Y to
	  "Matrox I2C support" and "G400 second head support" right below.
	  G450/G550 secondary head and digital output are supported without
	  additional modules.

	  The driver starts in monitor mode. You must use the matroxset tool 
	  (available at <ftp://platan.vc.cvut.cz/pub/linux/matrox-latest/>) to 
	  swap primary and secondary head outputs, or to change output mode.  
	  Secondary head driver always start in 640x480 resolution and you 
	  must use fbset to change it.

	  Do not forget that second head supports only 16 and 32 bpp
	  packed pixels, so it is a good idea to compile them into the kernel
	  too. You can use only some font widths, as the driver uses generic
	  painting procedures (the secondary head does not use acceleration
	  engine).

	  G450/G550 hardware can display TV picture only from secondary CRTC,
	  and it performs no scaling, so picture must have 525 or 625 lines.

config FB_MATROX_I2C
	tristate "Matrox I2C support"
	depends on FB_MATROX
	select FB_DDC
	---help---
	  This drivers creates I2C buses which are needed for accessing the
	  DDC (I2C) bus present on all Matroxes, an I2C bus which
	  interconnects Matrox optional devices, like MGA-TVO on G200 and
	  G400, and the secondary head DDC bus, present on G400 only.

	  You can say Y or M here if you want to experiment with monitor
	  detection code. You must say Y or M here if you want to use either
	  second head of G400 or MGA-TVO on G200 or G400.

	  If you compile it as module, it will create a module named
	  i2c-matroxfb.

config FB_MATROX_MAVEN
	tristate "G400 second head support"
	depends on FB_MATROX_G && FB_MATROX_I2C
	---help---
	  WARNING !!! This support does not work with G450 !!!

	  Say Y or M here if you want to use a secondary head (meaning two
	  monitors in parallel) on G400 or MGA-TVO add-on on G200. Secondary
	  head is not compatible with accelerated XFree 3.3.x SVGA servers -
	  secondary head output is blanked while you are in X. With XFree
	  3.9.17 preview you can use both heads if you use SVGA over fbdev or
	  the fbdev driver on first head and the fbdev driver on second head.

	  If you compile it as module, two modules are created,
	  matroxfb_crtc2 and matroxfb_maven. Matroxfb_maven is needed for
	  both G200 and G400, matroxfb_crtc2 is needed only by G400. You must
	  also load i2c-matroxfb to get it to run.

	  The driver starts in monitor mode and you must use the matroxset
	  tool (available at
	  <ftp://platan.vc.cvut.cz/pub/linux/matrox-latest/>) to switch it to
	  PAL or NTSC or to swap primary and secondary head outputs.
	  Secondary head driver also always start in 640x480 resolution, you
	  must use fbset to change it.

	  Also do not forget that second head supports only 16 and 32 bpp
	  packed pixels, so it is a good idea to compile them into the kernel
	  too.  You can use only some font widths, as the driver uses generic
	  painting procedures (the secondary head does not use acceleration
	  engine).

config FB_MATROX_MULTIHEAD
	bool "Multihead support"
	depends on FB_MATROX
	---help---
	  Say Y here if you have more than one (supported) Matrox device in
	  your computer and you want to use all of them for different monitors
	  ("multihead"). If you have only one device, you should say N because
	  the driver compiled with Y is larger and a bit slower, especially on
	  ia32 (ix86).

	  If you said M to "Matrox unified accelerated driver" and N here, you
	  will still be able to use several Matrox devices simultaneously:
	  insert several instances of the module matroxfb into the kernel
	  with insmod, supplying the parameter "dev=N" where N is 0, 1, etc.
	  for the different Matrox devices. This method is slightly faster but
	  uses 40 KB of kernel memory per Matrox card.

	  There is no need for enabling 'Matrox multihead support' if you have
	  only one Matrox card in the box.

config FB_RADEON
	tristate "ATI Radeon display support"
	depends on FB && PCI
	select FB_BACKLIGHT if FB_RADEON_BACKLIGHT
	select FB_MODE_HELPERS
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_MACMODES if PPC_OF
	help
	  Choose this option if you want to use an ATI Radeon graphics card as
	  a framebuffer device.  There are both PCI and AGP versions.  You
	  don't need to choose this to run the Radeon in plain VGA mode.

	  There is a product page at
	  http://apps.ati.com/ATIcompare/

config FB_RADEON_I2C
	bool "DDC/I2C for ATI Radeon support"
	depends on FB_RADEON
	select FB_DDC
	default y
	help
	  Say Y here if you want DDC/I2C support for your Radeon board. 

config FB_RADEON_BACKLIGHT
	bool "Support for backlight control"
	depends on FB_RADEON
	default y
	help
	  Say Y here if you want to control the backlight of your display.

config FB_RADEON_DEBUG
	bool "Lots of debug output from Radeon driver"
	depends on FB_RADEON
	default n
	help
	  Say Y here if you want the Radeon driver to output all sorts
	  of debugging information to provide to the maintainer when
	  something goes wrong.

config FB_ATY128
	tristate "ATI Rage128 display support"
	depends on FB && PCI
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_BACKLIGHT if FB_ATY128_BACKLIGHT
	select FB_MACMODES if PPC_PMAC
	help
	  This driver supports graphics boards with the ATI Rage128 chips.
	  Say Y if you have such a graphics board and read
	  <file:Documentation/fb/aty128fb.txt>.

	  To compile this driver as a module, choose M here: the
	  module will be called aty128fb.

config FB_ATY128_BACKLIGHT
	bool "Support for backlight control"
	depends on FB_ATY128
	default y
	help
	  Say Y here if you want to control the backlight of your display.

config FB_ATY
	tristate "ATI Mach64 display support" if PCI || ATARI
	depends on FB && !SPARC32
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_BACKLIGHT if FB_ATY_BACKLIGHT
	select FB_MACMODES if PPC
	help
	  This driver supports graphics boards with the ATI Mach64 chips.
	  Say Y if you have such a graphics board.

	  To compile this driver as a module, choose M here: the
	  module will be called atyfb.

config FB_ATY_CT
	bool "Mach64 CT/VT/GT/LT (incl. 3D RAGE) support"
	depends on PCI && FB_ATY
	default y if SPARC64 && PCI
	help
	  Say Y here to support use of ATI's 64-bit Rage boards (or other
	  boards based on the Mach64 CT, VT, GT, and LT chipsets) as a
	  framebuffer device.  The ATI product support page for these boards
	  is at <http://support.ati.com/products/pc/mach64/>.

config FB_ATY_GENERIC_LCD
	bool "Mach64 generic LCD support (EXPERIMENTAL)"
	depends on FB_ATY_CT
	help
	  Say Y if you have a laptop with an ATI Rage LT PRO, Rage Mobility,
	  Rage XC, or Rage XL chipset.

config FB_ATY_GX
	bool "Mach64 GX support" if PCI
	depends on FB_ATY
	default y if ATARI
	help
	  Say Y here to support use of the ATI Mach64 Graphics Expression
	  board (or other boards based on the Mach64 GX chipset) as a
	  framebuffer device.  The ATI product support page for these boards
	  is at
	  <http://support.ati.com/products/pc/mach64/graphics_xpression.html>.

config FB_ATY_BACKLIGHT
	bool "Support for backlight control"
	depends on FB_ATY
	default y
	help
	  Say Y here if you want to control the backlight of your display.

config FB_S3
	tristate "S3 Trio/Virge support"
	depends on FB && PCI
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select FB_TILEBLITTING
	select FB_SVGALIB
	select VGASTATE
	select FONT_8x16 if FRAMEBUFFER_CONSOLE
	---help---
	  Driver for graphics boards with S3 Trio / S3 Virge chip.

config FB_SAVAGE
	tristate "S3 Savage support"
	depends on FB && PCI && g received.
 *	Return 0 if permission is granted.
 *
 * Security hook for dentry
 *
 * @dentry_open
 *	Save open-time permission checking state for later use upon
 *	file_permission, and recheck access if anything has changed
 *	since inode_permission.
 *
 * Security hooks for task operations.
 *
 * @task_create:
 *	Check permission before creating a child process.  See the clone(2)
 *	manual page for definitions of the @clone_flags.
 *	@clone_flags contains the flags indicating what should be shared.
 *	Return 0 if permission is granted.
 * @cred_free:
 *	@cred points to the credentials.
 *	Deallocate and clear the cred->security field in a set of credentials.
 * @cred_prepare:
 *	@new points to the new credentials.
 *	@old points to the original credentials.
 *	@gfp indicates the atomicity of any memory allocations.
 *	Prepare a new set of credentials by copying the data from the old set.
 * @cred_commit:
 *	@new points to the new credentials.
 *	@old points to the original credentials.
 *	Install a new set of credentials.
 * @kernel_act_as:
 *	Set the credentials for a kernel service to act as (subjective context).
 *	@new points to the credentials to be modified.
 *	@secid specifies the security ID to be set
 *	The current task must be the one that nominated @secid.
 *	Return 0 if successful.
 * @kernel_create_files_as:
 *	Set the file creation context in a set of credentials to be the same as
 *	the objective context of the specified inode.
 *	@new points to the credentials to be modified.
 *	@inode points to the inode to use as a reference.
 *	The current task must be the one that nominated @inode.
 *	Return 0 if successful.
 * @task_setuid:
 *	Check permission before setting one or more of the user identity
 *	attributes of the current process.  The @flags parameter indicates
 *	which of the set*uid system calls invoked this hook and how to
 *	interpret the @id0, @id1, and @id2 parameters.  See the LSM_SETID
 *	definitions at the beginning of this file for the @flags values and
 *	their meanings.
 *	@id0 contains a uid.
 *	@id1 contains a uid.
 *	@id2 contains a uid.
 *	@flags contains one of the LSM_SETID_* values.
 *	Return 0 if permission is granted.
 * @task_fix_setuid:
 *	Update the module's state after setting one or more of the user
 *	identity attributes of the current process.  The @flags parameter
 *	indicates which of the set*uid system calls invoked this hook.  If
 *	@new is the set of credentials that will be installed.  Modifications
 *	should be made to this rather than to @current->cred.
 *	@old is the set of credentials that are being replaces
 *	@flags contains one of the LSM_SETID_* values.
 *	Return 0 on success.
 * @task_setgid:
 *	Check permission before setting one or more of the group identity
 *	attributes of the current process.  The @flags parameter indicates
 *	which of the set*gid system calls invoked this hook and how to
 *	interpret the @id0, @id1, and @id2 parameters.  See the LSM_SETID
 *	definitions at the beginning of this file for the @flags values and
 *	their meanings.
 *	@id0 contains a gid.
 *	@id1 contains a gid.
 *	@id2 contains a gid.
 *	@flags contains one of the LSM_SETID_* values.
 *	Return 0 if permission is granted.
 * @task_setpgid:
 *	Check permission before setting the process group identifier of the
 *	process @p to @pgid.
 *	@p contains the task_struct for process being modified.
 *	@pgid contains the new pgid.
 *	Return 0 if permission is granted.
 * @task_getpgid:
 *	Check permission before getting the process group identifier of the
 *	process @p.
 *	@p contains the task_struct for the process.
 *	Return 0 if permission is granted.
 * @task_getsid:
 *	Check permission before getting the session identifier of the process
 *	@p.
 *	@p contains the task_struct for the process.
 *	Return 0 if permission is granted.
 * @task_getsecid:
 *	Retrieve the security identifier of the process @p.
 *	@p contains the task_struct for the process and place is into @secid.
 *	In case of failure, @secid will be set to zero.
 *
 * @task_setgroups:
 *	Check permission before setting the supplementary group set of the
 *	current process.
 *	@group_info contains the new group information.
 *	Return 0 if permission is granted.
 * @task_setnice:
 *	Check permission before setting the nice value of @p to @nice.
 *	@p contains the task_struct of process.
 *	@nice contains the new nice value.
 *	Return 0 if permission is granted.
 * @task_setioprio
 *	Check permission before setting the ioprio value of @p to @ioprio.
 *	@p contains the task_struct of process.
 *	@ioprio contains the new ioprio value
 *	Return 0 if permission is granted.
 * @task_getioprio
 *	Check permission before getting the ioprio value of @p.
 *	@p contains the task_struct of process.
 *	Return 0 if permission is granted.
 * @task_setrlimit:
 *	Check permission before setting the resource limits of the current
 *	process for @resource to @new_rlim.  The old resource limit values can
 *	be examined by dereferencing (current->signal->rlim + resource).
 *	@resource contains the resource whose limit is being set.
 *	@new_rlim contains the new limits for @resource.
 *	Return 0 if permission is granted.
 * @task_setscheduler:
 *	Check permission before setting scheduling policy and/or parameters of
 *	process @p based on @policy and @lp.
 *	@p contains the task_struct for process.
 *	@policy contains the scheduling policy.
 *	@lp contains the scheduling parameters.
 *	Return 0 if permission is granted.
 * @task_getscheduler:
 *	Check permission before obtaining scheduling information for process
 *	@p.
 *	@p contains the task_struct for process.
 *	Return 0 if permission is granted.
 * @task_movememory
 *	Check permission before moving memory owned by process @p.
 *	@p contains the task_struct for process.
 *	Return 0 if permission is granted.
 * @task_kill:
 *	Check permission before sending signal @sig to @p.  @info can be NULL,
 *	the constant 1, or a pointer to a siginfo structure.  If @info is 1 or
 *	SI_FROMKERNEL(info) is true, then the signal should be viewed as coming
 *	from the kernel and should typically be permitted.
 *	SIGIO signals are handled separately by the send_sigiotask hook in
 *	file_security_ops.
 *	@p contains the task_struct for process.
 *	@info contains the signal information.
 *	@sig contains the signal value.
 *	@secid contains the sid of the process where the signal originated
 *	Return 0 if permission is granted.
 * @task_wait:
 *	Check permission before allowing a process to reap a child process @p
 *	and collect its status information.
 *	@p contains the task_struct for process.
 *	Return 0 if permission is granted.
 * @task_prctl:
 *	Check permission before performing a process control operation on the
 *	current process.
 *	@option contains the operation.
 *	@arg2 contains a argument.
 *	@arg3 contains a argument.
 *	@arg4 contains a argument.
 *	@arg5 contains a argument.
 *	Return -ENOSYS if no-one wanted to handle this op, any other value to
 *	cause prctl() to return immediately with that value.
 * @task_to_inode:
 *	Set the security attributes for an inode based on an associated task's
 *	security attributes, e.g. for /proc/pid inodes.
 *	@p contains the task_struct for the task.
 *	@inode contains the inode structure for the inode.
 *
 * Security hooks for Netlink messaging.
 *
 * @netlink_send:
 *	Save security information for a netlink message so that permission
 *	checking can be performed when the message is processed.  The security
 *	information can be saved using the eff_cap field of the
 *	netlink_skb_parms structure.  Also may be used to provide fine
 *	grained control over message transmission.
 *	@sk associated sock of task sending the message.,
 *	@skb contains the sk_buff structure for the netlink message.
 *	Return 0 if the information was successfully saved and message
 *	is allowed to be transmitted.
 * @netlink_recv:
 *	Check permission before processing the received netlink message in
 *	@skb.
 *	@skb contains the sk_buff structure for the netlink message.
 *	@cap indicates the capability required
 *	Return 0 if permission is granted.
 *
 * Security hooks for Unix domain networkA TURBOchannel framebuffer support"
	depends on (FB = y) && TC
 	select FB_CFB_FILLRECT
 	select FB_CFB_COPYAREA
 	select FB_CFB_IMAGEBLIT
	help
	  Support for the PMAG-AA TURBOchannel framebuffer card (1280x1024x1)
	  used mainly in the MIPS-based DECstation series.

config FB_PMAG_BA
	tristate "PMAG-BA TURBOchannel framebuffer support"
	depends on FB && TC
 	select FB_CFB_FILLRECT
 	select FB_CFB_COPYAREA
 	select FB_CFB_IMAGEBLIT
	help
	  Support for the PMAG-BA TURBOchannel framebuffer card (1024x864x8)
	  used mainly in the MIPS-based DECstation series.

config FB_PMAGB_B
	tristate "PMAGB-B TURBOchannel framebuffer support"
	depends on FB && TC
 	select FB_CFB_FILLRECT
 	select FB_CFB_COPYAREA
 	select FB_CFB_IMAGEBLIT
	help
	  Support for the PMAGB-B TURBOchannel framebuffer card used mainly
	  in the MIPS-based DECstation series. The card is currently only
	  supported in 1280x1024x8 mode.

config FB_MAXINE
	bool "Maxine (Personal DECstation) onboard framebuffer support"
	depends on (FB = y) && MACH_DECSTATION
 	select FB_CFB_FILLRECT
 	select FB_CFB_COPYAREA
 	select FB_CFB_IMAGEBLIT
	help
	  Support for the onboard framebuffer (1024x768x8) in the Personal
	  DECstation series (Personal DECstation 5000/20, /25, /33, /50,
	  Codename "Maxine").

config FB_G364
	bool "G364 frame buffer support"
	depends on (FB = y) && (MIPS_MAGNUM_4000 || OLIVETTI_M700)
 	select FB_CFB_FILLRECT
 	select FB_CFB_COPYAREA
 	select FB_CFB_IMAGEBLIT
	help
	  The G364 driver is the framebuffer used in MIPS Magnum 4000 and
	  Olivetti M700-10 systems.

config FB_68328
	bool "Motorola 68328 native frame buffer support"
	depends on (FB = y) && (M68328 || M68EZ328 || M68VZ328)
 	select FB_CFB_FILLRECT
 	select FB_CFB_COPYAREA
 	select FB_CFB_IMAGEBLIT
	help
	  Say Y here if you want to support the built-in frame buffer of
	  the Motorola 68328 CPU family.

config FB_PXA168
	tristate "PXA168/910 LCD framebuffer support"
	depends on FB && (CPU_PXA168 || CPU_PXA910)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for the built-in LCD controller in the Marvell
	  MMP processor.

config FB_PXA
	tristate "PXA LCD framebuffer support"
	depends on FB && ARCH_PXA
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for the built-in LCD controller in the Intel
	  PXA2x0 processor.

	  This driver is also available as a module ( = code which can be
	  inserted and removed from the running kernel whenever you want). The
	  module will be called pxafb. If you want to compile it as a module,
	  say M here and read <file:Documentation/kbuild/modules.txt>.

	  If unsure, say N.

config FB_PXA_OVERLAY
	bool "Support PXA27x/PXA3xx Overlay(s) as framebuffer"
	default n
	depends on FB_PXA && (PXA27x || PXA3xx)

config FB_PXA_SMARTPANEL
	bool "PXA Smartpanel LCD support"
	default n
	depends on FB_PXA

config FB_PXA_PARAMETERS
	bool "PXA LCD command line parameters"
	default n
	depends on FB_PXA
	---help---
	  Enable the use of kernel command line or module parameters
	  to configure the physical properties of the LCD panel when
	  using the PXA LCD driver.

	  This option allows you to override the panel parameters
	  supplied by the platform in order to support multiple
	  different models of flatpanel. If you will only be using a
	  single model of flatpanel then you can safely leave this
	  option disabled.

	  <file:Documentation/fb/pxafb.txt> describes the available parameters.

config FB_MBX
	tristate "2700G LCD framebuffer support"
	depends on FB && ARCH_PXA
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Framebuffer driver for the Intel 2700G (Marathon) Graphics
	  Accelerator

config FB_MBX_DEBUG
       bool "Enable debugging info via debugfs"
       depends on FB_MBX && DEBUG_FS
       default n
       ---help---
         Enable this if you want debugging information using the debug
         filesystem (debugfs)

         If unsure, say N.

config FB_FSL_DIU
	tristate "Freescale DIU framebuffer support"
	depends on FB && FSL_SOC
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	select PPC_LIB_RHEAP
	---help---
	  Framebuffer driver for the Freescale SoC DIU

config FB_W100
	tristate "W100 frame buffer support"
	depends on FB && ARCH_PXA
 	select FB_CFB_FILLRECT
 	select FB_CFB_COPYAREA
 	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for the w100 as found on the Sharp SL-Cxx series.
	  It can also drive the w3220 chip found on iPAQ hx4700.

	  This driver is also available as a module ( = code which can be
	  inserted and removed from the running kernel whenever you want). The
	  module will be called w100fb. If you want to compile it as a module,
	  say M here and read <file:Documentation/kbuild/modules.txt>.

	  If unsure, say N.

config FB_SH_MOBILE_LCDC
	tristate "SuperH Mobile LCDC framebuffer support"
	depends on FB && SUPERH
	select FB_SYS_FILLRECT
	select FB_SYS_COPYAREA
	select FB_SYS_IMAGEBLIT
	select FB_SYS_FOPS
	select FB_DEFERRED_IO
	---help---
	  Frame buffer driver for the on-chip SH-Mobile LCD controller.

config FB_TMIO
	tristate "Toshiba Mobile IO FrameBuffer support"
	depends on FB && MFD_CORE
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for the Toshiba Mobile IO integrated as found
	  on the Sharp SL-6000 series

	  This driver is also available as a module ( = code which can be
	  inserted and removed from the running kernel whenever you want). The
	  module will be called tmiofb. If you want to compile it as a module,
	  say M here and read <file:Documentation/kbuild/modules.txt>.

	  If unsure, say N.

config FB_TMIO_ACCELL
	bool "tmiofb acceleration"
	depends on FB_TMIO
	default y

config FB_S3C
	tristate "Samsung S3C framebuffer support"
	depends on FB && ARCH_S3C64XX
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for the built-in FB controller in the Samsung
	  SoC line from the S3C2443 onwards, including the S3C2416, S3C2450,
	  and the S3C64XX series such as the S3C6400 and S3C6410.

	  These chips all have the same basic framebuffer design with the
	  actual capabilities depending on the chip. For instance the S3C6400
	  and S3C6410 support 4 hardware windows whereas the S3C24XX series
	  currently only have two.

	  Currently the support is only for the S3C6400 and S3C6410 SoCs.

config FB_S3C_DEBUG_REGWRITE
       bool "Debug register writes"
       depends on FB_S3C
       ---help---
         Show all register writes via printk(KERN_DEBUG)

config FB_S3C2410
	tristate "S3C2410 LCD framebuffer support"
	depends on FB && ARCH_S3C2410
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for the built-in LCD controller in the Samsung
	  S3C2410 processor.

	  This driver is also available as a module ( = code which can be
	  inserted and removed from the running kernel whenever you want). The
	  module will be called s3c2410fb. If you want to compile it as a module,
	  say M here and read <file:Documentation/kbuild/modules.txt>.

	  If unsure, say N.
config FB_S3C2410_DEBUG
	bool "S3C2410 lcd debug messages"
	depends on FB_S3C2410
	help
	  Turn on debugging messages. Note that you can set/unset at run time
	  through sysfs

config FB_SM501
	tristate "Silicon Motion SM501 framebuffer support"
	depends on FB && MFD_SM501
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for the CRT and LCD controllers in the Silicon
	  Motion SM501.

	  This driver is also available as a module ( = code which can be
	  inserted and removed from the running kernel whenever you want). The
	  module will be called sm501fb. If you want to compile it as a module,
	  say M here and read <file:Documentation/kbuild/modules.txt>.

	  If unsure, say N.


config FB_PNX4008_DUM
	tristate "Display Update Module support on Philips PNX4008 board"
	depends on FB && ARCH_PNX4008
	---help---
	  Say Y here to enable support for PNX4008 Display Update Module (DUM)

config FB_PNX4008_DUM_RGB
	tristate "RGB Framebuffer support on Philips PNX4008 board"
	depends on FB_PNX4008_DUM
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Say Y here to enable support for PNX4008 RGB Framebuffer

config FB_STMP37XX
	tristate "STMP 37XX LCD Framebuffer driver"
	depends on FB && (ARCH_STMP37XX || ARCH_STMP378X)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Say Y here to enable support for the framebuffer driver for the
	  Sigmatel STMP37XX board.

config FB_IBM_GXT4500
	tristate "Framebuffer support for IBM GXT4500P adaptor"
	depends on FB && PPC
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Say Y here to enable support for the IBM GXT4500P display
	  adaptor, found on some IBM System P (pSeries) machines.

config FB_PS3
	tristate "PS3 GPU framebuffer driver"
	depends on FB && PS3_PS3AV
	select FB_SYS_FILLRECT
	select FB_SYS_COPYAREA
	select FB_SYS_IMAGEBLIT
	select FB_SYS_FOPS
	select VT_HW_CONSOLE_BINDING if FRAMEBUFFER_CONSOLE
	---help---
	  Include support for the virtual frame buffer in the PS3 platform.

config FB_PS3_DEFAULT_SIZE_M
	int "PS3 default frame buffer size (in MiB)"
	depends on FB_PS3
	default 9
	---help---
	  This is the default size (in MiB) of the virtual frame buffer in
	  the PS3.
	  The default value can be overridden on the kernel command line
	  using the "ps3fb" option (e.g. "ps3fb=9M");

config FB_XILINX
	tristate "Xilinx frame buffer support"
	depends on FB && (XILINX_VIRTEX || MICROBLAZE)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Include support for the Xilinx ML300/ML403 reference design
	  framebuffer. ML300 carries a 640*480 LCD display on the board,
	  ML403 uses a standard DB15 VGA connector.

config FB_COBALT
	tristate "Cobalt server LCD frame buffer support"
	depends on FB && MIPS_COBALT

config FB_SH7760
	bool "SH7760/SH7763/SH7720/SH7721 LCDC support"
	depends on FB && (CPU_SUBTYPE_SH7760 || CPU_SUBTYPE_SH7763 \
		|| CPU_SUBTYPE_SH7720 || CPU_SUBTYPE_SH7721)
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Support for the SH7760/SH7763/SH7720/SH7721 integrated
	  (D)STN/TFT LCD Controller.
	  Supports display resolutions up to 1024x1024 pixel, grayscale and
	  color operation, with depths ranging from 1 bpp to 8 bpp monochrome
	  and 8, 15 or 16 bpp color; 90 degrees clockwise display rotation for
	  panels <= 320 pixel horizontal resolution.

config FB_VIRTUAL
	tristate "Virtual Frame Buffer support (ONLY FOR TESTING!)"
	depends on FB
	select FB_SYS_FILLRECT
	select FB_SYS_COPYAREA
	select FB_SYS_IMAGEBLIT
	select FB_SYS_FOPS
	---help---
	  This is a `virtual' frame buffer device. It operates on a chunk of
	  unswappable kernel memory instead of on the memory of a graphics
	  board. This means you cannot see any output sent to this frame
	  buffer device, while it does consume precious memory. The main use
	  of this frame buffer device is testing and debugging the frame
	  buffer subsystem. Do NOT enable it for normal systems! To protect
	  the innocent, it has to be enabled explicitly at boot time using the
	  kernel option `video=vfb:'.

	  To compile this driver as a module, choose M here: the
	  module will be called vfb. In order to load it, you must use
	  the vfb_enable=1 option.

	  If unsure, say N.

config XEN_FBDEV_FRONTEND
	tristate "Xen virtual frame buffer support"
	depends on FB && XEN
	select FB_SYS_FILLRECT
	select FB_SYS_COPYAREA
	select FB_SYS_IMAGEBLIT
	select FB_SYS_FOPS
	select FB_DEFERRED_IO
	default y
	help
	  This driver implements the front-end of the Xen virtual
	  frame buffer driver.  It communicates with a back-end
	  in another domain.

config FB_METRONOME
	tristate "E-Ink Metronome/8track controller support"
	depends on FB
	select FB_SYS_FILLRECT
	select FB_SYS_COPYAREA
	select FB_SYS_IMAGEBLIT
	select FB_SYS_FOPS
	select FB_DEFERRED_IO
	help
	  This driver implements support for the E-Ink Metronome
	  controller. The pre-release name for this device was 8track
	  and could also have been called by some vendors as PVI-nnnn.

config FB_MB862XX
	tristate "Fujitsu MB862xx GDC support"
	depends on FB
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	---help---
	  Frame buffer driver for Fujitsu Carmine/Coral-P(A)/Lime controllers.

config FB_MB862XX_PCI_GDC
	bool "Carmine/Coral-P(A) GDC"
	depends on PCI && FB_MB862XX
	---help---
	  This enables framebuffer support for Fujitsu Carmine/Coral-P(A)
	  PCI graphics controller devices.

config FB_MB862XX_LIME
	bool "Lime GDC"
	depends on FB_MB862XX
	depends on OF && !FB_MB862XX_PCI_GDC
	depends on PPC
	select FB_FOREIGN_ENDIAN
	select FB_LITTLE_ENDIAN
	---help---
	  Framebuffer support for Fujitsu Lime GDC on host CPU bus.

config FB_PRE_INIT_FB
	bool "Don't reinitialize, use bootloader's GDC/Display configuration"
	depends on FB_MB862XX_LIME
	---help---
	  Select this option if display contents should be inherited as set by
	  the bootloader.

config FB_MX3
	tristate "MX3 Framebuffer support"
	depends on FB && MX3_IPU
	select FB_CFB_FILLRECT
	select FB_CFB_COPYAREA
	select FB_CFB_IMAGEBLIT
	default y
	help
	  This is a framebuffer device for the i.MX31 LCD Controller. So
	  far only synchronous displays are supported. If you plan to use
	  an LCD display with your i.MX31 system, say Y here.

config FB_BROADSHEET
	tristate "E-Ink Broadsheet/Epson S1D13521 controller support"
	depends on FB
	select FB_SYS_FILLRECT
	select FB_SYS_COPYAREA
	select FB_SYS_IMAGEBLIT
	select FB_SYS_FOPS
	select FB_DEFERRED_IO
	help
	  This driver implements support for the E-Ink Broadsheet
	  controller. The release name for this device was Epson S1D13521
	  and could also have been called by other names when coupled with
	  a bridge adapter.

source "drivers/video/omap/Kconfig"

source "drivers/video/backlight/Kconfig"
source "drivers/video/display/Kconfig"

if VT
	source "drivers/video/console/Kconfig"
endif

if FB || SGI_NEWPORT_CONSOLE
	source "drivers/video/logo/Kconfig"
endif

endmenu
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       ��r���t<�H��u3�x@�@ v� � ��u �t$p��$�   �D$�D$p�$��t��  ����   �4$�D$    �4�r���t<�H��u+�x@�@ v� � ��)u�t$h��$�   �D$�D$h�_  ����   �4$�D$    ���r���t<�H��u3�x@�@ v� � ��(u �t$`��$�   �D$�D$`�$�i�t��  ��tU�4$�D$    ��r���tA�H��u8�x@�@ v� ��$�   � ��$t��;u�t$X�L$�D$X�$� �t��  ���,$��s���t �|$P��$�   �D$�D$P�$��t��  1�����   �w����   �<$�D$    ���r���tK�H��u>�x@�@ v� � H��w*� T ��s �|$H��$�   �D$�D$H�$袞t��  ��tP�<$�D$    ��r���t<�H��u3�x@�@ ��$�   v� � ��.u�|$@�L$�D$@�$�^�t���  ���,$��t���t �t$8��$�   �D$�D$8�$�@�t��  ����  �~���\  �4$�D$    ��r���tK�H��u:�x@�@ v� ���  #��   u �t$0��$�   �D$�D$0�$��t��,  ����   �4$�D$    ��r���t>�H��u5�x@�@ v� � = B  u �t$(��$�   �D$�D$(�$蜝t���   ����   �4$�D$    �O�r���t=�H��u0�x@�@ v� � ��9u�t$ ��$�   �D$�D$ �$�T�t��}��tR�4$�D$    ���r���t>�H��u5�x@�@ v� � ��8��(��t$��u���9t�
��)t��.t�,$��s���t�t$��$�   �D$�D$�$��t���|^_[]�             USWV���    [��\`S �l$4�D$0� 1҅��    ��   1҃x�    r�$�D$   �7�r�1҅��    td1��H���    uT�P��@w7�p �x$�@   )щ�����1��� E�D��������� E����� D���@ ��x��!������  �|$�T$�������4$�D$   �]r��ǋM�E)�9�v�|$�t$�,$�]r�����|$�t$�$��]r�}�D$0� 1҅��    t1҃x�    rr�$�D$   �G�r�1҅��    tW1��H���    uG�P��@w7�p �x$�@   )щ�����1��� E�D��������� E����� D���@ ��x�|$�T$�,$�5]r��ƍ��S���,$�D$   �\r��ǋN�F)�9�v�|$�l$�4$�\r�����|$�l$�$��\r�~�D$����L$����D$�4$�L$��\r��H;Hs�Q�P�]��$�D$]   �g\r���^_[]Éx|���,$�D$   �\r��ǋN�F)�9�v�|$�l$�4$�	\r����|$�l$�$�G\r�~�  USWV���    [���]S �l$4�D$0�l$�$��t��������<$�D$   �[r��ƋM�E)�9�v�t$�|$�,$���[r���t$�|$�$��[r�u��l$0�E 1���t4�x��r.�$�D$   �W�r���t�H��u�x@�@ v� �8����<$葙t��ƅ�t0�4$�D$�����[r��ǋM�E)�9�vL�|$�t$�,$��Zr��|�|$���|���<$�D$   ��Zr��ƋM�E)�9�v'�t$�|$�,$��Zr��*�|$�t$�$�[r�}�-�t$�|$�$��Zr�u��L$�L$�$�D$    �jt��E;Es�H�M� ]��,$�D$]   �|Zr���^_[]�    USWV��,�    [��,\S �t$D�������,$�D$   �Zr��ǋN�F)�9�v�|$�l$�4$�Zr�����|$�l$�$�AZr�~�D$@�D$�D$ �$�:�r����D$ �|$$�V�N)�9�v�|$�D$�4$�Yr�����|$�D$�$��Yr�~���L���,$�D$   �vYr��ǋN�F)�9�v�|$�l$�4$�hYr�����|$�l$�$�Yr�~�D$@�D$�D$�$��r����D$�|$�V�N)�9�v�|$�D$�4$�Yr��Ƌ~��|$�D$�$�OYr�~�~;~s�G�F�]��4$�D$]   ��Xr���,^_[]� USWV���    [�ìZS �t$4�������,$�D$   �Xr��ǋN�F)�9�v�|$�l$�4$�Xr�����|$�l$�$��Xr�~�D$0� ��t@�xr:�$�D$   �N�r���t&�H��u�D$�D$�$�ar����D$�|$��D$    �D$    1�1��V�N)�9�v�|$�D$�4$��Wr�����|$�D$�$�0Xr�~���|���,$�D$   �Wr��ǋN�F)�9�v�|$�l$�4$�Wr�����|$�l$�$��Wr�~�D$0� 1҅��    t1҃x�    rr�$�D$   �d�r�1҅��    tW1��H���    uG�P��@w7�h �x$�@   )щ�����1��� E�D��������� E����� D���@ ��x�|$�T$�4$�RWr��H;Hs�Q�P�]��$�D$]   ��Vr���^_[]�       USWV��,�    [�ÜXS �D$@� ����
  �t$D�x��   �$�D$   聸r���tu�H��ul�D$�D$ �$��_r����|$$ tb�������<$�D$   �4Vr����ƍE�D$�U�M�E)�9���  �T$�t$�|$�,$�Vr��ƋF��  �D$     �D$$    �n�F�D$�l$���|���<$�D$   ��Ur��ƋM �l$�E )�9�v�t$�|$�D$D�$�Ur�����t$�|$�$��Ur�u �t$D�D$@� 1���t0�xr*�$�D$   �q�r���t�H��u�x@�@ v� �8�|$�4$�[Vr��ƍ��|���,$�D$   �#Ur��ǋN�F)�9�v�|$�l$�4$�Ur�����|$�l$�$�QUr�~�D$@� 1Ʌ��    tH1Ƀx�    r;�$�D$   �жr�1Ʌ��    t 1��P���    u�x@�@ v� ��P�T$�L$�4$�եr��ƍ�xq���,$�D$	   �mTr��ǋN�F)�9�v�|$�l$�4$�_Tr�����|$�l$�$�Tr�~�D$@� 1Ʌ��    tH1Ƀx�    r;�$�D$   ��r�1Ʌ��    t 1��P���    u�x@�@ v� ��P�T$�L$�4$��r��ƍ��|���,$�D$
   �Sr��ǋN�F)�9�v�|$�l$�4$�Sr�����|$�l$�$��Sr�~�D$@� 1Ʌ��    tH1Ƀx�    r;�$�D$   �d�r�1Ʌ��    t 1��P���    u�x@�@ v� ��P�T$�L$�4$�i�r��D$@�0����  �~����  �4$�D$    � �r�����  �H����  �x@�@ v� � ��$t	��;��  1��
r*�4$�D$	   踴r���t�H��u�x@�@ v� �(�,$��t��ƅ��M  ���|���,$�D$   �fRr��ǋD$��T$�)�9���   �|$�l$�D$D�$�JRr�����   �t$�|$�$�׉|$�}Rr������L$ �|$$�V)�9�v�|$�L$�4$�Rr�����|$�L$�$�@Rr�~�������,$�D$   ��Qr��ǋN�F)�9�v�|$�l$�4$�Qr���|$�l$�$��Qr�~�l$�����|$�l$�$����Qr�} �|$D�4$�D$�����bQr��ŋO�G)�9�v�l$�t$�<$�TQr���l$�t$�$�Qr�o�������<$�D$   �Qr��ƋD$��l$�E )�9�v�t$�|$�D$D�$� Qr���t$�|$�$�>Qr�u �D$@�0���=  �~��	��   �4$�D$   ���r���t1�H��u#�x@�@ v� � t���|���<$�D$   �C��	r}�4$�D$   �|�r���ti�H��u`�x@�@ v� � tP���|���<$�D$   �9Pr��ƋD$��E )�9�v�t$�|$�D$D�$�$Pr���t$�|$�$�bPr�u �D$@� ���a  �x	r}�$�D$   ��r���ti�H��u`�x@�@ v� � @tP���|���<$�D$   �Or��ƋD$��E )�9�v�t$�|$�D$D�$�Or���t$�|$�$��Or�u �D$@�0����  �~	rK�4$�D$   �Z�r���t/�H��u&�x@�@ v� � t���|���<$�D$   �   ���7  �~���,  �4$�D$    ��r���t(�H��u�x@�@ v� � ����   ����   �4$�D$    �Ȱr���t�H��u�x@�@ v� � ��tp����   �4$�D$    萰r���t$�H��u�x@�@ v� � ��t8����   �4$�D$    �X�r���tl�H��uc�x@�@ v� � ��uP���|���<$�D$   �Nr��ƋD$��E )�9�v�t$�|$�D$D�$��Mr���t$�|$�$�;Nr�u �D$@� ���:  �x	r~�$�D$   �įr���tj�H��ua�x@�@ v� �@tP���|���<$�D$
   �Mr��ƋD$��E )�9�v�t$�|$�D$D�$�kMr���t$�|$�$�Mr�u �D$@� ����  �x	r~�$�D$   �2�r���tj�H��ua�x@�@ v� �@tP���|���<$�D$
   ��Lr��ƋD$��E )�9�v�t$�|$�D$D�$��Lr���t$�|$�$�Mr�u �D$@� ���  �x	r~�$�D$   蠮r���tj�H��ua�x@�@ v� �@@tP���|���<$�D$   �\Lr��ƋD$��E )�9�v�t$�|$�D$D�$�GLr���t$�|$�$�Lr�u �D$@� ����   �x	r~�$�D$   ��r���tj�H��ua�x@�@ v� �@�tP��	}���<$�D$   ��Kr��ƋD$��E )�9�v�t$�|$�D$D�$�Kr���t$�|$�$��Kr�u ��,^_[]�        USWV���    [��lMS �t$4�D$0�t$�$�9�t���}���,$�D$   �CKr��ǋN�F)�9�v�|$�l$�4$�5Kr�����|$�l$�$�qKr�~�D$0� ��tc�x
r]�$�D$	   ���r���tI�H����t1Ʌ�t#�yr2�$�D$   �Ӭr���t�H��u�D$�D$�$�&Tr�����D$    �D$    �D$�|$�V�N)�9�v�|$�D$�4$�}Jr��Ƌ~��|$�D$�$�Jr�~�~;~s�G�F�]��4$�D$]   �`Jr���^_[]�        USWV���    [��LS �t$4�|$0�t$�<$�هt����t3�xr7�D$    �$�D$
   ��r���t#�H��t1��D$��D$    ��D$    �������,$�D$   �Ir��ǋN�F)�9�v�|$�l$�4$�Ir�����|$�l$�$��Ir�~�L$1���t�A�D$�4$�jJr��ƍ�%}���,$�D$   �2Ir��ǋN�F)�9�v�|$�l$�4$�$Ir���|$�l$�$�bIr�~��^_[]�       USWV���    [���JS �l$4���|���4$�D$   ��Hr��ǋM�E)�9�v�|$�t$�,$�Hr�����|$�t$�$��Hr�}��D$0� 1���t0�xr*�$�D$   �z�r���t�H��u�x@�@ v� �0�t$�<$�dIr��H;Hs�Q�P�]��$�D$]   �QHr��D$0� ����  �x	rv�$�D$   ��r���tb�H��uY�x@�@ v� �8 tI��0}���4$�D$	   ��Gr��ǋM�E)�9�v�|$�t$�,$�Gr���|$�t$�$��Gr�}�D$0� ���]  �x
rv�$�D$	   胩r���tb�H��uY�x@�@ v� �8 tI���|���4$�D$   �@Gr��ǋM�E)�9�v�|$�t$�,$�2Gr���|$�t$�$�pGr�}�D$0�8����  �w���    r.�<$�D$   ��r�1��t1��H��u�x@�@ v� �(��r,�<$�D$   迨r���t�H��u�x@�@ v� � �
1��1��1�9���   ��9}���4$�D$	   �kFr��ǋl$4�M�E)�9�v�|$�t$�,$�YFr�����|$�t$�$�Fr�}��D$0� 1���t0�xr*�$�D$   ��r���t�H��u�x@�@ v� �0�t$�<$�Gr��Ǎ������4$�D$   ��Er��ŋO�G)�9�v�l$�t$�<$��Er���l$�t$�$� Fr�o�D$0�8���l$4�_  �w����   �<$�D$   ��r���t1�H��u#�x@�@ v� � t���|���4$�D$   �C��rv�<$�D$   �:�r���tb�H��uY�x@�@ v� � tI���|���4$�D$   ��Dr��ǋM�E)�9�v�|$�t$�,$��Dr���|$�t$�$�'Er�}�D$0� ����  �xrw�$�D$   谦r���tc�H��uZ�x@�@ v� �@@tI���|���4$�D$   �lDr��ǋM�E)�9�v�|$�t$�,$�^Dr���|$�t$�$�Dr�}�D$0� ����   �xrw�$�D$   �%�r���tc�H��uZ�x@�@ v� �@�tI��	}���4$�D$   ��Cr��ǋM�E)�9�v�|$�t$�,$��Cr���|$�t$�$�Dr�}�D$0� ��tx�xrr�$�D$   螥r���t^�H��uU�D$�D$�$��Lr����|$ tK�������4$�D$   �QCr��ǋM�E)�9�v/�|$�t$�,$�CCr��ŋE�0�D$    �D$    ��^_[]É|$�t$�$�dCr��E��E�L$�t$�U)�9�v�t$�L$�,$��Br��ŋu��t$�L$�$�$Cr�u�u;us�F�E�]뒉,$�D$]   ��Br��            USWV��,�    [��|DS �l$D�D$@� ����   �x��   �$�D$   �a�r���t{�H��ur�D$�D$ �$�Kr����|$$ th�������D$�$�D$   �Br��ƍU�}�M�E)�9���  �T$�|$�t$�D$�D$�,$��Ar��ƋF�  �D$     �D$$    �}�E�D$���|���,$�D$   �Ar��Ƌ�D$� )�9�v �|$�t$�l$�D$D�$�Ar��Ƌl$� �|$�t$�l$�$�Ar��D$0�t$D�ŋD$@� 1���t0�xr*�$�D$   �@�r���t�H��u�x@�@ v� �8�|$�4$�*Br��H;Hs�Q�P�]��$�D$]   �Ar��D$@� ����  �x
��   �$�D$	   �Ϣr�����   �H����   �x@�@ v� �8 ��   ��0}���<$�D$	   �@r��ƋD$��E )�9�vi�t$�|$�D$D�$�k@r��f�T$�t$�L$�L$�$�@r�����|$��L$ �|$$�V)�9���   �|$�L$�4$� @r��Ƌ~�   �t$�|$�$�V@r�u �D$@� ����   �x��   �$�D$
   �ۡr�����   �H����   �x@�@ v� �8 ��   ���|���<$�D$   �?r��ƋD$��E )�9�vU�t$�|$�D$D�$�w?r��R�|$�L$�$�?r�~�~;~s�G�F�]��4$�D$]   �_?r��|$�n����t$�|$�$�v?r�u ��,^_[]�           USWV��,�    [���@S �t$D�D$@� ����   �x��   �$�D$   �Ѡr���tw�H��un�D$�D$ �$�$Hr����|$$ td�������D$�$�D$   �>r��ǍF�D$�n�N�F)�9��  �|$�D$�D$�4$�`>r��ƋF�  �D$     �D$$    �F�D$�v���|���,$�D$   �>r��ǋD$��)�9�v�|$�l$�D$D�$�>r�����|$�l$�$�>>r�>�t$D�D$@� 1���t5�xr/�$�D$   �Ɵr���t�H��u�x@�@ v� ���� #8�|$�4$�>r��H;Hs�Q�P�]��$�D$]   �=r���,^_[]É|$�L$�L$�$�=r��E ��E �L$ �|$$�V)�9�v�|$�L$�4$�3=r��Ƌ~��|$�L$�$�l=r�~�~;~s�G�F�]��4$�D$]   �=r�������               USWV��,�    [�ü>S �t$D�D$@� ����   �x��   �$�D$   衞r���tw�H��un�D$�D$ �$��Er����|$$ td�������D$�$�D$   �P<r��ǍF�D$�n�N�F)�9���  �|$�D$�D$�4$�0<r��ƋF�  �D$     �D$$    �F�D$�v���|���,$�D$   ��;r��ǋD$��)�9�v�|$�l$�D$D�$��;r�����|$�l$�$�<r�>�t$D�D$@� 1���t0�xr*�$�D$   薝r���t�H��u�x@�@ v� �8�|$�4$�<r��ƍ�B}���,$�D$   �H;r��ǋN�F)�9�v�|$�l$�4$�:;r�����|$�l$�$�v;r�~�D$@� 1���t0�xr*�$�D$   ��r���t�H��u�x@�@ v� �8�|$�4$��;r��H;Hs�Q�P�]��$�D$]   ��:r���,^_[]É|$�L$�L$�$��:r��E ��E �L$ �|$$�V)�9�v�|$�L$�4$�s:r��Ƌ~��|$�L$�$�:r�~�~;~s�G�F�]��4$�D$]   �V:r����#���               USWV��,�    [���;S �t$D�D$@� ����   �x��   �$�D$   ��r���tw�H��un�D$�D$ �$�4Cr����|$$ td�������D$�$�D$   �9r��ǍF�D$�n�N�F)�9���   �|$�D$�D$�4$�p9r��ƋF��   �D$     �D$$    �F�D$�v���|���,$�D$   �&9r��ǋD$��)�9�v�|$�l$�D$D�$�9r�����|$�l$�$�N9r�>�t$D�D$@� 1���t0�xr*�$�D$   �֚r���t�H��u�x@�@ v� �8�|$�4$��9r��H;Hs�Q�P�]��$�D$]   �8r���,^_[]É|$�L$�L$�$��8r��E ��E �L$ �|$$�V)�9�v�|$�L$�4$�H8r��Ƌ~��|$�L$�$�8r�~�~;~s�G�F�]��4$�D$]   �+8r�������    USWV��<�    [���9S �|$T�l$P�E �$�j<r��ƋE ��tp�xrj�$�D$   軙r���tV�H��uM�D$�D$0�$�Ar����|$4 tC�t$�D$0�t$4�W�O)�9���   �t$�D$�<$�h7r�����   �D$0    �D$4    �E ����  �x��  �$�D$   �,�r�����  �H����  �D$�D$(�$���r����|$, ��  �t$��P}���D$�$�D$   ��6r��Ɖ��M�E)�9�vp�t$�D$�D$�,$�6r��q�t$�D$�$��6r�w�����2���D$�$�D$   �v6r��N�V)�9���   �D$�D$�D$�4$�b6r����   �t$�L$�L$�$�6r�u�T$�D$(�D$ �D$ �$����   �������<$�D$   ��5r��ƋM�E)�9�v{�t$�|$�,$��5r��|�D$�L$�L$�$�D$�$6r���D$A�E 1���t^�xrX�L$�$�D$   託r���t<�H���L$u3�x@�@ v� ���� #0��t$�|$�$��5r�u��<^_[]ËL$�t$�$�h6r��t$�����               USWV��,�׉��    [��7S �|$D �  �D$@�|$�$�>�r���tc�xr]�$�D$   ���r���tI�H��u@�x t:�$�D$    �іr���t&�H��u�D$�D$ �$�$>r����D$ �l$$��D$     �D$$    1�1��V�N)�9�v�l$�D$�4$�w4r��n��l$�D$�$�4r�n�n;ns�E�F�E :����4$�D$:   �Y4r��l$@�M ����� �L$�$�05r��}    r5�F;Fs�H�N� :����4$�D$:   �4r��M�L$�$��4r��|$�,$��s��D$�D$�$��r����|$ ��   �|$���}���<$�D$   �3r��ŋN�F)�9�v�l$�|$�4$�3r���l$�|$�$�3r�n�D$�D$�D$�$��T$�	�����%����,$�D$   �#3r��ǋN�F)�9�v�|$�l$�4$�3r���|$�l$�$�S3r�~��,^_[]�        �D$�L$��     �D$�L$��     S���    [�ï4S �D$�1���t�T$9Qv�T$�$蟔r��L$���[�   S���    [��o4S �D$�1���t�T$9Qv�T$�$�_�r��L$���[�   USWV��<�    [��,4S �|$P������D$0�$�D$   �2r��D$4�D$0�D$�<$�'�r��ƅ�u!�D$    �,��������$��r��$�6r��F��u��4$�P��D$��T}���D$(�$�D$   �1r��D$,�D$(�D$�<$蹞r��ƅ�u�&��$踖r��$�06r��F��u��4$�P��D$�G(�D$�o09���   �������o�|$�D$ �$�ݒr�����c}���4$�D$
   �1r�9D$$r��t*�L$ �D$�t$�$�:r���t;l$��u�� ���������<$�hot���D$;l$�����|$P�G���9�tS�\$�������H(�P@����������R9�t)�r ������v 9�t�~, t��F,    �F(    ��ᐋ@89�u���\$���Ã�<^_[]�     SV��$�    [��>2S �t$0��m}���D$�$�D$   �!0r��D$�D$�D$�4$�nt�1Ʌ�t�x@�@ v� ��ȃ�$^[�              USWV��L�    [���1S �L$`�D$d�A    �    �A    �A    �D$I��*����L$@�D$H�L$@�L$�$�&�r��D$ ���  �D$ �$�>�r��D$����   1��t$�D$ �$�0�r�����   �x��   �$�D$	   �>�r��T$����   �B����   �B�D$$����   1퐐9jv{�l$�$�����r����ǅ�te�G��u\�rV�<$�D$   �֐r��T$��t>�H��u5�D$8�|$<�D$8�D$�D$`�D$�D$(�$���?mt���������������E9l$$�u��������F;t$������L^_[]�            USWV���    [��<0S �l$8�|$4�t$0�G1҅���   �\$��u ��������	1ŉЍP��T$!Ս��9�tB�D$1��   ����������������t?����Å����D���#l$���9�u���D$�D$���L$0��A�A �A��EЋt$0�|$4�l$8�\$�T$�l$�<$�7lt��M ��M�H�O����N�F��^_[]�  USWV���    [��</S �|$4�l$0�M�E��   �4@9�s���+M����9���   �D$8�  ��D$�,$��kt��M1�����   �D$0��7�t$��������	1�I�L$!ˍڋ<�9���   1�   ����������   ����������D��v#\$�ڋ<�9|$u��~   �D$�,$�Akt��M1���ti�D$0��7�t$��������	1�I�L$!ˍڋ<�9�t@1�   �����������t'����������D��v#\$�ڋ<�9|$u����EŋL$0�A�8�t�I��^_[]�        USWV���    [���-S �|$ �D$$�7�oH����	�����	ȉ���	�����	ȉ���	�B��@�@   GʉO1���t���$�,r����t��D$�t$�<$�Djt��4$�<-r��,�G    �G    �O��t������t� ���������u��^_[]�      USWV���D$�@    �@    �@��t"�L$�	�����������t����������u�L$$�T$ �������9���   �2�������t�t$�D$�x1ۅ�tx�D$�(�L$��������	1�O�<$!��\� �D� 9�tP�   1ɐ�����������t(����������D���#4$�\� �D� 9D$u����Eِ��������������D$��B�C�D$�@�L$$�D�����^_[]�            SWV���    [���+S �t$ �F1���t'H�    t�Ѓ��!   )Ѹ   ����?�@   O�;~u9�F    �F    ����   ���������������t� ���������u��d��$�R+r��~��t?���<$� *r���F    �F    �N��t1����������t� ���������u���    �F    �F    ��^_[� USWV���    [���*S �|$4�l$0�M�E��   �4@9�s���+M����9���   �D$8�  ��D$�,$�gt��M1�����   �D$0��7�t$��������	1�I�L$!ˍڋ<�9���   1�   ����������   ����������D��v#\$�ڋ<�9|$u��~   �D$�,$�!gt��M1���ti�D$0��7�t$��������	1�I�L$!ˍڋ<�9�t@1�   �����������t'����������D��v#\$�ڋ<�9|$u����EŋL$0�A�8�t�I��^_[]�        USWV���    [�Ì)S �|$ �D$$�7�oH����	�����	ȉ���	�����	ȉ���	�B��@�@   GʉO1���t���$��'r����t��D$�t$�<$�$ft��4$��(r��,�G    �G    �O��t������t� ���������u��^_[]�      USWV���D$�@    �@    �@��t"�L$�	�����������t����������u�L$$�T$ �������9���   �2�������t�t$�D$�x1ۅ�tx�D$�(�L$��������	1�O�<$!��\� �D� 9�tP�   1ɐ�����������t(����������D���#4$�\� �D� 9D$u����Eِ��������������D$��B�C�D$�@�L$$�D�����^_[]�            USWV���    [�ì'S �|$4�t$0�N�F��   �,@9�s���+N����9�v.�D$8�L��D$�4$�}dt��D$�D$�|$�4$�Ybt��D$�$�D$�4$�Wdt��D$�D$�|$�4$�3bt��D$�F�8�u�x�t�N��^_[]�      USWV���    [���&S �t$0�D$4�>�nH����	�����	ȉ���	�����	ȉ���	�B��@�@   GʉN1���tk��$�M%r������   �|$�F    �F    �N��t#k����������t�@����� ���������u�����   �D$��hk��Ɛ���������������E��M����u�} �tb�|$�����������|$�����u�} ��|$t>�N�L$�L$�L$�D$�׉<$��`t��D$�M���L$�	�H�M�H�G���|$�������u��D$�$�%r��=�F    �F    �N��t(k���������������t�@����� ���������u��^_[]�               USWV���D$,�l$$�]���  �D$(��L$�P�$������	1�k�%���҃��ʉ���
����1�1����������������������1�1��ɺ	   ��ʉ�������1�1�����������ϋm �����1�K�D$   �D$    � ����������D�t$�L$�A�L$��!�k��L5 �|5 9|$u	�$;T5tG�ڃ��u�|5����(������������u�|5��t$u��$�������������t$��     ��D$,�����E΋D$,�1�����^_[]ËD$�H1���t)�D$� ~k����  �D���ы��  k�0�D����         �D$�H1���y�D$� �ы��  k�0�D$����          SWV�D$�L$�T$�r��t"�|$�~(k����  �T2�����     �;�     �    �-�֋��  k�0�t3����1��R�ҋ��  k�0�L$����^_[�         SWV���    [���"S �t$$�D$ �H��x1���tW�k����  �D��C��ы��  k�0�L$���t!�L$�|$�<$��r����t$�<$��_t������  �D�����^_[�           USWV��,�    [��l"S �t$@�D$D�D$(�|$(�|$�D$ �$�D$   �E�r����l$ ����   �<$�D$    ���r��Ɖ<$�D$   ��r����ǉL$�D$�$�D$   ���r����D$�D$�D$@�@    1������ G��������   G�	�t$@��,$�Y$r��L$� ��t)�L$�l$�$�D$    ��^t��#�    �F    ��l$�$�D$    �^t��F��,^_[]�      SWV�� �    [��M!S �t$0�D$4�L$8�L$�$�V^t��D$�|$�<$�V�r����<$�[�r���t�D$�xr�<$�D$   ��    �F    �R�<$�D$   ��r��|$�    �F    ��t-1�=��� Fȉ�<$�X#r�� �|$�$�D$    ��]t��F�� ^_[�       USWV���    [��| S �|$<�t$0�    �F    ��th�l$8�L$41ҁ���� Gʉ������   G�	ȉ�<$��"r��L$@� ��t�L$�|$�$�D$    �E]t���|$�$�D$    �?]t��F��^_[]�   USWV��<�    [���S �|$T�D$P�|$8���  1���tp�\$���  ��������	1�M!�4ы�9�tv��1��   ������������t%����������D��m!ڍ4ы�;D$Tu��:��E��|$T�\$�D$P�\$���  �t$�L$8�L$�$�\t��Ɖ>�F    �F���e  �D$X��t�F�U  �T$P���  ���  ���  9��\$u �,$�D$�   ���A\t������  ���  �l$)���i�����G�~�D$8�D$$   �D$(    �D$,�������w
�  ��r�D$$�$���5}r��ꋫ�������l$ �T$0�|$4���  ;��  sh1���tW�G   �G    �D$,�G�������w
�  ��r�G�L$$����L$�$�Jr��T$P�/�D$0�G���  �����  ��D$ �D$�D$�$�T[t��D$,�������w
�  ��r�D$$�$�O{r��F��<^_[]�    USWV��   �    [�ÙS ��$�   ��$�   ��$�   ���  ��$�   ��$�   ��$�   �D$��$�   �l$�4$��Zt���$�   ��t�B���a  �%�T$�l$�4$�Zt��:��$�   �B�B    ��$�   ��t�B�'  ��$�   �ƍ��  �D$���  ���  ��9�u$�D$�$�D$�   ���hZt����  ���  )���i𫪪��։r�D$<   �D$@    �|$D�������w
�  ��r�D$<�$���#{r����T$���������|$8��L$H�t$L�r�D$$   �D$(    ��$�   �T$,�������w
�  ��r�D$$�$����zr���|$ �L$0�͉t$4�T$T�D$T   �D$X    �D$D�D$\�������w
�  ��r�D$<����D$�$��|r��t$<�|$P�D$H�D$`�D$l   �D$p    �D$,�D$t�������w
�  ��r�D$$����D$�D$l�$�|r��|$h�D$0�D$x�鋩�  ;��  ��   1�����   �E   �E    �D$\�E�������w
�  ��r�E�L$T����L$�$�+|r��} �D$`�E�E   �E     �D$t�E$�������w
�  ��r�E�L$l����L$�$��{r��}�D$x�E(��$�   ���  ��0���  ��D$P�D$�D$�$�#Xt��D$t�������w
�  ��r�D$l�$��wr��D$\�������w
�  ��r�D$T�$�wr��D$,�������w
�  ��r�D$$�$�wr��D$D�������w
�  ��r�4$�swr��D$�@�Č   ^_[]� USWV��<�    [�üS �D$P�H1�����   �D$T� ~k����  �D��D$    ��ы��  k�0�D
�L
$����L$�(����,$�r��D$�$��$r��ǋD$P�Ƌ%��� �D$�<$�D$    �D$    �?~r��D$,�F�D$�<$�D$    �D$    �~r��D$0�l$4�D$�D$8�D$,�D$ �D$$   �D$ �D$�D$�$�yr���<^_[]�           USWV���    [�ìS �t$0�D$4�D$�|$�|$�D$�$�D$   腗r����l$��te�<$�D$   �
�r��Ɖ<$�D$   ���r�1Ɂ���� G����=�   G�	�t$0��,$��r�� �l$�$�D$    �ZUt��F��    �F    ��^_[]�               �               USWV��,�    [���S �l$@�E���5  �|$H�L$D�	~k����  �D���Ћ��  k�0�D� ����D$(t=�D$(�D$�D$ �$�պr����D$ �t$$�W�O)�9�vH�t$�D$�<$�^r��b�������,$�D$
   �6r��ƋO�G)�9�v'�t$�l$�<$�(r��(�t$�D$�$�fr�w��t$�l$�$�Qr�w�l$@�G;Gs�H�O� :����<$�D$:   ��r��M ����� �L$�$��r��}    r5�G;Gs�H�O� :����<$�D$:   �r��M�L$�$�r��M1���y�D$D� �ы��  k�0�D$����D$�D$�$�!�r����|$ ��   ���}���,$�D$   �r��ƋO�G)�9�v�t$�l$�<$�r���t$�l$�$�Mr�w�|$�D$D�D$�D$�$�2r���%����,$�D$   �r��ƋO�G)�9�v�t$�l$�<$�r���t$�l$�$��r�w��,^_[]� USWV���    [��lS ��l������u.�4$�	-r���t"��p���� ���͌U�Eȋ�t�����4$��,r��D$ ��t�����x���������� 3:1��i-8�����i��ߝ�i�i-8��i�i-8���1�1Ή���i��ߝ����1�i�i-8��^_[]�         USWV��,�    [�ìS �L$D�t$@��F)���i�����9��S  �n��)���i�����1���t��kD$D�$�r��ǉ��n�D$ k��L$(�kD$D�9���   �L$�D$�E�)Љ։t$$����������ڋD$ �D��D$��D$(�t��|$ ���������D$(�������������}��F�   �F�    �E�F��������w�  ��r�����F�M����L$�$��tr��T$$�D$(�F��E�����9���u�kD$�L$ ��D$@�(�p��L$�H�L$�H9�t �������F���4$�P9�u����N�F��t�,$�r���,^_[]�  USWV��,�    [��S �|$D�t$@��N)���iɫ���9��  �N�V)���iꫪ���D$$    �L$(1���tk�0�$�or��D$k�0��L$ �L$k�0��L$$�D$�D$�4$�ePt��l$�t$ 9���   �F�)蹫��������҉T$�t$�������������wЋG�������w�  ��r������G�$��or��G܃������w�  ��r���������������ԉ<$�or�9���u�kD$0�L$��L$ �D$��t�$��r���,^_[]�        USWV��,�    X���S �D$�|$@�_�G�ك�����|   �W��   ���  ���F  �T$���  ��������	1�N!���9�t"�   ����������  ��!���9�u��������D$���  ���  �|$@�_��   ���t���t���  �G�$�\$�nr��_���_��   �Ћ��  �L$k�0�D1�L1$�������Ս��  �D$ �L$$�D$(�D$�D$ �D$�$�\$�>Nt���t�D$(� �����@�������  ���  �G��������t���t��t�G�$�\$�nr��G���G�D$�D0,    �D0    �6�|$@�؃�����t���t��t�G�$�\$��mr��_���_�G    ��,^_[]�           USWV��,�    [���S �l$D�E����t1ɋ|$@����   �G����   �L$�o����   �G���  ���  �ƃ�����  �l$�����D$��	1�I!ȋ,�9�t%�   ��������������  ��!ȋ,�9�u��������D$���  ���  �|$@�G�  ��@��,^_[]���G�����9���  �w����w
�  ��r�4$�lr��G��	�G�������w�  ����  �4$�mr��  �Ћ��  �L$k�0�T$�D�L$���������  �D$ �L$$�D$(�D$�D$ �D$�$�Lt���t�D$(� �����@�������  ���  �G������T$9�tG�Սw����w
�  ��r�4$��kr��G��	�G�������w
�  ��r�4$��lr��L$�T$�D�L$� ����	����W�w�t$�L$�D$�$�Kt��O9���   �G�ы��  k�0�D,    �D    �   �|$@�D$���̓���T$9�tO�w����w
�  ��r�4$���kr���o��	Չo�Ѓ������w
�  ��r�4$���lr���G�O�L$�T$�$�}Jt�;Gt�G    ��,^_[]�         S���    [��S �D$�H�������w
�  ��r���$�yjr��������[��             USWV���D$,�l$$�]���,  �D$(��L$�P�$������	1�������	1��Ѓ��ȉ���
����1�1щ��������������������1�1��ɺ	   ��ʉ�������1�1�����������ϋm �����1�K�D$   �D$    �)�������������������D�t$�L$�A�L$��!�k��L5 �|5 9|$u	�$;T5tG�ڃ��u�|5����(������������u�|5��t$u��$�������������t$��     ��D$,�����E΋D$,�1�����^_[]�USWV��,�    [��LS �t$@�F��V)���iꫪ���V)���i����������s'�M�9�B��L$$�D$$    �D$(1����    u�(�L$$�D$$    �D$(�UUUk�0�$���w	r���Ɖ��t$k�0k�0����|$ �|$���   �D.   �D.    �T$D�B�D.�������w
�  ��r�D.�J����L$�$�kr��T$D���������D$��B�D.�D.   �D.     �B$�D.$�������w
�  ��r�D.�J����L$�$���9kr����D$�D.�B(�D.(�D50�D$ �D$�D$�D$@�$�Gt��l$�t$ 9���   �F�)蹫��������҉T$�t$����������������wЋG�������w�  ��r������G�$�gr��G܃������w�  ��r���������������ԉ<$��fr�9���u�kD$0�L$��L$ �D$��t�$� 	r���,^_[]�        USWV���    [���S �t$4�D$0��L$�h9͋~��   ���������D$�����EЉD$�G�   �G�    �E܉G܃������w
�  ��r�GԋMԃ���L$�$�ir��D$�G��E��G��G�   �G�    �E�G�������w�  ��r��G�M����L$�$�kir��D$�G��E��G��~��Љ~�D$�L$9ȉ��?����T$0�
��T$0�:�N�B�N�J�F�B�N�J�F�F���^_[]�         USWV���    [�üS �|$4�t$0�N�F��   �,@9�s���+N����9�v.�D$8�L��D$�4$�MEt��D$�D$�|$�4$��Dt��D$�$�D$�4$�'Et��D$�D$�|$�4$��Dt��D$�F�8�u�x�t�N��^_[]�      USWV���    [��S �t$0�D$4��L$�~H����	�����	ȉ���	�����	ȉ���	�B��@�@   GʉN1��ɽ����tk��$�Tr���|$ ��   �F    �F    �N��t#k������������������t�(�h�����u�����   �D$�hk��Ɛ���������E��M����u�} �tb�|$�����������|$�����u�} ��|$t>�N�L$�L$�L$�D$�׉<$�~Ct��D$�M���L$�	�H�M�H�G���|$�������u��D$�$�r��5�F    �F    �N��t k���������������t�(�h�����u��^_[]�       USWV��,�    [��lS �T$@��J)���iɫ����L$ �i�J)���i���������
=TUUw�9�B�1����Ƹ    tk��$�r���kL$ ��ʉT$(����   �|$�D   �D    �T$D�B�D�������w�L$$�  ���L$$r*�L$$�D�J����L$�$�l$����er��L$$��l$���������|$(��B�D�|$�L$$k��k��D$@�(�x9���   �T$�L$�G�)�l$(����������ڋD$ �D��D$ �t$�D$$�t����������D$$��������������o��F�   �F�    �G�F��������w�  ��r�����F�O����L$�$��dr��D$$�F��G�����9l$(��u�kD$ �L$��D$@�(�p��L$�H�L$�H9�t&���������F���4$�P9�u���t$(�0�P�H��t�,$�6r���,^_[]�              USWV���    [��,S �|$4�l$0�M�E��   �4@9�s���+M����9���   �D$8�  ��D$�,$��@t��M1�����   �D$0��7�t$��������	1�I�L$!ˍڋ<�9���   1�   ����������   ����������D��v#\$�ڋ<�9|$u��~   �D$�,$�A@t��M1���ti�D$0��7�t$��������	1�I�L$!ˍڋ<�9�t@1�   �����������t'����������D��v#\$�ڋ<�9|$u����EŋL$0�A�8�t�I��^_[]�        USWV���    [�üS �|$ �D$$�7�oH����	�����	ȉ���	�����	ȉ���	�B��@�@   GʉO1���t���$� r����t��D$�t$�<$�D?t��4$�,r��,�G    �G    �O��t������t� ���������u��^_[]�      USWV���D$�@    �@    �@��t"�L$�	�����������t����������u�L$$�T$ �������9���   �2�������t�t$�D$�x1ۅ�tx�D$�(�L$��������	1�O�<$!��\� �D� 9�tP�   1ɐ�����������t(����������D���#4$�\� �D� 9D$u����Eِ��������������D$��B�C�D$�@�L$$�D�����^_[]�            S���    [����R �D$�@��t�$��=t���[�        �               S���    [�ß�R ����  �$��=t���[�             USWV���    [��l�R �|$4�t$0�D$8�L$<�F    �N����������F    �F�~��8����,$�D$   �%�q��, uf� yU�D$�l$�D$�D$�<$�l�r���t5�x t/�$�D$    ��^r��H��t1���t�x@�@ v� � �F��^_[]�           SWV���    [�Ý�R �|$$�t$ ��N�L$�<$�P@�~ t!����~���L$�<$�P��V�T$�$�Q4��^_[�         SV���    [��>�R �t$ �D$$����~���T$�$�Q��V�T$�$�Q4���	���T$�$�Q��V�T$�$�QD��^[�               SV���    [����R �t$ �D$$������T$�$�Q��V�T$�$�Q4���	���T$�$�Q��V�T$�$�QH��^[�               USWV���    [��\�R �t$$�|$ �G��tZ�o��D$�4$�Q�� ��It2��i����T$�$�ы�W�T$�$�Q4���k����T$�$�Q���k����T$�$�ы�O�L$�4$�P@��^_[]�             �D$�x ����� USWV��,�    [�ì�R �l$D�t$@�F���$�g�q��D$�4$�;�s��D$ �t$ �t$�|$(�<$�D$   �k{r����|$�D$�$踟r����D$�E �4$�D$    �ڜr��L$H��4$�D$   �Ĝr��L$L���,^_[]�      USWV��   �    [����R ��$�   �������D$p�$�D$
   ���q��D$t1�� �    ��   �G���$��q��D$�<$�Z�s���$�   ��$�   �|$��$�   �4$�D$   �zr����t$�D$x�$�Ξr����D$x�D$p�<$�D$    ��r��Ɖ<$�D$   �ݛr���$�   �D$8�D$9�L$p�L$0��i����|$4�L$0�L$@�t$D�D$H�D$I�T$H�D$)�|$ �D$(�ʃ�u�L$(�L$X�D$ �D$P�L$X���C��u�D$X �D$Y�1��/�с� �  ��   u�L$@���L$@�L$P�|$T�T$X�D$Y��D$�D$�D$���u�D$�D$h�D$�D$`�5��u�D$h �D$i�%�̓�u�L$P���L$P�L$`�D$d�T$h�D$i�D$`�D$�,$�XXr��Ĉ   ^_[]�            USWV���    [����R �L$0�T$4��֋h�L$�|$�<$�8t����|$�4$�Ջ��k����T$�$�Q��T$0�R�T$�$�Q@�D$t�D$�$�j�q���^_[]�  SV��$�    [��n�R ��T	 ��t=�D$0�@�D$�$�D$�����J�q��D$�D$�D$�4$�D$    �Z[s��1�����$^[�             SV��$�    [����R ��\	 ��t=�D$0�@�D$�$�D$�������q��D$�D$�D$�4$�D$    ��Zs��1�����$^[�             SV��$�    [�Î�R ��d	 ��t=�D$0�@�D$�$�D$�����j�q��D$�D$�D$�4$�D$    �zZs��1�����$^[�             SWV��0�    [���R �D$@�L$P�T$L�t$H�|$D�D$   �D$   �|$�t$ ��D$$�L$,���������L$�L$�L$�$�g3s���0^_[�SWV��0�    [�í�R �D$@�L$P�T$L�t$H�|$D�D$   �D$   �|$�t$ ��D$$�L$,���������L$�L$�L$�$��2s���0^_[�SWV��0�    [��=�R �D$@�L$P�T$L�t$H�|$D�D$   �D$   �|$�t$ ��D$$�L$,���������L$�L$�L$�$�2s���0^_[��               �    X����R �������            �    X����R �������            �    X����R �������            �    X��c�R �������            �    X��C�R �������            �    X��#�R �������            �    X���R �������            �    X����R �������            USWV��|�    [�ü�R ��$�   �D$$    �D$     �D$(    ��$�   ��t�D$��$�   ��
w(�D- �D$`�t$a�9�D$d    �D$`    �D$h    �>�}����<$���q��Ɖt$h���|$`�l$d�l$�D$�D$�4$��q��. ��$�   �D$! �D$  �D$ �$�D$    �o2r��D$h�D$(�D$`�D$ �~(�D$ �t�D$$������   �$   �c�q��ƽ   ��t]�F    �F    ����������n�D$ �t�D$(�D$p�D$$�
�L$!�L$p��D$t�D$p�D$�,$�D$    �{Vs��/�G�w��t
�$�w2t��/�D$d    �D$`    �D$h    �D$`�D$�,$�}2t���tA�D$`��$�   t�D$h�$���q���$�   �F�D$ t�D$(�$���q�1���|^_[]��D$4    �D$0    �D$8    ��=���D$�$�D$   ��q��ƊD$ �D$�u	�D$����D$$�D$�D$�,0��
w�6�D$0�|$1�'������,$� �q��ǉ|$8���l$0�t$4�D$ �D$�t$�D$�D$�<$��q��7 �D$u�D$!��D$(�L$�L$�D$�t$0�4$�f0r���Z���<$�D$   ���q��D$�|$�4$�@0r��H�L$H� �D$@�@    �     �@    �D$`�u�L$a����D$d�L$h�D$�L$�D$@�$��/r��H�L$X� �D$P�@    �     �@    �D$P�$�D$    ��r�     �   �          S���    [��/�R �D$�D$��0�$�fr���[�        S���    [����R �D$$�L$ �D$�L$��0�$��er���[�S���    [����R �D$(��t)�D$$�L$ ��d������T$�D$�L$�D$�$�0t���[�          �               �    X��s�R �������            S���    [��O�R �D$������������$�vSs���������[��          SVP�    [���R �t$����������F�$�7Ss��4$�o/t��������^[��  S���    [����R �D$���$�Ss���[�            �    X����R �������            �    X����R �������            1��             SWV���    [��M�R �L$$�t$ �F�P;Ps
�z�x�
��ɉL$�$�R�q�����^_[�         SWV���    [����R �L$$�t$ �F�P;Ps
�z�x�
��ɉL$�$��q�����^_[�         SWV���    [�í�R �L$$�t$ �F�P;Ps
�z�x�
��ɉL$�$��q�����^_[�         USWV���    [��\�R �t$ �~�D$$�l$(�W�O)�9�v�l$�D$�<$�B�q���l$�D$�$��q�o����^_[]�   USWV���    [����R �t$$�D$ �h�4$�D$�������q��ǋM�E)�9�v�|$�t$�,$���q���|$�t$�$��q�}�D$ ��^_[]�   SV���    [�Î�R �D$$�t$ �N���t�P�@�@��T$�D$�$�q�q�����^[�         SV���    [��>�R �D$$�t$ �N�D$�$�H�q�����^[�SV���    [���R �D$$�t$ �N�D$�$�(�q�����^[�SV���    [����R �D$$�L$(�t$ �V�L$�D$�$�=r�����^[�        SV���    [�Þ�R �D$$�L$(�t$ �V�L$�D$�$���q�����^[�        SV���    [��^�R �D$$�t$ �N�D$�$���r�����^[�SV���    [��.�R �D$$�t$ �N�D$�$�8�q�����^[�SV���    [����R �D$$�t$ �N�D$�$��q�����^[�SV���    [����R �D$$�t$ �F�D$�$�D�r�����^[�            SV���    [�Î�R �D$$�t$ �N�L$�$�x�r�����^[�USWV���    [��\�R �t$0�D$4�~�D$�D$�$���q����D$�l$�W�O)�9�v�l$�D$�<$�+�q���l$�D$�$�i�q�o����^_[]�            SV���    [����R �D$$�t$ �N�P`��t�Pd�@h���`@��T$�D$�$��q�����^[�     �               �    X����R �������            �L$�D$��	�H�@    �@     �H0�@    �@    �@    �@    �H$�H(�H@�H,�HL�H@�HD�H|�HH���   �H|���   ���   ���   ���   ���   ���   ���   ���   ���   ���   ���   ���   ���   � USWV��  �    [�é�R ��$�  �F$�N()����V��$�   ��$�   ��$�   �D$�$�Jr��N��$�  �D$��$�  �$�s+t���$D  ��$8  ��$<  ��$�  ��$@  ��$�   ��$�   ��$�   Ǆ$�      Ǆ$0  ����Ǆ$,  ����Ǆ$(  ����Ǆ$$  ����Ǆ$   ����Ǆ$  ����Ǆ$  ����Ǆ$  ����Ǆ$  ����Ǆ$  ����Ǆ$  ����Ǆ$  ����Ǆ$   ����Ǆ$�   ����Ǆ$�   ����Ǆ$�   ����Ǆ$�       Ǆ$�       �F@�vD)Ɖ���   ��i������t$$1��   ���������$�  �@@�(����D$��$�   �$�Z�q���t.��$�  �@@�4(�����$<  ;�$@  s�0��$<  �t$$G9�r��3��$D  �D$��$8  �$�D$   �D$    ��q���$<  븋�$8  ��$<  )�����$�  �F�T$0�L$4�D$0�D$�$�Hr��N��$�   �D$��$�   �$�X)t��F|���   )����V�L$4�D$0�D$0�D$�$�KHr���$�   �N��$�   �D$��$�   �$�)t���$�   �$��fr��D$����  ��$�   1퐐��l$��$�   �D$��$�   �$�fr�����$�   ��$�   ��$�   ��$�   ��$�   ��$�   ��$�  �	��$�   ��$�   �D$�$�(t�����   �l$ �$�D$$�
Gr���1���uB�q��$�   �D$��$�   �$�D$   �D$    ��q���$�   �/��������������|$�D$$�$��Fr��Ƌ�$�   ;�$�   s��0��$�   G9�uϋD$$�$�C#t���$�   �l$ ��$�   �$��'t��ǅ�tS��$�   ��$�   )�����$�  �R�L$4�D$0�D$0�D$�$�Fr���$�   �D$��$�   �$�`'t���$�   9�t�$���q�E;l$�b�����$�  ���   ���   )����V�L$4�D$0�D$0�D$�$�+Fr��N��$�   �D$��$�   �$��&t��l$<�l$0�l$4�L$|�L$8���   ���   )׉����   ��iǫ����D$$��1�   ���������������$�  ���   �L$8�<2���9�s�8�D$4���D$4E;l$$r��*�D$<�D$�D$0�$�D$   �D$    ��q��D$4���t$0�l$<)�����$�  �ϋO��$�  ��$�  ��$�  �D$�$�$Er��O�L$(�D$�D$(�$��%t��D$09�t�$�m�q���$�   �$��q���$8  ��$D  9�t�$�D�q��Ĝ  ^_[]�         SV��$�    [��n�R �t$0�D$4�@�D$8�D$�D$�D$�$�Dr����$^[�           USWV���   �    [���R ��$�   �G�$��q��$�D$    �D$    �D$$  �HIr���$�   �G��$�   ��$�   Ǆ$�      ��$�   �D$�$��jr��G�G��$�   Ǆ$�      ��$�   �D$�$��jr��G�G��$�   Ǆ$�      ��$�   �D$�$�jr��G�G��$�   Ǆ$�      ��$�   �D$�$�qjr��G�G�t$x�D$|   �D$x�D$�$�Kjr��G�G�$�}�q��$�D$    �D$    �D$  �-Hr��D$@�w��$�   ��$�   ��$�   ��$�   ��$�   ��$�   ��$�   �D$�4$�kr���$�   ��$�   ��$�   ��$�   ��$�   �D$�4$�rkr���$�   ��$�   ��$�   Ǆ$�      ��$�   �D$�4$�[Br��D$D�G�$��q���$�   �L$�$�D$    �D$    �IGr��D$H�G��$�   �D$8�D$8�D$�$��jr��D$L�G�$�`�q���$  �L$�$�D$    �D$    ��Fr��D$P�G��$  �D$0�D$0�D$�$�jr��D$T�G�$���q���$  �L$�$�D$    �D$    �Fr��D$X�G�D$\�G�D$`�G�D$d�G�D$h�G�D$l�G��$  �D$(�D$(�D$�$�jr��D$p�G�$�d�q���$  �L$�$�D$    �D$    �Fr��D$t�G�L$@�L$ �D$$   �D$ �D$�$��@r��ƀ�$    t@�?��*����D$�$�D$   �^�q��D$�D$�D$�<$�@r��t$�$�*@r���$�   �0���   ^_[]�     SWV��0�    [���R �L$@�D$P�t$D�V�|$H�|$(�|$L�D$     �D$$    �ƴ   �t$�D$ �D$�D$�|$�D$(�D$�$:   �   ��0^_[�         USWV��l���    [��$�   ��$�   �Ê�R ��$�    ��   ��<$�L$���q���   �l$�$�D$    �D$    �Dr��D$<��$�   � �D$@�t$D�<$��q���$�   �L$�$�D$    �D$    �]Dr��D$H��$�   � �D$0�D$0�D$�<$��gr��D$L�D$<�D$(�D$,   �D$(�   ��<$�L$�6�q���   �l$�$�D$    �D$    ��Cr��D$\��$�   � �D$`�t$d�<$���q���$�   �L$�$�D$    �D$    �Cr��D$h�D$\�D$P�D$T   �D$P�D$�<$�\>r��L$��$�   ��D$   �D$    �D$ �������w
�  ��r�D$�$�<r��F;Fsq��t<�    �@    �L$ �H�������w
�  ��r�L$����L$�$��>r��F�D$ �������w
�  ��r�D$�$��:r���l^_[]É4$�D$    �t��F�w���         SWV��0�    [��-�R �L$@�D$P�t$D�V�|$H�|$(�|$L�D$     �D$$    �ƴ   �t$�D$ �D$�D$�|$�D$(�D$�$:   �1�����0^_[�         USWV��<�    [�ì�R �l$`�t$T�~�D$X�D$8�D$\�D$�D$0�$�&~r����D$0�D$d�D$(�ƴ   �t$�D$(�D$�l$�D$�D$8�D$�$   �L$P��������<^_[]�  SWV��0�    [���R �L$@�D$P�t$D�V�|$H�|$(�|$L�D$T�D$ �ƴ   �t$�D$ �D$�D$�|$�D$(�D$�$   �%�����0^_[�             USWV��L�    [�Ü�R �L$d�A�Ή$��q��$�D$    �D$    �D$)  ��@r��D$ �n�D$h�L$l�t$p�|$t�L$<�D$8�D$8�D$�,$�Ydr��D$D�|$4�t$0�D$0�D$�,$�9dr��D$H�D$D�D$(�D$,   �D$(�D$�,$�1;r��D$$�D$d�@�L$ �L$�D$   �D$�D$�$�;r��L$`���L^_[]�   USWV��,�    [�Ü�R �l$P�t$T�|$D�G�$��q��$�D$    �D$    �D$(  ��?r��D$ �G�D$H�D$�D$�D$�$�_cr��D$$�G�$�0?r��t$�l$�$�D$    �x?r��D$(�G�L$ �L$�D$   �D$�D$�$�-:r��L$@���,^_[]�              SWV��P�    [�ý�R �t$`�|$d�G�$�;�q��$�D$    �D$    �D$;  ��>r��D$(�D$,    �D$0    �G�D$h�D$ �D$ �D$�$�tbr��D$4�G�$���q��$�D$    �D$    �D$    �>r��D$8�G�$�>r��$�D$    �D$    �D$    �V>r��D$<�G�$��=r��$�D$    �D$    �D$    �'>r��D$@�G�$�=r��$�D$    �D$    �D$    ��=r��D$D�G�$��q��$�D$    �D$    �D$    ��=r��D$H�G�$���q��$�D$    �D$    �D$    �=r��D$L�G�L$(�L$�D$
   �D$�D$�$�O8r����P^_[�     SWV�� �    [����R �t$0�|$4��o���D$�$�D$   ���q��D$�D$�D$�|$�4$�t���^_[�        USWV��L�    [�l$p�t$t�|$d�G�Ì�R �$��q��$�D$    �D$    �D$$  �<r��D$$�D$(    �D$,    �G�D$h�D$�D$�D$�$�?`r��D$0�G�$��q��$�D$    �D$    �D$    �P<r��D$4�G�$��;r��t$�l$�$�D$    �)<r��D$8�G�$�;r��L$|�L$�L$x�L$�$�D$    ��;r��D$<�G�$�;r��$�D$    �D$    �D$    ��;r��D$@�G�$���q��$�D$    �D$    �D$    �;r��D$D�G�$��q���$�   �L$�$�D$    �D$    �j;r��D$H�G�L$$�L$�D$
   �D$�D$�$�6r��L$`���L^_[]�                SWV��P�    [�í�R �t$`�|$d�G�$�+�q��   L$h�L$�$�D$    �D$    ��:r��D$(�D$,    �D$0    �G�D$     �D$$    �D$ �D$�$�[^r��D$4�G�$��q��$�D$    �D$    �D$    �l:r��D$8�G�$��9r��$�D$    �D$    �D$    �=:r��D$<�G�$��9r��$�D$    �D$    �D$    �:r��D$@�G�$�9r��$�D$    �D$    �D$    ��9r��D$D�G�$� �q��$�D$    �D$    �D$    �9r��D$H�D$l�D$�D$�$��ur����D$�D$L�G�L$(�L$�D$
   �D$�D$�$�F4r����P^_[�            USWV��\�    [�l$|��$�   �|$t�G����R �$�S�q��$�D$    �D$    �D$  �9r��D$4�D$8    �D$<    �G��$�   �D$(�D$(�D$�$�\r��D$@�G�$���q��$�D$    �D$    �D$    �8r��D$D�G�$�+8r��t$�l$�$�D$    �s8r��D$H�G�$�8r���$�   �L$��$�   �L$�$�D$    �>8r��D$L�G�$��7r��$�D$    �D$    �D$    �8r��D$P�G�$�0�q��$�D$    �D$    �D$    ��7r��D$T�D$x�D$�D$ �$�tr����D$ �D$X�G�L$4�L$�D$
   �D$�D$�$�v2r��L$p���\^_[]�       SWV��P�    [���R �t$`�|$d�G�$��q��$�D$    �D$    �D$  �;7r��D$$�D$(    �D$,    �D$0    �G�$�D�q��$�D$    �D$    �D$    ��6r��D$4�G�$�6r��$�D$    �D$    �D$    ��6r��D$8�G�$�V6r��$�D$    �D$    �D$    �6r��D$<�G�$�'6r��$�D$    �D$    �D$    �g6r��D$@�G�$��q��$�D$    �D$    �D$    �86r��D$D�D$h�D$�D$ �$�`rr����D$ �D$H�D$l�D$�D$�$�Arr����D$�D$L�G�L$$�L$�D$   �D$�D$�$�0r����P^_[�     SWV��P�    [��M�R �t$`�|$d�G�$���q��   L$h�L$�$�D$    �D$    �v5r��D$(�D$,    �D$0    �D$4    �G�$��q��$�D$    �D$    �D$    �/5r��D$8�G�$��4r��$�D$    �D$    �D$    � 5r��D$<�G�$�4r��$�D$    �D$    �D$    ��4r��D$@�G�$�b4r��$�D$    �D$    �D$    �4r��D$D�G�$���q��$�D$    �D$    �D$    �s4r��D$H�D$l�D$�D$ �$�pr����D$ �D$L�G�L$(�L$�D$
   �D$�D$�$�	/r����P^_[�               USWV��l�    [�Ü�R ��$�   ��$�   �G�$��q��$�D$    �D$    �D$  ��3r��D$@��$�   �$��t��D$D��$�   �l$h�D$h�$�րr�1Ʉ�D͉L$8�D$8�D$�D$0�$�or����D$0�D$H�G��$�   �D$(�D$(�D$�$�Wr��D$L�G�$�g�q���$�   �L$�$�D$    �D$    �3r��D$P�G�$�2r��$�D$    �D$    �D$    ��2r��D$T�G�$�v2r��$�D$    �D$    �D$    �2r��D$X�G�$�G2r��$�D$    �D$    �D$    �2r��D$\�G�$��q��$�D$    �D$    �D$    �X2r��D$`��$�   �D$�D$ �$�}nr����D$ �D$d�G�L$@�L$�D$
   �D$�D$�$��,r����l^_[]�                SWV��P�    [��}�R �t$`�|$d�G�$���q��$�D$    �D$    �D$*  �1r��D$(�D$,    �D$h�D$�D$ �$��mr����D$ �D$0�D$4    �G�$��q��$�D$    �D$    �D$    �M1r��D$8�G�$��0r��$�D$    �D$    �D$    �1r��D$<�G�$�0r��$�D$    �D$    �D$    ��0r��D$@�G�$�0r��$�D$    �D$    �D$    ��0r��D$D�G�$���q��$�D$    �D$    �D$    �0r��D$H�D$l�D$�D$�$�lr����D$�D$L�G�L$(�L$�D$
   �D$�D$�$�'+r����P^_[�             USWV��L�    [�ü�R �l$p�t$t�|$d�G�$�6�q��$�D$    �D$    �D$  ��/r��D$$�D$(    �D$h�D$�D$ �$�lr����D$ �D$,�D$0    �G�$���q��$�D$    �D$    �D$    �/r��D$4�G�$�/r��$�D$    �D$    �D$    �Y/r��D$8�G�$��.r��$�D$    �D$    �D$    �*/r��D$<�G�$�.r��t$�l$�$�D$    �/r��D$@�G�$�$�q��L$x�L$�$�D$    �D$    ��.r��D$D�D$l�D$�D$�$��jr����D$�D$H�G�L$$�L$�D$
   �D$�D$�$�j)r��L$`���L^_[]�           USWV��l�    [��$�   ��$�   �G����R �$�t�q��$�D$    �D$    �D$  �$.r��D$@��$�   �$�!
t��D$D��$�   �l$h�D$h�$�6{r�1Ʉ�D͉L$8�D$8�D$�D$0�$�jr����D$0�D$H�G��$�   �D$(�D$(�D$�$�fQr��D$L�G�$���q���$�   �L$�$�D$    �D$    �t-r��D$P�G�$�-r��t$��$�   �L$�$�D$    �F-r��D$T�G�$��,r���$�   �L$��$�   �L$�$�D$    �-r��D$X�G�$�,r���$�   �L$��$�   �L$�$�D$    ��,r��D$\�G�$���q���$�   �L$�$�D$    �D$    �,r��D$`��$�   �D$�D$ �$��hr����D$ �D$d�G�L$@�L$�D$
   �D$�D$�$�='r���$�   ���l^_[]�           USWV��l�    [����R ��$�   �   �$�   �E�$�?�q��$�D$    �D$    �D$  ��+r��D$<��$�   �$��t��D$@��$�   �|$h�D$h�$�yr�1Ʉ�DωL$8�D$8�D$�D$0�$��gr����D$0�D$D�E��$�   �D$(�D$(�D$�$�1Or��D$H�E�$��q���$�   �L$�$�D$    �D$    �?+r��D$L�E�$��*r��$�D$    �D$    �D$    �+r��D$P�E�$�*r��$�D$    �D$    �D$    ��*r��D$T�E�$�r*r��$�D$    �D$    �D$    �*r��D$X�E�$���q��t$�$�D$    �D$    �*r��D$\��$�   �D$�D$ �$�fr����D$ �D$`��$�   �D$d�E�L$<�L$�D$   �D$�D$�$�%r���$�   ���l^_[]�             USWV��|�    [��$�   ��$�   �G�Ü�R �$��q��$�D$    �D$    �D$  ��)r��D$@��$�   �$��t��D$D��$�   �l$x�D$x�$��vr�1Ʉ�D͉L$H�G��$�   �D$8�D$8�D$�$�%Mr��D$L�G�$��q���$�   �L$�$�D$    �D$    �3)r��D$P�G�$��(r��t$��$�   �L$�$�D$    �)r��D$T�G�$�(r���$�   �L$��$�   �L$�$�D$    ��(r��D$X�G�$�a(r���$�   �L$��$�   �L$�$�D$    �(r��D$\�G�$��q���$�   �L$�$�D$    �D$    �i(r��D$`��$�   �D$d�G��$�   �D$0�D$0�D$�$��Kr��D$h�G��$�   �D$(�D$(�D$�$��Kr��D$l�G��$�   �D$ �D$ �D$�$�Kr��D$p�G�$��q���$�   �L$�$�D$    �D$    �'r��D$t�G�L$@�L$�D$   �D$�D$�$�c"r���$�   ���|^_[]� USWV��\�    [��$�   �l$t�E����R �$�w�q��$�D$    �D$    �D$  �''r��D$,��$�   �$�$t��D$0��$�   �t$X�D$X�$�9tr�1Ʉ�DΉL$4�E�D$x�D$ �D$ �D$�$�Jr��D$8�E�$���q���$�   �L$�$�D$    �D$    �&r��D$<�E�$�*&r��|$��$�   �L$�$�D$    �k&r��D$@�E�$��%r���$�   �L$��$�   �L$�$�D$    �6&r��D$D�E�$��%r���$�   �L$��$�   �L$�$�D$    �&r��D$H�E�$�"�q���$�   �L$�$�D$    �D$    ��%r��D$L��$�   �D$P��$�   �D$T�E�L$,�L$�D$   �D$�D$�$�n r��L$p���\^_[]�               SWV��P�    [����R �t$`�|$d�G�$�{�q��$�D$    �D$    �D$ B �+%r��D$0�G�D$h�D$(�D$(�D$�$��Hr��D$4�D$p�D$8�G�$��q��L$t�L$�$�D$    �D$    ��$r��D$<�G�D$x�D$ �D$ �D$�$�fHr��D$@�G��$�   �D$�D$�D$�$�<Hr��D$D�G�$��q���$�   �L$�$�D$    �D$    �J$r��D$H��$�   �D$L�G�L$0�L$�D$   �D$�D$�$��r����P^_[�          USWV��\�    [�Ì�R ��$�   �|$t�G�$��q��$�D$    �D$    �D$/  �#r��D$<�t$x�t$X�D$X�$��pr�1Ʉ�DΉL$8�D$8�D$�D$0�$��_r����D$0�D$@�G�D$|�D$(�D$(�D$�$�Gr��D$D��$�   �D$�D$ �$�w_r����D$ �D$H�l$L�G�$�M�q���$�   �L$�$�D$    �D$    ��"r��D$P�G�$��q���$�   �L$�$�D$    �D$    ��"r��D$T�G�L$<�L$�D$   �D$�D$�$�}r��L$p���\^_[]�              USWV��\�    [���R ��$�   �|$t�G�$��q��   L$x�L$�$�D$    �D$    �2"r��D$8�l$|�l$X�D$X�$�Zor�1Ʉ�D͉L$0�D$0�D$�D$(�$�;^r����D$(�D$<�G��$�   �D$ �D$ �D$�$�Er��D$@��$�   �D$�D$�$��]r����D$�D$D�t$H��$�   �D$L�G�$��q���$�   �L$�$�D$    �D$    �g!r��D$P�G�$��q���$�   �L$�$�D$    �D$    �5!r��D$T�G�L$8�L$�D$   �D$�D$�$��r��L$p���\^_[]�           USWV��L�    [��|�R �D$`�L$d�T$x�t$|��$�   �l$h�l$H�l$t�l$8��$�   �D$l�D$@�l$(�|$$�t$ �T$�T$8�T$�D$@�D$�T$H�T$�L$�$�D$0   ��s���H^_[]�          USWV��L�    [��ܺR ��$�   ��$�   �|$d�D$h�D$H�D$l�D$@�D$t�D$8�G�D$x�D$0�D$0�D$�$�Cr��t$(�l$$��$�   �L$ �D$�D$8�D$�D$@�D$�D$H�D$�|$�D$`�$�D$A  ���s���H^_[]�        USWV��L�    [���R �D$`�L$d�T$|��$�   ��$�   �l$h�l$H�D$l�D$@�l$t�l$8�l$x�|$(�t$$�T$ �l$�T$8�T$�D$@�D$�T$H�T$�L$�$�D$A  �#�s���H^_[]�          USWV��   �    [��$�   �G��y�R �$���q��$�D$    �D$    �D$  �r��D$L��$�   �$��s��D$P��$�   ��$�   ��$�   �$�kr�1���E�l$H�D$H�D$�D$@�$�Zr����D$@�D$T�G��$�   �D$8�D$8�D$�$��Ar��D$X�G�$�E�q���$�   �L$�$�D$    �D$    ��r��D$\�G�$�r���$�   �L$��$�   �L$�$�D$    �r��D$`�G�$�Nr���$�   �L$��$�   �L$�$�D$    �r��D$d�G�$��q���$�   �L$��$�   �L$�$�D$    �Sr��D$h�G�$�t�q���$�   �L$�$�D$    �D$    �!r��D$l��$�   �D$�D$0�$�FYr����D$0�D$p��$�   �D$t�G�$��q��$�D$    �D$    �D$    ��r��D$x��$�   �D$�D$(�$��Xr����D$(�D$|��$�   ��$�   ��$�   ��t*��$�   �O� �D$ �D$ �D$�$�@r��Ɖ�$�   �G�L$L�L$�D$   �D$�D$�$�r��틌$�   �t�D$�D$�D$�<$�=�s��Č   ^_[]� SWV�� �    [�Í�R �t$0�~@�D$4�D$   �D$    �D$�������w
�  ��r�D$�$��r��FD;FHsp��t<�    �@    �L$�H�������w
�  ��r�L$����L$�$�6r��FD�D$�������w
�  ��r�D$�$�mr��� ^_[É<$�D$    ��s��FD�x���              USWV��   �    [��$�   �G�Ù�R �$��q��$�D$    �D$    �D$  ��r��D$L��$�   �$���s��D$P��$�   ��$�   ��$�   �$��gr�1���E�l$H�D$H�D$�D$@�$�Vr����D$@�D$T�G��$�   �D$8�D$8�D$�$�>r��D$X�G�$�e�q���$�   �L$�$�D$    �D$    �r��D$\�G�$�r���$�   �L$��$�   �L$�$�D$    ��r��D$`�G�$�nr���$�   �L$��$�   �L$�$�D$    �r��D$d�G�$�ɿq��$�D$    �D$    �D$    �yr��D$h�G�$蚿q���$�   �L$�$�D$    �D$    �Gr��D$l��$�   �D$�D$0�$�lUr����D$0�D$p��$�   �D$t�G�$�;�q���$�   �L$�$�D$    �D$    ��r��D$x��$�   �D$�D$(�$�Ur����D$(�D$|Ǆ$�       ��$�   ��t*��$�   �O� �D$ �D$ �D$�$�D<r��Ɖ�$�   �G�L$L�L$�D$   �D$�D$�$�4r��틌$�   �t�D$�D$�D$�<$�c�s��Č   ^_[]�       USWV��|�    [��$�   �G�ì�R �$�+�q��$�D$    �D$    �D$  ��r��D$<��$�   �$���s��D$@��$�   �l$x�D$x�$��dr�1���E�l$8�D$8�D$�D$0�$��Sr����D$0�D$D�G��$�   �D$(�D$(�D$�$�;r��D$H�G�$�~�q���$�   �L$�$�D$    �D$    �+r��D$L�G�$�r���$�   �L$��$�   �L$�$�D$    ��r��D$P�G�$�r���$�   �L$��$�   �L$�$�D$    ��r��D$T�G�$�Rr��$�D$    �D$    �D$    �r��D$X�G�$購q���$�   �L$�$�D$    �D$    �`r��D$\�D$`    ��$�   �D$d�G�$�n�q���$�   �L$�$�D$    �D$    �r��D$h�D$l    �D$p    ��$�   ��t*��$�   �O� �D$ �D$ �D$�$�9r��Ɖt$t�G�L$<�L$�D$   �D$�D$�$�r��틌$�   �t�D$�D$�D$�<$��s���|^_[]�             USWV��l�    [����R ��$�   ��$�   �G�$�t�q��$�D$    �D$    �D$  �$r��D$$�G�$�E�q��$���q��D$(�D$,    �o��=&���D$`�$�D$   脭q��D$d�D$`�D$�,$�8r��D$0�G�$��q��$�D$    �D$    �D$    �r��D$4�G�$�.r��$�D$    �D$    �D$    �nr��D$8�G�$��r��$�D$    �D$    �D$    �?r��D$<�G�$��r��$�D$    �D$    �D$    �r��D$@�G�$�1�q���$�   �L$�$�D$    �D$    ��r��D$D�D$H    ��$�   �D$L�G�$��q��$�D$    �D$    �D$    �r��D$P�D$T    �D$X    �D$\    �G�L$$�L$�D$   �D$�D$�$�9r����l^_[]�              USWV��   �    [��$�   �G��ɭR �$�H�q��$�D$    �D$    �D$  ��r��D$L��$�   �$���s��D$P��$�   ��$�   ��$�   �$�`r�1���E�l$H�D$H�D$�D$@�$��Nr����D$@�D$T�G��$�   �D$8�D$8�D$�$�46r��D$X�G�$蕸q���$�   �L$�$�D$    �D$    �Br��D$\�G�$��r���$�   �L$��$�   �L$�$�D$    �r��D$`�G�$�r���$�   �L$��$�   �L$�$�D$    ��r��D$d�G�$���q��$�D$    �D$    �D$    �r��D$h�G�$�ʷq��$�D$    �D$    �D$    �zr��D$l��$�   �D$�D$0�$�Mr����D$0�D$p��$�   �D$t�G�$�n�q��$�D$    �D$    �D$    �r��D$x�D$|    Ǆ$�       ��$�   ��t*��$�   �O� �D$(�D$(�D$�$�4r��Ɖ�$�   �G�L$L�L$ �D$$   �D$ �D$�$�r��Ƌ�$�   �0�G(;G,s+�0�G(��t�t$�D$�D$�<$��s��Č   ^_[]� �G$�O0�L$�$�D$   �D$    �d�q��G(�               USWV��l�    [��$�   ��$�   �G�ü�R �$�4�q��$�D$    �D$    �D$  ��r��D$$�D$(    �D$,    �o��=&���D$`�$�D$   �S�q��D$d�D$`�D$�,$�[3r��D$0�G�$輵q��$�D$    �D$    �D$    �lr��D$4�G�$��r��t$��$�   �L$�$�D$    �>r��D$8�G�$��r���$�   �L$��$�   �L$�$�D$    �	r��D$<�G�$�*�q��$�D$    �D$    �D$    ��r��D$@�G�$���q��$�D$    �D$    �D$    �r��D$D��$�   �D$�D$ �$��Jr����D$ �D$H��$�   �D$L�G�$蟴q��$�D$    �D$    �D$    �Or��D$P�D$T    �D$X    �D$\    �G�L$$�L$�D$   �D$�D$�$��r���$�   ���l^_[]�          USWV��l�    [��$�   ��$�   �G��|�R �$���q��$�D$    �D$    �D$  �r��D$$�D$(    �D$,    �o��=&���D$`�$�D$   ��q��D$d�D$`�D$�,$�1r��D$0�G�$�|�q��$�D$    �D$    �D$    �,r��D$4�G�$�r��t$��$�   �L$�$�D$    ��r��D$8�G�$�r���$�   �L$��$�   �L$�$�D$    ��r��D$<�G�$��q��$�D$    �D$    �D$    �r��D$@�G�$軲q��$�D$    �D$    �D$   �kr��D$D��$�   �D$�D$ �$�Hr����D$ �D$H��$�   �D$L�G�$�_�q��$�D$    �D$    �D$    �r��D$P�D$T    �D$X    �D$\    �G�L$$�L$�D$   �D$�D$�$�r���$�   ���l^_[]�          USWV��\�    [��<�R �|$x�t$p�<$�D$   ��Fr��@t	���%  �D$4�D$(�D$,�t$X�t$0��D$�@�D$��to1�|$4�������������t$0�l$�D$�$��r�9�s��|$,���|$,E9l$u��.�ƍD$4�D$�D$(�$�D$   �D$    ��q����|$,뼋|$t�D$x�$�D$   �Fr��ƃ�@�G�$��q��t$�$�D$    �D$    �
r��L$(�A �G�L$(�L$ �T$,)����T$$�D$ �D$�$�Mr��L$p��D$(�L$49�t�$裤q���\^_[]�          USWV��\�    [��̤R �|$x�t$p�<$�D$   �UEr���t	���'  �D$4�D$(�D$,�t$X�t$0��D$�@�D$��tn1�|$4������������t$0�l$�D$�$�lr�9�s��|$,���|$,E9l$u��.�ƍD$4�D$�D$(�$�D$   �D$    蜦q����|$,뼋|$t�D$x�$�D$   �Dr��Ɓ�@  �G�$�y�q��t$�$�D$    �D$    �-	r��L$(�A �G�L$(�L$ �T$,)����T$$�D$ �D$�$��r��L$p��D$(�L$49�t�$�0�q���\^_[]�       SWV�� �    [��]�R �t$0�|$4�G�$�ۮq��$�D$    �D$    �D$  �r��D$�G�L$�L$�D$   �D$�D$�$�@r���� ^_[�      USWV��|�    [��$�   �G��ܢR �$�[�q��   �$�   �L$�$�D$    �D$    �r��D$<��$�   �$� �s��D$@��$�   �l$x�D$x�$�Ur�1���E�l$8�D$8�D$�D$0�$��Cr����D$0�D$D�G��$�   �D$(�D$(�D$�$�E+r��D$H�G�$覭q���$�   �L$�$�D$    �D$    �Sr��D$L�G�$��r���$�   �L$��$�   �L$�$�D$    �r��D$P�G�$�r���$�   �L$��$�   �L$�$�D$    ��r��D$T�G�$�
�q��$�D$    �D$    �D$    �r��D$X�G�$�۬q��$�D$    �D$    �D$   �r��D$\�D$`    �D$d    �G�$蜬q���$�   �L$�$�D$    �D$    �Ir��D$h�D$l    �D$p    ��$�   ��t*��$�   �O� �D$ �D$ �D$�$��)r��Ɖt$t�G�L$<�L$�D$   �D$�D$�$� r��틌$�   �t�D$�D$�D$�<$���s���|^_[]�           USWV��|�    [��$�   �G��,�R �$諫q��   �$�   �L$�$�D$    �D$    �Sr��D$<��$�   �$�P�s��D$@��$�   �l$x�D$x�$�eRr�1���E�l$8�D$8�D$�D$0�$�FAr����D$0�D$D�G��$�   �D$(�D$(�D$�$�(r��D$H�G�$���q���$�   �L$�$�D$    �D$    �r��D$L�G�$�4r���$�   �L$��$�   �L$�$�D$    �nr��D$P�G�$��r���$�   �L$��$�   �L$�$�D$    �9r��D$T�G�$�Z�q��$�D$    �D$    �D$    �
r��D$X�G�$�+�q��$�D$    �D$    �D$   ��r��D$\�D$`    �D$d    �G�$��q���$�   �L$�$�D$    �D$    �r��D$h�D$l    �D$p    ��$�   ��t*��$�   �O� �D$ �D$ �D$�$�'r��Ɖt$t�G�L$<�L$�D$   �D$�D$�$�%r��틌$�   �t�D$�D$�D$�<$�4�s���|^_[]�           USWV��,�    [�l$H�t$L�|$D�G��|�R �$���q��$�D$    �D$    �D$!  �r��D$ �G�$�7r��t$�l$�$�D$    �r��D$$�G�$�r��L$T�L$�L$P�L$�$�D$    �Pr��D$(�G�L$ �L$�D$   �D$�D$�$��q��L$@���,^_[]�      USWV��l�    [�Ü�R ��$�   �E�$��q��$�D$    �D$    �D$4  ��r��D$8�E�$��q��$褧q��D$<�D$@    �E��$�   ��$�   �t$4�|$0�D$0�D$�$�;%r��D$D�E�t$,�|$(�D$(�D$�$�%r��D$H�E��$�   �D$ �D$ �D$�$��$r��D$L��$�   �D$P�E�$�D�q���$�   �L$�$�D$    �D$    �� r��D$T��$�   �D$X�E�$��q���$�   �L$�$�D$    �D$    � r��D$\�E�$�Ԧq��$�D$    �D$    �D$   � r��D$`��$�   �D$d�D$h    �E�L$8�L$�D$   �D$�D$�$�&�q��Ƌ��   ;��   s�0���   ��$�   �0��l^_[]� ���   ���   �L$�$�D$   �D$    ��q����   �          SWV��P�    [��m�R �t$`�L$d�T$t�D$|�|$l�|$L�|$D�|$h�|$H�|$@�|$p�|$8�|$x�|$0��$�   �|$(���D$$�D$0�D$ �T$�D$8�D$�D$@�D$�D$H�D$�L$�4$��s���L^_[�    USWV��l�    [��̙R ��$�   �E�$�K�q��$�D$    �D$    �D$4  ���q��D$4�E�$��q��$�Ԥq��D$8��$�   �t$h�D$h�$�	Lr�1Ʉ�DΉL$<�E��$�   ��$�   �|$,�t$(�D$(�D$�$�Q"r��D$@�E�|$$�t$ �D$ �D$�$�."r��D$D�E��$�   �D$�D$�D$�$�"r��D$H��$�   �D$L�E�$�Z�q���$�   �L$�$�D$    �D$    ��q��D$P��$�   �D$T�E�$��q���$�   �L$�$�D$    �D$    ���q��D$X�E�$��q��$�D$    �D$    �D$   ��q��D$\��$�   �D$`��$�   �D$d�E�L$4�L$�D$   �D$�D$�$�9�q��Ƌ��   ;��   s�0���   ��$�   �0��l^_[]� ���   ���   �L$�$�D$   �D$    �%�q����   �             USWV��L�    [��|�R �l$d��$�   �|$l�|$H�D$H�$��Ir��E�$��q��   L$h�L$�$�D$    �D$    ��q��D$(�|$H�D$H�$�Ir�1Ʉ�DωL$,�E�D$p�D$ �D$ �D$�$� r��D$0�D$x�D$4�E�$�e�q���t$|�t$�$�D$    �D$    ��q��D$8��$�   �D$<�E�$�(�q���$�   �L$�$�D$    �D$    ���q��D$@�E�$���q��$计q��D$D�E�L$(�L$�D$   �D$�D$�$�s�q��ƀ�$�    t2�|$�D$�$�G9r����E �L$�L$�$��s��t$�$���q��D$`�0��L^_[]�      USWV��\�    [��̕R ��$�   �|$t�G�$�G�q��   L$x�L$�$�D$    �D$    ���q��D$4�t$|�t$X�D$X�$�Hr�1Ʉ�DΉL$8�G��$�   �D$(�D$(�D$�$�ir��D$<��$�   �D$@�G�$迠q����$�   �l$�$�D$    �D$    �i�q��D$D��$�   �D$H�G�$��q��$�7�q��D$L�G�$�h�q��$� �q��D$P�G��$�   �D$ �D$ �D$�$���q��D$T�G�L$4�L$�D$	   �D$�D$�$tate_pol_flow_match(struct xfrm_state *x,
			struct xfrm_policy *xp, struct flowi *fl)
{
	return 1;
}

static inline int security_xfrm_decode_session(struct sk_buff *skb, u32 *secid)
{
	return 0;
}

static inline void security_skb_classify_flow(struct sk_buff *skb, struct flowi *fl)
{
}

#endif	/* CONFIG_SECURITY_NETWORK_XFRM */

#ifdef CONFIG_SECURITY_PATH
int security_path_unlink(struct path *dir, struct dentry *dentry);
int security_path_mkdir(struct path *dir, struct dentry *dentry, int mode);
int security_path_rmdir(struct path *dir, struct dentry *dentry);
int security_path_mknod(struct path *dir, struct dentry *dentry, int mode,
			unsigned int dev);
int security_path_truncate(struct path *path, loff_t length,
			   unsigned int time_attrs);
int security_path_symlink(struct path *dir, struct dentry *dentry,
			  const char *old_name);
int security_path_link(struct dentry *old_dentry, struct path *new_dir,
		       struct dentry *new_dentry);
int security_path_rename(struct path *old_dir, struct dentry *old_dentry,
			 struct path *new_dir, struct dentry *new_dentry);
#else	/* CONFIG_SECURITY_PATH */
static inline int security_path_unlink(struct path *dir, struct dentry *dentry)
{
	return 0;
}

static inline int security_path_mkdir(struct path *dir, struct dentry *dentry,
				      int mode)
{
	return 0;
}

static inline int security_path_rmdir(struct path *dir, struct dentry *dentry)
{
	return 0;
}

static inline int security_path_mknod(struct path *dir, struct dentry *dentry,
				      int mode, unsigned int dev)
{
	return 0;
}

static inline int security_path_truncate(struct path *path, loff_t length,
					 unsigned int time_attrs)
{
	return 0;
}

static inline int security_path_symlink(struct path *dir, struct dentry *dentry,
					const char *old_name)
{
	return 0;
}

static inline int security_path_link(struct dentry *old_dentry,
				     struct path *new_dir,
				     struct dentry *new_dentry)
{
	return 0;
}

static inline int security_path_rename(struct path *old_dir,
				       struct dentry *old_dentry,
				       struct path *new_dir,
				       struct dentry *new_dentry)
{
	return 0;
}
#endif	/* CONFIG_SECURITY_PATH */

#ifdef CONFIG_KEYS
#ifdef CONFIG_SECURITY

int security_key_alloc(struct key *key, const struct cred *cred, unsigned long flags);
void security_key_free(struct key *key);
int security_key_permission(key_ref_t key_ref,
			    const struct cred *cred, key_perm_t perm);
int security_key_getsecurity(struct key *key, char **_buffer);

#else

static inline int security_key_alloc(struct key *key,
				     const struct cred *cred,
				     unsigned long flags)
{
	return 0;
}

static inline void security_key_free(struct key *key)
{
}

static inline int security_key_permission(key_ref_t key_ref,
					  const struct cred *cred,
					  key_perm_t perm)
{
	return 0;
}

static inline int security_key_getsecurity(struct key *key, char **_buffer)
{
	*_buffer = NULL;
	return 0;
}

#endif
#endif /* CONFIG_KEYS */

#ifdef CONFIG_AUDIT
#ifdef CONFIG_SECURITY
int security_audit_rule_init(u32 field, u32 op, char *rulestr, void **lsmrule);
int security_audit_rule_known(struct audit_krule *krule);
int security_audit_rule_match(u32 secid, u32 field, u32 op, void *lsmrule,
			      struct audit_context *actx);
void security_audit_rule_free(void *lsmrule);

#else

static inline int security_audit_rule_init(u32 field, u32 op, char *rulestr,
					   void **lsmrule)
{
	return 0;
}

static inline int security_audit_rule_known(struct audit_krule *krule)
{
	return 0;
}

static inline int security_audit_rule_match(u32 secid, u32 field, u32 op,
				   void *lsmrule, struct audit_context *actx)
{
	return 0;
}

static inline void security_audit_rule_free(void *lsmrule)
{ }

#endif /* CONFIG_SECURITY */
#endif /* CONFIG_AUDIT */

#ifdef CONFIG_SECURITYFS

extern struct dentry *securityfs_create_file(const char *name, mode_t mode,
					     struct dentry *parent, void *data,
					     const struct file_operations *fops);
extern struct dentry *securityfs_create_dir(const char *name, struct dentry *parent);
extern void securityfs_remove(struct dentry *dentry);

#else /* CONFIG_SECURITYFS */

static inline struct dentry *securityfs_create_dir(const char *name,
						   struct dentry *parent)
{
	return ERR_PTR(-ENODEV);
}

static inline struct dentry *securityfs_create_file(const char *name,
						    mode_t mode,
						    struct dentry *parent,
						    void *data,
						    const struct file_operations *fops)
{
	return ERR_PTR(-ENODEV);
}

static inline void securityfs_remove(struct dentry *dentry)
{}

#endif

#ifdef CONFIG_SECURITY

static inline char *alloc_secdata(void)
{
	return (char *)get_zeroed_page(GFP_KERNEL);
}

static inline void free_secdata(void *secdata)
{
	free_page((unsigned long)secdata);
}

#else

static inline char *alloc_secdata(void)
{
        return (char *)1;
}

static inline void free_secdata(void *secdata)
{ }
#endif /* CONFIG_SECURITY */

#endif /* ! __LINUX_SECURITY_H */

