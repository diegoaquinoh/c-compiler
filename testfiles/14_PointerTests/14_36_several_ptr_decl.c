int main() {
    int x, y, *p, *q, **pp, ***ppp;

    x = 19;
    y = 0;

    p = &x;
    q = &y;
    pp = &p;
    ppp = &pp;

    **pp = **pp + 4;
    ***ppp = ***ppp + 3;
    *q = ***ppp;

    return *q;
}
