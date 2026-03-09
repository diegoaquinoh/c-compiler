int main() {
  int a;
  int b;
  int c;
  a = 1;
  b = 2;
  c = 3;
  a = b;
  b = c;
  c = a;
  a = 4;
  b = a;
  c = b;
  return c;
}