int classify(int x) {
    switch (x % 4) {
        case 0:
            return 40;
        case 1:
            return 10;
        case 2:
            return 20;
        default:
            return 30;
    }
}

int main() {
    int n = 1;
    int score = 0;

    while (n <= 8) {
        score = score + classify(n);
        n = n + 1;
    }

    return score;
}
