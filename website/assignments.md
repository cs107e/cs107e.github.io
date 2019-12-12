---
permalink: /assignments/
title: Assignments
---
### List of assignments
{% include itemlist.html type="assign" %}

### Assignment schedule
The quarter starts with a set of 7 weekly assignments. Each week's assignment is released after Wednesday lab and due before Tuesday lab of the next week. These assignments fit together to construct a complete working computer system of your very own -- so cool! You will use the final weeks of the quarter to design and complete a final project of your own choosing.


### Common questions about assignments
---
*Written by Pat Hanrahan*

**What is the expected assignment workload?**

Students in previous quarters report spending between 10 and 20 hours completing each of the weekly assignments.
The assignments build on each other,
and it is difficult to recover from skipping an assignment. 
The last assignment is a final project, 
and you will have two weeks to do the project. 
The final project is more open-ended than the assignments.
Most students will spend more than 20 hours on their final project.

**What programming environment and tools are used?**

Students will use their own laptop for the assignments.
If you use a Mac,
you will be using the Terminal program and unix command line tools.
If you use Windows,
you will install a virtual machine running Ubuntu linux to use
 for your assignments.

When developing on the Raspberry Pi,
you will compile programs on your laptop and download them to the Raspberry Pi.
We use a suite of open-source cross-development tools for arm processors that include  `gcc` , `gdb`, and `binutils`.
Other tools such as `make` and `git` will also be used.
You will also be expected to become proficient with the unix shell and 
a unix text editor such as `emacs` or `vi`.

Assignments are distributed to students as `git` repositories.
Students edit, compile, and debug their assignments on their laptop.
When they are finished, 
they hand in the assignment by committing their files 
into their personal git repository 
and creating a pull-request. Consult [Assignment 0](/assignments/assign0/) for information about using git.

When the pull-request is created, code is run through a sanity check 
to ensure the code compiles. Students should ensure their code aligns with 
cs107e required [assignment directory structure](dir_structure) so it 
passes sanity check and may be evaluated by the graders. 

**What is the policy on late assignments?**

Students are given a few self-granted grace days, a penalty is applied for
further late work, and there is a hard deadline after which no further late
work will be accepted. Read our detailed [late policy](/policies/#late-policy)
for the nitty-gritty mechanics.

**What is the assignment collaboration policy?**

The programming assignments are to be done individually and should represent
independent, original work. We adhere to the Stanford and CS department Honor
Code policies, and offer specific examples of its application to CS107E
coursework in our [course collaboration
policy](/policies/#collaboration-policy).

The final project is usually done in teams of 2 or 3. If working in a team, all partners are expected to contribute equally to the effort.

**How are assignments graded?**

Each assignment will have a clear specification of what the program should do. We will thoroughly test the required functionality of your submission and identify any issues that need to be corrected.

We will also look at your code and assess the code quality.
Good code should be decomposed into small, clear, well-named functions so that 
no code is repeated and it is easy to follow the intent of each function.

We evaluate code quality using a bucket system. Code that needs 
improvement falls in the [-] bucket, code that meets our expectations is in the 
[ok] bucket, and exceptional code falls in the [+] bucket.
We will also provide comments on your code 
to help you improve your code quality.
Please incorporate our feedback into future assignments;
it will improve your grade and more importantly your coding style.

Assignments will have a required basic part and various optional extensions.  You are expected to do the basic part of each assignment, and are encouraged to attempt some of the extensions.
You can do as many extensions as you want to accumulate additional points.

If you do not complete the basic part of an assignment,
you may have trouble completing the next assignment because the assignments build on one another.
To help you in these situations, we have written
reference implementations of the libraries you need.
If your version doesn't work,
you can fall back on our version.

The last assignment requires you to use code from
many of the previous assignments.
To encourage you to complete all assignments and fix any bugs within them,
if you hand-in the last assignment using only your own code,
we will award you bonus points for building a complete working system. 

**How are course grades determined?**

Your course grade is a combination of your participation in lab and your work on the assignments and final project. 

If you have your sights set on earning an __A__ course grade, aim to:
+ Attend all labs 
+ Assignments 
    - Basic requirements are complete and fully-functional for all assignments
    - In addition, 3 or more assignment extensions successfully completed
    - Earn the full system bonus for using all your own code on the last assignment
    - Code quality (style and tests) at/above the [ok] bucket
+ Outstanding final project, excellent execution

For a __B__ course grade, you need __solid__ work through the quarter:
+ Attend all labs
+ Assignments
    - Basic requirements are complete and mostly fully-functional for all assignments
    - At least 1 assignment extension attempted/completed or full system bonus achieved
    - Code quality (style and tests) trending to/above [ok].
+ Good final project

Work that is not completed satisfactorily will earn grades **C and below**.

