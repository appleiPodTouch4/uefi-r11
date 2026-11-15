#include <Library/ConfigurationMapLib.h>

//
// OPPO R11 (r11) platform configuration map for the SDM660 UEFI drivers.
//
// These entries configure the vendor DXE drivers: CPU fusing, shared IMEM,
// download cookie, SD card GPIO state, PSHOLD register and the TZ application
// region. Values follow the SDM660 UEFI reference set.
//
// Board-specific entries that still need the R11's own values are marked
// TODO(r11); the stock values are kept so the firmware boots and reports them.
//
STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR
gConfigurationDescriptor[] = {
  // Configuration map
  {"NumCpusFuseAddr", 0x5C04C},
  {"EnableShell", 0x1},
  {"SharedIMEMBaseAddr", 0x146BF000},
  {"DloadCookieAddr", 0x01FD3000},
  {"DloadCookieValue", 0x10},
  {"MemoryCaptureModeOffset", 0x1C},
  {"AbnormalResetOccurredOffset", 0x24},
  {"DBIDumpDDRBase", 0x9FFD0000},
  {"NumCpus", 4},
  {"NumActiveCores", 8},
  {"MaxLogFileSize", 0x400000},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x00010001},
  {"PwrBtnShutdownFlag", 0x0},
  // TODO(r11): SDC1 (eMMC) pad configuration is board wiring; verify the GPIO
  // numbers against the R11 pinctrl dump before trusting storage in the kernel.
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOff", 0xA00},
  {"EnableSDHCSwitch", 0x1},
  {"PSHoldOffset", 0xC000},
  {"PSHoldSHFT", 0x0},
  {"GCCResetValueAddress", 0x146BF028},
  {"SecurityFlag", 0xC4},
  {"TzAppsRegnAddr", 0x86D00000},
  {"TzAppsRegnSize", 0x02200000},
  {"EnableLogFsSyncInRetail", 0x0},
  {"EnableSecurityHoleForSplashPartition", 0x1}
};

VOID
GetConfigurationMap (
  OUT EFI_CONFIGURATION_ENTRY_DESCRIPTOR **ConfigurationDescriptor,
  OUT UINT8                               *ConfigurationDescriptorCount
  )
{
  *ConfigurationDescriptor      = gConfigurationDescriptor;
  *ConfigurationDescriptorCount = ARRAY_SIZE (gConfigurationDescriptor);
}
