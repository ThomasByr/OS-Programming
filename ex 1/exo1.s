//! ./z33-cli run {__file__} {__function__} -{__o__} --{__option__}

calcul:
    // x*x - 2*x*y - y*y
    // [b, rtn, x, y]
    push %b

    ld [%sp+2], %a           // x
    ld [%sp+3], %b           // y

    mul 2, %a
    mul %b, %a               // 2*x*y

    ld [%sp+2], %b           // x
    mul %b, %b               // x*x
    sub %a, %b               // x*x - 2*x*y
    swap %a, %b

    ld [%sp+3], %b           // y
    mul %b, %b               // y*y
    sub %b, %a               // x*x - 2*x*y - y*y

    pop %b
    rtn


prodscal:
    // scallar product between two vectors
    // [i, p, b, rtn, v1, v2, n]
    push %b
    push 0                   // p
    push 0                   // i
    jmp prodscal_for

prodscal_for:
    ld [%sp], %b             // i
    ld [%sp+6], %a           // n
    cmp %b, %a               // i >= n ?
    jge prodscal_end

    ld [%sp+4], %a           // int *v1
    add %b, %a               // v1[i]
    ld [%a], %a
    push %a

    ld [%sp+6], %a           // int *v2 (we pushed a before so +6)
    add %b, %a               // v2[i]
    ld [%a], %a
    pop %b
    mul %b, %a               // v1[i]*v2[i]

    ld [%sp+1], %b
    add %a, %b
    st %b, [%sp+1]           // p += v1[i]*v2[i]

    ld [%sp], %b
    add 1, %b                // i++
    st %b, [%sp]
    jmp prodscal_for

prodscal_end:
    add 1, %sp
    pop %a
    pop %b
    rtn


racine:
    // square root of a number by dichotomy
    // [r, sup, inf, b, rtn, n]
    push %b
    push 1                   // inf
    ld [%sp+3], %b           // n
    push %b                  // sup
    ld [%sp], %a             // r = (inf+sup) / 2
    add 1, %a
    div 2, %a
    push %a
    jmp racine_loop

racine_loop:
    // 1st while condition
    ld [%sp+5], %a
    ld [%sp], %b
    mul %b, %b
    cmp %b, %a               // n <= r*r ?
    jle racine_inwhile
    jmp racine_if

racine_inwhile:
    // 2nd while condition
    ld [%sp], %b
    add 1, %b
    mul %b, %b
    cmp %b, %a               // n <= (r+1)*(r+1) ?
    jle racine_if
    jmp racine_end

racine_if:
    // if r*r > n
    ld [%sp], %b
    mul %b, %b
    ld [%sp+5], %a
    cmp %b, %a               // n <= r*r ?
    jle racine_else

    ld [%sp], %b
    st %b, [%sp+1]           // inf = r

    ld [%sp+1], %a
    ld [%sp+2], %b
    add %b, %a
    div 2, %a
    st %a, [%sp]             // r = (inf+sup) / 2
    jmp racine_loop

racine_else:
    // if (r+1)*(r+1) <= n
    ld [%sp], %b
    st %b, [%sp+2]           // sup = r

    ld [%sp+1], %a
    add %b, %a
    div 2, %a
    st %a, [%sp]             // r = (inf+sup) / 2
    jmp racine_loop

racine_end:
    pop %a
    add 2, %sp
    pop %b
    rtn


main_calcul:
    push 2
    push 11
    call calcul
    reset

v1:
    .word 1
    .word 2
    .word 3
v2:
    .word 4
    .word 5
    .word 6

main_prodscal:
    push 3
    push v1
    push v2
    call prodscal
    reset

main_racine:
    push 122
    call racine
    reset
