int main() {
    int i = 0;
    int acc = 0;
    while (i < 4) {
        if (i < 2) {
            acc = acc + 1;
        } else {
            acc = acc + 2;
        }
        i = i + 1;
    }
    return acc;
}
