int add(int a, int b) {
    return a + b;
}

int double_it(int x) {
    return x + x;
}

int main() {
    return add(double_it(3), double_it(4));
}
