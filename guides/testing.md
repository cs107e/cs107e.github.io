---
title: Test Strategy
---

*Written by Pat Hanrahan*


If you become a software engineer, people will rely on your software.
Bugs in software can be dangerous or even deadly. Users do not enjoy
using software that is buggy and crashes. Fixing bugs once the software
is released is very costly. Most importantly, good engineers take pride
in building things that work well and are robust.

In this course, the assignments build on themselves,
and you will be using the software 
you build in early assignments in later assignments.
Remember also we give a bonus if you hand in
the final assignment and it uses only your software 
(and not our reference implementations).

The key to writing working software is developing tests.
In this course we use an approach called test-driven development.
As you develop a module,
you first write a unit test that describes the expected behaviour,
then implement it and see if it passes this test.
This is also sometimes called test-as-you-go.
Testing as you go means starting with a simple first step,
testing it, and then moving onto the next step.
Each step adds more functionality.
The beauty of this approach is that each step is relatively
simple and easy to debug.
Imagine the opposite approach, where you write hundreds of
lines of code, this code does not work, and you need to figure
out what doesn't work!

In this course, you are expected to develop good unit tests for your software.
We evaluate your tests similarly to how we evaluate style,
using a bucket system.
Weak or limited tests fall into the [-] bucket,
solid tests are in the [ok] bucket,
and exceptionally comprehensive tests falls in the [+] bucket.
We will also provide comments on your tests to help you improve your testing
approach. 
Please incorporate our feedback into future assignments;
it will improve your grade and more importantly your effectiveness as a programmer.
We guarantee future employers will appreciate your ability to 
write tested code!

Here are some things we look for in good tests.

- Are the tests comprehensive? Is all the functionality tested?

- Did you test fundamental properties of a system? For example, if you
write to memory, then if you read memory it should return what you wrote.

- Did you anticipate potential problems, tricky cases, on boundary conditions?

- Did you develop the tests in a good order? Did you test basic functionality
before more advanced functionality? Did you take small, carefully chosen steps?

