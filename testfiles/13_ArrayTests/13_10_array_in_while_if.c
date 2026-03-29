int main() {
    int a[4];
    int i = 0;
    while (i < 4) {
        a[i] = i;
        i = i + 1;
    }
    if (a[3] == 3) {
        return 1;
    }
    return 0;
}
