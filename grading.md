## How assignments are graded

*Written by Julie Zelenski, modified for CS107e by Pat Hanrahan*

We know that you will invest a lot of time into completing the
programming assignments and the results of those efforts will be
your biggest source of accomplishment (and frustration :-) in this
course. We will celebrate and reward your work by counting the
assignments for a healthy chunk of your course grade. Therefore,
we want you to understand our standards and the grading process we
use, so you can be sure to capture all the points you deserve.

We view grading as an important commitment we are making to help
you grow as a programmer. Our grading process is designed to
thoroughly exercise your program and the grading TA will review
your code to provide comprehensive and thoughtful feedback. Our
grade report will highlight the strengths of your work, as well as
point out the ways in which you can improve. Some might say that
our grading is thorough to excess (we are known for poking our nose
into every nook and cranny and torturing your code with our battery
of tests :-) but the care we take shows our respect and reciprocation
for the effort you put into creating it. In order to be able to
provide a thorough evaluation for all submissions with a relatively
small staff, we have developed automated tools to assist with the
mechanical aspects (the infamous "autotester") so as to free up our
human expertise for providing qualitative feedback in the code
review.

We evaluate each submission in two broad categories: functionality
and code quality. For functionality, we assess your program's
effectiveness from an external perspective. We are not looking at
code, but testing its behavior. For code quality, we apply quality
metrics and do a individual review of your code to appraise its
design and readability.  

### How we evaluate functionality

Functionality measures how successfully the program executes on a
comprehensive set of test cases. We create our test suite by working
from the original program specification to identify a list of
expected behaviors and write a test case for each. We use the
autotester to run a submission on the each test and award points
for each successful result. Thus, the resulting functionality score
is a direct reflection of how much observably correct behavior your
program exhibited. This process is completely automated; the grader
does not search your code to find bugs and deduct for them, nor do
they appraise the code and award points for tasks that are attempted
or close to correct.

Our functionality test suite is a mix of:

* **basic** cases that exercise the basic functionality on simple inputs 

* **robustness** checks for correct behavior on unusual cases
    and graceful handling of required error conditions 
    
* **stress** cases that test for correct handling of large, complex inputs

A basic test focuses on a specific core feature to be tested in
isolation. The robustness tests add coverage of more fringe behaviors.
We architect the basic and robustness tests to be as independent
and orthogonal as possible. The stress cases push the program harder
on a mixed set of features, focusing on the mainstream operations
and avoiding trafficking in edge cases. The cases together form a
full-coverage test suite.

You may ask: might this double-ding some errors? Yes, a program
that fails a basic case often fails a stress case due to the same
root cause. But this bug was prevalent in both simple and complex
scenarios, which means there was more opportunity to observe it and
it was more detrimental to the program's overall correctness. In
other words, we are ok with applying both deductions and have
configured the point values with this in mind. :-) Contrast this
with a bug that only triggers in a narrow context and thus fails
just one test. The smaller deduction indicates this bug was more
obscure, harder to uncover, and only slightly diminished correctness.
The total functionality score is not computed by counting up bugs
and penalizing them; we award points for observable, correct
functionality. A submission could lose a large number of points for
several failures all stemming from a single, critical off-by-one
error; another submission can lose the same number of points due
to a plethora of underlying bugs. Both submissions received the
same score because they demonstrated the same amount of "working/not
working" when tested.

What about partial credit? Earning partial credit depends on having
correctly implemented some observable functionality. Consider two
incomplete submissions. One attempts all the required functionality
but falls short (imagine it had a tiny flaw in the initial task,
such as reading the input file, which causes all output to be wrong
despite that the rest of the code may have been totally correct).
This submission would earn few functionality points. The second
attempts only some features, but correctly implements those. This
submission can earn all the points for the cases that it handles
correctly, despite the broken/missing code for the additional
requirements. This strongly suggests a development strategy where
you add functionality feature-by-feature, not attempting to implement
all the required functionality in one fell swoop. I call this the
"always have a working program" strategy. During development, the
program will not yet include all the required features, but the
code so far works correctly for the features that are attempted.
From there, extend the program to handle additional cases to earn
additional functionality points, taking care not to break any of
the currently working features.  

