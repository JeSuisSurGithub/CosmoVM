; Code

; Loads a full sector into memory
; AZ: LBA Index
; BZ: Dest ptr
locate sector_load
    push az
    push bz
    push cz
    push hz

    movi cz, 0
    out cz, 0x61                        ; Set read mode

    mov cz, az
    out cz, 0x62                        ; Read from specified LBA

    movi cz, 0
    out cz, 0x63                        ; Request read

    ; Copy disk read buffer to memory
    movi hz, 0x200                      ; Copy 512 bytes
    locate sector_load_loop
        in cz, 0x64                     ; Get character
        storb bz, cz                    ; Copy to buffer
        inc bz,                         ; Increment pointer
    lop sector_load_loop

    pop hz
    pop cz
    pop bz
    pop az
    ret