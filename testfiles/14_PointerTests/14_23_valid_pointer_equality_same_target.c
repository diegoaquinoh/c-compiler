int main() {
    int x;
    int *p;
    int *q;
    x = 4;
    p = &x;
    q = &x;
    return p == q;
}
