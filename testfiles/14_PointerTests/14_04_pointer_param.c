int inc_through_ptr(int *p) {
    *p = *p + 1;
    return *p;
}

int main() {
    int x;
    x = 9;
    return inc_through_ptr(&x);
}
