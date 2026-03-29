int main() {
    int x;
    int *p;
    int **pp;
    x = 14;
    p = &x;
    pp = &p;
    return **pp;
}
