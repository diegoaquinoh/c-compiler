int main() {
    int x;
    int *p;
    int **pp;
    x = 5;
    p = &x;
    pp = &p;
    return **pp;
}
