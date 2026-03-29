int main() {
    int x;
    int *p;
    x = 9;
    p = &x;
    if (p != 0) {
        return *p;
    }
    return 0;
}