### The mechanics of the autotester

Understanding of the process we use can help you properly prepare
your project for submission and avoid surprises in grading.

**Output conformance is required.** The autotester is just a fancy
version of our sanity check tool. It executes your program and
compares its output to what is expected. It is vital that your
program produce conformant output in order to be recognized as the
exact match the autotester is looking for. If you change the format
or leave stray debugging printfs behind, your output may be misread.
For example, consider the outputs below:

Expected output         Output of program A            Output of program B 
---------------         -------------------             -------------------- 
apples 4                apples = 4                      4 apples 
bananas 2               bananas = 2                     2 bananas 
pears 3                 pears = 3                       3 pears

Although programs A and B seem to compute the appropriate information,
they don't match the output format (A adds an extra = character and
B inverted the columns). The A and B programs would fail every
functionality test-- yikes! To avoid this happening to you, be sure
to use sanity check and follow through to resolve any discrepancies.
We do not adjust scores that were mis-evaluated because the submission
didn't conform to sanity check.

**Pass/fail scoring.** Each automated test is scored as either passed
or failed, without gradations for "more" or "less wrong" as such
distinctions are difficult to automate and fraught with peril. Is
missing some lines of output better than having the right lines in
the wrong order or producing the correct output then crashing? Both
are scored by the autotester as incorrect for that test.

**Timeout.** The autotester employs a hard timeout to avoid stalling
on potentially infinite loops. The hard timeout is generous but not
unbounded (typically 10x the sample). A grossly inefficient program
that executes more than an order of magnitude more slowly than the
reference runs the risk of losing functionality points due to
tripping the hard timeout on tests it was unable to complete in
time. We do not re-run tests with ever-increasing timeouts to
accommodate these programs.

**Grader judgment.** Most functionality cases are automatically scored
without any involvement from the grader. For robustness cases and
other error conditions, the autotester observes whether the program
detect the problem, how it reports it to the user, and whether it
appropriately handles it. The wording of your error messages is not
required to match the sample program, but doing so guarantees full
credit from the autotester. When the wording doesn't match, the
autotester defers to the grading TA who makes the call on whether
the feedback is sufficiently informative, accurate, and actionable
to earn full credit. (Not trying to stifle your creativity, but the
risk-free strategy to avoid losing points is to follow our example
:-) 

### How we evaluate quality

In addition to the automated tests for functionality, we also
evaluate how well your program meets our standards for clean,
well-written, well-designed code. Although good quality code is
highly correlated with correct functionality, the two can diverge,
e.g. a well-written program can contain a lurking functionality
flaw or a mess of spaghetti code can manage to work correctly despite
its design. Make it your goal for your submission to shine in both
areas!

We use automated tests for these quality metrics:

* Clean compiler

We expect a clean build: no warnings, no errors. Any error will
block the build, meaning we won't be able to the test the
program, so build errors absolutely must be addressed before
submitting. Warnings are the way the compiler draws attention
to a code passage that isn't an outright error but appears
suspect. Some warnings are mild/harmless, but others are
critically important. If you get in the habit of keeping your
code compiling cleanly, you'll never miss a crucial message in
a sea of warnings you are casually ignoring. We apply a small
deduction if you leave behind unresolved build warnings.

* Clean run under valgrind

We look for an error-free, leak-free report from Valgrind. In
scoring a Valgrind report, leaks warrant only a minor deduction
whereas memory errors are heavily penalized. Anytime you have
an error in a Valgrind report, consider it a severe red flag
and immediately prioritize investigating and resolving it.
Unresolved memory errors can cause all manner of functionality
errors due to unpredictable behavior and crashes. Submitting a
program with a memory error will not only lose points in the
Valgrind-specific test, but runs the risk of failing many other
tests that stumble over the memory bug. Leaks, on the other
hand, are mostly quite harmless and working to plug them can
(and should) be postponed until near the end of your development.
Unresolved leaks rarely cause failures outside of the
Valgrind-specific test.

* Reasonable efficiency

