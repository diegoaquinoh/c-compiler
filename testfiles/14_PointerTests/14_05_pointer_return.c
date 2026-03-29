int *id_ptr(int *p) {
    return p;
}

int main() {
    int x;
    int *q;
    x = 13;
    q = id_ptr(&x);
    return *q;
}
