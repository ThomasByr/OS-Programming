
calcul:
    // x*x - 2*x*y - y*y
    push %b

    ld [%sp+2], %a // x
    ld [%sp+3], %b // y

    mul 2, %a
    mul %b, %a     // 2xy

    ld [%sp+2], %b // x
    mul %b, %b     // xx
    sub %a, %b     // xx - 2xy
    swap %a, %b

    ld [%sp+3], %b // y
    mul %b, %b     // yy
    sub %b, %a     // xx - 2xy - yy

    pop %b
    rtn


prodscal:
    // scallar product between two vectors
    rtn


racine:
    // square root of a number by dichotomy
    push %b
    push 1
    ld [%sp+3], %b
    push %b

    ld [%sp], %a
    sub 1, %a
    div 2, %a
    add 1, %a
    push %a

    jmp racine_loop
racine_loop:
    ld [%sp+5], %a
    ld [%sp+1], %b
    mul %b, %b
    cmp %b, %a
    jle racine_inwhile
    jmp racine_cond
racine_inwhile:
    ld [%sp+1], %b
    mul %b, %b
    cmp %b, %a
    jgt racine_else

    ld [%sp+1], %b
    st %b, [%sp+2]
    jmp racine_loop
racine_cond:
    ld [%sp+1], %b
    add 1, %b
    mul %b, %b
    cmp %b, %a
    jle racine_loop
    jmp racine_end
racine_else:
    ld [%sp+1], %b
    st %b, [%sp+3]
    ld [%sp+2], %a
    sub %b, %a
    div 2, %a
    add %b, %a
    st %a, [%sp+1]
    jmp racine_loop
racine_end:
    pop %a
    pop %b
    pop %b
    pop %b
    rtn


main_calcul:
    push 2
    push 2
    call calcul
    reset


main_prodscal:
    reset


main_racine:
    push 10
    call racine
    reset
