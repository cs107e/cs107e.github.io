---
layout: standalone
title: "CS107E: Computer Systems from the Ground Up"
---

# Computer Systems from the Ground Up
---

<style>
.orange-bg {
    background-color: #FFC87C;
}

.orange-bg li {
    font-size: small;
}

.pink-bg {
    background-color: #F1A6BD;
}

</style>

<div class="alert pink-bg" markdown="1">
<img src="mango.png" width=36> __Calling Mango Pi-oneer alums:__ If your MangoPi is gathering dust, please consider recycling it with us!  Reach out to Julie to learn how it will help.
</div>

<a name="enrollment"></a>
<div class="alert orange-bg" markdown="1">
### __Welcome to prospective students of CS107E academic year 2025-2026!__

CS107E is scheduled to be taught in __Winter and Spring quarters of 2025-26__. This year we will be continuing the re-design of the course for a __Risc-V__ processor. Our chosen SBC is the spunky pink [Mango-Pi MQ-Pro](https://mangopi.org/mangopi_mqpro).

<a name="enrollment"></a>
Enrollment timeline for __Winter Quarter 2026__:

- Enrollment will be settled __before the start of the quarter__ (during fall-winter break) to allow students to plan their schedules.
- We plan to hold an informal info session in December 2025 for prospective students. Come hear more about the course and bring your questions! If you think the course is a good fit, be sure to submit the questionnaire to be considered for enrollment.
- Enrollment is limited to 40 students. We use a student questionnaire for selection.
    - The process is intended to be lightweight and not competitive/ranked. Our one strict filter is to confirm the essential prereq (CS106B or equivalent, nothing further is expected).
    - We use the questionnaire to hear about your preparation, interest, and how the course fits with your plans. Stanford students are crazy awesome, we know all of you can succeed at whatever you put your mind to and would surely be wonderful students! Our tie-breaker is to identify students drawn to CS107E in specific and who are in a good position to get a lot out of the course. We know our course is a bit quirky :-)
    - Priority is given to freshmen and sophomores (i.e. those needing their first introduction to systems). We are open to all majors; the most common major of past students has been "Undeclared", followed by CS, EE, and MechE.
- The questionnaire will be posted after the info session and open for a week or two.  After closing the form, we will review submissions, email decisions and confirm enrollment commitments.
- Please __do not pre-enroll on Axess__, just submit the questionnaire and wait to hear back before enrolling. If you like, you can pre-enroll in a class (e.g. CS107) you are considering as an alternative if CS107E doesn’t work out for you.

For information about whether CS107E is right for you, please read below for answers to common questions.
</div>

{% comment %}
Thank you to all who submitted the questionnaire, we appreciate your interest in the course. The winter quarter cohort is now enrolled to capacity (Dec 27 2024).

