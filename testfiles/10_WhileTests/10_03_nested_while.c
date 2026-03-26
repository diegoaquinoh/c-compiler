int main() {
    int i = 0;
    int j = 0;
    while (i < 2) {
        j = 0;
        while (j < 3) {
            j = j + 1;
        }
        i = i + 1;
    }
    return i + j;
}