We measure submissions against the benchmark solution and observe
whether it performs similarly both in terms of runtime and
memory usage. Our assignment rubric typically designates a small
number of points for runtime and memory efficiency. A submission
earns full credit by being in the same ballpark as the sample
program (i.e. "ballpark" translates to within a factor of 2-3).
Our sample is written with a straightforward approach and does
not pursue aggressive optimization. Your submission can achieve
the same results without heroics, and that it what we want to
encourage. There is no bonus for outperforming this benchmark
and we especially do not want you to sacrifice elegance or
complicate the design in the name of efficiency (which will
displease the TA during code review). Note that gross inefficiency
(beyond 10x) puts you at risk of losing much more than the
designated efficiency points due to the hard timeout on the
autotester. if your program is in danger of running afoul of
the hard timeout, it is a clear sign you need to bring your
attention to correcting the inefficiency to avoid losing points
for functionality tests that exceed hard timeout in addition
to the regular efficiency deductions.

For both the Valgrind and efficiency tests, we try to ignore issues
of functional correctness if they don't otherwise interfere with
the evaluation. For example, if the program attempts to reassemble
and gets the wrong answer, its Valgrind report or runtime can still
be evaluated. However, if the program is sufficiently buggy/incomplete
(e.g. discards input or crashes), such inconclusive results can
lead to loss of points.

Unless otherwise indicated in the problem statement, we do not
require recovery, memory deallocation, or other cleanup from fatal
errors, you are allowed to simply exit(1).  

### Code review

The most important part of the quality feedback is the TA's commentary
from the code review. The TA will read your code as in the role of
a team manager giving feedback before accepting the code into the
team's repository. Our standards should be familiar from CS106:
clear, elegant code that is readable, cleanly designed, well-decomposed,
and commented appropriately. Read Nick Parlante's hilarious Landmarks
in coding quality. The TA's review will identify notable issues
found when your reading your code and point out the highlights and
opportunities for improvement. The TA also assigns a bucket for the
key tasks being evaluated. Most assignments will be to the median
bucket (designed [ok]) which means the code is "reasonable". It
likely has a number of small issues, but on balance is holding
steady in the midst of the peer group and is not notably distinguished
up or down. Code that is outstanding we will reward with the [+]
bucket, while code that is more troubled will land in the [-] bucket.
In rare cases where we need to send an even stronger message, there
is a [--] bucket. Your most important takeaways from the code review
will come in detailed feedback given in the embedded comments marking
up your code, so that's where your should focus your attention. The
bucket serves to confirm that you're meeting expectation [ok] or
that you are over[+]/under[-] performing.

Below is a list of code quality expectations in the base rubric for
the code review of all assignments. There may be additional entries
in a rubric specific to a particular assignment.

    Cleanliness/readability
	code is free of clutter: remove all dead code and unused
	vars/fns use of whitespace/indentation is tidy and consistent
	    split long lines if necessary (screenwidth ~120 chars)
	    2-4 spaces per indent level (spaces not tabs to avoid
	    editor inconsistency) whitespace used to visually support
	    logical separation
	variable/function names are consistent and descriptive
	    good names can remove the need for additional commentary
	    use consistent scheme for capitalization/underscores
	use constants/#define'd/sizeof, not hardcoded numbers
	comments are present where warranted, prose is correct and
	helpful
	    overview comments (per-function, per-module) with summary
	    highights inline comments used sparingly where needed
	    to decipher dense/complex lines no excess verbiage that
	    reiterates what code itself already says

    Language conventions
	choose the most clean, direct, conventional syntax available
	to you, e.g. ptr->field instead of (*ptr).field be
	thoughtful/consistent in use of array subscripts versus
	pointer arithmetic
	    functionally equivalent but more common to use subscript
	    when accessing an individual array element, pointer
	    arithmetic when accessing subarray
	avoid unnecessary use of obscure constructs (such as comma
	operator, unions) use standard language features appropriately,
	e.g.
	    bool type from stdbool.h, static qualifier on private
	    functions, const for read-only pointers

    Program design
	program flow decomposed into manageable, logical pieces
	function interfaces are clean and well-encapsulated appropriate
	algorithms used, coded cleanly common code is unified, not
	duplicated
	    when you need same lines more the once, don't copy and
	    paste -- unify!
	use standard library functions rather than re-implement
	that functionality, e.g.
	    string manipulation, formatted I/O, sort/search, type
	    conversion, etc.

    Data structures
	data structures are well-chosen and appropriate no redundant
	storage/copying of data no global variables

    Pointers/memory
	no unnecessary levels of indirection in variable/parameter
	declarations uses specific pointee type whenever possible,
	void* only where required low-level pointer manipulation/raw
	memory ops used only when required allocation uses appropriate
	storage (stack versus heap, based on requirements) allocations
	are of appropriate size use typecasts only and exactly where
	necessary and appropriate

