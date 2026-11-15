##
#  OPPO R11 (r11) UEFI platform, Mu-Silicium layout.
#
#  Derived from Platforms/Xiaomi/lavenderPkg (also SDM660 + PM660) and the
#  memory map the stock R11 XBL itself declares. See README.md for what is
#  verified, what is reused from the SDM660 vendor set and what is still missing.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  PLATFORM_NAME                  = r11
  PLATFORM_GUID                  = 7C1E1D0A-4B36-4E63-9E4E-5C1D0F6A2B11
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/r11Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = r11Pkg/r11.fdf

  #
  # 0: SimpleFbDxe over the continuous-splash buffer (0x9D400000).
  # 1: vendor DisplayDxe from Binaries/r11 (needs an R11 panel driver; not yet).
  #
  USE_CUSTOM_DISPLAY_DRIVER      = 0

!include StarlordPkg/StarlordPkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR: one window from 0x80000000, ABL adds it to the UEFI memory map.
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI FD and stack. Must equal Resources/Configs/r11.toml [uefi_fd] and the
  # windows the stock XBL reserves ("UEFI FD" 0x9FC00000+0x300000,
  # "UEFI Stack" 0x9FF90000+0x40000).
  #
  gArmTokenSpaceGuid.PcdFdBaseAddress|0x9FC00000
  gArmTokenSpaceGuid.PcdFdSize|0x00300000
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x9FF90000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"OPPO"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"OPPO R11"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"r11"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"CPH1707"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"OPPO R11"

  #
  # Simple frame buffer: the stock continuous-splash buffer, 1080x1920 ARGB32.
  # SimpleFbDxe finds it by the "Display Reserved" entry of MemoryMapLib.
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1920
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Platform PEI
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

  #
  # Advanced Logger: the in-memory log lives in a fixed 1 MiB window carved out
  # of the top of the "DXE Heap" region (Library/MemoryMapLib/MemoryMapLib.c
  # calls the same range "AdvLogger"). It sits just below the continuous-splash
  # buffer, so it survives the whole UEFI run and is never handed to an
  # allocator. SEC runs with DDR already up (ABL loaded us there), so the same
  # address is valid from the first DEBUG() of SiliciumPkg/Sec onwards.
  #
  # 0x100 pages = 1 MiB, which is what the window below is sized for.
  #
  gAdvLoggerPkgTokenSpaceGuid.PcdAdvancedLoggerBase|0x9D300000
  gAdvLoggerPkgTokenSpaceGuid.PcdAdvancedLoggerPages|0x00000100
  gAdvLoggerPkgTokenSpaceGuid.PcdAdvancedLoggerHdwPortDebugPrintErrorLevel|0xFFFFFFFF

[PcdsFeatureFlag]
  #
  # The buffer is pre-placed, not allocated, and there is no pre-DXE logger: the
  # PeilessArm SEC library formats the header and builds gAdvancedLoggerHobGuid,
  # DXE core picks the HOB up and publishes gAdvancedLoggerProtocolGuid.
  #
  gAdvLoggerPkgTokenSpaceGuid.PcdAdvancedLoggerFixedInRAM|FALSE
  gAdvLoggerPkgTokenSpaceGuid.PcdAdvancedLoggerLocator|TRUE
  gAdvLoggerPkgTokenSpaceGuid.PcdAdvancedLoggerAutoWrapEnable|TRUE

