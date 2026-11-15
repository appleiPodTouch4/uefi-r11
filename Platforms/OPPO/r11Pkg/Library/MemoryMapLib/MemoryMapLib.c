#include <Library/MemoryMapLib.h>

//
// OPPO R11 (r11, SDM660 + PM660 + PM660L) UEFI memory map.
//
// The DRAM windows are the ones the stock XBL declares in its own platform
// configuration (strings found in ofp_extracted/xbl.elf):
//
//   "0x94000000, 0x09400000, \"DXE Heap\", AddMem, SYS_MEM, ..."
//   "0x9FC00000, 0x00300000, \"UEFI FD\",  AddMem, SYS_MEM, ..."
//   "0x9FF90000, 0x00040000, \"UEFI Stack\",AddMem, SYS_MEM, ..."
//
// and the device tree of the same package, which reserves the continuous
// splash buffer at 0x9D400000 with length 0x2400000
// (/reserved-memory/splash_region in UniXNU-r11/inputs/r11-stock.dts).
//
// Do not move "Display Reserved": SimpleFbDxe finds the framebuffer by this
// exact name, and ABL keeps drawing the boot logo there.
//
STATIC
EFI_MEMORY_REGION_DESCRIPTOR
gMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // DDR regions
  {"Kernel",             0x80000000, 0x05800000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"SMEM",               0x86000000, 0x00200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  //
  // The stock XBL declares DXE Heap as 0x94000000 + 0x09400000, which runs into
  // the splash carve-out at 0x9D400000. Shorten it to end at 0x9D300000 and use
  // the last MiB for the Advanced Logger memory log: r11.dsc sets
  // PcdAdvancedLoggerBase to 0x9D300000 with 0x100 pages. Keeping the log here
  // rather than in the "Log Buffer" entry below matters, because that 32 KiB
  // window belongs to the vendor ABL logger and is in its own format.
  //
  {"DXE Heap",           0x94000000, 0x09300000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"AdvLogger",          0x9D300000, 0x00100000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"Display Reserved",   0x9D400000, 0x02400000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},
  {"FV Region",          0x9F800000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"ABOOT FV",           0x9FA00000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"UEFI FD",            0x9FC00000, 0x00300000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"SEC Heap",           0x9FF00000, 0x0008C000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"MMU PageTables",     0x9FF8D000, 0x00003000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"UEFI Stack",         0x9FF90000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"DBI Dump",           0x9FFD0000, 0x00027000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},
  {"Log Buffer",         0x9FFF7000, 0x00008000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},
  {"Info Blk",           0x9FFFF000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},

  // Other memory regions
  {"IMEM Base",          0x14680000, 0x00040000, NoHob,  MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},
  {"IMEM Cookie Base",   0x146BF000, 0x00001000, AddDev, MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},
  {"QDSS_STM",           0x16000000, 0x01000000, AddDev, MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},

  // Register regions
  {"BOOT_CONFIG",        0x00070000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GCC CLK CTL",        0x00100000, 0x000B0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"RPM MSG RAM",        0x00778000, 0x00008000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SECURITY CONTROL",   0x00780000, 0x00007000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PRNG_CFG_PRNG",      0x00790000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MPM2_SLP_CNTR",      0x010A3000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MPM2_TSENS0",        0x010AA000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MPM2_TSENS0_TM",     0x010AB000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MPM2_PSHOLD",        0x010AC000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MPM2_TSENS1",        0x010AD000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MPM2_TSENS1_TM",     0x010AE000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PCIE WRAPPER AHB",   0x01C00000, 0x00007000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"CRYPTO0 CRYPTO",     0x01DC0000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"TCSR_TCSR_REGS",     0x01FC0000, 0x00026000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  // TLMM: the R11 device tree maps pinctrl@03000000 with length 0xc00000.
  {"TLMM CSR",           0x03000000, 0x00C00000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GPUCC",              0x05065000, 0x00009000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"QDSS_QDSS",          0x06000000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"HMSS_QLL",           0x06400000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PMIC ARB SPMI",      0x08000000, 0x02800000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"USB30_PRIM",         0x0A800000, 0x0011B000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PERIPH_SS",          0x0C000000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MMSS",               0x0C800000, 0x00800000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"QDSS",               0x15060000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"APCS_CC",            0x17800000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"QTIMER",             0x17900000, 0x00030000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"APCS_GIC500_GICD",   0x17A00000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"APCS_GIC500_GICR",   0x17B00000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}
};

VOID
GetMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR **MemoryDescriptor,
  OUT UINT8                         *MemoryDescriptorCount
  )
{
  *MemoryDescriptor      = gMemoryDescriptor;
  *MemoryDescriptorCount = ARRAY_SIZE (gMemoryDescriptor);
}
