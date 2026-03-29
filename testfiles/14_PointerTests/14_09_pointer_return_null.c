int *mk_null() {
    return 0;
}

int main() {
    int *p;
    p = mk_null();
    return p == 0;
}
