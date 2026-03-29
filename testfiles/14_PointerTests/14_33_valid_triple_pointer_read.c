int main() {
    int x;
    int *p;
    int **pp;
    int ***ppp;
    x = 17;
    p = &x;
    pp = &p;
    ppp = &pp;
    return ***ppp;
}
