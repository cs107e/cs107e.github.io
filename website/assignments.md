---
permalink: /assignments/
title: Assignments
---
### List of assignments
{% include itemlist.html type="assign" %}

### Assignment schedule
The quarter starts with a set of 7 weekly assignments. Each week's assignment is released after Wednesday lab and due before Tuesday lab of the next week. These assignments fit together to construct a complete working computer system of your very own -- so cool! You will use the final weeks of the quarter to design and complete a final project of your own choosing.

<A name="faq"></A>
### Common questions about assignments
---
*Written by Pat Hanrahan and Julie Zelenski*

#### What is the expected assignment workload?

Students in previous quarters report spending between 10 and 20 hours completing each of the weekly assignments.
The assignments build on each other,
and it is difficult to recover from skipping an assignment. 
The last assignment is a final project, 
and you will have two weeks to do the project. 
The final project is more open-ended than the assignments.
Most students will spend more than 20 hours on their final project.

#### What programming environment and tools are used?

Students will use their own laptop for the assignments.
If you use a Mac,
you will be using the Terminal program and command line tools.
If you use Windows,
you will install a subsystem running Ubuntu linux to use
 for your assignments.

When developing on the Raspberry Pi,
you will compile programs on your laptop and download them to the Raspberry Pi.
We use a suite of open-source cross-development tools for arm processors that include  `gcc` , `gdb`, and `binutils`.
Other tools such as `make` and `git` will also be used.
You will also be expected to become proficient with the unix shell and 
a unix text editor such as `emacs` or `vi`.

Assignments are distributed to students as `git` repositories.
You edit, compile, and debug programs in the local assignments repository on your laptop. When finished, 
you submit by creating a pull-request. Consult [Assignment 0](/assignments/assign0/) for information about using git.

When the pull-request is created, code is run through a sanity check 
to ensure the code compiles. Students should ensure their code aligns with 
specified [assignment directory structure](dir_structure) so it 
passes sanity check and may be evaluated by the graders. 

#### What is the policy on late assignments?

Students are given a few self-granted grace days, a penalty is applied for
further late work, and there is a hard deadline after which no further late
work will be accepted. Read our [course late policy](/policies/#late-policy)
for the details.

#### What is the assignment collaboration policy?

The programming assignments are to be done individually and should represent
independent, original work. We adhere to the Stanford and CS department Honor
Code policies, and offer specific examples of its application to CS107E
coursework in our [course collaboration
policy](/policies/#collaboration-policy).

The final project is usually done in teams of 2 or 3. If working in a team, all partners are expected to contribute equally to the effort.

#### How are assignments structured?
Each assignment writeup has a required basic part and various optional extensions. 

The __basic part__ is required of all students. This basic part follows directly from the material covered in lecture and lab. It has a tight specification to which your submission is expected to match precisely. We provide suggested steps to guide you through completing the basic part. 

The __extensions__ are optional add-ons that allow you to explore further and earn additional points. These are not required, but it is highly encouraged that you attempt some of the extensions. Extensions are given with a looser specification and much less guidance than the basic part. If tackling an extension, you will need to research advanced material on your own and apply your creativity in problem-solving.

In assignments 2 through 7, you will build up a library of useful utilities for bare metal programming on the Raspberry Pi.  These assignments build on one another. To complete assignment 5, say, you will need working versions of assignments 2 through 4. 

If you do not complete the basic part of an assignment,
you may have trouble completing the next assignment.
To help you in these situations, we provide our
reference implementations.
If your version doesn't work,
you can fall back on our version.

The last assignment requires you to use code from
many of the previous assignments.
To encourage you to complete all assignments and fix any bugs within them,
if you hand-in the last assignment using only your own code,
we will award you additional bonus points for building a complete working system. 

#### How are assignments graded?
Correctly completing the basic part requirements earns full functionality points. We test functionality by exercising your code against our grading tests.  For each bug or test failure, we apply a point-based deduction and file a github issue on your submission to report the problem. You may be able to earn back some of the points originally deducted by making a timely follow-up resubmission that resolves the issue.

We will also look at your code and assess the code quality.
Good code should be decomposed into small, clear, well-named functions so that 
no code is repeated and it is easy to follow the intent of each function.

We evaluate code quality using a bucket system. Code that needs 
improvement falls in the `-` bucket, code that meets our expectations is in the 
`ok` bucket, and exceptional code falls in the `+` bucket.
We will also provide comments on your code 
to help you improve your code quality.
Please incorporate our feedback into future assignments;
it will improve your grade and more importantly your coding style.



For details on course grades, see our [course grading
policy](/policies/#grading-policy).

