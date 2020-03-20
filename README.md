# patronage20-embedded



Code style guide
======
## Base
* Use four (4) spaces for indentation.
* Optimize for readability using names that would be clear even to people on a different team.
* Maximum length of a line of code is 120 characters long.

## Comments
Comments make code more readable, not only for others (e.g., your TF) but also for you, especially when hours, days, weeks, months, or years pass between writing and reading your own code. 

Within functions, use “inline comments” and keep them short (e.g., one line), else it becomes difficult to distinguish comments from code, even with syntax highlighting. Place the comment above the line(s) to which it applies. No need to write in full sentences, but do capitalize the comment’s first word (unless it’s the name of a function, variable, or the like), and do leave one space between the `/*` and your comment’s first character, as in:
```c
/* Convert Fahrenheit to Celsius */
float c = 5.f / 9.f * (f - 32.f);
```
Atop your .c and .h files should be a comment that summarize what your program (or that particular file) does, as in:
```c
/* Says hello to the world */
```
Atop each of your functions (except, perhaps, main), meanwhile, should be a comment that summarizes what your function is doing, as in:
```c
/* Returns the square of n */
int square(int n)
{
    return n * n;
}
```
## Conditions
Conditions should be styled as follows:
```c
if (x > 0) {
    printf("x is positive\n");
}
else if (x < 0) {
    printf("x is negative\n");
}
else {
    printf("x is zero\n");
}
```
Notice how:
* the curly braces start after end of `if` condition with space between them.
* there’s a single space after each `if`;
* each call to `printf` is indented with 4 spaces;
* there are single spaces around the `>` and around the `<`; and
* there isn’t any space immediately after each `(` or immediately before each `)`.
* One-liners don't need braces. In some very simple cases it's also OK to use commas instead of braces. E.g.:
```C
if (x > 0)
    a++, b++;
```

## Switches
Declare a `switch` as follows:
```c
switch (n) {
    case -1:
        printf("n is -1\n");
        break;

    case 1: 
        {
            int variable = 2;
            square(variable);
            break;
        }

    default:
        printf("n is neither -1 nor 1\n");
        break;
}
```
Notice how:
* each curly brace is on its own line;
* there’s a single space after `switch`;
* there isn’t any space immediately after each `(` or immediately before each `)`;
* the switch’s cases are indented with 4 spaces;
* the cases’ bodies are indented further with 4 spaces; and
* each `case` (including `default`) ends with a `break`.
* use brackets only if you declare variables there. If you need to declare variables within a case, I'd strongly suggest to reconsider your code design. If - after this reconsideration - you still need to declare variables within case, then indent this braces by 4 spaces. Moreover - no implicit fall-throughs - at least leave a comment in such case, e.g. `/* fall-through */`

## Loops
Whenever you need temporary variables for iteration, use `i`, then `j`, then `k`, unless more specific names would make your code more readable:

```c
int i;
for (i = 0; i < LIMIT; i++) {
    for (int j = 0; j < LIMIT; j++) {
        for (int k = 0; k < LIMIT; k++) {
            /* Do something */
        }
    }
}
```

`do...while` loops always with braces, like this:
```c
do {
    /* Do something */
} while (/*condition*/);
```

It's the only case when we write something after the closing bracket, so it would be obvious that `while ()` ends the loop and does not start it.

## Variables
Do not define all of your variables at the very top of your functions but, rather, when and where you actually need them.
```c
int i;
for (i = 0; i < LIMIT; i++) {
    printf("%i\n", i);
}
```
Though it’s fine to use variables like `i`, `j`, and `k` for iteration, most of your variables should be more specifically named. If your variable’s name warrants two words (e.g., `is_ready`), put an underscore between them.
Keep assignment and declaration in one line.

## Functions
Avoid functions longer than 40 lines.
Functions names should start with lower-case and have a `_` separator between each word.
```c
static int do_something(void);
```
