int main() {
    int n = 29;
    int d = 2;

    while (d < n) {
        if (n % d == 0) {
            return 0;
        }
        d = d + 1;
    }

    return 1;
}
