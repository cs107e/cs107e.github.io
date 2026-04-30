---
title: "Style Reflection Assign 4"
toc: true
attribution: Written by Ben Ruland
---


### Decomposition
We love well-decomposed code! Your previous courses have likely preached to you at length on the value of decomposition but we want to touch upon it again here since it is so important. Decomposition is an indispensable tool for managing program complexity. When you're tasked with implementing a large and complex operation, trying to bang it out as a one enormous monolithic function is just not the move here. That "do everything-everywhere-all-at-once" function is hard to write, often hard to use, and nearly impossible to re-use. If instead you start by decomposing into a sequence of tasks, each to be implemented as a smaller single-purpose function, you reap a lot of benefits:

- Five 10-line functions are easier to manage than one 50-line function
- Each function can be implemented and tested as an independent unit
- The functions can be re-used, both within the complex operation and in other contexts

The strategy is to decompose __problems, not programs__. This means do your decomposition __before__ writing code, not after. If you have already slogged through writing a 500-line `main()` that does everything, trying to now decompose the code as an afterthought, ... ugh! Not only would this be an irritating chore, it totally misses out on the benefits of the upfront decomposition.  It is so much easier to solve and verify smaller subproblems. If you break it down from the start, while it's still just a whiteboard sketch of rectangles, you have an outline that guides you through translating those rectangles one-by-one into working functions.

Here are some of the properties to aim for when designing a nicely decomposed function:

- Fully performs a single coherent task
- Does not do too large a share of the work
- Is not unnecessarily entangled with other functions
- Stores data at the narrowest scope possible
- Helps indicate and subdivide the structure of the overall program
- Helps remove redundancy that would otherwise be present in the overall program
- Function interface (parameters, return value) is clean and well-encapsulated

### Unifying common code

A huge benefit of well-designed and decomposed functions is that they facilitate code reuse. If we see the up-front opportunity to unify code, such as a recurring task that fits neatly into its own function, we should certainly take it, as writing less code is clearly a win. But if you have previously written a function and now find yourself needing something similar but with an added twist, you may be tempted to copy and paste the previous code to hack up a second function. This is __not__ what we mean by "code reuse".  Instead, revisit the earlier function and how you might adapt it to be more general or build an extended version that calls on the earlier function.  Sometimes you will not even realize that two code paths can be merged until after having written both. It's not too late! You can go back and refactor the code and still reap a lot of the win.  Why should we put in so much effort to unify code we have already written?


- Unifying into a single code path lessens the testing load and reduces surface area for bugs to hide. If you have two functions that do almost the same thing but not quite, you need duplicate test cases and have twice as much code that could have a bug in it. It is particularly disheartening when you have two functions that are mostly copy-paste and track down a difficult bug in one and fix it without realizing the same bug also affects the second copy only to have to later track down that same bug all over again.

-  Less code to maintain and manage means it is simpler to make changes when you need to add features, update functionality, and fix bugs. When you have gobs of nearly duplicate code, not only is it more work to make those updates, but it's also all to easy to miss an edit or mess things up in a selected few of the darker corners. When all code is running through one unified path, it "lives or dies" together, which means that if you introduce an error, it will be obvious right away, and once you have it correct, it should be correct for all.

- The unified version is simpler for anyone using your code. Consider `strtonum` which accepts a string in either base 10 or 16 and figures out which to use internally. If we instead decomposed into separate functions one per base `strtonum_dec` and `strtonum_hex` and so on, we have put it on the client to figure out which function is the one they need to call.

Unifying code is not always straightforward. Often times the task will have slightly different behavior in each place it is used. If there is a lot of variance in the use cases, unifying all to one function could result in a long list of parameters and an unwieldy interface. But here's a tip: the beauty of separating the interface from the implementation means they don't have to be one-to-one correspondence.  It can make good sense to unify the implementation into single general-purpose workhorse, but publish an interface with many small wrapper functions that each call the workhouse behind the scenes.