- The [questionnaire form](https://forms.gle/i8G9ARAKQMu9s9UN6) is open now and will close on __5pm Sept 23__.
   You are welcome to fill out the questionnaire in advance, but it’s also possible to wait to attend the first lecture before deciding to apply. After the form closes, we will do a speedy review and make decisions as soon as possible so you can plan.
{% endcomment %}

## What is CS107/CS107E? What topics do these courses cover?
CS107 is the second systems course in Stanford’s undergraduate core sequence and introduces students to __computer systems__ focusing on these five fundamental concepts: hardware, architecture, assembly code, the C language, and software development tools. Our classic __CS107__ course teaches these concepts on a hosted Linux system using standard libraries and tools. The alternative __CS107E__ explores the same concepts through bare-metal programming on a small single board computer (SBC) using hardware add-ons such as LEDs, buttons, and sensors.

Both versions of CS107 cover the C programming language, data representation, machine-level code, computer arithmetic, compilation, memory organization and management, program execution, debugging, and performance. CS107 has light coverage of floating point and computer security that CS107E does not. CS107E includes topics in hardware and I/O that CS107 does not.

## What are the differences between CS107 and CS107E? How do I determine which course is right for me?
CS107 and C107E are considered two embodiments of the same course. They both cover the same core concepts and assign significant programming projects in C and assembly. Both promote effective development and testing through use of good engineering practices and developer tools. Either course satisfies the requirement for the CS major or minor and serves as a prerequisite for follow-on systems courses.

The major difference is the system being explored. CS107 students work on Linux running on the x86 architecture. This modern, hosted system provides the advantages of sophisticated libraries and tools, but it puts you at arms’ length away from the hardware (no direct access to processor, I/O, or framebuffer). CS107E runs bare-metal (no OS or libraries) on a Risc-V SBC. There is nothing standing between you and the hardware, but the environment is somewhat more primitive and edit/compile/debug must be done via cross-system tools.

Here are a few other issues you may want to consider in comparing the two:

+ __Assignments:__ The CS107E assignments fit together in a tightly coupled sequence where students start from scratch and incrementally build a complete working system, a simple console. CS107 has a flexible sequencing of topics and each assignment explores a different aspect of systems programming in a software-centric context. A student who falls behind would face challenges in either course, but situation could be particularly difficult for CS107E given the interlocking nature of its assignments.
- __Final project:__ CS107E concludes with a significant open-ended project where students work in teams on an ambitious project of their own design that implements a complete hardware/software system. The final task of CS107 is an individual project to develop a dynamic memory allocator.
- __Exams:__ There are no exams in CS107E. CS107 typically has both a midterm and final exam.
- __Difficulty:__ Our intention is to maintain comparable difficulty and workload for CS107 and CS107E. The assigned programming projects are demanding, and we know this work is invaluable for your learning. You will work hard in either course and leave with sharpened programming skills and a deep understanding of computer systems.
- __Tools:__ Both courses use a similar gcc toolchain (CS107E is cross-compile), but CS107 students have access to richer suite of tools (gdb/valgrind/callgrind) and the linux/x86 mileage may have a more direct payoff in subsequent projects due to the ubiquity of such systems.
- __CS vs EE:__  Because CS107E touches on both hardware and software, it serves a mix of EE and CS students. CS107E could be especially appealing to a student interested in the hardware/software interface and embedded systems, whereas CS107 is a better fit for exploring the software layers of a modern operating system. Either CS107 or CS107E applies to the systems core requirements for the CS major or the hardware/software track of the EE major.
- __History:__  CS107 has been operating in a fairly stable form since 2010; that longevity and larger audience makes for a very mature course. In contrast, CS107E is a younger upstart in the midst of moving to Risc-V SBC to replace our former Raspberry Pi (ARM) system. We welcome a spirit of adventure, and warn there may be some surprises and lack of polish in the midst of this upheaval. Your flexibility and self-sufficiency will be assets in working through any unexpected obstacles.
- __Scale:__ CS107’s large audience and longer history connect you to an extensive community of practice. This means a larger course staff, many more office hours, widespread "institutional knowledge" (e.g. upperclass friends who could advise when you get stuck), and additional academic support from CS107ACE and CS107-specific tutoring. CS107E may be small in numbers, but it is strong in spirit. The small tight-knit community is a chance to get to know your instructors and CAs and work closely and bond with your peers.
- __Materials:__ Take a look at recent course materials to get additional information. The current CS107 course web site is at [cs107.stanford.edu](http://cs107.stanford.edu); the [CS107 FAQ](http://cs107.stanford.edu/faq.html) may be a good place to start. The CS107E course web site is at [cs107e.github.io](https://cs107e.github.io) and you’re reading the CS107E FAQ right now. Reviewing the syllabus, labs, and assignments will give you a better feel and allow you to consider the fit of each course in relation to your experience and own learning goals.

Whether you take CS107 or CS107E, you’ll learn how a computer system operates and work hard to gain mastery over these topics and advance from a novice programmer to an effective practitioner. Students who do well in either course are excellently positioned to apply these powerful skills to future CS, EE, or ME projects!

## Axess says that CS107E has limited capacity and enrollment requires instructor consent. What does this mean?
Enrollment is limited to 40 students. We use a lightweight questionnaire to select based on the "fit" between the student and course. See above for [instructions and deadlines](#enrollment).

## What equipment will I need to participate in CS107E?
- __What we provide:__ We will prepare a [parts kit](https://cs107e.github.io/guides/bom/) for each CS107E student. The kit contains the SBC, cables, breadboard, jumpers, LEDs, transistors, and other components. There is a $75 fee for the parts kit. In the past the SBC was yours to keep, but due to rising tariffs and a throttled supply chain, we will need to collect the SBC at course end and recycle for future use.
- __What you provide:__ You will need a computer (Mac OS, Linux, or Windows) onto which you can install the needed cross-development tools.

## What is the schedule for lecture and lab? Is attendance required?

#### Schedule for __Winter and Spring Quarter 2026__
- Lectures meet __Monday and Friday 10:30am-12pm__. We use lecture time to cover highly relevant content. Lectures are __not recorded__ and __lecture attendance is expected__. Consistent attendance and active participation are highly correlated with student success and we want you to have this optimal experience! ([An ode to "did I miss anything"?](http://cs.stanford.edu/~zelenski/didimissanything.html))
- Labs meet on __Tuesday or Wednesday evening__. You must attend one of the two times; __lab participation is mandatory__. During lab, you will work on guided exercises in small groups with the support of the course staff. Lab is also an opportunity to build community with your peers, get to know the course staff, and have fun!
- The final project demos are __Friday__ of the final exam week. Participation in the __project demo session is mandatory__.

If your schedule doesn’t permit you to consistently attend lecture or lab or you have a conflict with the project demos, this is not the course/quarter for you. Consider enrolling in CS107 or waiting to take CS107E in a future quarter.

## Is CS107E open to remote students and/or available to SCPD?
CS107E does not have a virtual option. Although we were able to muddle through a few virtual quarters during the pandemic, we know the course works best with active synchronous participation, so that remains our focus. For a virtual option, consider instead CS107 which is regularly offered via SCPD.

## What are the course prerequisites for CS107/CS107E?
CS107 and CS107E require successful completion of CS106B (or equivalent) and eagerness to advance to the next level. You should be an accomplished programmer who has practical C/C++ skills using recursion, dynamic data structures (pointers, linked lists, trees), data abstraction, classic data structures (lists, stacks, queues, sets, maps), and standard algorithms (searching, sorting, hashing). You should have an appreciation of the intrinsic value of good engineering and design and you will be expected to produce well-decomposed, readable code. If you feel on the fence in determining your placement between CS106B and CS107(E), our strong recommendation is to pursue CS106B – we love this course! It is lots of fun, well-taught, and produces thoughtful and accomplished apprentice programmers. You will exit CS106B well-prepared to go on to a satisfying and successful experience in CS107(E).

## Still have questions?
If your question is not answered here, email us at `cs107e@cs.stanford.edu` and we can help you out!
