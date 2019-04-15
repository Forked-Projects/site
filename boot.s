# it's just training loader on pure asm that simulates shall for fun.
# working commands are time, ls, iam and cat file.txt (it's not really cat =)
# for making it bootable everybody can do next steps.
# as -o boot.o boot.s -- builds object file.
# ld -o boot.bin --oformat binary -Ttext 0x7c00 boot.o -- makes binary block
# of 512 bytes that can be written for example on usb flash using dd so make
# usb flash able to boot due to the last bytes of block equal to 0x55aa

.code16
.text
  .global _start
  
_start:

shell:
  movb $0x0e, %ah
  movb $0x0a, %al  # new line
  int  $0x10       # and
  movb $0x0d, %al  # carriage return
  int  $0x10
  movb $0x24, %al  # print $ character
  int  $0x10
  movb $0x20, %al  # print space too
  int  $0x10
read_chars:        # main reading of printed string to the stack
  movb $0x00, %ah  # read pressed char
  int  $0x16
  push %ax
  movb $0x0e, %ah  # print pressed char for interactivity
  int  $0x10
  cmp  $0x0d, %al  # call echo when Enter was pressed
  je   router
  jmp  read_chars

router:
  pop  %ax
  pop  %ax
  cmp  $0x65, %al  # check if last letter in shell was e
  je   time_check
  cmp  $0x73, %al  # check if last letter in shell was s
  je   ls_check
  cmp  $0x6d, %al  # check if last letter in shell was m
  je   iam_check
  cmp  $0x74, %al  # check if last letter in shell was t
  je   txt_check
time_check:
  pop  %ax
  cmp  $0x6d, %al  # check if last letter before e was m
  je   time_check2
time_check2:
  pop  %ax
  cmp  $0x69, %al  # check if last letter before m was i
  je   time_check3
time_check3:
  pop  %ax
  cmp  $0x74, %al  # check if last letter before i was t
  je   time
ls_check:
  pop  %ax
  cmp  $0x6c, %al  # check if last letter before s was l
  je   ls
iam_check:
  pop  %ax
  cmp  $0x61, %al  # check if last letter before m was a
  je   iam_check2
iam_check2:
  pop  %ax
  cmp  $0x69, %al  # check if last letter before a was i
  je   iam
txt_check:
  pop  %ax
  cmp  $0x78, %al  # check if last letter before t was x
  je   txt_check2
txt_check2:
  pop  %ax
  cmp  $0x74, %al  # check if last letter before x was t
  je   txt
jmp  shell

time:
  movb $0x0e, %ah
  movb $0x0a, %al     # new line
  int  $0x10          # and
  movb $0x0d, %al     # carriage return
  int  $0x10
  clc
  movb $0x02, %ah     # read RTC clock
  int  $0x1a
  
  movb $0x0e, %ah
  push %cx
  shrb $4,    %ch
  add  $0x30, %ch
  movb %ch,   %al     # print first digit of hours in decimal
  int  $0x10
  pop  %cx

  push %cx
  shlb $4,    %ch
  shrb $4,    %ch
  add  $0x30, %ch
  movb %ch,   %al   # second digit of hours
  int  $0x10
  pop  %cx

  movb $0x3a, %al   # print colon between hours and minutes
  int  $0x10

  push %cx
  shrb $4,    %cl
  add  $0x30, %cl
  movb %cl,   %al   # print minutes
  int  $0x10
  pop  %cx

  push %cx
  shlb $4,    %cl
  shrb $4,    %cl
  add  $0x30, %cl
  movb %cl,   %al
  int  $0x10
  pop  %cx

  jmp  shell

ls:
  xor  %eax, %eax
  movb $0x0e, %ah
  movb $0x0a, %al  # new line
  int  $0x10       # and
  movb $0x0d, %al  # carriage return
  int  $0x10
  mov  $ls_msg,  %si
  movb $0x0e, %ah
ls_char:
  lodsb
  cmp  $0x00, %al
  je   shell
  int  $0x10
  jmp  ls_char

ls_msg:
  .asciz "/bin  /dev  /etc  /lib  /usr  /kernel"

iam:
  xor  %eax, %eax
  movb $0x0e, %ah
  movb $0x0a, %al  # new line
  int  $0x10       # and
  movb $0x0d, %al  # carriage return
  int  $0x10
  mov  $iam_msg,  %si
  movb $0x0e, %ah
iam_char:
  lodsb
  cmp  $0x00, %al
  je   shell
  int  $0x10
  jmp  iam_char

iam_msg:
  .asciz "LosharaOS x86 0.01 beta. Copyright by Valentine Astakhov"

txt:
  xor  %eax, %eax
  movb $0x0e, %ah
  movb $0x0a, %al  # new line
  int  $0x10       # and
  movb $0x0d, %al  # carriage return
  int  $0x10
  mov  $txt_msg,  %si
  movb $0x0e, %ah
txt_char:
  lodsb
  cmp  $0x00, %al
  je   shell
  int  $0x10
  jmp  txt_char

txt_msg:
  .asciz "don't believe your eyes"

.fill 510-(.-_start), 1, 0

.byte 0x55
.byte 0xaa
