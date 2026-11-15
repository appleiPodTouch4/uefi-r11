#!/bin/sh
# Assemble a Mu-Silicium checkout that contains the OPPO R11 platform package.
#
#   ./scripts/prepare-base.sh [WORKDIR]
#
# WORKDIR defaults to ../uefi-r11-base (next to this repository). The result is a
# complete Mu-Silicium tree plus:
#   Platforms/OPPO/r11Pkg          this repository's platform package
#   Resources/Configs/r11.toml     FD base/size and boot image geometry
#   Resources/Scripts/mkbootimg.py Android boot image packer used by "unixnu bootimg"
#   BootShim/BootShim.S            entry shim compiled per FD base/size
#   Binaries/r11                   SDM660 vendor DXE binaries (see README)
#
# The script is idempotent: re-running it updates the Python-free parts of the
# checkout and re-copies the overlay.
set -eu

here=$(cd "$(dirname "$0")/.." && pwd)
work=${1:-$(dirname "$here")/uefi-r11-base}
mu=https://github.com/Project-Silicium/Mu-Silicium.git
edk2=$(dirname "$work")/Mu_Basecore

say() { printf '%s\n' "$*" >&2; }

need() {
  command -v "$1" >/dev/null 2>&1 || { say "missing tool: $1 ($2)"; exit 1; }
}

need git "install the Xcode command line tools"
need python3 "python3 is required by EDK II BaseTools"
need make "install the Xcode command line tools"
command -v clang >/dev/null 2>&1 || { say "missing clang: install the Xcode command line tools"; exit 1; }

if [ ! -d "$work/.git" ]; then
  say "cloning Mu-Silicium into $work"
  git clone --depth 1 "$mu" "$work"
else
  say "reusing $work"
fi

say "initialising submodules (Mu, Mu_OEM_Sample, Binaries)"
git -C "$work" submodule update --init --depth 1 -- Common/Mu Common/Mu_OEM_Sample Binaries

if [ ! -d "$edk2/.git" ]; then
  say "cloning Mu_Basecore into $edk2 (the --edk2 input of the firmware step)"
  git clone --depth 1 https://github.com/microsoft/mu_basecore.git "$edk2"
fi

say "copying the r11 overlay"
mkdir -p "$work/Platforms/OPPO"
rm -rf "$work/Platforms/OPPO/r11Pkg"
cp -R "$here/Platforms/OPPO/r11Pkg" "$work/Platforms/OPPO/r11Pkg"
mkdir -p "$work/Resources/Configs" "$work/Resources/Scripts" "$work/BootShim"
cp "$here/Resources/Configs/r11.toml" "$work/Resources/Configs/r11.toml"
cp "$here/Resources/Scripts/mkbootimg.py" "$work/Resources/Scripts/mkbootimg.py"
cp "$here/BootShim/BootShim.S" "$work/BootShim/BootShim.S"

# Vendor DXE binaries. The R11's own XBL is a single 2017 UEFI 4.1 image, so the
# SDM660 driver set is taken from the closest UEFI device in the same platform
# family. Every file here is board configuration as much as SoC code: see
# README.md, "Vendor binaries", before trusting storage, display or charging.
if [ ! -d "$work/Binaries/r11" ]; then
  # Prefer the SDM660 reference device (lavender, Redmi Note 7); otherwise the
  # first directory that actually carries the platform DXE drivers.
  src=""
  for candidate in "$work/Binaries/lavender" "$work"/Binaries/*; do
    [ -d "$candidate/QcomPkg/Drivers/ClockDxe" ] && { src="$candidate"; break; }
  done
  if [ -n "$src" ]; then
    say "seeding Binaries/r11 from $(basename "$src")"
    cp -R "$src" "$work/Binaries/r11"
    cat > "$work/Binaries/r11/PROVENANCE.txt" <<EOF
Seeded from $(basename "$src") by uefi-r11/scripts/prepare-base.sh.

These are Qualcomm SDM660 vendor DXE drivers taken from another UEFI device in
the same platform family. SoC-generic drivers (Clock, TLMM, I2C, SPMI, PMIC,
SDCC, SMEM, ChipInfo, NPA, DALSYS) are expected to work unchanged.

Replace with R11 values before relying on them:
  RawFiles/QcomChargerCfg.cfg   OPPO VOOC charger profile
  RawFiles/BATTERY.PROVISION    BLP-635 battery profile
  QcomPkg/Drivers/ButtonsDxe    R11 key GPIOs
  QcomPkg/Drivers/DisplayDxe    R11 panel (S6E3FA3); unused while
                                USE_CUSTOM_DISPLAY_DRIVER = 0
EOF
  else
    say "warning: no Binaries/<device>/QcomPkg found; the DXE list will not resolve"
  fi
fi

cat <<EOF

prepared $work

next:
  cd $(dirname "$here")/UniXNU
  ./unixnu defconfig r11_defconfig -D ../UniXNU-r11
  ./unixnu build firmware -D ../UniXNU-r11 --uefi ../uefi-r11-base --edk2 ../Mu_Basecore

EOF
