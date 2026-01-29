---
title: "Style Reflection Assign 3"
toc: true
attribution: Written by Ben Ruland
---

## Struct overlay

When working with a memory-mapped peripheral such as the Mango Pi gpio, your code ends up littered with various addresses and pointer arithmetic offsets, magic numbers that you dug out the manual. A neat technique for taming some of that complexity is to define a C-struct whose fields match the layout of the registers. This leads to more expressive and cleaner code, fewer magic numbers and a lot less pointer arithmetic. Easier to write and easier to read, a win all around! Let’s take a look at an example.

Imagine a device that has four 32-bit (4-byte) registers. The first two are config registers, the third is a control register, and the fourth is a data register. The address of the first config register is at `0x1000`, each subsequent register is on offset of 4 from the previous. We can define a matching struct for the device registers as follows and initialize one pointer to the base address.

```
struct device {
 uint32_t config[2];
 uint32_t control;
 uint32_t data;
};

struct device *dev = (struct device *)0x1000;
```

To set the device's control register, all you need is:

```
dev->control = val;
```
How great is that?! All the pointer arithmetic just happens for us. How about updating the second config register to mask off the least significant bit?
```
dev->config[1] &= ~0x1;
```
That’s pretty nice. Accessing the config array at index 1 will start from the base address of config array within the struct (`0x1000`) and add the size of one `uint32_t` (4 bytes) to get to the second config register `0x1004`; all of this done for us by the compiler.

A common scenario in memory-mapped peripherals is to have a sequence of devices with same registers arrayed in contiguous memory, such as the GPIO pins with their different groups; `PB`, `PC`, `PD`, and so on. Each group has identical memory layout of registers, with minor tweaks in how many indexes the group makes use of. We can define a struct for a group, and then overlay an array of the group structs across all. Taking our example above, imagine several of the device, each with same layouts, laid out in contiguous memory. We can treat as an array of structs. Accessing the data register of the second device would look like
```
struct device *all_devs = (struct device*)0x1000;
uint32_t data = all_devs[1].data;
```
We encourage you to sketch out the memory layout on paper and trace through the code to convince yourself of how and why it works. If you get stuck or aren’t sure come find us in office hours!

## Organizing your program data

Every program operates on data: arrays, structs, pointers, strings and more. The choices we made in how we organize that data can make a profound difference in the cleanliness, efficiency, and elegance of the code we write. We have already seen one example in the previous section, choosing to represent memory-mapped peripheral as one array of structs makes for much simpler code than managing than a large list of independent registers. Let’s look at another example, the clock application.

In the clock application, the program needs to track the countdown time. How we choose to represent that time informs how we code the rest of the module. One option is to represent time as minutes and seconds. This is a fine choice and even arguably the one most intuitive to a human. A different way we can choose to represent time is as a single variable, seconds. So if we start a countdown for 1 minute 20 seconds, we represent it internally as a single variable of 80 seconds. When we need the data in another form we simply do some math to convert from seconds into another time unit, hours or minutes as needed. A human wouldn't find 278 seconds a particularly intuitive way to represent 4 minutes and 18 seconds, but a program would likely benefit from this design choice.

### Simplicity
First there is the simplicity of it, simple data structures lead to easier to write code, less bugs, and efficient implementations. This is a common theme throughout engineering, the simplest solution that meets the requirements is often the best solution. One variable is simpler than two.

### Avoid state duplication
Second, we avoid duplicating state. Storing separate values for minutes and seconds means two pieces of state are tracking the same thing - time. If you were trying to add 10 minutes to the time, would you need to edit the seconds variables, the minutes variable or both?  What if you are trying to subtract 10 seconds from the time? We have defined what appears to be two independent variables, but in fact the two are bound up together and will generally need to be operated on as a pair. The relationship of seconds to minutes is implicit in how we represent the data. But if we store in seconds and write a function to convert to minutes as needed, we have made it explicit. This point might seem pedantic in such as simple example, but in a large codebase with a lot of complex data, being explicit about the relationships is necessary. We encourage you to think of examples you might have already seen where this could be useful!

### Lazy Computation: 
Finally, the choice of storing our time as seconds embodies a principle everyone loves - laziness. While procrastination doesn't always work out for us humans, it's a great policy for a program to be lazy about its computations. Modern computers have oodles of computing power, but a very small number of registers and the operations to load and store to memory are quite slow. Thus we usually prefer to store the minimal amount of state and compute all other values on demand and only when needed. (This choice also maintains simplicity and avoids state duplication, yeah!). Being lazy and waiting until we need the value has the added benefit of avoiding unnecessary computations. Consider how this could work for the clock application.  If we store time as seconds and minutes, we are repeating the math to convert seconds into minutes each time we update the time, which is not very lazy of us. But storing as seconds lets us push that computation downstream, we do the math only when needed i.e  to display the time. Hooray for laziness!

There are tradeoffs to consider in any arrangement, and no one best answer for all situations. A good place to start is aim for the simplest and most minimal approach that supports the program's needs. If we find ourselves having to jump through lots of complicated hoops to access our data then we typically will take a pause and consider whether there is a better way to represent the data we are working with.

