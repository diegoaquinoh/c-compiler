void repeat(int c, int n) {
    int i = 0;
    while (i < n) {
        putchar(c);
        i = i + 1;
    }
}

void spaces(int n) {
    repeat(' ', n);
}

void print_border() {
    repeat('=', 53);
    putchar('\n');
}

void print_line_1() {
    repeat('I', 5); spaces(1);
    repeat('F', 5); spaces(1);
    repeat('P', 4); spaces(1); spaces(1);
    putchar('L'); spaces(4); spaces(1);
    repeat('D', 4); spaces(1); spaces(1);
    repeat('C', 4); spaces(1); spaces(1);
    spaces(1); repeat('O', 3); spaces(1); spaces(1);
    putchar('M'); spaces(3); putchar('M'); spaces(1);
    repeat('P', 4); spaces(1);
    putchar('\n');
}

void print_line_2() {
    spaces(2); putchar('I'); spaces(2); spaces(1);
    putchar('F'); spaces(4); spaces(1);
    putchar('P'); spaces(3); putchar('P'); spaces(1);
    putchar('L'); spaces(4); spaces(1);
    putchar('D'); spaces(3); putchar('D'); spaces(1);
    putchar('C'); spaces(4); spaces(1);
    putchar('O'); spaces(3); putchar('O'); spaces(1);
    repeat('M', 2); spaces(1); repeat('M', 1); spaces(2);
    putchar('P'); spaces(2); putchar('P'); spaces(1);
    putchar('\n');
}

void print_line_3() {
    spaces(2); putchar('I'); spaces(2); spaces(1);
    repeat('F', 4); spaces(1); spaces(1);
    repeat('P', 4); spaces(1); spaces(1);
    putchar('L'); spaces(4); spaces(1);
    putchar('D'); spaces(3); putchar('D'); spaces(1);
    putchar('C'); spaces(4); spaces(1);
    putchar('O'); spaces(3); putchar('O'); spaces(1);
    putchar('M'); spaces(1); putchar('M'); spaces(1); putchar('M'); spaces(1);
    repeat('P', 4); spaces(1);
    putchar('\n');
}

void print_line_4() {
    spaces(2); putchar('I'); spaces(2); spaces(1);
    putchar('F'); spaces(4); spaces(1);
    putchar('P'); spaces(4); spaces(1);
    putchar('L'); spaces(4); spaces(1);
    putchar('D'); spaces(3); putchar('D'); spaces(1);
    putchar('C'); spaces(4); spaces(1);
    putchar('O'); spaces(3); putchar('O'); spaces(1);
    putchar('M'); spaces(3); putchar('M'); spaces(1);
    putchar('P'); spaces(4);
    putchar('\n');
}

void print_line_5() {
    repeat('I', 5); spaces(1);
    putchar('F'); spaces(4); spaces(1);
    putchar('P'); spaces(4); spaces(1);
    repeat('L', 5); spaces(1);
    repeat('D', 4); spaces(1); spaces(1);
    repeat('C', 4); spaces(1); spaces(1);
    spaces(1); repeat('O', 3); spaces(1); spaces(1);
    putchar('M'); spaces(3); putchar('M'); spaces(1);
    putchar('P'); spaces(4);
    putchar('\n');
}

int main() {
    print_border();
    print_line_1();
    print_line_2();
    print_line_3();
    print_line_4();
    print_line_5();
    print_border();

    return 0;
}
