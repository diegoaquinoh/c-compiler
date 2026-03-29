int *id(int *p) {
    return p;
}

int main() {
    int x;
    int *p;
    x = 77;
    p = id(&x);
    return *p;
}
