---
title: "Style Reflection Assign 5"
toc: true
attribution: Written by Ben Ruland
---

### Testing

Writing good tests and obtaining high test coverage is often overlooked. It isn’t as exciting as coding up the shiny new feature. Besides you’ve already got the feature made and it passes all the old tests, clearly it works so why fix what isn’t broke? There is lots to do, you know it’s important but you’ll get around to it later when things quiet down. There is no end of reasoning and excuses we will provide to avoid writing tests for our code. So we offer a different way to think about writing tests. Instead of thinking of testing as something separate from the code you write, think of it as part of your feature. It’s a bit like the bumper of your car, you don’t need it to drive but you do need it to say you have a whole car and not a pile of junk.

A common question that comes up is how much testing is enough testing? The clever answer is “there is never enough testing”, the technically correct answer is “100% coverage is achieved when you have tested every code path of your program”, and the practical response is “when you feel confident enough considering the risk imposed by your changes”. The practical response strikes a good bargain between correctness and productivity. If we chased after full coverage on every feature we would probably never ship the product, and if we ignored the risk our changes created and focused on features we would spend all our time fixing bugs and cleaning up damages.

### Risk

Let’s take a moment to talk about risk. Weighing risk vs reward is something we are all well familiar with in our lives. We do it many times everyday. It is something to get in the habit of applying to our programming as well. In a perfect world we would love to assume that the sky is the limit, whatever that complicated feature is it’s just a matter of how long will it take to program. But this isn’t the reality. There is always some amount of risk, which often plays out as how many bugs will this change introduce and how bad will the bugs be. For example, if you are working on some new notification window for an app if you make a mistake in this feature its highly unlikely there will be an outcome worse than the user being mildly annoyed when using your app. This is a pretty low risk change. However if you were say working on a library that nearly every part of the OS is built on, like say malloc, a mistake here would make the entire device unusable. The complexity of the change you make also has an impact on risk, which is why sometimes it can be preferable to go with a suboptimal yet simple solution over the optimal but really complicated one. So the greater the risk the more time we should spend validating and testing our code.

### Testability

While programming for your classes you will not usually need to think too hard about the testability of your code. This is because we have to grade your code and so have had to think about testability quite a bit already. All of your assignments are curated and designed to be testable. This will not always be the case, especially as you go out and work on large and highly complex code bases. So what makes code testable? There are a lot of good answers to this, we think many of them boil down to our ability to control the inputs to our process and observe its behavior and outputs. Breaking complex functionality down into many short and simple functions, using simple data structures, avoiding deeply nested loops, and removing edge cases are all practical ways of making your code easier to observe. It is often necessary to add functionality to process the outputs of your program into something more understandable and testable. Your heap dump function is a great example of this!

Controlling inputs to your program can be a very tricky challenge and sometimes requires a large amount of foresight and planning from the start. You have had a taste of this already when writing your gpio library, where it can be difficult to control memory so you can properly test your gpio_read. You might face it again when working on your keyboard, you’ll notice your keyboard is really good at reliably sending correct scan codes. It is hard to find a way to reliably send incorrect scan codes to test your programs behavior. Similarly, it is common to work with programs that rely on other programs for their input. You might not have access to the other programs source code or it might just be too complicated to force it to behave how you want, so it becomes necessary to design into your program the ability to mock up an input.

### Black Box and Clear Box Testing


By now we hope we have given you the impression that testing is a deep, complex, and interesting part of the field. There are many terms and concepts that describe different methods of testing. Here we will explain two main categories; black box testing and clear box testing. You are actually already fairly familiar with clear box testing as its the form of testing we have been using most commonly so far. It is a method of testing that targets the internal structures of your program. In other words, its when we write tests that assume some knowledge of how the internals of the program work. For example when writing malloc you might have 20 bytes of free space, and you know your header is 8 bytes so you know you can make a malloc call for 12 bytes and it should fill that free space. If, knowing all of this, you write a test that checks to see this happens then you have written a clear box test, because you have made assumptions about the underlying implementation. Clear box testing is not itself bad, in fact it can be a very useful and efficient method for testing, however it does have some pitfalls. One shortcoming is that it can lead to too stringent of tests, that is it can be easy to write a test that passes now but will fail later when you make a minor but correct change to the function. Another issue you might have run into; because this method relies on your understanding of the internal functionality, if you have a misconception of that functionality you’ll end up with an incorrect test. In fact such tests can be dangerous because they are likely to pass when they really shouldn’t. It can also be difficult to enumerate every possible combination of behaviors and outcomes with clear box testing.

