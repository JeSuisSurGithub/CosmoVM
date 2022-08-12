jmp &start

; Data
.test_str
wb str "Text mode displaying for 3 seconds\0"

; START
.start
movri %xz $1024             ; Set stack base

movri %ez $0                ; Set text mode
out %ez $0x44               ; Send to port 0x44(set video mode)

; Keyboard
movri %az $0xC17F
movri %bz $44               ; SDL Scancode for space key
out %bz $0x51               ; Send to port 0x51(key selector)

.kb_loop
in %cz $0x52                ; Get requested key state(space key)
cmpi %cz $1                 ; If not pressed jump to putN
jne &putN

.putY
movbmi %az $0x59            ; Ascii 'Y'
jmp &kb_loop_exit           ; Exit loop

.putN
movbmi %az $0x4E            ; Ascii 'N'
jmp &kb_loop                ; While space is not pressed

.kb_loop_exit

; Print
movri %fz &test_str         ; Move pointer to string
xor %ez %ez                 ; Zero EZ
call &print

; Wait cycles
movri %jz $90               ; Wait 180 cycles, wcyl + lop -> 2 cycles
.wait
wcyl                        ; Do nothing
lop &wait

; Switch to graphics mode, it'll interpret outputed characters as 8bit pixels
movri %ez $1
out %ez $0x44

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