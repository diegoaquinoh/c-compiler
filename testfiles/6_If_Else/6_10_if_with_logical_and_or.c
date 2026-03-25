int main() {
    int x = 0;
    int a = 1;
    int b = 0;
    if (a && b) {
        x = 1;
    } else {
        x = 2;
    }

    if (a || b)
        x = x + 3;
    else
        x = x + 5;

    return x;
}
