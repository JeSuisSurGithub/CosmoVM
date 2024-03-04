jmp &start

; Data
.test_str
wb str "Press space to continue...\0"

; START
.start
movri %xz $1024             ; Set stack base

movri %ez $0                ; Set text mode
out %ez $0x44               ; Send to port 0x44(set video mode)

; Print
movri %fz &test_str         ; Move pointer to string
xor %ez %ez                 ; Zero EZ
call &print

; Keyboard
movri %az $0xC17F
movri %bz $44               ; SDL Scancode for space key
out %bz $0x51               ; Send to port 0x51(key selector)

.kb_loop
in %cz $0x52                ; Get requested key state(space key)
cmpi %cz $1
jne &kb_loop                ; While space is not pressed

; Switch to graphics mode, it'll interpret outputed characters as 8-bit pixels
movri %ez $1
out %ez $0x44

; Disk read
movri %ez $0
out %ez $0x44

movri %bz $0
out %bz $0x61               ; Set read mode
out %bz $0x62               ; Read from LBA 0
out %bz $0x63               ; Request read
movri %dz &test_str         ; Re use str pointer
movri %jz $12               ; Move 12 characters
.getc_loop
in %cz $0x64                ; Get character
movbmr %dz %cz              ; Copy to buffer
inc %dz                     ; Increment buffer pointer
lop &getc_loop

movri %fz &test_str
call &print

; Disk write
movri %bz $1
out %bz $0x61               ; Set Write mode
out %bz $0x62               ; Read from LBA 1
movri %cz $10               ; Move new line (LF)
out %cz $0x64
movri %cz $0x30             ; Write 10 ascii zeros
movri %jz $10
.putc_loop
out %cz $0x64
lop &putc_loop
out %bz $0x63               ; Request write

; END
stsd                        ; Set shutdown flag

; Print function
; FZ: Pointer to string
; EZ: Video memory offset
.print
    push %az
    push %bz
    push %fz

    movri %az $0xC17F       ; Video memory address
    add %az %ez

    .print_loop
    movbrm %bz %fz          ; Move pointed character
    cmpi %bz $0             ; Check for null
    je &print_exit          ; Exit if null
    movbmr %az %bz
    inc %fz
    inc %az
    jmp &print_loop

    .print_exit
    pop %fz
    pop %bz
    pop %az
    ret
