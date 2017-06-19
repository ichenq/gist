


int ftol(float f)
{
    int a         = *(int*)(&f);
    int sign      = (a >> 31);
    int mantissa  = (a & ((1 << 23) - 1)) | (1 << 23);
    int exponent  = ((a & 0x7fffffff) >> 23) - 127;
    int r         = ((unsigned int)(mantissa) << 8) >> (31 - exponent);
    return ((r ^ (sign)) - sign ) &~ (exponent >> 31);
}


// Faster version that uses some global memory and requires rounding mode to be on:
//extern "C" __declspec(naked) 
void __cdecl _ftol()
{
    const static int zpfp[2] = { 0xBEFFFFFF, 0x3EFFFFFF };

    __asm
    {
        SUB	ESP, 4
        FST	DWORD PTR [ESP]
        MOV	EAX, DWORD PTR [ESP]
        SHR	EAX, 29
        AND	EAX, 4
        FADD	DWORD PTR [zpfp+EAX]
        FISTP	DWORD PTR [ESP]
        POP	EAX
        RET
    }
}
