int main() {
    int x = 0;
    if ((x = 1)) {
        x = x + 1;
    } else {
        x = 0;
    }
    return x;
}
