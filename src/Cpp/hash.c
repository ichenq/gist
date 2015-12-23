#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef uint8_t byte;

/*
 *   PHP中出现的字符串Hash函数
 */
unsigned long hashpjw(char* arKey, unsigned int nKeyLength)
{
    unsigned long h = 0, g;
    char* arEnd = arKey + nKeyLength;

    while (arKey < arEnd)
    {
        h = (h << 4) + *arKey++;

        if ((g = (h & 0xF0000000)))
        {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }

    return h;
}


/*
 *   OpenSSL中出现的字符串Hash函数
 */

/* The following hash seems to work very well on normal text strings
 * no collisions on /usr/dict/words and it distributes on %2^n quite
 * well, not as good as MD5, but still good.
 */
unsigned long lh_strhash(const char* c)
{
    unsigned long ret = 0;
    long n;
    unsigned long v;
    int r;

    if ((c == NULL) || (*c == '\0'))
        return(ret);

    n = 0x100;

    while (*c)
    {
        v = n | (*c);
        n += 0x100;
        r = (int)((v >> 2) ^ v) & 0x0f;
        ret = (ret << r) | (ret >> (32 - r));
        ret &= 0xFFFFFFFFL;
        ret ^= v * v;
        c++;
    }

    return((ret >> 16) ^ ret);
}

/*
 *   MySql中出现的字符串Hash函数
 */

#if 0

/* Calc hashvalue for a key */
uint32_t calc_hashnr(const int8_t* key, uint32_t length)
{
    register uint32_t nr = 1, nr2 = 4;

    while (length--)
    {
        nr ^= (((nr & 63) + nr2) * ((uint32_t) (uint8_t) * key++)) + (nr << 8);
        nr2 += 3;
    }

    return((uint32_t) nr);
}

/* Calc hashvalue for a key, case indepenently */
uint32_t calc_hashnr_caseup(const int8_t* key, uint32_t length)
{
    register uint32_t nr = 1, nr2 = 4;

    while (length--)
    {
        nr ^= (((nr & 63) + nr2) * ((uint32_t) (uint8_t) toupper(*key++))) + (nr << 8);
        nr2 += 3;
    }

    return((uint32_t) nr);
}
#else
/*
 * Fowler/Noll/Vo hash
 *
 * The basis of the hash algorithm was taken from an idea sent by email to the
 * IEEE Posix P1003.2 mailing list from Phong Vo (kpv@research.att.com) and
 * Glenn Fowler (gsf@research.att.com). Landon Curt Noll (chongo@toad.com)
 * later improved on their algorithm.
 *
 * The magic is in the interesting relationship between the special prime
 * 16777619 (2^24 + 403) and 2^32 and 2^8.
 *
 * This hash produces the fewest collisions of any function that we've seen so
 * far, and works well on both numbers and strings.
 */
uint32_t calc_hashnr(const byte* key, uint32_t len)
{
    const byte* end = key + len;
    uint32_t hash;

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (uint32_t) * (uint8_t*) key;
    }

    return (hash);
}

uint32_t calc_hashnr_caseup(const byte* key, uint32_t len)
{
    const byte* end = key + len;
    uint32_t hash;

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (uint32_t) (uint8_t) toupper(*key);
    }

    return (hash);
}
#endif



/*
 *   另一个经典字符串Hash函数
 */

unsigned int hash(char* str)
{
    register unsigned int h;
    register unsigned char* p;

    for(h = 0, p = (unsigned char*)str; *p ; p++)
        h = 31 * h + *p;

    return h;
}
