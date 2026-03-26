int main() {
    int a = 1;
    int b = 2;
    if (1) {
        a = 10;
        int b = 20;
        b = b + 1;
    }
    return a + b;
}
