int bar(int x);
int baz(int x);

int foo(int x) {
    return bar(x) + baz(x);
}

int bar(int x) {
    return x * 2;
}

int baz(int x) {
    return x * 3;
}

int main() {
    return foo(2);
}
