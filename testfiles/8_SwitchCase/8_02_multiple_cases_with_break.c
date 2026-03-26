int main() {
    int a = 3;
    int b = 0;
    switch (a) {
        case 1:
            b = 10;
            break;
        case 3:
            b = 30;
            break;
        default:
            b = 99;
    }
    return b;
}
