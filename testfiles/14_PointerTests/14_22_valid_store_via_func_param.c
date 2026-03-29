int store(int *p, int v) {
    *p = v;
    return *p;
}

int main() {
    int x;
    x = 0;
    store(&x, 19);
    return x;
}
