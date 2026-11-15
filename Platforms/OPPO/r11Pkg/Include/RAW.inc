##
#  OPPO R11 (r11) raw configuration files embedded in the firmware volume.
#
#  Provenance: the SDM660 UEFI reference set (Binaries/r11/RawFiles). Each file
#  is board configuration, not SoC code, so treat them as inputs to regenerate:
#    SecParti.cfg        secure partition memory layout, must match the stock XBL
#    tracer_event.cfg    QDSS trace events
#    QcomChargerCfg.cfg  charger profile (R11: OPPO VOOC over a pic16f/stm8s MCU)
#    BATTERY.PROVISION   battery profile (R11: BLP-635 3000 mAh)
#  See README.md, "Vendor binaries".
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

  FILE FREEFORM = A1E235DE-E825-4591-9623-C43175811826 {
    SECTION RAW = Binaries/r11/RawFiles/SecParti.cfg
    SECTION UI = "SecParti.cfg"
  }

  FILE FREEFORM = F12A1F3D-4318-A006-5F7F-3FAAF6DBB4B5 {
    SECTION RAW = Binaries/r11/RawFiles/tracer_event.cfg
    SECTION UI = "tracer_event.cfg"
  }

  FILE FREEFORM = 45FE4B7C-150C-45DA-A021-4BEB2048EC6F {
    SECTION RAW = Binaries/r11/RawFiles/QcomChargerCfg.cfg
    SECTION UI = "QcomChargerCfg.cfg"
  }

  FILE FREEFORM = F780C779-DD7C-47CD-BD1A-5EB414C51704 {
    SECTION RAW = Binaries/r11/RawFiles/BATTERY.PROVISION
    SECTION UI = "BATTERY.PROVISION"
  }
