int main() {
    int x;
    int *p;
    x = 1;
    p = &x;
    if (*p) {
        return 33;
    }
    return 0;
}
