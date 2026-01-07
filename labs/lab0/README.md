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


Welcome to CS107e lab! Lab is one of the most beloved components of our course; a place where we come together to share and learn as a community.

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
misunderstanding can stop your progress cold. __Working with the support of your classmates and staff__ can make the difference between quickly resolving that sticking point versus hours of frustration wrestling on your own in the dark.

Each lab has a set of check-in questions to answer as
you go. __Touch base with the staff on each question__ to confirm your
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
3. __If you run Windows__, follow this [WSL setup guide](/guides/install/wsl-setup) to install WSL and Ubuntu on your system:
    - The download/install takes some time, please try to complete before lab on reliable wifi!
    - If you hit a snag, bring the issues to lab and we can help you resolve.

Bring to this week's lab:
- The laptop you will be using this quarter, full battery (and/or charger)

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
you should have a `lab0` folder containing two text files.

### 2. Install dev tools
Open our [guide on installing devtools](/guides/install/devtools) in a new browser tab. Follow the instructions step-by-step for your platform. Reach out for help if you run into any snags!

Be sure to finish with the [final check steps](/guides/install/devtools#finalcheck) that verify your install is complete and environment is correctly configured. Check-in with us to confirm your success[^1]

### 3. Command line practice
Over the course of CS107E, you'll get comfortable using the shell command line. To start, read through and follow all the steps in our [Unix Command Line Guide](/guides/unix). This guide will acquaint you with the basic commands for navigating the filesystem and viewing files.

> __Pro tip: pay attention to command responses__ As you explore, you may be trying commands copied from our guides or found in a tutorial. Rather than enter commands blindly, try to understand the intention of a command and its expected response. If you mistype or misuse the command, the action will fail or be applied incorrectly. Be sure to read each response carefully so you don't overlook the error message or bad outcome even when it is unhelpfully phrased or buried in other long-winded chatter. If you need help interpreting unix-speak, grab a staff member!
{: .callout-warning}

Here are some additional commands to explore that can help boost your productivity. Try these out now!

+ Typing a long path of a file or directory? Use tab-complete! Start typing the path and press `Tab` and the shell will automatically extend the partial path to the matching name. Try navigating to new directories in your file system and see what you can get to successfully tab-complete.
+ Want to repeat a previous command without having to re-type the whole thing? The up and down arrow keys (`↑` and `↓`) move through the commands in your history. Type `history` and see everything you've done in this session. Search through your history with `ctrl-r`!
+ You edit a command retrieved from your history (such as to correct a typo). Move the cursor with forward and backward with left and right arrows,`ctrl-a` to beginning of line, `ctrl-e` to end, `ctrl-k` deletes from cursor to end, `ctrl-u` deletes entire line.  (these are same line movement commands as Emacs, which you'll learn about in a bit!)
+ To redirect the output of a command into a file (for example, if a command produces a lot of output that you want to be able to read through at your leisure), use the `>` symbol like this:
```console
$ command_with_long_output > saved_out.txt
```
+ To view the contents of a file without opening it in a text editor, use `cat`, short for concatenate, like this:
```console
$ cat myfile.txt
```
+ Use `man` to pull up the manual page for a command, including sample usage and list of option flags. Try `man cp` and look for information about the `-r` option flag? A super-handy resource you'll want to bookmark is <http://explainshell.com> which can be used to dissect a command and its options. Ask it for an explanation of `cp -r` or another command you are curious about.

This is just the tip of the iceberg, there is a vast collection of additional commands available: `grep`, `diff`, `echo`, `find`, `sed`, and more. You can learn more these commands from the `man` page, entering on <http://explainshell.com>, asking Google or ChatCPT, or reading a tutorial. Make a point to learn a little more about the command-line each day to solve and you'll soon be well on your way to becoming an expert! 🔥

### 4. Editors: emacs and vim
In CS106A/B, you worked in a IDE (integrated development environments) such as PyCharm, Qt Creator, or Eclipse. While IDEs can be lovely for some tasks, in this course, we turn to the powerful __text editors__ that are the backbone of a professional developer's toolkit: Emacs and Vim.

No matter which editor you decide to make your primary, you want to know enough Emacs and Vim that you can open a file, make an edit, save and close in a pinch. This means you'll always be able to make a quick edit on a system you've just `ssh`'ed into where this is the only editor available.

Go through the steps for both Emacs and Vim now. Change directory to the subfolder `lab0` in your `mycode` repo. The folder contains two text files, `emacs_is_better.txt`and `vim_is_better.txt`.

- __Try emacs__
    - Open a file in emacs. (Note: on macOS, the emacs-style editor goes by the name `mg`)
    ```console
    $ emacs emacs_is_better.txt  # mg emacs_is_better.txt   (if on macOS)
    ```
    - Type to insert characters.
    - Once you're finished, type `ctrl-x` and then `ctrl-s` to save.
    - To exit, type `ctrl-x` and then `ctrl-c`.
- __Try vim__
    - Use this command to open a file in vim:
    ```console
    $ vim vim_is_better.txt
    ```
    - Vim starts in "command mode". You must first switch to "insert mode" by typing `i`, then you can type to insert characters.
    - When done editing, type `ESC` (escape key) to switch to command mode and type `:wq` to write the file and quit.

At this point, you might be wondering---why are programmers obsessed with these tools? These editors seem dated and rely on cryptic controls. For sure, the learning curve is much higher than your typical friendly GUI tool, but your investment of effort to learn and master the tool will make you into an editing ninja. These editors support highly agile and efficient workflows and allow extensive customization, far beyond what most GUI tools support.

Why are these tools so much faster? For one, they are launched directly from the command line and they are super-fast to open and minimal operating overhead. These editors support an extensive vocabulary of powerful commands for moving the cursor, searching text, editing, etc all accessible via key strokes. You can really streamline your workflow by keeping your hands on the keyboard and not always reaching for your mouse.

How can I configure and customize my editor? After some time working with your editor, you might decide that you want to display line numbers by default. Or that you prefer a different color scheme for syntax highlighting. Or have your editor always show you an XKCD comic upon startup. You can also add your own custom commands and key bindings. You Emacs or Vim `.config` file allows you to configure your default environment exactly to your liking. (Ask the CAs to share what they keep in their personal configs for ideas).

![XKCD Text Editors](images/real_programmers.png){: .zoom .w-75}

We won't dictate which text editor you must use (and the staff is divided into teams about which is better, anyway), but whichever you pick, make an investment in learning and mastering its features. For a start, right now, learn how to do the following things in your preferred text editor __without__ touching your mouse:
+ Move your cursor to the end of the line. Now move it to the start of the line. Practice moving back and forth.
+ Move your cursor one line down. Move it one line up. Go up and down and up down.
+ Scroll up a screenful and down a screenful. Practice makes perfect!
+ Learn how to turn on display of line numbers.  We promise you'll thank us later when there's a bug in line 173.
+ Delete an entire line of text all at once.
+ Now undo your last change. Hint: you'll have to break any previous habit of typing `ctrl-z` for undo. In unix-speak, `ctrl-z` sends a SIGSTP which pauses the process, try `fg` (foreground) to resume if that happens. (sneak peek of CS110...)
+ Find out how to search through the file for a given piece of text. Even better learn how to do a find-and-replace operation -- very handy when you need to refactor some code.
+ Learn how to copy, cut, and paste code, and then practice doing that over and over.

Spend the next week practicing the above tasks to build up your muscle memory. In the following weeks, look to further expand your repertoire by learning additional commands. You will feel a bit pokey at first when you have to keep looking up how to do things, we promise that with practice it'll become easier and easier, until you can't even remember a time when you needed to leave your keyboard while writing code💪.

Let us know how you're doing on with this check-in question[^2].

### 5. Website scavenger hunt
Our course website is where you'll find lecture slides, lab writeups, and assignment specifications, along with additional resources such as our course guides and curated set of readings, resources, and demos. Take a moment to acquaint yourself with what's available and how it's organized.  The __Search__ tab is handy for finding matching content across all pages.

See if you can find answers to such questions as:

+ I lost my Mango Pi reference card. Where can I find a new copy of the pinout? (p.s... Is there an command-line version I can use ?)
+ Where can I find the recommended readings to review before a lecture?
+ I've never used the gdb debugger before. Where do I start to learn?
+ What is the course policy on use of generative AI tools such as ChatGPT?
+ We saw a cool RISC-V simulator in lecture. How can I play with that myself?
+ I am curious about the final project. Are there examples of what has been done in the past to take inspiration from?

If a topic has you curious or confused, start by checking the course website for helpful information. If can't find the information you need, reach out on Ed or come visit us in office hours. We want you to always feel comfortable coming to us with any questions you have, as well as suggestions on how we can improve the quality of the resources available to you. You're ready for this check-in question [^3].

### 6. Binary numbers

Now that you know where the guides are located on the course web site, find the guides to binary numbers and bitmasking.  Read through both guides and then try working through these [bitwise practice problems](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/lab1/practice.html) courtesy of CS107. A fun practice tool is this [online visualizer for bitwise expressions](https://mdrach.github.io/cs107-apps/) written by CS107 CA Max Drach. If you have any trouble completing these binary/bitwise exercises, ask us for help[^4]

After have practiced with manual conversation to get the hang of it, now shunt the heavy lifting onto your tools. My goto for conversions is to fire up gdb. The gdb `print` command (nickname `p`) defaults to decimal format. Use `p/format` to instead select other formats such as `x` for hex, `t` for binary, and `c` for char. Try it out now!

```console?prompt=(gdb)
$ riscv64-unknown-elf-gdb
(gdb) p 68
$1 = 68
(gdb) p/x 68
$2 = 44
(gdb) p/c 68
$3 = 'D'
(gdb) p/t 68
$4 = 1000100
(gdb) p/t ~(7 << 3)
$4 = 11111111111111111111111111000111
```

## Check in with TA

We want you to periodically touch base with us as you work through the exercises. The check-in allows us to
verify your understanding and help with any unresolved issues. 

The points we'd like you to check in with us for this lab are listed below [^5]:

[^1]: Have you installed the development tools and confirmed each of the [final check](/guides/install/devtools#finalcheck) steps of the install guide?

[^2]: Do you feel comfortable with simple command-line tasks, basic use of your editor, and know where to go to learn more?

[^3]: Tell us which guide topics you have bookmarked to go back for a deeper read.

[^4]: If you had any trouble completing the binary/bitwise exercises, ask us for clarification.

[^5]:  Before leaving, check in and let us know which (if any) of the tasks were you not able to complete. Do you need assistance? How can we help?
