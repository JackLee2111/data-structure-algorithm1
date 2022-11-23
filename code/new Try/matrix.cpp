int uadd_ok(unsigned x, unsigned y)
{
    return ((x & 0xC0000000) & (y & 0xC0000000));
}
