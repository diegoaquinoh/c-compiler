int main() {
    int x;
    int *p;
    int *q;
    p = &x;
    q = p;
    return p < q;
}