[LibraryClasses]
  MemoryMapLib|r11Pkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|r11Pkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf

  #
  # UniXNU modules. StarlordPkg.dsc.inc already maps the MdePkg libraries, but
  # UniXNULib is ours: without this line EDK II fails with
  # "Instance of library class [UniXNULib] is not found".
  #
  UniXNULib|UniXNUPkg/Library/UniXNULib/UniXNULib.inf
  FdtLib|MdePkg/Library/BaseFdtLib/BaseFdtLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf

  #
  # UniXNURamDiskDxe expands an LZMA-packed payload. The volume carries the
  # 51.3 MiB prelinked kernel, and ABL will not place a ramdisk that large
  # (32 MiB boots, 64 MiB is silent), so the payload ships packed and is
  # expanded into the kernel window during DXE.
  #
  LzmaCustomDecompressLib|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf

  #
  # Advanced Logger. SiliciumPkg maps DebugLib to BaseDebugLibSerialPort on
  # DEBUG, which here means FrameBufferSerialPortLib: the messages go straight to
  # the panel and scroll away. Routing DebugLib through the Advanced Logger keeps
  # the same on-screen output through AdvancedLoggerHdwPortLib (which writes to
  # SerialPortLib) and keeps every line in the 1 MiB memory log as well, so
  # "Tools -> Advanced Logger" can page the whole run out as QR codes.
  #
  # These are listed after the !include so they win over SiliciumPkg's mapping.
  #
  DebugLib|AdvLoggerPkg/Library/BaseDebugLibAdvancedLogger/BaseDebugLibAdvancedLogger.inf
  AssertLib|AdvLoggerPkg/Library/AssertLib/AssertLib.inf
  AdvancedLoggerHdwPortLib|AdvLoggerPkg/Library/AdvancedLoggerHdwPortLib/AdvancedLoggerHdwPortLib.inf
  AdvancedLoggerAccessLib|AdvLoggerPkg/Library/AdvancedLoggerAccessLib/AdvancedLoggerAccessLib.inf
  QrEncoderLib|MsGraphicsPkg/Library/QrEncoderLib/QrEncoderLib.inf

[LibraryClasses.common.SEC]
  AdvancedLoggerLib|AdvLoggerPkg/Library/AdvancedLoggerLib/PeilessArm/AdvancedLoggerLib.inf

[LibraryClasses.common.DXE_CORE]
  AdvancedLoggerLib|AdvLoggerPkg/Library/AdvancedLoggerLib/DxeCore/AdvancedLoggerLib.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION]
  AdvancedLoggerLib|AdvLoggerPkg/Library/AdvancedLoggerLib/Dxe/AdvancedLoggerLib.inf

  #
  # SiliciumPkg's global [LibraryClasses] maps ExtractGuidedSectionLib to the
  # PrePi instance, which is correct for a PeiLess SEC but wrong for DXE: that
  # constructor allocates handler tables and publishes them with
  # BuildGuidDataHob, and HobLib is DxeHobLib here, so the call lands on
  # "ASSERT (FALSE)" -- "PEI HOB is read only for DXE phase" -- and the platform
  # dies before the menu. LzmaCustomDecompressLib registers itself through this
  # class, which is how UniXNURamDiskDxe ran into it.
  #
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  AdvancedLoggerLib|AdvLoggerPkg/Library/AdvancedLoggerLib/Runtime/AdvancedLoggerLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  AdvancedLoggerLib|AdvLoggerPkg/Library/AdvancedLoggerLib/Smm/AdvancedLoggerLib.inf

[Components]
  #
  # UniXNU and DarwinBoot. They must be listed here and in the FDF: the DSC entry
  # builds them, the FDF entry embeds them in the firmware volume.
  #
  UniXNUPkg/Application/UniXNU/UniXNU.inf
  UniXNUPkg/Driver/UniXNUDxe/UniXNUDxe.inf
  UniXNUPkg/Driver/UniXNURamDiskDxe/UniXNURamDiskDxe.inf
  DarwinBootPkg/Driver/DarwinBoot.inf

  #
  # Tools -> Advanced Logger: pages the firmware log out as QR codes. Depends on
  # gAdvancedLoggerProtocolGuid, so it only loads when the memory log exists.
  #
  UniXNUPkg/Driver/AdvancedLogDxe/AdvancedLogDxe.inf

  #
  # No ACPI tables: UniXNU hands XNU a device tree (ADT), not ACPI. Dropping
  # AcpiTableDxe keeps the FD smaller and removes the Silicium-ACPI dependency.
  # Add r11/AcpiTables.inf when a second OS or a driver needs ACPI.
  #
