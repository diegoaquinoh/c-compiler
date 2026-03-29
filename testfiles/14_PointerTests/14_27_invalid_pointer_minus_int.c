int main() {
    int x;
    int *p;
    p = &x;
    p = p - 1;
    return p != 0;
}
