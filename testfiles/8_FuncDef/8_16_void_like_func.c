int side(int x) {
    int y = x + 1;
    return y;
}

int main() {
    int a = 10;
    int b = side(a);
    int c = side(b);
    return c;
}
