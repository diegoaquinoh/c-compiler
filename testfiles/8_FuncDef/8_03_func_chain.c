int square(int x) {
    return x * x;
}

int add_squares(int a, int b) {
    int sa = square(a);
    int sb = square(b);
    return sa + sb;
}

int main() {
    return add_squares(3, 4);
}
