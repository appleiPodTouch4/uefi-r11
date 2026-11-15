# uefi-r11 — UniXNU UEFI firmware for the OPPO R11 (SDM660)

A Mu-Silicium platform package for the OPPO R11 (CPH1707 / 16051) plus the pieces
UniXNU needs in the firmware volume: the boot manager, its boot-option driver,
the payload RAM-disk driver and the DarwinBoot XNU loader.

This directory is an **overlay**, not a full firmware tree. `scripts/prepare-base.sh`
clones Mu-Silicium and Mu_Basecore, then copies `Platforms/OPPO/r11Pkg`,
`Resources/Configs/r11.toml`, `Resources/Scripts/mkbootimg.py` and
`BootShim/BootShim.S` into the checkout. `./unixnu build firmware --uefi <checkout>`
builds it.

```
uefi-r11/
  BootShim/BootShim.S                      upstream Mu-Silicium shim (BSD-2-Clause-Patent)
  Resources/Configs/r11.toml               FD base/size, boot image geometry
  Resources/Scripts/mkbootimg.py           AOSP boot image packer (Apache-2.0)
  Platforms/OPPO/r11Pkg/
    r11.dsc                                platform description and PCDs
    r11.fdf                                FD/FV layout, UniXNU modules
    r11.dec
    Include/{APRIORI,DXE,RAW}.inc          driver list and dispatch order
    Library/MemoryMapLib/                  DRAM and register map (XBL-verified)
    Library/ConfigurationMapLib/           SDM660 vendor driver configuration
    DeviceTree/r11.dtb                     firmware DTB (16051 tree from the stock boot.img)
  scripts/prepare-base.sh                  assemble the build tree
```

## What is verified

| Claim | Evidence |
| --- | --- |
| FD window is `0x9FC00000` + `0x300000` | the stock `xbl.elf` contains `0x9FC00000, 0x00300000, "UEFI FD", AddMem` in its own platform configuration, plus `0x94000000, 0x09400000, "DXE Heap"` and `0x9FF90000, 0x00040000, "UEFI Stack"` |
| Boot image geometry | stock `boot.img` header: version 0, page 4096, kernel `0x8000`, ramdisk `0x01000000`, tags `0x100`, second `0x00f00000` |
| Splash buffer | stock device tree `/reserved-memory/splash_region` = `0x9D400000` + `0x02400000`, identical to the `Display Reserved` region the SDM660 memory map expects |
| GIC / timer | GICv3 `0x17A00000` + redistributors `0x17B00000`, 19.2 MHz arch timer, PSCI over SMC |
| Panel and touch identity | `oppo16051samsung s6e3fa3 1080p cmd mode dsi panel`, Synaptics `0x20` at `0x0C175000` |

## Display strategy

`USE_CUSTOM_DISPLAY_DRIVER = 0`, so the firmware uses `SimpleFbDxe`, which finds
its framebuffer by the `Display Reserved` memory-map entry. That is the buffer
the vendor bootloader already owns and draws the OPPO logo into, so the UniXNU
menu appears over the logo without any MDSS or DSI code. It works only while the
bootloader leaves the panel powered; a real panel driver (S6E3FA3, command mode,
init sequence in the stock device tree) is required for the kernel console and
for backlight control. `USE_CUSTOM_DISPLAY_DRIVER = 1` switches to the vendor
`DisplayDxe`, which needs that R11 panel driver to be useful.

## Vendor binaries

`Binaries/r11/` is seeded from another SDM660 UEFI device (`Binaries/lavender`,
Redmi Note 7) because the R11's own `xbl.elf` is a single 2017 UEFI 4.1 image
with no separable DXE modules. `prepare-base.sh` writes `PROVENANCE.txt` there.

| Group | Members | Expectation |
| --- | --- | --- |
| SoC generic | `ClockDxe`, `TLMMDxe`, `I2CDxe`, `SPMIDxe`, `PmicDxeLa`, `SdccDxe`, `SmemDxe`, `ChipInfoDxe`, `NpaDxe`, `DALSYSDxe` | same silicon, same LA configuration: expected to work |
| Board config | `QcomChargerCfg.cfg`, `BATTERY.PROVISION`, `ButtonsDxe` GPIOs | must be replaced with R11 values (VOOC charger MCU, BLP-635 battery) |
| Board code | `DisplayDxe` | not used; panel differs |

Treat any behaviour that depends on a board config file as unverified until it is
measured on the device.

## Build

```sh
./scripts/prepare-base.sh                       # -> ../uefi-r11-base, ../Mu_Basecore
cd ../UniXNU
./unixnu defconfig r11_defconfig -D ../UniXNU-r11
./unixnu build firmware -D ../UniXNU-r11 --uefi ../uefi-r11-base --edk2 ../Mu_Basecore
```

The driver runs `scripts/check-r11.py` first (FD base/size consistency, UniXNU
modules in DSC **and** FDF, FAT driver present, vendor binary directory), then
EDK II `build` with `AARCH64 CLANGPDB` and `-D FD_BASE -D FD_SIZE -D FD_BLOCKS`.
The output must be exactly `0x300000` bytes:

```
../uefi-r11-base/Build/r11Pkg/DEBUG_CLANGPDB/FV/R11_UEFI.fd
```

## Packing the boot image

```sh
cd ../UniXNU
./unixnu bootimg -D ../UniXNU-r11 --uefi ../uefi-r11-base \
  --dtb ../uefi-r11-base/Platforms/OPPO/r11Pkg/DeviceTree/r11.dtb
```

`bootimg` compiles `BootShim.S` with the FD base/size, gzips shim + FD, appends
the device tree and packs the Android boot image with the stock geometry, then
unpacks it again and compares every part.

## First boot checklist

1. `fastboot getvar unlocked` reports `yes` (the R11 must already be unlocked).
2. `fastboot boot boot.img` — no flash, so nothing is lost if the firmware stops.
3. The UniXNU menu appears over the OPPO logo. No menu means the payload RAM
   disk was not published: check that the FDF embeds `UniXNURamDiskDxe`, `DiskIoDxe`
   and `Fat`, and that the boot image carries the payload in its ramdisk field.
4. Selecting the XNU entry hands over to DarwinBoot. The green bar or a stall at
   handoff is recorded with the framebuffer checkpoint.

## Not in this tree yet

* R11 panel driver (MDSS/DSI, S6E3FA3 command mode) and backlight.
* R11 charger and battery profiles for the vendor drivers.
* Advanced Logger and the Tools menu entry for log export without a UART.
* ACPI tables — not needed: UniXNU hands XNU an ADT, and nothing else consumes ACPI
  on this target.
