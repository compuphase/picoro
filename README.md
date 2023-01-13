# picoro
A minimal implementation of coroutines in plain C.
The original version was implemented and published by Tony Finch.
His description of how it works is on his blog: [Coroutines in less than 20 lines of standard C](https://fanf.livejournal.com/105413.html).

This fork exists mostly to add some documentation.

## Usage
```
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
  s = resume(c, "noot");
  printf("Resume, step 3 - %s\n", s);
  if (resumable(c)) {
    s = resume(c, "mies");
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
```

When the above program is run, the output is:
```
make coroutine
Resume, step 1
wim - aap
Resume, step 2 - teun
zus - aap - noot
Resume, step 3 - vuur
jet - aap - mies
Resume, step 4 - gijs
completed
```

1. After making the coroutine, it is idle. It does not run until the first time it is resumed.
2. When resuming a coroutine for the first time, the second parameter passed to `resume()` becomes the argument of the coroutine's function. Here, the call `resume(c, "aap")` makes `fun()` receive `aap` in `arg`.
3. Any next time the main thread calls `resume()`, the parameter is passed as the return value of `yield()`.
4. The parameter passed to `yield()` is passed as the return value of `resume()`.
5. When a coroutine runs to completion, its return value is also passed as the return value of `resume()` (as if it were the final "yield", so to speak).
6. Function `yield()` passes execution back to the thread that created the coroutine. Calling `yield()` when no coroutine has been set up, is ok (it simply does not yield). However, calling `resume()` on a coroutine that has already run to completion causes an assert. The caller should check that a coroutine is *resumable* before resuming it.

In case you are wondering about the test words "aap", "noot", "mies", etc. instead of the more ubiquitous "foo", "bar" & "baz", do an internet search for "leesplankje". Yes, that is how I learned to read.
