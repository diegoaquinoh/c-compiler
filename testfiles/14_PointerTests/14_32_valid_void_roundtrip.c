int main() {
    int x;
    int *p;
    void *v;
    int *q;
    x = 31;
    p = &x;
    v = p;
    q = v;
    return *q;
}
