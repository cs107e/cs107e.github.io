---
layout: page
title: Course Policies
permalink: /policies/
---

## Table of Contents

*  [Late Policy](#late-policy)
*  [Collaboration Policy](#collaboration-policy)
*  [Collaboration Policy FAQ](#collaboration-policy-faq)

## Late Policy

*Written by Pat Hanrahan*

Hofstadter's Law: "It always takes longer than you think, even when you take
Hofstadter's Law into account."

The rules:

1.  The cutoff for on-time submission is **midnight** of the due date. Late
    days are counted in 24-hour periods. Submitting one minute past midnight
    until midnight of the next day is one day late, and so on. We use the
    timestamp of your final git commit as the the time you submitted
    your assignment.

2.  You are given **4 "grace days"** (self-granted extensions) which you can
    use to give yourself extra time without penalty.

3.  Instructor-granted extensions are only considered **after all grace days
    are used** and only given in exceptional situations.

4.  Late work handed in when you have run out of grace is **discounted 10% per
    day late**. It is always to your advantage to complete all the assignments
    even if they are late.

5.  Late submissions (penalty or not) are not accepted after the hard deadline,
    which is **4 days past the original due date,** but may be restricted to
    fewer days on a per-assignment basis.

6.  The final project must be submitted on time.

## Collaboration Policy

*Originally written by Julie Zelenski, some wording borrowed from a
collaboration policy distributed by Brown CS.*

The programming assignments are an integral part of the CS107e learning
experience. We have a great set of projects lined up for you and the time you
spend designing your own solution and working toward its successful
implementation can be fun, challenging, illuminating, time-consuming,
rewarding, and frustrating. Your pride upon finishing is a fantastic high and
your efforts earn you powerful skills and deep understanding. Don't cheat
yourself out of this incredible learning opportunity! Borrowing someone else's
design, building on another's code, being lead by another person, and other
such "shortcuts" jeopardize the chance to develop your own mastery and
compromise your integrity.

This document is designed to make clear our expectations and restrictions on
collaboration. We expect your assignment submissions to be your own independent
effort and not be derived from the thoughts and work of others. You should do
your own thinking, your own design, your own coding, and your own debugging.
This doesn't mean that you can't ask questions or get help when you get stuck,
but any help must remain within acceptable limits, as detailed in the concrete
examples given below.

All of you should be familiar with the principles of the [Stanford Honor
Code](http://web.stanford.edu/class/cs107/collaboration.html). The [CS
department Honor Code policy](http://csmajor.stanford.edu/HonorCode.shtml)
further explains how it applies in CS courses. Students are to uphold their
obligations to do honorable work and encourage others to do the same. On our
part, we will treat students with trust and respect. When alerted to any
possible infraction, we will protect the honorable students' interests by
investigating and prosecuting dishonorable behavior.

#### Assistance that is allowed and requires no citation

These things are encouraged and allowed for all students:

**Discussing course topics and materials**

You may freely talk through any material from the lectures,
labs, textbook, and course documents with other students.

**Asking/answering questions about the programming language, libraries, and tools**

For example: "Does strcmp compare strings case-insensitively?
What does the keyword static mean? How do I view my git
history? What does this error message mean in a Valgrind report?"

**Clarifying the requirements or specification of an assignment**

For example: "Do the results have to be sorted? What is the
expected response if the input is empty?"

**Sharing generic advice and techniques for coding or debugging**

For example: "I test each function right after I finish writing
it. When my program crashes, I first look at the stack trace
in the debugger. I used custom sanity check to test these kinds
of inputs. Valgrind callgrind is great for finding performance bottlenecks."

**Use of web or public resources for background information**

You may search for and use external resources (web sites, blogs,
forums, etc.) for information on course topics, as reference
material on the programming language and libraries, and to
resolve technical difficulties in use of the tools. The allowable
use of these resources does not extend to assistance specific
to the assignment code. (Please see below for restrictions on
use of external resources)

**Any discussion between student and the staff**

You are welcome to discuss any aspects of design, code, or
debugging with the course staff. They are the best folks to
talk to because they are knowledgeable about all the material
and know how to help you without overly influencing or leading you.

#### Assistance that is allowed but must be cited

Whereas the discussions listed above are allowable, certain discussions
require citation. The difference between the two comes in what is
being discussed and in how much detail. You may discuss general
topics (course materials, tools, language) in any depth without
citation, but discussion or help specific to the particulars of an
assignment requires citation. The particulars include such things
as the program's design, data structures, choice of algorithms,
implementation strategies, testing, and debugging. Some examples:

**Discussing the design of an assignment**

Design is a crucial part of the programming process, and we
want you to learn to work through design problems on your own.
Only after you completed your own independently-conceived design,
you may compare and contrast with a peer who has also completed
their own design. You both must cite this discussion and note
any ideas taken away from it.

**Helping another student to debug**

Two students should not sit down and debug jointly at the same
computer, but a student could describe symptoms to a peer who
responds with suggestions and hints (e.g. "it sounds like you
might have forgotten to terminate the strings" or "you might
be accessing freed memory, have you tried commenting out all
the free statements?"). Receiving specific debugging advice
must be cited.

**Sharing specific test inputs or strategy**

You may exchange strategies for testing (e.g. "here's my approach
to using custom sanity check"), brainstorm together about test
cases (e.g. "be sure to verify handling when file can't be
opened"), and share text inputs (fragment files, corpus files,
and so on) but this collaboration must be cited. Sharing actual
code used for testing is off-limits.

**Re-using your own work from a previous quarter**

This special rule applies to students who are re-taking the
course. Re-using your previous quarter's work is generally
permissible as long as the work was your original, independent
creation and the self re-use is clearly cited. A specific
exception to this general rule is the final project on which 
any re-use is expressly disallowed. For the
final project, a re-taking student must take a distinct approach
from any previous submission and write new code and new
documentation from scratch. Re-using the same design or borrowing
code/documentation from a previous submission will not be
considered a Honor Code violation, but such a submission will
not be accepted for grading.

A required citation must be specific, complete, and truthful. You
must clearly identify the source of the help/discussion (person's
name, book title, URL), describe the nature and extent of the
assistance, and indicate how you believe the help may have influenced
your work. A vague mention of "discussion with friends in my dorm"
does not meet the obligations for a required citation. A misleading,
incomplete, or untruthful citation can be considered an aggravating
factor when a case is referred to the Office of Community Standards.

Failing to make a necessary citation can be charged as an Honor
Code violation. Some former students have acknowledged they were
unsure about the appropriateness of the assistance and chose not
to cite to avoid drawing scrutiny. If in doubt, cite, as it is never
a problem to give proper credit where due. If the assistance is
truthfully cited, even if impermissible, we can work with you to
discount the work rather than handle as a Honor Code violation.

#### Assistance that is **_not_** allowed

Whereas discussion as described above is acceptable if cited, these
discussions should never become so detailed that they involve jointly
writing or exchanging/sharing passages of code. Your code must
represent your original, independent work and it should not be
developed in conjunction with or derived from anyone else's. You
should never be intimate with another's code nor allow others to
be intimate with yours. Here are specific examples of unpermitted
aid:

**Copying code**

It is an act of plagiarism to submit work which is copied or
derived from the work of others and represent it as your own.
It does not matter if the plagiarized source was a current or
former student, outside the course, or found on the web, or
whether the amount was an entire program or just a small part;
in all cases, it is unacceptable. You should never be in
possession of anyone's else code, whether in printed, written,
or electronic form.

**Reviewing the code/design of another**

You are not permitted to have another person "walk you through"
their approach nor may you use their work "as a reference" to
look at if you get stuck. This prohibition applies to both code
and design, to isolated passages as well as the entire program,
and whether the review is conducted verbally or in
printed/written/electronic form.

**Joint development/debugging**

You are not permitted to work with another to jointly develop
a design, write code, or debug. Two students should never be
working together on a passage of code/pseudocode whether on
paper, on a whiteboard, or in editor/debugger of a shared
computer.

**Use of web or public resources for assignment-specific code**

You should not be searching external resources for solutions,
whether in the form of code, pseudocode, or design. Should you
find full/partial solutions anyway, you are to turn away and
report the location to us. A submission must not be adopted
from or influenced by study of any external resource.

**Sharing your code/design**

You must not share your code with individual students nor
publicly broadcast it. You should not post your assignment code
in a public site for the review of others nor ask in a public
forum for others to resolve issues specific to your assignment
code. Although you will submit assignment code using Github, 
we have private accounts set up for this and we expect that you will 
keep this code private. Even after the course ends, you should not put 
your assignment code on a public Github account.
You are expected to take reasonable security precautions
to maintain your work privately. If we request that you
remove/protect code that has been improperly shared, you are
expected to comply in a timely manner.

Note that the giver of unpermitted aid, i.e. the student who provides
the code/design/walkthrough to another, can also be charged with a
violation.

If you are ever unsure whether something you intend to do is
acceptable, play it safe and ask us first. A student's explanation
that "I thought it was okay; I didn't learn the course policies"
will not excuse the violation.

The Honor Code is a powerful assertion that we as a community proudly
dedicate ourselves to upholding the highest standards of academic
integrity. The vast majority will do right by CS107E -- we ask a lot
of you and you will consistently meet those challenges by creating
work that authentically represents your own effort. We demonstrate
our respect and appreciation for your honor and efforts by doing
our part to make absolutely clear our expectations and hold accountable
those students who act in violation.

## Collaboration Policy FAQ

**Can I include my solution code in my portfolio for internship/scholarship
applications?**

We're delighted to hear that you are proud of your work and want
to show it off! We are supportive, but ask that you contain the
code to the intended audience. You may share your code via direct
communication, privately-maintained repositories, restricted access,
or another controlled channel. (as a suggestion, bitbucket.org
offers academic users unlimited private repos). Making your code
freely accessible on the web or maintaining a public repository at
a code-sharing site will be a temptation for others who would exploit
it. This is a bad situation we would much rather prevent than
prosecute.  

**What does it mean to have "third-party responsibility"
under the Honor Code?**

The students and faculty agree to work together to establish and
maintain standards for honorable work. Students are not the primary
enforcers of the policy, but they give meaning and value to the
Honor Code by showing their respect for it and encouraging others
to the same. You are to model appropriate behavior in word and deed
and should decline to aid or support another's actions in violation.
If asked by another to act inappropriately or you observe something
improper, take reasonable action to rectify the problem: draw
attention to it, ask the student to stop, remove the temptation,
or alert the course staff.  

**It's been said that "you're only cheating
yourself". I do honest work; why do I care if another student
doesn't?**

It is sad that a student with the wherewithal to be granted the
privilege of a Stanford education would disrespect it in this way,
and it is certainly true that they are the one who stands to lose
the most by their actions. You can feel secure that your hard work
earns you knowledge, skills, and pride that they will never have.
If even one acts in violation, it undermines the integrity of the
system and the fairness by which we all treat one another. When
dishonest dealings are rewarded with course grades as good or better
than those given for sincere achievement, it diminishes us all. We
demonstrate our respect for those who do right but holding accountable
those who do not. We ask you to do your part by not providing
unpermitted assistance nor giving tacit approval to dishonorable
actions.  

**I'm nervous about adding a citation, as I'm concerned it
will cause my work to be scrutinized and suffer repercussions. What
would you advise?**

If you fear that citing your collaboration will invite trouble,
your dilemma is not in whether to admit to it, but in reconciling
that you are engaging in activities that you are not comfortable
openly acknowledging. The choice to not cite your sources can be
seen as trying to avoid drawing attention to actions you knew were
dubious. You should only be engaging in collaboration that you are
confident is appropriate, for which you have no qualms about openly
citing. A citation that gives proper credit is not only welcome,
but necessary. Citing appropriate help never causes a problem and
often avoids them. Even in the case where the collaboration was
inappropriate, a proper citation can downgrade the severity of the
response (see below in "I messed up").  

**Can you further clarify
appropriate and inappropriate use of external resources?**

External resources can be used to research course topics and answer
general questions, e.g. review 2's complement addition, ask "does
strncpy null-terminate the destination string?", or learn how to
use a gdb watchpoint. You are not to search for nor use external
resources to obtain solutions/answers to assignment-specific tasks.
The prohibition applies to solutions in any form (e.g. design,
algorithm, code, pseudocode) and whether a complete solution or
just part. A few examples of inappropriate use: searching for code
to implement portions of the assignment, adopting pseudocode found
on Wikipedia for an assignment-specific algorithm, or posting a
broken passage of your assignment code on StackOverflow and asking
others to debug it. When you have assignment-specific issues, the
appropriate resource to tap into is us -- come by office hours,
post to the forum or send us email. We're happy to help!

Appropriate use of external resources does not generally require
citation, but if your code/design was materially influenced by what
you viewed, it is appropriate to cite that resource and indicate
the affected passage.

**I have stumbled across what appears to be a
full/partial solution that is public or otherwise unprotected. What
should I do?**

Your primary obligation is to make an immediate about-face and
follow up with us so we can arrange for its removal. If you peruse
the code just "to see how they did it", you are headed down a
slippery slope of being inappropriately influenced by its ideas and
can easily lead to adoption of its approach and copying of code.
Even if you manage to avert your eyes at first glance, knowing of
an available solution can be a powerful siren song that calls you
back if you get stuck or want a hint. Contact us immediately so we
can take action to remove the temptation. In this way, it will no
longer entice you and any other student who might use it in a moment
of weakness.  

**I'm in a tough situation (out of time, behind on
material, stressed) and my panic has me considering less-than-honorable
ways to get my assignment finished. Help!**

As much as we all intend and aspire to be worthy, in the heat of
the moment with mounting pressures and the coincidence of opportunity,
temptation can weaken resistance. If you find yourself vulnerable,
I ask you to please, please, please step away from any immediate
action. Taking a break or getting a good night's sleep may be enough
to restore your sense of perspective. Even better would be to reach
out to me or someone else you trust and ask for guidance in working
through a bad situation. There are always other options. You can
ask for more help, you can take a late day even if it means a
penalty, you can submit a program that's known to be imperfect, or
you can not submit a program at all. Maybe you can find relief by
allowing yourself to reset your expectations about the course grade
you'll earn, changing the grading option to remove pressure, or
even withdrawing from the course. Even the most drastic of these
options has consequences that are small potatoes compared to
compromising your character and facing judicial charges. Please
come to me and I promise to support you in finding a way through
your situation that will maintain your integrity.

**I messed up. I
took actions that I suspect/know were in violation of the Honor
Code. What do I do now?**

Acknowledging responsibility to yourself is a hugely difficult but
very important first step. Your next step is to reach out to me. I
am more than willing to work constructively with a forthright student
who owns their behavior and seeks to make things right. Within a
limited time frame, we can accept a request to retract the compromised
work or allow an amended citation that properly credits its authorship.
Acknowledgment of diminished self-contribution may result in
discounted/zero credit for the work, but an accurate explanation
rectifies the mis-representation that would have otherwise been the
basis for a formal judicial charge. No matter what the violation
or your follow up actions, it is never too late to change course
and commit yourself to acting with integrity going forward. Even
in the situation where a formal case is being pursued with the
Office of Community Standards, a student's efforts to be upfront
and cooperate in the process are considered favorably. In contrast,
false denials and further prevarication will be treated as aggravating
factors.
