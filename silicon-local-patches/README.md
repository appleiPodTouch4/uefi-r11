# silicon-local-patches

对 `uefi-r11-base/Silicon/Silicium/SiliciumPkg/Sec/` 的本地改动存档（prepare-base.sh
重建 base 树时不会保留，需要手动拷回对应路径）。

## 内容

| 文件 | 改动 |
| --- | --- |
| `Sec/AArch64/ModuleEntryPoint.S` | 入口第一条指令把 x0（ABL 按 Linux boot protocol 传进来的 FDT 指针）存进 `mSecEntryArg`，随后才允许各初始化步骤把它当临时寄存器清掉 |
| `Sec/Sec.c` | `SecMain` 在 HOB 列表建立后，把 `mSecEntryArg` 用 GUID `DAEE82E6-8090-4E72-BFB9-78C13CD4E359` 发布成 entry-argument HOB，供 `UniXNURamDiskDxe` 读取 `/chosen/linux,initrd-*` |

没有这两处，`UniXNURamDiskDxe` 第一步 `GetFirstGuidHob` 就失败——payload 卷从未被
发布，菜单永远只有内置配置（"no boot entry found"）。
