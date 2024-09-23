---
released: true
permalink: /labs/lab0/
title: 'Lab 0: Orientation'
toc: true
attribution: Lab written by our wonderful CS107e TAs
readings: |
  Please do the Lab 0 [prelab preparation](/labs/lab0/#prelab-preparation) before coming to lab.
---

{% comment %}
Task list to copy/paste when creating PR for this lab:

__Before releasing lab0:__
- [ ] Review writeup/code/checkin questions (instructor)
- [ ] Walk through (SL)
- [ ] Followup on issues from previous quarter postmortem (#461)

__To prep for lab0:__
- [ ] Review tips recorded in private/staff_notes.md and private/answers.md
- [ ] Review installation instructions, bring up-to-date for latest versions
- [ ] Bring blank labels and pens -- students make name tag and decorate kit box
- [ ] Create Google sheet with class roster to record lab attendance

{% endcomment %}


Welcome to CS107e lab! Lab is one of the most beloved components of this course. We aim to create a lab experience brings us together to teach and learn from one another in the same great spirit of collaboration and community.

## Goals

During this lab you will:

- Start getting to know one another
- Confirm working install of developer tools
- Set up your local mycode repo
- Practice with the unix command line and text editor
- Explore the resources available on the course website
- Review binary and hexadecimal number representations

## How does lab work?
Lab is a time to experiment and explore. After we introduce topics in
readings/lectures, you'll do guided exercises in lab to further your
understanding of the material, get hand-on practice with the tools in
a supported environment, and ready yourself to succeed at this week's
assignment.

Bare metal programming requires precision. A trivial typo or slight
misunderstanding can stop your progress cold. **Working with the support of your classmates and staff** can make the difference between quickly resolving that sticking point versus hours of frustration wrestling on your own in the dark.

Each lab has a set of check-in questions to answer as
you go. **Touch base with the staff on each question** to confirm your
understanding and resolve any confusion. The check-in questions are
intentionally simple and your responses are not graded; we use them as a gauge of how you're doing with the material so that we know better
how to help guide you.

To get the most out of lab, don't set your goal at merely finishing
the absolute minimum required in the shortest possible time.  If you
already have a good handle on the material, use the lab period to dive
into further nooks and crannies or help out those peers who could
benefit from your experience.  You should also get to know the
staff.  They are masters of the craft, and you will learn a lot
by talking to them and asking them questions.

The combination of hands-on experimentation, give and take with your
peers, and the expert guidance of our staff is what makes lab time
truly special.  Your sincere participation can really accelerate your learning!

## Prelab preparation

To prepare, please do the following __before__ coming to this lab:

1. From our course guides, please review:
    -  [Unix command line](/guides/unix) 
    -  [git](/guides/git) version control system
    -  [binary and hexadecimal](/guides/numbers) numbers
2. Check out these CS107 guides on using the `emacs` or `vim` text editor:
    - [emacs](https://cs107.stanford.edu/resources/emacs.html)
    - [vim](https://cs107.stanford.edu/resources/vim.html)
3. **If you run Windows**, follow this [WSL setup guide](/guides/install/wsl-setup) to install WSL and Ubuntu on your system:
    - The download/install takes some time, please try to complete before lab on reliable wifi!
    - If you hit a snag, bring the issues to lab and we can help you resolve.

## Lab exercises
<a name="step1"></a>

### 1. Set up your local `mycode` repository
In this course, we use git repositories to share code between you and us.
Each of you has a remote GitHub repo connected to a local repo stored on your laptop.
You will start each lab and assignment by pulling the starter code into your local repo.

You need to create your local repo as a one-time configuration step.
Do this now by following the instructions in the [Github setup guide](/guides/setup-mycode-repo). 

Then, once you've finished that, skim through the regular [Git workflow guide](/guides/cs107e-git) to understand the workflow for retrieving starter code and submitting your work.

Here is the standard workflow to pull courseware updates and `lab0` starter files:

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror lab0-starter
```
If you run into any trouble in setting up git, grab a staff member for help resolving. After pulling the starter files into your repo,
you should have a `lab0` folder containing two text files. [^2]

### 2. Install the development tools
Head over to the [toolchain installation guide](/guides/install/devtools) and run through the process of installing your toolchain---note that the steps will depend on whether you're running on a Mac or WSL.

Make sure that once you've finished, you run through the __final check steps__ at the end of the [installation guide](/guides/install/devtools#finalcheck) to verify that the needed tools are installed and your environment is correctly configured. Check-in with us to confirm all is well. [^1]

{% include checkstep.html content="confirm $CS107E, cross-compile build and debugger" %}
```console?prompt=(gdb),$
$ cd $CS107E/sample_build
$ make clean && make all
rm -f *.o *.bin *.elf *.list *~
riscv64-unknown-elf-gcc ... blah blah blah ...
$ riscv64-unknown-elf-gdb
GNU gdb (GDB) 13.2
... blah blah blah ...
(gdb) target sim
Connected to the simulator.
(gdb) quit
```

### 3. Command line practice
Over the course of CS107E, you'll get comfortable navigating the command line. To start, read through and follow all the steps in our [Unix Command Line Guide](/guides/unix).

> __Pro tip: pay attention to command responses__ As you explore, you may be trying commands copied from our guides or found in a tutorial. Rather than enter commands blindly, try to understand the intention of a command and its expected response. If you mistype or misuse the command, the action will fail or be applied incorrectly. Be sure to read each response carefully so you don't overlook the error message or bad outcome even when it is unhelpfully phrased or buried in other long-winded chatter. If you need help interpreting unix-speak, grab a staff member!
{: .callout-warning}

While you should make sure you understand the [Unix guide](/guides/unix), because it covers the basics, there's a couple more tricks to speed up your workflow. Practice using each of these commands below---you'll be amazed at how much they boost your productivity!

+ Typing a long path of a file or directory? Use ✨tab-complete✨! When you press `[tab]`, UNIX will automatically fill in your partially typed command. Try navigating to new directories in your file system and see what you can get to successfully tab-complete.
+ To replay a previous command, use your arrow keys (↑ and ↓) to move through the commands in your history. Type `history` and see everything you've done in this session.
+ Used a long command a while ago, but don't want to type it all out again? Search through your history with `ctrl+r`!
+ Want to go back to your home directory? Instead of typing `cd ~`, save a couple keystrokes with just `cd`.
+ To move to the beginning of the line, type `ctrl+a`. To get to the end, type `ctrl+e`. (p.s... these are the same in Emacs, which you'll learn about in a bit!)
+ To redirect the output of a command into a file (for example, if you wrote tests with a lot of output, which you want to be able to read through at your leisure), use the `>` symbol like this:
```console
$ command_with_long_output > new_file.txt
```
+ Want to look at the contents of a file without opening it in a text editor? Use `cat`, short for concatenate, like this:
```console
$ cat file_I_want_to_look.at
```
+ Use `man` to access the manual page for a command, including sample usage and list of option flags. For example,
type `man cp` into your terminal. What comes up? What does the `-r` option flag do for `cp`, for example? (What does the command `man man` show you?) Another handy tool to understand commands is <http://explainshell.com>, which can be used to break down and explain a command and its options. Try putting `cp -r` there to see an explanation.

Over time, you might learn about other helpful commands, like `grep`, `diff`, and `echo`. Curious about how to use these, or any other commands you've seen used? Get more information from the `man` page, explainshell, a google search, or other reference document. Make a point to learn a new command each day when you sit down to work and you'll soon be well on your way to becoming a command line ninja! 🔥

### 4. Editors
When you've taken CS classes in the past (like 106A/B), you've likely relied on IDEs (integrated development environments) like PyCharm, Qt Creator, or Eclipse. While those are great for some tasks, we're going to rely on a new set of tools for this class: __text editors__. Have you ever opened a file in Notepad or TextEdit? The editors we'll be using (Emacs, Vim, etc.), aren't too different at a basic level, except that they're built specifically for programming and can be configured to do all kinds of fancy things to make you super speedy 🏃💨.

![XKCD Text Editors](images/real_programmers.png){: .zoom .w-75}

Every programmer has their own (often strongly held) opinion about which editor they prefer to use. Common choices are __Emacs__, __Vim__, __Nano__, __Sublime__, and __VSCode__. While these editors can often feel overwhelming at first---especially command line editors, like Emacs and Vim---you will be so grateful for the time you invest in learning them well during this class.

No matter which editor you decide to make your home base, we think it's a worthwhile investment for everyone to learn enough Emacs and Vim that you could open a file, make an edit, and save & close the file without any trouble (this skill will serve you well in the future, like if you have to quickly edit code on systems you've just `ssh`'ed into).

First, let's try out Emacs. Change directory into the subfolder named `editors` within `lab0` in your `mycode` repo. The folder contains two text files, `emacs_is_better.txt`and `vim_is_better.txt`.  Use the command below to open a file in the emacs editor. (Note: on recent macOS versions, use `mg` in place of `emacs`. `mg` is a lightweight emacs-style editor that comes pre-installed. You can also install full emacs via homebrew.)
```console
$ emacs emacs_is_better.txt
```
If that worked, type away! Once you're finished, hit `ctrl+x` and then `ctrl+s` to save your document. To exit, hit `ctrl+x`  and then `ctrl+c`.

Now that you've tried Emacs, let's get a taste of Vim. Use this comment to open vim on the text file:
```console
$ vim vim_is_better.txt
```
Before we can type in Vim, we need to switch out of command mode into insert mode. Type `i` and you should be able to start writing away! Once you feel ready to quit, type `ESC` (the escape character) and you'll be back into command mode. From there, to save and quit, type `:wq`.

As long as you are able to open, edit, and exit in Emacs and Vim, you could attack any command line and start writing code (albeit, not very efficiently).

At this point, you might be wondering---why are programmers obsessed with these tools? There are a million tools that can edit text far more intuitively, from Google Docs to iMessage to a text box in MS Paint---and while that's all true, the benefit of these text editors is 1) agility & speed and 2) configurability.

Why are these tools so much faster? For one, they can be launched directly from the command line, as we'll see below. Once you get comfortable programming in an editor, you should be able to do everything you need with your keyboard, and never touch your mouse. While it might not seem like a big deal, flipping back and forth between the command line and your code quickly will speed up your workflow a lot.

How are these tools configurable? Well, once you've spent some time working with your editor, you might decide that you want to permanently add line numbers. Or rainbow indent markers. Or have "lambda" always be replaced with λ. Or have your editor always show you an XKCD comic upon startup. Once you feel comfortable with Emacs or Vim, you can look into editing their `.config` files (or, you can feel free to ask the TAs if they're willing to share their personal configs!).

We won't dictate which text editor you have to use (and the staff is divided into teams about which is better, anyway), but whichever you pick, make an investment in learning and mastering its features. For a start, right now, learn how to do the following things in your preferred text editor __without__ touching your mouse. Our [command line guide](/guides/unix) has links to common Emacs and Vim commands, but you can also find all this information online. Practice the following:
+ Move your cursor to the end of the line. Now move it to the start of the line. Practice moving back and forth.
+ Move your cursor one line down. Move it one line up. Go up and down and up down.
+ Scroll the screen down. Now scroll it back up. Repeat. Practice makes perfect!
+ If you can't see line numbers on the side of your screen, learn how to display them. We promise you'll thank us later when there's a bug in line 173.
+ Delete an entire line of text all at once.
+ Now undo that! Hint: if you're working through the command line, you'll likely regret typing `ctrl+z` like you're used to, which sends a SIGSTP to the process---a little sneak peek of CS110!
+ Search through your code. This one can take some practice, because many text editors have different kinds of searches. Find one that works for you.
+ Learn how to copy, cut, and paste code around your document, and then practice doing that over and over.

Now that you've learned how to do these, spend the next week practicing the commands! Your goal is to build up muscle memory around these commands---so even though it'll be annoying for the first week to keep looking up how to do this or that, we promise that with practice it'll become easier and easier, until you can't even remember a time when you needed to leave your keyboard while writing code💪. 

Let us know how you're doing on getting acclimated in your environment with this check-in question [^3].

### 5. Website scavenger hunt
Our course website is where you'll find lecture slides, lab writeups, and assignment specifications, along with a trove of additional resources such as our course guides and curated set of readings, resources, and demos. We encourage you to explore a bit and become acquainted with what's available and how it's organized.  The __Search__ tab is handy way to find matching content across all pages on the site.

As you explore, see if you find answers to such questions as:

+ Oh no! I lost my copy of the Mango Pi pinout diagram. Where can I find a new copy of the one CS107E uses? (p.s... Is there an interactive version available as well?)
+ Where can I find the recommended readings to review before a lecture?
+ I've never used the gdb debugger before. Where do I start and where can I learn more?
+ What is the course policy on collaboration?
+ Breadboards make no sense to me. How do I know which holes are connected?
+ We saw a cool RISC-V simulator in lecture. How can I play with that myself?
+ I'm working hard to write tests, but I just don't feel like I'm covering my bases. What am I doing wrong? Is there advice on software testing more broadly?
+ I am curious about the final project. Are there examples of what other groups have done in the past to take inspiration from?

If a topic has you curious or confused, start by checking the course website for helpful information. If can't find the information you need, reach out on Ed or come visit us in office hours. We want you to always feel comfortable coming to us with any questions you have, as well as suggestions on how we can improve the quality of the resources available to you. You're ready for this check-in question [^4].

### 6. Binary numbers

Now that you know where the guides are located on the course web site, find the guide to binary numbers. Practice binary/hex conversions by writing down answers to the questions in the guide. Then, check out these [bitwise practice problems](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/lab1/practice.html) courtesy of CS107. Another fun practice tool is this [online visualizer for bitwise expressions](https://mdrach.github.io/cs107-apps/) written by CS107 CA Max Drach. Check it out!


## Check in with TA

Each table group should periodically touch base with us as you work through the exercises. The check-in allows us to
verify your understanding and help with any unresolved issues. 

The four points we'd like you to check in with us for this lab are listed below:

[^1]: Have you installed the development tools and confirmed each of the [final check](/guides/install/devtools#finalcheck) steps of the install guide?

[^2]: Confirm you have successfully configured your local `mycode` repo and were able to pull starter code.

[^3]: Do you feel comfortable with simple command-line tasks, basic use of your editor, and know where to go to learn more?

[^4]: You have explored the course website and know how to navigate and find information.

<style>
    .footnote:before {
        content: ' Q-';
    }
    .footnotes:before {
        content: "Check-in questions";
        font-size: 125%;
    }
</style>
