int get_val() {
    return 5;
}

int main() {
    int r = 0;
    if (1) {
        int x = get_val();
        r = r + x;
    }
    if (1) {
        int x = get_val() + get_val();
        r = r + x;
    }
    return r;
}
