---
title: "Style Reflection Assign 1"
toc: true
attribution: Written by Ben Ruland
---

### What is a style reflection?
Each assignment has a companion "style reflection", a guided activity that directs you to make assessments about your code and allows you to share your progress in achieving your quality goals and your plans for how you can continue to improve.  The style reflection will typically consist of a short reading and a few prompt questions. You earn your code quality grade with your efforts to meaningfully engage with these materials and answer the prompts with thoughtful consideration.

The style reflections are not an exercise in prose writing, so please don't stress about the eloquence of your words. We prefer you to keep your language simple and in your own voice. We want to see sincere introspection and hear your thoughts. We expect you to do your own reading and your own writing. Limited use of AI tools in the role of an ethical tutor would be acceptable (e.g. such as to explain a concept from the reading that you were not familiar), but you should not be prompting the AI to do the reading/writing for you or other such assistance.

In this first reflection, the readings offer perspectives on the standards for well-written code. We want to hear your thoughts on those standards, which of them you plan to adopt and how we can help you achieve your goals.

## Why care about style?

Perhaps you already care deeply about style, or you might be a bit indifferent. At where you are now, like some of us once did, you might find it annoying to spend time on it at all. We hope to inspire all of you to learn to see style as an essential part of software development and an integral component of your work that you are motivated and proud to excel at.

Writing code is itself a craft; Donald Knuth titled his seminal works “The Art of Computer Programming” for good reason! [Knuth’s Turing award speech ](https://www.cs.bilkent.edu.tr/~canf/knuth1974.pdf) on the subject is a great optional read. A good craftsperson takes pride in polishing and refining their work to achieve not only a functional result but an aesthetic one: a delicious and beautifully-presented meal, a sturdy chair that is comfortable and pleasing to sit in, a piece of tech that is elegant and easy to use. For us it is a well-crafted program, one that is straightforward to understand, modify, and maintain. Writing elegant code to solve a difficult problem brings us an immense sense of pride and accomplishment; we want you to have that experience too.

While in college, you’ll write most of your code alone and have it read by only a TA who offers a smattering of style feedback. You may feel investing in good style is not important given the small audience who reads it, but this will no longer be the case as you move into your careers. Your code will be read by your coworkers, your bosses, you might even have folks reading your code years after you wrote it who seek you out to ask questions about it! Rather than adopt the attitude of “no point worrying about it now, I’ll write good code later when it matters”, use the learning opportunities now to develop your sense of style so that when the time comes, you’ll already be skilled at writing code that will not only pass muster with your team, but be celebrated and serve as an example to others.

For a quick fun read, this handout [landmarks in coding quality](../landmarks) written by my colleague Nick Parlante is the funniest and most true advice to students ever.


## Style in assembly

This first assignment is a bit of an outlier. Most of you will not be writing much, if any, assembly during your careers. A systems programmer needs to understand/read assembly but rarely hand-construct it. Accordingly, we offer just two quick thoughts on style when writing assembly code.

- __Comment, comment, comment__

    A good rule of thumb is that the volume of commenting goes up with the difficulty of writing the code. Writing assembly code is hard and reading is similarly difficult. What is worthy of comments? Guide the user to understand how the assembly operates: which registers are being used for which purpose, how control flows, explain the values for magic numbers, and so on. It would not be unheard of to comment on nearly single assembly instruction. If you were to return to this assembly program a month or year later to fix a bug or make a modification, what essential information would be helpful in re-orienting with the code?
- __Assembly code is not indented__

    An assembly program listing should line up all instructions in one column (possibly with labels outdented), using no other indentation. In languages such as python and C, indentation and block structure dictate variable scope and control flow. But assembly is entirely unstructured! It has no concept of block or scope; registers are “global”, all control flow is explicit. Thus, indenting your assembly could be confusing or misleading to a reader.

## Style in C

You will be writing in C for the rest of the quarter. Unlike assembly which by its very nature is terse and cryptic, the broad expressiveness of the C language will enable you to write very readable code. However, this doesn’t occur just by happenstance, you must develop an intentional practice of good choices.


### Style guides
Most every course/research team/company adopts a general set of conventions for code style. These are often expressed as a list of “do” and “donts”. When you read one, you may personally disagree with some of the priorities, find some details to be nitpicky and pedantic or be irked that one shop’s conventions directly contradict another’s. However, writing code that ignores or trangresses the style guide will stand out like a sore thumb and is typically rejected and sent back for rewrite. After forcing yourself to align to the style guidelines, you may later come to appreciate why those practices were chosen.

We have linked a few style guides below for you to peruse. The first two are written for students in a systems courses such as this one (our mainstream CS107, CMU’s systems course). The last two are the industrial-strength guides used by Google and the linux kernel team. It is interesting to see how the pros do it!

- C style guide for our [CS107 course](https://cs107.stanford.edu/styleguide)
- C style guide from [Prof. Eno Thereska at CMU](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html#formatting)
- C++ style guide for [Google sources](https://google.github.io/styleguide/cppguide.html)
- C style guide for [linux kernel](https://www.kernel.org/doc/html/v4.10/process/coding-style.html)

Which practices in the guides do you already agree with and use in your code? What new practices are you eager to adopt?

### The value of code reading
The strength of the open source software community and volumes of code publically shared on the web (`github.com`, `bitbucket.com`, etc) present great code-reading opportunities for a young padawan. There is so much you can learn by reading the code of others and reflecting on their choices.  Some of the code you find will be work of the masters, so elegant it takes your breath away, and at the other end of the spectrum code is code so gnarly that you need to wash your eyeballs after. Think back to (or look to find) a piece of published code that you thought was particularly nifty because of its style and design choices. It could be code you saw in 106B lecture, code offered as an answer to a question on stack overflow, a project you viewed on github, from an blog post, ... Perhaps the code had a novel approach to solving a problem or applied a programming technique that was new to you or you were simply struck by how tidy the code was and how easy it was to understand. If you were sharing this code with your peers, what would you tell them about what this code stand out to you?


## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign1` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers.

1. Tell us about your journey thus far in developing your programming style, what a-ha moments have you had, struggles, aspirations, or victories?

2. Review the C style guides linked abve, pick a few points that stood out to you and tell us about them. It could be something you think is elegant, something you were surprised by, something you want to learn more about, or something you plan to adopt for yourself.

3. Why is it good practice to always enclose body of an if statement in braces, even if the body is just one line and doesn't require the braces? More specifically, what is the error that can occur if you do not use braces? Hint: It's in the style guides.

4. Point us to a URL for a piece of published code whose style and design choices you found particularly pleasing or inspiring or learned something interesting from.  What made that code stand out to you?

5. You'll be writing a lot of code this quarter, which means lots of opportunity to practice and improve your craft. What aspects of coding quality are you most eager to improve upon in the code you will produce?  How can we help you reach those goals?
