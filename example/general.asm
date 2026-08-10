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
        addi az, 1
        addi bz, 1
    lop memcpy_loop

    pop hz
    pop bz
    pop az
    ret