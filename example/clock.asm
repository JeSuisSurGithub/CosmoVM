; Data
locate int_to_str_buf
    str /00000/
    u8 0

locate print_datetime_buf
    str /0000/00/00 00:00:00/
    u8 0

; Code

; Converts integer to string
; AZ: argument integer to convert
; BZ: return ptr to int_to_str_buf
; CZ: return character count
locate int_to_str
    push az
    push hz
    push gz

    ; Clear buffer
    ; BZ: Iterator
    ; HZ: Loop count
    movi bz, int_to_str_buf
    movi hz, 5
    locate int_to_str_clear_loop
        storbi bz, 0
        inc bz
    lop int_to_str_clear_loop

    ; Convert
    ; BZ: Reverse iterator
    ; CZ: Digit count
    movi bz, int_to_str_buf
    addi bz, 4
    movi cz, 0

    locate int_to_str_loop
        ; Check if there are digits left
        cmpi az, 0
        je int_to_str_exit

        divi az, 10
        mov az, gz

        addi hz, 0x30
        storb bz, hz

        dec bz
        inc cz
    jmp int_to_str_loop

    locate int_to_str_exit
        ; Rearrange string

        ; Shift all character
        ; BZ: Copy ptr
        ; DZ: Ptr to start
        ; HZ: Character count
        movi az, int_to_str_buf
        movi bz, int_to_str_buf
        addi bz, 5
        sub bz, cz

        mov hz, cz
        call memcpy

        ; Set null at end
        ; BZ: Ptr after converted characters
        ; HZ: Character count left to null
        movi bz, int_to_str_buf
        add bz, cz

        movi hz, 5
        sub hz, cz
        locate int_to_str_clean2
            storbi bz, 0
            inc bz
        lop int_to_str_clean2

        ; Setup return
        movi bz, int_to_str_buf

        pop gz
        pop hz
        pop az
        ret

; Prints datetime
; EZ: Offset print
locate print_datetime
    push az
    push dz
    push hz

    ; Convert date
    in az, 0x31
    call int_to_str

    movi az, print_datetime_buf
    mov hz, cz
    call memcpy

    ; Convert month
    in az, 0x32
    call int_to_str

    movi az, print_datetime_buf
    addi az, 5
    ; Shift if only one digit
        movi dz, 2
        sub dz, cz
        add az, dz
    mov hz, cz
    call memcpy

    ; Convert day
    in az, 0x33
    call int_to_str

    movi az, print_datetime_buf
    addi az, 8
    ; Shift if only one digit
        movi dz, 2
        sub dz, cz
        add az, dz
    mov hz, cz
    call memcpy

    ; Convert hour
    in az, 0x34
    call int_to_str

    movi az, print_datetime_buf
    addi az, 11
    ; Shift if only one digit
        movi dz, 2
        sub dz, cz
        add az, dz
    mov hz, cz
    call memcpy

    ; Convert minute
    in az, 0x35
    call int_to_str

    movi az, print_datetime_buf
    addi az, 14
    ; Shift if only one digit
        movi dz, 2
        sub dz, cz
        add az, dz
    mov hz, cz
    call memcpy

    ; Convert second
    in az, 0x36
    call int_to_str

    movi az, print_datetime_buf
    addi az, 17
    ; Shift if only one digit
        movi dz, 2
        sub dz, cz
        add az, dz
    mov hz, cz
    call memcpy

    ; Print
    movi az, print_datetime_buf
    mov bz, ez
    call print

    pop hz
    pop dz
    pop az
    ret