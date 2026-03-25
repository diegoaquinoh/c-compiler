int main() {
    int a = 1;
    int b = 0;
    switch (a) {
        case 1:
            b = 10;
        case 2:
            b = b + 5;
        default:
            b = b + 1;
    }
    return b;
}
