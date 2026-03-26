int double_it(int x) {
    return x + x;
}

int triple_it(int x) {
    return x + x + x;
}

int main() {
    int a = double_it(5);
    int b = triple_it(3);
    return a + b;
}
