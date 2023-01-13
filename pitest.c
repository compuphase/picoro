#include <stdio.h>
#include "picoro.h"

void *fun(void *arg)
{
  printf("wim - %s\n",(char*)arg);
  char *s = yield("teun");
  printf("zus - %s - %s\n", (char*)arg, s);
  s = yield("vuur");
  printf("jet - %s - %s\n", (char*)arg, s);
  return "gijs";
}

int main(void)
{
  printf("make coroutine\n");
  coro c = coroutine(fun);
  printf("Resume, step 1\n");
  char *s = resume(c, "aap");
  printf("Resume, step 2 - %s\n", s);
  s = resume(c, "mies");
  printf("Resume, step 3 - %s\n", s);
  if (resumable(c)) {
    s = resume(c, "wim");
    printf("Resume, step 4 - %s\n", s);
  } else {
    printf("completed\n");
  }
  if (resumable(c)) {
    s = resume(c, "lam");
    printf("Resume, step 5 - %s\n", s);
  } else {
    printf("completed\n");
  }
  return 0;
}
