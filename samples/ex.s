void f(int x) {
    int y;
    y = g(10, x+2);
    return y+20;
}

void g(int u, int v) {
    return (u+1)*(v+2);
}

// ---

f:
    sub 1, %sp // save momory place for y
    ld [%sp+2], %a
    add 2, %a

    push %a // x+2
    push 10
    call g
    add 2, %sp

    st %a, [%sp]
    ld [%sp], %a
    add 20, %a
    add 1, %sp
    rtn

g:
    ld [%sp+1], %a
    add 1, %a // u+1
    push %b
    ld [%sp+3], %b
    add 2, %b // v+2
    mul %b, %a // return value
    pop %b
    rtn
