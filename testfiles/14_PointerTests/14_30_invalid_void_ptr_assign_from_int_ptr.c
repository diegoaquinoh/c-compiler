int main() {
    int x;
    int *p;
    void *q;
    p = &x;
    q = p;
    return q != 0;
}
