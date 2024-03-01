#include <stdio.h>
#include <time.h>
void delay(int ms) // аргумент - требуемое время задержки в миллисекундах
{
  int c = clock() + ms;
  while (clock() < c);
}
int main() 
{
  printf("clock = %d\n", clock());
  delay(10);
  printf("clock = %d\n", clock());
  getchar();
  return 0;
}