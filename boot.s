.text
  .global _start
  
_start:

shell:
  mov  %esp, %ebx  # first save stack pointer to know where to stop echo
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
  je   echo
jmp  read_chars

echo:
  add  $4,   %esp  # do not count carriage return character
  movb $0x0e, %ah
  movb $0x0a, %al  # new line
  int  $0x10       # and
  movb $0x0d, %al  # carriage return
  int  $0x10
echo_char:
  pop  %ax
  movb $0x0e, %ah
  int  $0x10
  cmp  %ebx, %esp
  je   shell
jmp echo_char

date:
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

# newline


.fill 510-(.-_start), 1, 0

.byte 0x55
.byte 0xaa
