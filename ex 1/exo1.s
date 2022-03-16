
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
    push %b
    call get_last_arg
    push 0 // p
    push 0 // i
    jmp prodscal_for
prodscal_for:
    ld [%sp], %b
    cmp %b, %a
    jge prodscal_end
    add 3, %sp
    add %b, %sp
    add %a, %b
    ld [%sp+1], %a
    push %b
    add 1, %sp
    add %a, %sp
    ld [%sp+1], %b
    push %b
    add 1, %sp
    call get_last_arg
    sub %b, %sp
    sub 3, %sp
    pop %b
    pop %a
    mul %b, %a
    pop %b
    sub %b, %sp
    add 1, %b
    push %b
    ld [%sp+1], %b
    add %b, %a
    st %a, [%sp+1]
    jmp prodscal_for
prodscal_end:
    add 1, %sp
    pop %a
    pop %b
    rtn


get_last_arg:
    // get the last argument of a function
    push %b
    ld 9998, %a
    sub %sp, %b
    add %b, %sp
    ld [%sp+1], %a
    sub %b, %sp
    pop %b
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
    // 1st while condition
    ld [%sp+5], %a
    ld [%sp], %b
    mul %b, %b
    cmp %b, %a
    jle racine_inwhile
    jmp racine_if
racine_inwhile:
    // 2nd while condition
    ld [%sp], %b
    add 1, %b
    mul %b, %b
    cmp %b, %a
    jle racine_if
    jmp racine_end
racine_if:
    // if r*r > n
    ld [%sp], %b
    mul %b, %b
    ld [%sp+5], %a
    cmp %b, %a
    jle racine_else
    ld [%sp], %b
    st %b, [%sp+1]
    ld [%sp+1], %a
    ld [%sp+2], %b
    sub %b, %a
    div 2, %a
    add %b, %a
    st %a, [%sp]
    jmp racine_loop
racine_else:
    // if (r+1)*(r+1) <= n
    ld [%sp], %b
    st %b, [%sp+2]
    ld [%sp+1], %a
    sub %b, %a
    div 2, %a
    add %b, %a
    st %a, [%sp]
    jmp racine_loop
racine_end:
    pop %a
    add 2, %sp
    pop %b
    rtn


main_calcul:
    push 2
    push 2
    call calcul
    reset


main_prodscal:
    push 3
    push 1
    push 2
    push 3
    push 1
    push 2
    push 3
    call prodscal
    reset


main_racine:
    push 122
    call racine
    reset
