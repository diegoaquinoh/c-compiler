int main() {
    int x;
    int *p;
    int **pp;
    x = 1;
    p = &x;
    pp = &p;
    **pp = 23;
    return x;
}
