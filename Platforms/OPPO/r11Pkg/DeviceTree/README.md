# Firmware device tree for the OPPO R11

`r11.dtb` is byte-for-byte the 16051 device tree that the stock OPPO bootloader
appends to `boot.img`, extracted by `UniXNU-r11/scripts/extract-inputs.py`:

* source: `ofp_extracted/boot.img`, gzip member tail (multi-DTB block, three trees)
* selected member: `qcom,board-id = <0x8 0x0 0x3eb3>`, `qcom,msm-id = <0x13d 0x0>`
* size: 330172 bytes, sha256 `642cfb77bf5cdfedf2d14591df46eab0e8b76f91d2288c0b0aa5c519cd79ba11`

Two roles:

1. **Firmware/handoff DTB** — passed to `unixnu bootimg --dtb`. `BootShim` +
   FD are gzipped, the tree is appended, and ABL selects it by board id. It also
   carries `/chosen/linux,initrd-start|end` semantics used by `UniXNURamDiskDxe`
   to publish the payload.
2. **Reference** — panel init sequence, reserved memory, I2C layout. The readable
   dump is `UniXNU-r11/inputs/r11-stock.dts`.

Do not hand-edit this file: re-extract it instead. If a node has to be added for a
different bootloader, use `[[bootimg.dtb_node]]` in `board.toml` — but not
`/reserved-memory/splash_region`, which the stock tree already contains.

The XNU side does **not** use this tree. XNU receives the ADT generated from
`UniXNU-r11/dts/r11.dts` plus `UniXNU-sdm660-common/dts/sdm660.dtsi`.
