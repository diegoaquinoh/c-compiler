int compute(int a, int b) {
    int sum = a + b;
    int diff = a - b;
    int result = sum + diff;
    return result;
}

int main() {
    return compute(10, 3);
}