There is no rule for when to unify and when to leave it be. It is up to you as the programmer to weigh the tradeoff of how difficult and complicated a unified version would be to write vs how much it will improve your code (which is usually really asking how many times would this function be called and how much work would the function do). Sometimes the answer is to break down the code in question into smaller chunks and decompose a smaller simpler subset of the code, so you can still reduce the amount of repeated code while not overcomplicating each function. As mentioned previously, the simpler the function's behavior is the more likely you are to be able to make use of it elsewhere in your code.

### Over-optimization
All this talk about less code is good code might lead you to think that aiming to execute fewer instructions at runtime is another achievement in the same vein. That's a completely different metric, and one that has its own special considerations.

You might have started to consider about optimization during this assignment, and if not you'll likely start thinking/hearing about it as you continue on in your CS careers. We all love a good optimization however it is easy to fall into the trap of thinking more is always better. To quote Knuth: "Premature optimization is the root of all evil".

Working hard to optimize your code comes at a high cost; the work takes programmer time, the changes often impair the readability of the code, it can introduce subtle bugs or create frailty, or force tradeoffs with other metrics such as memory use. So when Knuth talks about premature optimization, what he means is that you should never start optimizing your code without first asking "does this really need to be optimized?".  Rather than mindlessly chasing a metric, be sure you have taken in the bigger picture and understand what is worth doing.

A common example of this is when a developer merrily starts optimizing part of a program. They spend a ton of time and effort to bring the runtime down. Their tests are looking great and measuring a 10x speedup, and they are very pleased. When they measure the impact on the whole program and see that its speed is unchanged, their heart sinks. What happened? The code the dev improved was not a bottleneck at all. It might seem silly when we put it this way, but this sort of mistake happens all the time.

This dilemma comes up in more subtle ways as well. Imagine a task that can be completed in a simple manner that makes two passes over the array. The extra pass is displeasing, but combining to a single pass turns out to be extra tricky and easy to get wrong. Is the optimization worth it?  If the function that is called sparingly and contributes only a minor cost to the overall operation, why not stick with the simple and correct thing? On the other hand, if the function is called very often and contributes the lion;s share to a heavy-duty operation, such as a matrix multiply for a math library, the need for performance warrants us taking pains to write it as efficiently as possible.

## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign4` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers. If you find you've answered the questions and are left far short of a page then feel free to choose a related topic and tell us about it and how it applies to the code you have written so far. 

1. Review your `printf` module and find at least one place where you have duplicated code that could have instead called upon a function written earlier in the `printf` or `strings` modules. Show how you could replace the duplicated code with a call to the existing function.

2. Consider how you implemented padding in `printf`. Padding applies across all format specifiers with some slight differences. Does your `printf` handle all of the padding in one unified code path? If not, tell us how you wrote it and how you could go about unifying the code. Please include some specific lines of code you would change.

3. Processing a `printf` format specifier is simplified by implementing a series of steps, rather than aiming to go from start to finish in one pass and in-place. For example, for format specifier `%8d`, you might first process the digits from right to left and write to a temporary buffer in backward order, then copy them out in reverse. A follow-up step applies padding and another pass copies to the final output string and truncates.  This approach makes a couple of extra passes when compared to a version that aimed to write the final characters to the output string in one go; a big-O of perhaps `O(4n)` instead of `O(n)`. Explain the advantages of the stepwise, layered approach and why optimizing into a single pass would likely cost much more than any benefit it provided.

4. By now, you have first-hand experience of how unsafe C is about memory and pointers. Consider the function `strlen` as an example of a function that is inherently unsafe.  What happens when a client calls `strlen` with a pointer that is invalid or does not point to a well-formed C-string?

    4.b Now take a look at the specification for the function `[strnlen](https://man7.org/linux/man-pages/man3/strnlen.3.html)`. Tell us how using `strnlen` can be safer than `strlen`. Does it fix all of the problems with `strlen`?
