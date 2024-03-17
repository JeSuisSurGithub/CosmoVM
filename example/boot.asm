jmp start

; Data
locate prompt
    str /Press enter to boot.../
    u8 0

locate stack_base
    u16 0xB000

; Code
locate start
    ; Setup stack
    loadi sb, stack_base

    ; Set text mode
    call clear_text_mode
    movi az, 0
    out az, 0x44

    ; Print message
    movi az, prompt                       ; Move pointer to string
    movi bz, 0                            ; 0 offset

    call print

    call wait_enter_pressed

    ; Load sectors 1 and 2
    movi az, 1
    movi bz, 0x200
    call sector_load

    movi az, 2
    movi bz, 0x400
    call sector_load

    ; Go stage 2
    pushi 0x0200                        ; Set MO to 0x200
    pushi 0x0000                        ; Set XA to 0x0
    ret