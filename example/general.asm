; Code

; AZ: Dest ptr
; BZ: Src ptr
; HZ: Count
locate memcpy
    push az
    push bz
    push hz

    locate memcpy_loop
        copyb az, bz
        inc az
        inc bz
    lop memcpy_loop

    pop hz
    pop bz
    pop az
    ret