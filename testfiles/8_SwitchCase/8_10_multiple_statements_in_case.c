int main() {
    int a = 2;
    int b = 1;
    switch (a) {
        case 2:
            b = b + 1;
            b = b * 3;
            break;
        default:
            b = 0;
    }
    return b;
}
