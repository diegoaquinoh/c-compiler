int inc(int x) {
    return x + 1;
}

int double_inc(int x) {
    return inc(inc(x));
}

int main() {
    return double_inc(5);
}
