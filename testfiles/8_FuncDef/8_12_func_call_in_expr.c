int double_it(int x) {
    return x + x;
}

int main() {
    int a = double_it(3) + double_it(4);
    return a;
}
