int foo(int x) {
    if (1) {
        int x = 99;
        return x;
    }
    return x;
}

int main() {
    return foo(5);
}
