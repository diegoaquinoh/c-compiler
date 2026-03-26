int main() {
    int x = 1;
    if (x) {
        int y = 10;
        x = x + y;
    } else {
        int y = 20;
        x = x + y;
    }
    return x;
}
