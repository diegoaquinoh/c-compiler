int foo(int x) {
    int y = x + 1;
    return y;
}

int main() {
    int x = 10;
    int y = 20;
    int r = foo(x);
    return r + y;
}