### Investing in testing

Succeeding on the functionality tests is a measure of your achievement
in implementing the program requirements, but is also a reflection
of your testing efforts. The more thorough you are in testing, the
more bugs you can find and fix. High assignment scores are strongly
correlated with good testing. I believe that most submissions come
in with no known bugs, that is, had you observed the bug, you would
have fixed it before submitting. But not everyone puts the same
time into finding the bugs in the first place. Testing is an important
part of the process. Your efforts to identify the inputs or code
paths that need to be exercised to get full coverage and testing
and re-testing throughout development can make a huge difference
in results. (read our thoughts on effective software testing for
advice on tactics)

When I create an assignment, I also put together extensive tests
we plan to subject your submissions to. Why not give those tests
to you up front? Is this stinginess just further evidence of my
commitment to student cruelty? Oh, probably :-) But it is also true
that your boss/advisor is not going to hand you an exhaustive test
suite when assigning you a task. Along with designing and writing
the code, a professional engineer is responsible for constructing
test inputs, devising strategies, and/or building tools that allow
them to verify their work. These skills are not trivial and require
practice. Consider the CS107 assignments as chock-full of opportunities
for you to gain proficiency in testing.

It is such a bummer to have worked hard on your submission only to
get back our grading report with failures from bugs you would have
fixed if only you had known about them! This sad outcome is entirely
avoidable-- thorough testing allows you to find those bugs, which
means you can fix them and earn full points on functionality. That
totally rocks!  

### Interpreting our feedback

A grading report gives a list of the test cases and the pass/fail
result from executing it against your program. Each test is identified
with a short summary description of what was being tested. If you
would like to know more about a particular case, you may ask in
email or better, come to office hours where we can manually re-run
the test and walk you through it. Note that we do not give out test
cases as part of maintaining a level playing field for future
students. For the code review, we return your code marked up with
our comments. This individualized feedback on your code quality
will highlight opportunities for future improvement and commend
your successes. Please take the time to read and reflect on all of
the notations made by your grading TA, and use email or office hours
to resolve any questions or misunderstandings about the feedback.

### Frequently asked questions about assignment grading 

**Can I get a regrade? I think the autotester results are wrong.**

We have invested much time in our tools to try to ensure they
evaluate the submissions correctly and consistently and we are
fairly confident in them. But given we're human and it's software,
mistakes can happen and it is important to us to correct them. If
you believe there is a grading error due to a bug in our tools/tests,
please let us know so we will investigate further. If there is a
problem, we will be eager to fix it and correct the scores for all
affected submissions.  

**My program worked great on sanity check, but still failed many grading tests. Why?**

The information from sanity check is only as good as the test cases
being run. The default cases supplied with sanity check are trivial
checks of basic output conformance. Passing them is necessary, but
not sufficient, to ensure a good outcome. You gain broader coverage
by creating your own custom tests. The more comprehensive your
tests, the more confidence you can have in the results.

**I ran Valgrind myself and got a clean report but my assignment grade
docked me for errors/leaks. What gives?**

We run a Valgrind using one of our larger comprehensive test cases.
You should do the same. Getting a clean Valgrind report on a simple
test case confirms the correctness of that particular code path,
but only that path. When you run Valgrind on a diverse set of code
paths, you be able to additionally confirm the absence/presence of
memory issues lurking in those parts and will not be caught unaware
by what we find there.  

**How can I tell what bug caused me to fail a particular test? Can you send me the test input to reproduce it myself?**

The grade report includes a summary statement of the objective of
each test, but they are admittedly terse. If you're in need of
further detail, catch up with us in office hours or send us an
email. We can talk you through the test and its result on your
submission so you better understand what went wrong. Note that we
do not publish or release our test inputs.

