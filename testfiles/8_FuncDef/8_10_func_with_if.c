int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int main() {
    int a = abs_val(-7);
    int b = abs_val(3);
    return a + b;
}
