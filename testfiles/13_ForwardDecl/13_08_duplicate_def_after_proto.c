int foo(int x);

int foo(int x) {
    return x;
}

int foo(int x) {
    return x + 1;
}

int main() {
    return foo(5);
}
