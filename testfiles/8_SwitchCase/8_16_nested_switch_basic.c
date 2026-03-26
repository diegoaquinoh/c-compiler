int main() {
    int a = 1;
    int b = 2;

    switch (a) {
        case 1:
            switch (b) {
                case 2:
                    return 12;
                default:
                    return 10;
            }
        default:
            return 0;
    }
}
