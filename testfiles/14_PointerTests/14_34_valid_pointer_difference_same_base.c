int main() {
    int x;
    int *p;
    int *q;
    p = &x;
    q = p + 2;
    return q - p;
}
