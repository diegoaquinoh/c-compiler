double improve(double x) {
    int k = 0;
    while (k < 6) {
        x = 0.5 * (x + 2.0 / x);
        k = k + 1;
    }
    return x;
}

int main() {
    double r = improve(1.0);
    return r;
}
