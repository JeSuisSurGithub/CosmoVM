jmp start

; Current memory layout
; 0x0000 -> 0x01FF Stage1
; 0x0200 -> 0x05FF Stage2
; 0x0600 -> 0xAFFF Free
; 0xB000 -> 0xB4FF Stack
; 0xB500 -> 0xFFFF Video

locate boot_info
    str /Booted at: /
    u8 0

locate video_mem_size
    u16 0x4B00

locate video_mem_addr
    u16 0xB500

locate start
    movi az, boot_info
    movi bz, 80
    call print

    ; EZ Contains number of characters printed
    add ez, bz
    call print_datetime

    call wait_enter_pressed

    ; Switch to graphics mode
    movi ez, 1
    out ez, 0x44

    movi hz, 320
    loadi az, video_mem_addr
    sub az, mo
    locate video_copy_loop
        storbi az, 0xFF
        inc az
    lop video_copy_loop

    call wait_enter_pressed

    ; Write some random data to the disk
    movi bz, 1
    out bz, 0x61                        ; Set Write mode
    movi bz, 3
    out bz, 0x62                        ; Write to LBA 3

    ; Write 16 ascii zeros
    movi cz, 0x30
    movi hz, 16
    locate fputc_loop
        out cz, 0x64
    lop fputc_loop

    ; Flush write buffer
    out bz, 0x63

    ; Shutdown
    stxp                                ; Easier debug
    stsd