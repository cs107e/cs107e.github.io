---
title: "Style Reflection Assign 7"
toc: true
attribution: Written by Ben Ruland
---
{% comment %}
Style reflection given with assign 7 fall 2024

## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign7` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers.


---
https://edstem.org/us/courses/67356/discussion/5740519

1. After reading the section on assign 6 style, look over your code and tell us how you think you did in terms of style. Be specific, but also be kind to yourself in your assessment. Try to identify at least one thing about the code you submitted that you are particularly proud of, and one thing that you think could be improved.Usual

2. For purposes of reuse/unification, it can be desirable for the private implementation of a module to use functions already exported for public use. However, if this unification results in a significant performance hit, we may want to reconsider. The public function gl_draw_pixel includes bounds checking which is good for defending against client misuse. But when used internally this is adding significant overhead. Why is it totally unnecessary to bounds check in gl_clear? How could you implement proper clipping for gl_draw_char without running the check once for every pixel?

3. For this question we ask that you draw diagrams to show us the state of the program we describe, you are welcome to do an ascii diagram or to draw it on paper and add an image to your repo (png or jpeg only please).

You have a console that can display 3 lines, each line is 1 character wide. We have implemented an internal char buffer to reflect the contents of your console. This buffer uses a ring buffer implementation, the size of the buffer is 3 chars. We will give you a list of inputs, for each draw the state of the ring buffer after the input is placed into the ringbuffer. For each include pointers to where the ring buffer head is and where the ring buffer tail is. (The input is continuous, that is the ring buffer state does not reset in between listed inputs).

“A”

“ab”

“B”

“CDEF”

4. Review your gl_clear and jot down a couple of changes you think could make it faster. How could you try these changes out and measure the impact?

Performance Revisited

Previously we covered performance through the lens of premature optimization. We talked about being judicious in choosing when you optimize your code. In our gl module we have a perfect opportunity and use case for performance optimizations. When you enter the fun world of making computing go fast one of the first things you will notice is that highly optimized code looks horrible. Some common techniques, like loop unrolling, go directly against everything we teach you about having good style. But never fear, we are going to explore some great ways to maintain good style while optimizing your code.

Lets start by taking a look at one of the main culprits of our slow runtime, calling gl_draw_pixel from within gl_draw_rect or gl_draw_char. You might recall from our adventures observing the stack that whenever we make a function call we have to save all our registers onto the stack, and when we return we need to restore them. Writing and reading from the stack, while cheap, is not free. In most applications this overhead is negligible and we don’t even think of it. Writing the entire screen during our shell application would take over 500k calls to gl_draw_pixel. Each one of those function calls is incurring a handful of loads and stores in overhead, thats millions of instructions! So the simple solution here is to copy the functionality of gl_draw_pixel into our gl_draw_char and gl_draw_rect functions. But hey now we have duplicated code, that's bad style! We have two great options available to us solve our duplicated code issue, each with its own advantages. The first is to take advantage of our preprocessor and to write what we call a macro. You have probably written a few simple preprocessor macros already this term. Whenever you write #define you are making use of the preprocessor, it will do a simple find and replace of the value you defined with what you defined it to. So with some care we can write our gl_draw_pixel function as a macro,

#define gl_draw_pixel(x,y,c) {\
if (x < fb_max_width) && (y < fb_max_height){\
    my_fb[y][x] = c;\
    }\
}
This is really cool! We no longer have our code duplication issue and we solve the optimization problem. You might notice though that in declaring our macro we do not include any type information. In effect when you call this macro in your code the preprocessor is going to replace the call with the code defined in the macro before passing to the compiler. This means that we lose the type checking the compiler would normally do for us.

There is another way to achieve the same results with a bit more readability and keeping our type checking. We can use the inline keyword in our function declaration;

inline void gl_draw_pixel(int x, int y, color_t c) {
    /***** Your code goes here *****/
}
We can go about using our function as normal and the compiler will handle everything for us. Behind the scenes it will behave as if the code was just copy pasted at the call site rather than making a function call and affecting our stack! We really couldn’t ask for it to be more simple!

As it turns out many compilers provide a variety of special commands that allow you to give it guidance on what to do with your code. We call these compiler directives. In C they commonly start with #pragma. There are compiler directives for all sorts of things and each compiler will have its own set of directives it recognizes.

Using a Ring Buffer in Console

We think that the choice of using a ring buffer to store the character contents of your console is such a stylistically beautiful choice that we wanted to point it out here. There are a few things that come out of using the ring buffer. The first is that we do not have to ever worry about overflowing our buffer as our client continues to write to our console, that's a big win! Then we get scrolling almost for free! Instead of having to erase and rewrite the contents of our character buffer to mirror the screen, we can instead just shift which row of our ring buffer should be at the top of the screen and clear the last row. Wohoo! This is a great example of how a simple choice in data structure can greatly simplify the work we have to do.

-------

Below was final style reflection, not part of assign7, fall 2024
https://edstem.org/us/courses/67356/discussion/5838878

As mentioned in lecture this style assignment will be counted as extra credit. It will be due alongside your final projects.

This is the last style assignment of the quarter. As such we want to make this a reflection on the quarter as a whole. There is no reading for this week. Instead we ask that you take the time to answer our questions in depth as you consider your journey this quarter.

1. Reflect on all the code you wrote this quarter (you wrote quite a lot, you should be proud!). Identify a module/function of which you are particularly proud of. Be specific, include line numbers and filename. Tell us about what makes that piece of code special.

2. Similarly to the previous question. Tell us about a module/function you would most like to go back and rewrite if you had the time to do so. Again include line numbers and file. Tell us what you would change and why. What are some of the advantages/payoffs that could result from reworking the code? Explain your reasoning.

3. Write a letter to the future 107e students. Give them the style advice you think is most valuable to hear on the start of their 107e journey. We are looking for ½ to 1 page long letter here. We appreciate your thoughful responses and will collate the most inspiring and and helpful wisdom to share with the future classes -- Thank you!

{% endcomment %}