int main() {
    int x = 1;
    if (1) {
        int x = 2;
        if (1) {
            int x = 3;
            x = x + 10;
        }
        x = x + 10;
    }
    return x;
}
