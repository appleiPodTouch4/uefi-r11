_Head:
  adr   x1, _Payload    /* Set _Entry address */
  b     _Start          /* Jump to the real code */

#if REQUIRES_KERNEL_HEADER == 1
_StackBase:
  .quad FD_BASE         /* Text Offset */

_StackSize:
  /* The ARM64 header's image_size covers the whole image: shim plus firmware
   * volume. Upstream writes FD_SIZE only, which makes "unixnu bootimg" reject
   * the payload ("the ARM64 image header does not describe the shim plus the
   * firmware volume"). This is the only change against Mu-Silicium. */
  .quad FD_SIZE + (_Payload - _Head)  /* Image Size */
  .quad 0               /* Flags */
  .quad 0               /* Reserved */
  .quad 0               /* Reserved */
  .quad 0               /* Reserved */
  .ascii "ARM\x64"      /* ARM64 Magic */
  .long 0               /* Reserved */
#endif

_Start:
  mov   x4, x1
  ldr   x5, =FD_BASE
  cmp   x4, x5
  beq   _Entry
  ldr   x6,  =FD_SIZE

_CopyLoop:
  ldp   x2, x3, [x4], #16
  stp   x2, x3, [x5], #16
  subs  x6, x6, #16
  b.ne  _CopyLoop
  ldr   x5, =FD_BASE

_Entry:
  br    x5

_Dead:
  b     _Dead

.text
.align 4

_Payload:
