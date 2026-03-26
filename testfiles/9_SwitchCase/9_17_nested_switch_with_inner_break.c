int main() {
    int a = 1;
    int b = 2;
    int x = 0;

    switch (a) {
        case 1:
            x = 1;
            switch (b) {
                case 2:
                    x = x + 10;
                    break;
                default:
                    x = x + 20;
            }
            x = x + 100;
            break;
        default:
            x = -1;
    }

    return x;
}
