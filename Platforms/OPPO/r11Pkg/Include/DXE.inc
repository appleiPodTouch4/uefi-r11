##
#  OPPO R11 (r11) DXE driver list.
#
#  Two groups:
#    * open source EDK II / Mu / Silicium modules, built from source;
#    * Qualcomm SDM660 vendor DXE binaries, taken from the Binaries/r11
#      directory. R11's own XBL is a monolithic 2017 UEFI 4.1 image, so the
#      SDM660 driver set is taken from a sibling SDM660 UEFI device and must be
#      validated on this board: see README.md, "Vendor binaries".
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  # SDM660 platform services. SoC generic: same silicon, same LA configuration.
  INF Binaries/r11/QcomPkg/Drivers/EnvDxe/EnvDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/NpaDxe/NpaDxe.inf

  INF Binaries/r11/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/r11/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/r11/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/LimitsDxe/LimitsDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/HashDxe/HashDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/RNGDxe/RngDxe.inf

  # Storage: R11 is eMMC (SDCC). There is no UFS controller on this board, so
  # UFSDxe is deliberately not included.

  # File systems: the UniXNU payload is a FAT image in the boot ramdisk.
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  # Tools -> Advanced Logger, the QR export of the in-memory firmware log. Its
  # depex is gAdvancedLoggerProtocolGuid, so it stays dormant on a build without
  # the logger.
  INF UniXNUPkg/Driver/AdvancedLogDxe/AdvancedLogDxe.inf

  # Display: 0 = SimpleFbDxe over the continuous-splash buffer at 0x9D400000,
  # which XBL/ABL already owns and fills with the OPPO logo, so the menu is
  # visible without touching MDSS. 1 = vendor DisplayDxe, which needs an R11
  # panel driver (S6E3FA3 command mode) that does not exist yet.
!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/r11/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
  INF QcomPkg/Drivers/DisplayReEnablerDxe/DisplayReEnablerDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf

!endif

  # Buttons, charger, battery, thermal and USB.
  INF Binaries/r11/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/r11/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciPciEmulationDxe/XhciPciEmulationDxe.inf
  INF Binaries/generic/QcomPkg/Drivers/XhciDxe/XhciDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf

  # BDS: starts UniXNU, which is registered by UniXNUDxe as a boot option.
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
