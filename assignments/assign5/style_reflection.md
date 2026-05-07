---
title: "Style Reflection Assign 5"
toc: true
attribution: Written by Ben Ruland
---

### Testing

Writing good tests and ensuring comprehensive coverage is often overlooked. It isn’t as exciting as coding up the shiny new feature. You know it’s important but there's lot to do and you plan to get to testing later when things quiet down. You figure that QA will take care of the testing for you. There is no end of rationalizations engineers use to avoid writing their own tests. We offer a different way to think about writing tests. Instead of thinking of testing as something separate from the code you write, think of it as part of your feature. It’s a bit like the bumper of your car, you don’t need it to drive but you do need it to say you have a whole car and not a pile of junk.

How much testing is enough? The clever response is “there is never enough testing”, the technically correct answer is “100% coverage is achieved when you have tested every code path of your program”, and the practical response is “when you feel confident enough considering the risk imposed by your changes”. The practical response strikes a good bargain between correctness and productivity. If we chased after full coverage on every feature we would probably never ship the product, and if we ignored the risk our changes created and focused on features we would spend all our time fixing bugs and cleaning up damages.

### Risk

Let’s talk about how as humans we weigh risk versus reward and how it might apply here. When your boss asks you whether you can squeak a new feature into the upcoming release, you might consider only how long will it take to write the code. But writing the code isn't the whole job . There is always some amount of risk, what bugs might this change introduce and how bad will the bugs be. For example, if you are adding a new notification alert to your app and your code has an error, it's highly unlikely the outcome will be much worse than the user being mildly annoyed. This is a pretty low-risk change. However if you were making changes to a library that nearly every part of the OS is built on, like say malloc, a mistake here would make the entire device unusable. The complexity of the change also has an impact on risk, which is why you may prefer a a smaller, simpler change that is a bit suboptimal but low-risk over a major upheaval that that optimal fix that is complex and high-risk.

### Testability

While writing code for a course, its testability is likely already baked into the spec, as assignments are curated and designed to be testable for grading purposes. This will not always be the case, especially when working on large complex code bases. What makes for testable code? Some of it boils down to our ability to control the inputs to our process and observe its behavior and outputs. Breaking complex functionality down into many short and simple functions, using simple data structures, avoiding deeply nested loops, and removing edge cases are all practical ways of making your code easier to observe. It is often necessary to add functionality to process the outputs of your program into something more understandable and testable. Your `heap_dump` function is a great example of this!

Controlling inputs to your program is not always easy or obvious. You got a taste of this when writing your gpio library, where it can be difficult to control memory so you can properly test `gpio_read`. You'll face it again when trying to test the error-handling on your keyboard driver. The keyboard is quite reliable at sending well-formed scancodes. It is hard to force the keyboard to send an incorrect scancode, you may have to resort to messing with its physical connections to generate a blip or planted a forced error. For our grading tests, we wrote a mock PS/2 device that to generate bad scancodes on demand! Similarly, it is common to work with programs that rely on other programs for their input. You might not have access to the other program's source or it might be too complicated to force it to behave how you want, so it becomes necessary to design into your program the ability to mock up an input.

### Black Box and Clear Box Testing


Testing is a deep, complex, and interesting part of software engineering. Some of the vocabulary used for testing we want to introduce to you is __black box__ testing and __clear box__ testing. Clear box testing has been the form of testing you have been using most up to now. Clear box test cases are written using internal knowledge of the function under test. For example, when writing malloc you might have 20 bytes of free space, and you know your header is 8 bytes so you know you can make a malloc call for 12 bytes and it should fill that free space. If, knowing all of this, you write a test that checks to see this happens then you have written a clear box test that makes assumptions about the underlying implementation. Clear box testing is not itself bad, it's often very useful and efficient, however it does have some drawbacks. One shortcoming is that it can lead to too stringent of tests, that is it can be easy to write a test that passes now but will fail later when you make a minor but correct change to the function. Another issue you might have run into; because this method relies on your understanding of the internal functionality, if you have a misconception of that functionality you’ll end up with an incorrect test. In fact such tests can be dangerous because they are likely to pass when they really shouldn’t. It can also be difficult to enumerate every possible combination of behaviors and outcomes with clear box testing.

This is where black box testing can be handy. A black box test depends only on the observed public functionality of a program or function and not its internal workings. With black box testing we control the inputs to our program or function and concern ourselves only with receiving correct outputs. This approach has a few advantages. It can be easy to enumerate many possible outcomes and it removes our bias and helps us catch errors stemming from our own misunderstandings of how things work. Black box testing is usually not good at giving detailed information on what exactly went wrong. Since we make no assumptions about how the program or function works we usually can only tell that something went wrong, not where or why. It’s up to the engineer to dig deeper with more targeted tests to determine what is failing, this is where clear box testing comes in handy.


## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign5` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers. If you find you've answered the questions and are left far short of a page then feel free to choose a related topic and tell us about it and how it applies to the code you have written so far. 


1. You should feel very proud having printf and now malloc under your belt! We find that one of the best ways to grow as programmers is to take the time to review our code with a critical (but kind!) eye. Now that the dust has settled on printf, go back and look at how you did and what you could improve on. We provide a simple rubric below, review your `strings.c` and `printf.c` and tell us where you land on the rubric for each row. Feel free to add further reflections here as well. We are looking for your honest evaluation and want to be clear that the assessment you give yourself is not your grade :-)

2. Describe what test cases you used to confirm the padding features of `printf`. Tell us why a similar testing strategy would not work well for testing `malloc`, include at least one concrete example to make your point.

3. We talk about clear box testing versus black box testing. Describe a single clear box test case to verify/observe malloc recycling. Explain what this test confirms and what it doesn't confirm. Then tell us about what other test cases would be needed for confident coverage.

4. Consider our provided `heap_workflow.c` program. Read through the code and take the time to understand how it works. The function makes 4 different calls to `report_problem`, each one is detecting a different type of problem with your malloc implementation. Tell us what the 4 different problems are and how the workflow program is able to detect each. (Note you will have to dig a bit deeper than just the printout that each report gives). We are looking for a concrete explanation for each of the 4 cases.

5. We like our heap workflow as a test program; it tends to turn up a lot of bugs not caught earlier. If you experienced this yourself, tell us about the bug(s) it surfaced and how it managed to catch those bugs where your other tests did not. If you didn't have this experience and it did not catch any bugs for you, then reflect on why this test provides more confidence about the correctness of your allocator than test cases like those you wrote for assign2 and assign3. Give at least two specific reasons why.



<table border="1" cellspacing="0" cellpadding="8">
  <tr>
    <th>Category</th>
    <th>Great</th>
    <th>Okay</th>
    <th>Needs Improvement</th>
  </tr>
  <tr>
    <td markdown="span">__Character handling__
    </td>
    <td markdown="span">Consistent use of char constants (e.g., '0' or '9') rather than raw ASCII values (48, 57).
    Prefer inclusive range to identify alphabet/digits `ch >= '0' && ch <= '9'` rather than exclusive `ch > '/' && ch < ':'`.
    Adds helper functions for ops to convert char to digit value, check `isdigit`/`ishexdigit`, convert case `tolower`, etc.
    </td>
    <td markdown="span">
      Inconsistent use of char constants and raw ASCII values. <br>
      Inconsistent ranges. <br>
      Some helper functions.
    </td>
    <td markdown="span">
      Uses raw ASCII values. <br>
      No helper functions.
    </td>
  </tr>
  <tr>
    <td markdown="span">__Number processing__ within `num_to_string` `strtonum`</td>
    <td markdown="span">
      Processes digits __right to left (R → L)__ allow simple use of div/mod operators
    </td>
    <td markdown="span">
      Processes digits __(L → R)__ requires implement `pow`
    </td>
    <td markdown="span">
      Processes digits using switch-case or if-else
      for each possible digit value.
    </td>
  </tr>
  <tr>
    <td markdown="span">__Code unification__ within `num_to_string` `strtonum` </td>
    <td markdown="span">
      Uses a __single__ code path for converting both hexadecimal and decimal, parametrized by base
    </td>
    <td markdown="span">
      Some code unified between converting decimal and hex, but not all
    </td>
    <td markdown="span">
      Completely separate and redundant implementations for converting decimal and hex.
    </td>
  </tr>
  <tr>
    <td markdown="span">__Code reuse__ within `strlcat`</td>
    <td markdown="span">
      `strlcat` calls `strlen` and `memcpy` for reuse, no duplicate code
    </td>
    <td markdown="span">
       `strlcat` is a mix of reuse and duplicate code
    </td>
    <td markdown="span">
       `strlcat` fully repeats logic/code
    </td>
  </tr>
  <tr>
    <td markdown="span">__Code reuse__ within `snprintf`</td>
    <td markdown="span">
      `snprintf` calls `strtonum` to parse field width from format string, calls `memset` to create padding bytes, calls `strlcat` to append to output string
    </td>
    <td markdown="span">
      `snprintf` is a mix of reuse and duplicate code
    </td>
    <td markdown="span">
      `snprintf` fully repeats logic/code
    </td>
  </tr>
</table>
