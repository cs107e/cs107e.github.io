---
title: "Style Reflection Assign 1"
toc: true
attribution: Written by Ben Ruland
---

### What is a style reflection?
Each assignment in this course has a companion __style reflection__. This is guided activity to enter into a dialogue with us to reflect on the quality of your code and work toward refining and improving your design skills.  The style reflection will typically consist of a short reading and a few prompt questions for which you are to submit written answers.

Your reflection is "graded", but in a fairly lightweight manner (we want to see that you are meaningfully engaging with these materials and have answered the prompts with consideration).  Please don't labor over the eloquence of your expression; we ask for simple language and in your own voice. We want to hear where you're at, what you're thinking, and how we can help coach you toward your goals.

Note: If there is a reflection task for which we want you to use an AI tool, we will let you know, otherwise assume we are asking to do your own reading, your own thinking, and your own writing.


## Why care about style?
Start with this quick fun read: [Landmarks in coding quality](../landmarks) from Stanford lecturer Nick Parlante. In addition to being hilarious, his advice is spot on.

Perhaps you already a true believer who cares deeply about style and are passionate about continuing to improve. Or...  you are open to leveling up your skills but need some coaching on where to begin. Or... kinda indifferent and find worrying about style to be a bit of bother. We want to meet you where you are and see how we can support you on your journey. Our ultimate goal is to inspire you to see style as an essential part of software development and an integral component of your work that you are motivated and proud to excel at.

