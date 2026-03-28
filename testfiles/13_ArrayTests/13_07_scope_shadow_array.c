int main() {
    int a[2];
    a[0] = 5;
    {
        int a[2];
        a[0] = 9;
    }
    return a[0];
}
