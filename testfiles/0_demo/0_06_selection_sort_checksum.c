int main() {
    int a[6];
    int i = 0;

    a[0] = 9;
    a[1] = 3;
    a[2] = 7;
    a[3] = 1;
    a[4] = 8;
    a[5] = 2;

    while (i < 6) {
        int j = i + 1;
        int min_idx = i;

        while (j < 6) {
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
            j = j + 1;
        }

        int tmp = a[i];
        a[i] = a[min_idx];
        a[min_idx] = tmp;

        i = i + 1;
    }

    return a[0] + 2 * a[1] + 3 * a[2] + 4 * a[3] + 5 * a[4] + 6 * a[5];
}
