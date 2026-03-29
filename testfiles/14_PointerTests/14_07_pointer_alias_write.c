int main() {
    int x;
    int *p;
    int *q;
    x = 3;
    p = &x;
    q = p;
    *q = *p + 4;
    return x;
}