Writing code is a craft; Donald Knuth titled his seminal works “The Art of Computer Programming” for good reason! (for a wonderful optional read: check out [Knuth’s Turing award speech ](https://www.cs.bilkent.edu.tr/~canf/knuth1974.pdf)) A good craftsperson takes pride in polishing and refining their work to achieve not only a functional result but an aesthetic one: a delicious and beautifully-presented meal, a sturdy chair that is comfortable and pleasing to sit in, a piece of tech that is elegant and easy to use. A well-crafted program that is straightforward to understand, modify, and maintain is truly a work of art. Writing elegant code to solve a difficult problem brings us an immense sense of pride and accomplishment; we want you to have that experience too.

Much of the code you write in university is a solo project for whom the only reader is a TA who offers minimal feedback on style. Given the limited audience, it can feel hollow to expend a lot of energy into style (never mind all the competing demands on your time).  This will change as you move into your careers.  Your code will be read by your coworkers, peer reviewers, project architect, boss, ... folks may be reading your code months and years after you wrote it!  When you submit your first pull request to your team lead, they are absolutely going to read it and will have no qualms rejecting it if doesn't meet the quality standards. Not a great look for a new hire! Use the learning opportunities here in college to develop strong sensibilities about style so you can become skilled at writing code that will not only pass muster, but be celebrated and serve as an example to others.



## Style in assembly

For assign1, you write a complete program in raw assembly; this is likely not a task you'll repeat. A systems programmer needs solid reading fluency with assembly but only rarely has to hand-construct it. Accordingly, we offer just two quick thoughts on style for assembly code.

- __Comment, comment, comment__

   Assembly programs are dense! The reader is having to follow along with none of the helping trappings of variables, functions, arguments, control statements, etc. __Comments are an absolutely necessity for assembly__.  Use them to guide the reader in how the assembly operates: document which registers are being used for which purpose, diagram how control is flowing, explain the values being used as magic numbers, and so on. It would not be unheard of to have an inline comment on most every instruction. If you were to return to this assembly program a month or year later to fix a bug or make a modification, what essential information would enable you to easily re-orient yourself?

   Although under-commenting is the more major concern, at the other extreme over-commenting can bury the reader in a heap of obscuring clutter that hurts more than it helps. Always assume the audience is a literate peer who is reasonably fluent at code reading. A comment should add value on top of what the code already says. Do not restate things that are obvious from reading the code or vacuous blather (e.g. "This instruction adds register a0 to a1 and puts sum in a2"). Also take care to update your comments to stay in sync with changes in the code. If the comment and code disagree, no telling which is wrong...

- __Assembly code is not indented__

    An assembly program listing should line up all instructions in one column. Labels are typically out-dented and there is no other indentation. In languages such as python and C, indentation and block structure dictate variable scope and control flow. But assembly is entirely unstructured! It has no concept of block or scope; registers are “global”, all control flow is explicit. Thus, indenting your assembly could be confusing or misleading to a reader.

## Style in C

You will be writing in C for the rest of the quarter. Whereas an assembly program can't help but be terse and cryptic, the flexibility expressiveness of the C language makes it possible to write much more readable code. However, this doesn’t occur just by happenstance, you must develop an intentional practice of good choices.

### Style guides
Most every course/research team/company adopts a general set of conventions for code style, often expressed as a list of “do” and “donts”. When you read a particular set of rules, you may personally disagree with some of the priorities, find some details to be nitpicky and pedantic or be perplexed that this shop’s conventions directly contradict another’s. However, writing code that ignores or transgresses the style guide of your organization will stand out like a sore thumb and is typically rejected and sent back for rewrite. After forcing yourself to align to the standards, you may later come to appreciate why those practices were chosen.

We have linked a few style guides below for you to peruse. The first two are written for students in a systems courses such as CS107e (our mainstream CS107 and CMU’s systems course). The last two are the industrial-strength guides used by Google and the linux kernel team. It is interesting to see how the pros do it!

- C style guide for our [CS107 course](https://cs107.stanford.edu/styleguide)
- C style guide from [Prof. Eno Thereska at CMU](https://users.ece.cmu.edu/~eno/coding/CCodingStandard.html#formatting)
- C++ style guide for [Google sources](https://google.github.io/styleguide/cppguide.html)
- C style guide for [linux kernel](https://www.kernel.org/doc/html/v4.10/process/coding-style.html)

As you peruse the guides, consider how these rules resonates with the style you have developed for yourself up to now. How much would you have to change as a new hire to adapt to the guidelines? Many of the practices may be ones you agree with and already have adopted. Others may be guidelines you hadn't previously considered but seem good practices to adopt. Perhaps there are rules you find overly rigid and chafe to think about being subject to them. Try to reason through the "why" behind the decisions, what benefit is gained, even if your yourself would have lobbied for a different choice.

### The value of code reading
The strength of the open source software community and legions of code publicly shared on the web (`github.com`, `bitbucket.com`, etc) present great code-reading opportunities for a young padawan. There is much you can learn by reading the code of others and reflecting on their choices.  Reading the work of a master artist can be so elegant it takes your breath away; the next thing you read is code so gnarly that you need to wash your eyeballs after. Think back to (or look to find) a piece of published code that you found particularly striking because of its style and design choices. It could be code you saw in 106B lecture, code offered as an answer to a question on stack overflow, a project you viewed on github, read in a textbook or an blog post, ... Perhaps the code had a novel approach to solving a problem or applied a programming technique that was new to you or you were simply struck by how tidy the code was and how easy it was to understand. If you were sharing this code with your peers, what would you tell them about why this code stood out to you?  What lessons can you take away from this code to adopt in your own coding practice?


## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign1` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers.

1. Tell us a little bit about your journey thus far in developing your programming style. What importance do you place on style?  Any memorable experiences you have had in giving or receiving feedback on style? A struggle, aspiration, or victory to share?

2. Review the C style guides linked above, pick a few points that stood out to you and tell us about them. It could be something you think is elegant, something you were surprised by, something you want to learn more about, or something you plan to adopt for yourself.

3. Why is it good practice to always enclose body of an if statement in braces, even if the body is just one line and doesn't require the braces? More specifically, what is the error that can occur if you do not use braces?

4. Point us to a URL for a piece of published code whose style and design choices you found particularly pleasing or inspiring or learned something interesting from.  What made that code stand out to you?

5. You'll be writing a lot of code this quarter, which means lots of opportunity to practice and improve your craft. What aspects of coding quality are you most eager to improve upon in the code you will produce?  How can we help you reach those goals?