## Testing 

Testing is a broad topic that we will be touching on throughout this course. If there is one thing we can convince you of it is that you can never have enough test coverage, too many tests, nor spend too much time designing your tests.

While writing your gpio module, many of you experienced first-hand the challenges of writing robust test cases. You probably noticed that in order to test the `gpio_write` function, you depend on a working `gpio_read`, and to test `gpio_read` you need a working `gpio_write`. Circular dependencies like this are the pits. It's all too easy to make a bug in `gpio_read`, and put the same/symmetrical bug into `gpio_write`. The two bugs perfectly cancel each other out in such a way that your assert-based test cases all pass yet nothing actually works! What good is a test case if it can't actually detect the bug?

### Know your test case
Writing good test cases is hard, yet spending hours of your time to find a problem created by a malformed or ignorant test case is even worse. We want to stress the importance of taking your time and being thoughtful about how you write your tests. Be sure you understand what each test case can and cannot verify. Does the validity of the test case depend on some code that has not yet been established as trustworthy? What options do you have to remove such a dependency? For gpio, a simple solution could be to wire up an led and use `gpio_write` to light it up. That's solid proof! Once we confirm `gpio_write`, we can now use it to test our `gpio_read` with confidence. So the takeaway here is that we always want to start our testing from a place we can trust, and sometimes that means stepping back and taking a moment to test the test case itself.

Malformed test cases are all too easy to come by. A test case can output a false positive (passes when code should fail) or output a false negative (fails when code is correct). A false negative is easier to resolve since we see the failure and go investigate. But a false positive is silent but deadly; we remain blissfully unaware of the problem. Other test cases might be correctly written but only for certain inputs or their operation has a hidden dependency on state set by a previous test case and so on. Testing is complicated!  It helps when writing a test to stop and ask yourself a few questions. What exactly am I verifying with this test? How can I be sure this test will prove that? Are there any ways my program can come to this result that I haven't accounted for in my test? Could my program have unwanted side effects that I am not observing in this test?

### Test as you go
During assignment 2, we see a number of students who have implemented the gpio module, given it a quick testing once-over, and then launch into clock. If the clock application then doesn't work, is the problem in the clock code or the gpio module?  Hard to say. If instead you have done thorough testing on the gpio module as you were developing it, you are able to start on clock with confidence that you have a solid foundation to build on.  Testing your clock in steps as you go is important too. Imagine writing the entire countdown clock in one go; you run it and random segments light up in crazy patterns. Where is the bug?  Instead consider writing code to drawing a single digit on the display and stop. Running this program, you can verify the right segments light up and debug any problems.

Finding a bug can be like looking for a needle in a haystack. Imagine adding a little bit of hay (code) to the haystack and that code has a bug in it.  If you stop and take a look now, the bug should be right on top in the last bit of hay you just added. But if you keep writing more and more code, you're pitching a bunch more hay onto your haystack, burying your bug! It then becomes a lot harder to find because you don't know when and where the bug was introduced.

Finally, on the subject of testing we encourage you to break down your tests into small manageable cases. Not only does this help you pinpoint your bugs, but it also makes it much easier to write your tests and get them correct!

## Prompt questions
We noticed in the first couple assignment reflections that some responses were a bit sparse. We expect the entire style reflection to be about one page of text. We aren't going to be pedantic and apply a minimum word count, but we do want to see your sincere and thoughtful responses. If you find you've answered the questions and are left far short of a page then feel free to choose a related topic and tell us about it and how it applies to the code you have written so far.

Edit the text file named `style_reflection.txt` in your `assign3` directory to include your responses to the following questions. The entire document should be about one page. Please do not repeat the question prompts in the text file, just your answers.

1. Define a struct to represent the gpio registers and write the code for `gpio_get_function` that uses your struct. (copy and paste the code into style_reflection.txt)

2. Tell us how you choose to represent time in your clock application. What is one task you think would have been easier had you chosen a different implementation and what is one task that you think your implementation made easy?

3. a. Review the issues posted against your gpio module. If you passed all tests (i.e. no issues posted) skip to 3b. Pick one of your gpio issues. Review the information in the posted issue and try to sort out the cause of the test failure. What do you think is missing in your testing that allowed this bug to slip past you? Write a test case you could add to `test_gpio_timer.c` that would detect this bug and could later be used to confirm your fix for the bug.

    b. (optional if you answered 3a, but we recommend doing this anyways, it's a good one :D ). The strings and printf modules need to correctly read and write memory in all situations. It can be tricky to track down the bugs where these functions misbehave in their treatment of memory. Consider the provided test for the `memset` function. It confirms that `memset` wrote the value to the correct memory locations, but it does not also check that the function didn't write elsewhere.

    First tell us why we should care about verifying that `memset` is not writing anywhere besides where we told it to.

    Provide code for a test case that verifies that `memset` does not write anything into the 8 bytes before and after the memory we have designated it to write in.