This is where black box testing can be handy. Black box testing is the method of testing where we test only the functionality of a program or function and not its internal workings. With black box testing we control the inputs to our program or function and concern ourselves only with receiving correct outputs. This approach has a few strengths. It can be easy to enumerate many possible outcomes and it removes our bias and helps us catch errors stemming from our own misunderstandings of how things work. Black box testing is usually not good at giving detailed information on what exactly went wrong. Since we make no assumptions about how the program or function works we usually can only tell that something went wrong, not where or why. It’s up to the engineer to dig deeper with more targeted tests to determine what is failing, this is where clear box testing comes in handy.


## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign5` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers. If you find you've answered the questions and are left far short of a page then feel free to choose a related topic and tell us about it and how it applies to the code you have written so far. 


1. You should feel proud having printf and now malloc under your belt! We find that one of the best ways to learn and become better programmers is to take the time to review our code with a critical (but kind!) eye. Now that the dust has settled a little bit on printf, lets go back and review how we did and what we could improve on. We provide below a simple rubric below, review your `strings.c` and `printf.c` and tell us where you land on the rubric for each row. Feel free to add further reflections here as well. (Note how you asses yourself on the rubric is not going to reflect as a grade, this is meant to be instructive only!) 

2. Describe what you did to test the functionality of your padding function from the printf assignment. Tell us why the testing strategy you used here would not work well for testing malloc, include at least one concrete example to make your point.

3. We talk about clear box testing vs black box testing. Describe a single clear box test case to verify/observe recycling. Explain what this test confirms and what it doesn't confirm. Then tell us about what other test cases would be needed for confident coverage.

4. Take a look at our heap_workflow function. Read through the code and take the time to understand how it works. The function makes 4 different calls to report_problem, each one is detecting a different type of problem with your malloc implementation. Tell us what these 4 different problems are that it can detect and how it is able to make these detections. (Note you will have to dig a bit deeper than just the printout that each report gives). We are looking for a concrete explanation for each of the 4 cases. 

5. We like our heap workflow test, it tends to turn up a lot of bugs we hadn't caught earlier. If you experienced this yourself, tell us about the bug/s it caught for you and how it managed to catch those bugs where your other tests did not. If you didnt have this experience and it did not catch any bugs for you, then reflect on why this test helps us gain so much more confidence about our allocators corectness than our other tests like the one we just had you write in q2. Give at least two specific reasons why. 



<table border="1" cellspacing="0" cellpadding="8">
  <tr>
    <th>Category</th>
    <th>Great</th>
    <th>Okay</th>
    <th>Needs Improvement</th>
  </tr>
  <tr>
    <td><b>Character Handling</b></td>
    <td>
      Consistent use of char constants (e.g., '0' or '9') rather than raw ASCII values (48, 57). <br>
      Performs comparisons using an inclusive range (ch >= '0' && ch <= '9') rather than an exclusive range (ch > '/' && ch < ':'). <br>
      Uses helper functions such as <code>isdigit()</code> for digit conversion rather than subtraction (ch - '0').
    </td>
    <td>
      Mixed use of char constants and raw ASCII values. <br>
      Performs some exclusive range comparisons. <br>
      Does not use helper functions for digit conversion.
    </td>
    <td>
      Uses raw ASCII values, exclusive comparison ranges. <br>
      No use of helper functions.
    </td>
  </tr>
  <tr>
    <td><b>Numerical Processing Approach (num_to_string, strtonum)</b></td>
    <td>
      Processes numbers <b>right to left (R → L)</b> using division/modulus (div/mod), <br>
      rather than <b>L → R</b>, avoiding unnecessary use of <code>pow()</code>.
    </td>
    <td>
      Processes numbers <b>left to right (L → R)</b>.
    </td>
    <td>
      Processes numbers using switch-case statements <br>
      or if-statements for each possible digit value.
    </td>
  </tr>
  <tr>
    <td><b>Code Reusability (Hex & Dec Handling)</b></td>
    <td>
      Uses a <b>single</b> code path for both <b>hexadecimal and decimal</b> conversions, avoiding duplication.
    </td>
    <td>
      Some code duplication between hex and decimal handling.
    </td>
    <td>
      Completely separate and redundant implementations for hex and decimal processing.
    </td>
  </tr>
  <tr>
    <td><b>Code Reuse (strlcat)</b></td>
    <td>
      Calls <code>memcpy</code> within <code>strlcat</code> to avoid redundant code.
    </td>
    <td>
      Some use of <code>memcpy</code> in <code>strlcat</code> but still has some code duplication.
    </td>
    <td>
      Fully repeats logic instead of using <code>memcpy</code>.
    </td>
  </tr>
  <tr>
    <td><b>Code Reuse (strtonum)</b></td>
    <td>
      Calls <code>strtonum</code> for field width parsing to reuse code. <br>
      Makes use of <code>endptr</code>.
    </td>
    <td>
      Calls <code>strtonum</code> but does not make use of <code>endptr</code>.
    </td>
    <td>
      Does not call <code>strtonum</code>.
    </td>
  </tr>
</table>
