; Data

locate video_memory
    u16 0xB500

; Code

; Clears text buffer
; No args
locate clear_text_mode
    push az
    push bz
    push hz

    loadi az, video_memory
    movi bz, 0x20
    movi hz, 80
    muli hz, 25
    locate clear_text_mode_loop
        storb az, bz
        inc az
    lop clear_text_mode_loop

    pop hz
    pop bz
    pop az
    ret

; Print function
; AZ: Pointer to string
; BZ: Video memory offset
; EZ: Number of character outputted
locate print
    push az
    push bz
    push cz
    push dz

    loadi cz, video_memory      ; Video memory address
    add cz, bz
    sub cz, mo                  ; We want absolute address

    movi ez, 0

    locate print_loop
        loadb dz, az            ; Move pointed character
        cmpi dz, 0              ; Check for null
        je print_exit           ; Exit if null

        storb cz, dz
        inc cz
        inc az
        inc ez
    jmp print_loop

    locate print_exit
        pop dz
        pop cz
        pop bz
        pop az
        ret

; Returns when enter key is pressed
; No args
locate wait_enter_pressed
    push az

    ; Wait for input
    locate kb_loop
        in az, 0x53                     ; Get key pressed
        cmpi az, 40                     ; Check if enter key
    jne kb_loop

    pop az
    ret