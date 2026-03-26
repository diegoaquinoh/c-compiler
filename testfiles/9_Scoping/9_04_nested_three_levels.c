int main() {
    int a = 1;
    if (1) {
        int b = 2;
        if (1) {
            int c = 3;
            if (1) {
                int d = 4;
                a = a + b + c + d;
            }
        }
    }
    return a;
}
