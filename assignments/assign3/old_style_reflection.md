---
title: "Style Reflection Assign 3"
toc: true
attribution: Written by Ben Ruland
---


## Prompt questions
Edit the text file named `style_reflection.txt` in your `assign3` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers.

1.

-------------

https://edstem.org/us/courses/67356/discussion/5547980

Here is the Assign2 style prompt and reading. Since we are getting it out a bit late we are extending the deadline (normally 11:59pm Thursday) to 11:59pm Friday 10/25

To submit, create a file in assign2 folder named style.txt and push an assign2-style flag

We noticed in the first assignment reflections that some responses were a bit sparse. We want to be clear we are looking for a whole page written reflection to the prompt questions, and the copied questions don't count towards that length. We aren't going to be so pedantic as to look at your word count, but we are going to be looking to see you gave it a reasonable effort.

Prompt:

1. After reading the section on assign 2 style, look over your code and tell us how you think you did in terms of style. Be specific, but also be kind to yourself in your assessment. Try to identify at least one thing about the code you submitted that you are particularly proud of, and one thing that you think could be improved.

2. After reading the section on structs, write a struct to represent the gpio registers and show us how you would write gpio_get_function using your struct. (be sure copy and paste your code into the style.txt file)

3. After reading the data structures section, think back on how you implemented the clock. We list two options for how you might have represented time. Tell us how you chose to represent time, what is one task you think would have been easier had you done a different implementation, what is one task that you think your implementation made easy.

4. After reading the section on scope, review your clock code and tell us how many global variables you have. For each of your global variables, consider whether it should remain a global variable. If you think it should then make a short case as to why, if not then tell us how you would change it (i.e I’d make it a local variable in my display refresh loop, etc)


Reading:

Structs

When working with memory mapped io, structs are our best friend. Using a struct we can model the actual structure of the memory we are working with. This typically leads to much cleaner code that requires less pointer arithmetic. It's easier to write and easier to read, a win all around! So let’s take a look at an example.

Pretend we are working with a device that has four 32 bit registers. The first two are config registers, the third is a control register, and the fourth is a data register. The first config register starts at address 0x0040. We can create a struct for this as follows:


struct device {
 uint32_t config[2];
 uint32_t control;
 uint32_t data;
};

struct device* myDevice = (struct device*)0x0040;

So what does this get us ?
Say I want to access and write the control register, all I have to do is:

myDevice->control = someValue;

How great is that?! All the pointer arithmetic just happens for us. How about the second config register?

uint32_t secondConfig = myDevice->config[1];

That’s pretty nice. When we axess the config array at index 1 what we are really doing is taking the base address the struct starts at 0x0040 and adding the size of one uint32_t (4 bytes) to get to the second config register 0x0044. Now a common scenario in memory mapped IO is to have several devices with identical memory layouts right next to each other. Just like our GPIO pins with their different banks; PB, PC, PD, and so on. Each bank is set up with the same registers, with some minor differences in how many config registers they contain. We can leverage our struct to work smoothly with this kind of layout as well. Imagine we have a few of the devices from the earlier example, each with identical layouts. We can treat them as an array of structs, so accessing the data register of the second device would look like

struct device* myDevices = (struct device*)0x0040;
uint32_t deviceTwoData = myDevices[1].data;
We encourage you to mentally work through this code and convince yourself of why it works. If you get stuck or aren’t sure come find us in office hours!

How We Represent Our Data


Our code is often dictated by the data we work with. Our choice of how we structure that data can make a huge difference in the cleanliness, efficiency, and elegance of the code we write. We have already seen one example in the previous section, choosing to represent our memory as a struct. Let’s look at another example we should all be familiar with, the clock module we wrote.

In the clock module the data we are working with is time. How we choose to represent that time informs how we code the rest of the module. One option is for us to represent time as minutes and seconds. This is a fine choice and even arguably the most intuitive one that first leaps out at us. A different way we can choose to represent time is as a single variable, seconds. So if we receive a countdown for 1 minute 20 seconds, we represent it internally as a single variable of 80 seconds. Each option has an impact on the logic required to produce a working clock countdown.

It isn’t always clear what the best way to represent your data will be, there is a whole field devoted to this after all. However we have found that keeping our data structures as simple and minimal as necessary is a great place to start. If we find ourselves having to jump through lots of complicated hoops to access our data then we typically will take a pause and consider whether there is a better way to represent the data we are working with.


## More with less
Elegant code is often about doing more with less. This doesn’t necessarily mean compacting into the fewest lines of code or most terse expressions. (Playing “code golf” is always certainly guaranteed to result in unreadable code, please no!) But if your code repeats itself or does several very similar tasks, re-factoring to share the common code means less code to write and thus less code to test, less code to debug, less code to comment– a win all-around! Another goal for “less” is to reduce complexity and complication where possible, such as streamlining to the just-right amount of control flow statements, thinning out or decomposing deeply nested constructs, and avoiding overly complicated logic in favor of less complex designs.

By control flow statements we mean constructs that change what executes next in your program; if, else, for, while, switch etc. What do we mean by the “just-right” amount of control flow? Well this is where programming really becomes more of an art and a craft, there is no good one size fits all answer. The best we can say is to look at how others have written similar things, try imagining how you can clean something up, and form that intuition for what your best looks like. But why does this make for good code? A few major reasons, first the less control flow and complicated logic the easier it is to read the code and follow exactly what it is doing. Second, each complication you add to the control flow creates more surface area for errors along with requiring more test cases to ensure complete code coverage. Third, debugging clean simple code is a breeze while trying to step into a deeply nested set of logic can be very painful. As we mentioned, this is an esoteric concept that is not easily measured and often takes time and experience to learn. So be patient and ask plenty of questions, before you know it folks will be admiring your elegant solutions!

