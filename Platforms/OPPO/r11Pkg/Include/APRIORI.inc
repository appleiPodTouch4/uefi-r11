##
#  OPPO R11 (r11) APRIORI DXE list: dispatch order that the SDM660 platform
#  drivers depend on (SMEM before NPA, clocks before storage and display).
#  Mirrors the SDM660 vendor platform; new board modules go last.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/r11/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  INF Binaries/r11/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/DALSYSDxe/DALSYSDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/HWIODxe/HWIODxe.inf
  INF Binaries/r11/QcomPkg/Drivers/ChipInfoDxe/ChipInfoDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/PlatformInfoDxe/PlatformInfoDxe.inf

  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf

  INF Binaries/r11/QcomPkg/Drivers/GLinkDxe/GLinkDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/ClockDxe/ClockDxe.inf

  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf




  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf

  # The payload block device must exist before BDS starts UniXNU.
  INF UniXNUPkg/Driver/UniXNURamDiskDxe/UniXNURamDiskDxe.inf
  INF UniXNUPkg/Driver/UniXNUDxe/UniXNUDxe.inf

  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf

  INF Binaries/r11/QcomPkg/Drivers/TLMMDxe/TLMMDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/I2CDxe/I2CDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/SPMIDxe/SPMIDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/PmicDxe/PmicDxeLa.inf
  INF Binaries/r11/QcomPkg/Drivers/AdcDxe/AdcDxe.inf
  INF Binaries/r11/QcomPkg/Drivers/QcomChargerDxe/QcomChargerDxeLA.inf
  INF Binaries/r11/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
}
