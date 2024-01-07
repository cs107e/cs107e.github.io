---
permalink: /assignments/
title: Assignments
---
## List of assignments
{% include itemlist.html type="assign" %}

## Assignment schedule
The quarter starts with a set of 7 weekly assignments. Each week's assignment is released after lab each week and due before the next week's lab. These assignments fit together to construct a complete working computer system of your very own -- so cool! You will use the final weeks of the quarter to design and complete a final project of your own choosing.

<A name="faq"></A>
## Common questions about assignments
---
*Written by Pat Hanrahan and Julie Zelenski*

### What is the expected assignment workload?

Students in previous quarters report spending between 10 and 20 hours completing each of the weekly assignments.
The assignments build on each other,
and it is difficult to recover from skipping an assignment. 
The last assignment is a final project, 
and you will have two weeks to do the project. 
The final project is more open-ended than the assignments.
Most students will spend more than 20 hours on their final project.

### What programming environment and tools are used?

Students will use their own laptop for the assignments.
If you use a Mac, you will be using the Terminal program and command line tools.
If you use Windows, you will install a subsystem running Ubuntu linux to use  for your assignments.

When developing,
you will compile programs on your laptop and download them to the Pi.
We use a suite of open-source cross-development tools for arm processors that include  `gcc` , `gdb`, and `binutils`.
Other tools such as `make` and `git` will also be used.
You will also become versed with the unix shell and using
a unix text editor such as `emacs` or `vi`.

Assignments are distributed to students as `git` repositories.
You edit, compile, and debug programs in the local assignments repository on your laptop. When finished, 
you submit by creating a pull-request. Consult [Assignment 0](/assignments/assign0/) for information about using git.

### What is the policy on late assignments?

In Winter Quarter 2024, assignments will be due weekly Tuesday 5pm. On-time submission is awarded a small bonus that boosts course grade. After the deadline is a 2-day grace period that allows late submission with no penalty. All submissions are expected by end of the grace period.  Read our [course late policy](/policies/#late-policy)
for specifics.

### What is the assignment collaboration policy?

The programming assignments are to be done individually and should represent
independent, original work. We adhere to the Stanford and CS department Honor
Code policies, and offer specific examples of its application to CS107E
coursework in our [course collaboration
policy](/policies/#collaboration-policy).

The final project is usually done in teams of 2 or 3. If working in a team, all partners are expected to contribute equally to the effort.

### How are assignments structured?
Each assignment writeup has a required basic part and various optional extensions. 

The __core features__ are required of all students. The work follows directly from the material covered in lecture and lab. It has a tight specification to which your submission is expected to match. We provide suggested steps to guide you through completing the core part.

The __extensions__ are optional add-ons that allow you to explore further and earn additional points. These are not required, but it is highly encouraged that you attempt some of the extensions. Extensions are given with a looser specification and much less guidance than the core features. If tackling an extension, you will need to research advanced material on your own and apply your creativity in problem-solving.

In assignments 2 through 7, you will build up a library of useful utilities for bare metal programming on the Pi.  These assignments build on one another. To complete assignment 5, say, you will need working versions of assignments 2 through 4.

If you do not complete the core part of an assignment, you may have trouble completing the next assignment.
To help you in these situations, we provide our compiled reference implementations.
If your code doesn't work, you can fall back on our version.

The last assignment requires you to use code from
many of the previous assignments.
To encourage you to complete all assignments and resubmit to fix your bugs,
if you submit the last assignment using only your own code,
we will award you additional bonus points for building a complete working system. 

<a name="grading"></a>
### How are assignments graded?

We evaluate your assignments through these three lenses:
1. __Functionality__ (_does it pass the QA tests?_)
2. __Code quality__ (_does my code achieve my team/tech lead's standards?_)
3. __Timeliness__ (_did I meet the program manager's schedule?_)

We test __functionality__ by exercising your code against our grading tests.  For each bug or test failure, we file a GitHub issue to report the problem. An issue is categorized with its priority, Priority 1 being the most important and decreasing from there.  Priority 1 "Essential" tests exercise core functionality that is critical to the system's operation. Your system won't be fully usable until these high priority issues are addressed. Priority 2 "Comprehensive" tests thoroughly exercise the full range of functionality including lesser-traveled features. A Priority 3 "Just Mention" would be used for small concerns as a misbehavior in an obscure edge case or minor issue of polish.

For full credit on the core functionality, you must have passing results on all of the Priority 1 tests. You are encouraged to revise and resubmit in order to address any Priority 1 issues.  Lower priority bugs can be addressed at your discretion. A higher percentage of passed tests boosts your assignment grade.

For __code quality__, the grader will read your code from the role of a team manager giving feedback before accepting the code into the team's repository.  We are expecting clear code that is readable, well-designed, and commented appropriately.  Good code is decomposed into small, clear, well-named functions so that 
no code is repeated and it is easy to follow the intent of each function. 

We evaluate code quality using a bucket system. Code that needs 
improvement falls in the `-` bucket, code that meets our expectations is in the 
`ok` bucket, and exceptional code falls in the `+` bucket.
We will also provide comments on your code  to help you improve your code quality.
Please incorporate our feedback into future assignments;
it will improve your grade and more importantly your coding style.

Effectively managing your time to hit deadlines with working code is an accomplishment that we reward in the __timeliness__ category. Your timeliness score is boosted by making your submissions pass tests on-time (or within the grace period).

For details on overall course grades, see our [course grading policy](/policies/#grading-policy).

## What is the revise and resubmit policy?
Many of the assignments in this course build upon each other. It is a good idea, both for your learning and your grade, to revise and resubmit old assignments after seeing what tests they fail.

For a failed test case on core functionality, we will file a GitHub issue to report the bug. You are encouraged to review your issues and rework your code to fix the underlying bug(s). When you submit the revised code, we will run the tests again and update to show the now-passed results. 

The final opportunity for assignment revise and resubmit will be with the submission for Assignment 7. (Hint: don't wait til last minute -- much better to get those bug fixes in early rather than build up a big backlog!)

