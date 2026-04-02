---
released: true
permalink: /assignments/assign0/
title: "Assignment 0: Git oriented"
attribution: Assignment written by Maria Paula Hernandez, CS107e TA

toc: true
---

{% comment %}
Task list to copy/paste when creating PR for this assign:

__Before releasing assign0:__

- [ ] Review writeup/starter files/process (TA)
- [ ] Followup on issues from previous quarter postmortem ()

__To prep for assign0:__

- [ ]

{% endcomment %}


{% include duedate.html n=0 %}

<small>This assignment is not for a grade, but it's critical that you complete it on-time and understand the steps in the workflow you will be using for all assignments. If you run into troubles, please reach out on Ed or come by office hours!</small>

## Goals

Your goals for this assignment are to:

1. Learn and practice the git assignment workflow we use in this course
2. Start getting comfortable with the environment and tools
3. Review background topics binary/bits and simple circuits

## Steps

### 0. Configure git to use your chosen editor

Some git commands automatically open a text editor on your behalf. If you have not set an editor
in your git config, it uses a default choice.  Which editor does git use as its default?
Is this your preferred editor? If it is not, you can configure git to use the editor you prefer with the following command:

```console
$ git config --global core.editor [NAME-OF-YOUR-PREFERRED-EDITOR]
```

### 1. Pull starter code

The assignment workflow is to __pull__ the starter files, __edit__ the files,
__commit__ changes and lastly __tag__ and __push__ to submit your work.

Have our <a href="/guides/cs107e-git" target="gwg">Git Workflow Guide</a> open in a separate tab and find the the section "Assignment Workflow" to follow along. To get the starter files, follow the workflow instructions in the section <a href="/guides/cs107e-git#pull-assignment-starter-code" target="gwg">"Pull assignment starter code"</a>.

Pulling the starter code adds a new subfolder named `assign0` to  your `mycode` repo.
This folder contain the starter files for the assignment.

### 2. Practice with your environment
If you are pretty new to unix, you'll benefit from working through the all of the suggested explorations below for practice. If you are already feeling comfortable, give a skim over our list and maybe you can pick up a new trick or two.

- Navigate filesystem, poke through `mycode` repo
    - Learn core commands such as `pwd` (print working directory), `cd` (change directory), `ls` (list files), `cat` (show file contents), and commands to create/delete/rename files and directories (`mkdir`, `rmdir`, `touch`, `rm`, `cp`, `mv`)
    - Know the difference between an absolute and relative path and how to use the shorthand `~` (home directory), `.` (current directory), `..` (parent of current directory).
    - Change to your `mycode` repo and poke around to see how the folders are arranged. Change to `cs107e` and familiarize yourself with which files it contains. In lab0, you created an environment variable `$CS107E` which gives you a nickname for this path. Try `echo $CS107E` to view the value or `cd $CS107E` to change to the directory.  Try the `pinout.py` command.

- Learn your chosen editor
    - Confirm you have configured git to use your chosen editor.
    - Know how to use your editor to open a file, insert/delete characters, move the cursor, save, and exit.
    - Once you have the basics down, start thinking of which additional commands you want to learn: fancier movement, copy/paste, search, managing buffers, ...
    - Find helpful resources you can learn from: watch tutorial videos, read user manual, do a web search, post on Ed, come to OH, ask your handy LLM tutor.
    - You will be making heavy use of your chosen editor, so now is a good time to invest in becoming comfortable and facile with it!

- Plan arrangement of windows and tools that sets you up for an efficient workflow.
    - Customize window size, location, color, font to your taste.
    - It is a real timesaver to keep multiple terminals open, i.e keep your editor open in one window and use a separate window to compile and execute rather than trying to do all in one window and repeatedly exit and re-enter the editor.
    - Learn the key bindings so you can quickly switch between your code and your shell, without reaching for your mouse.

- Start building your repertoire of Unix commands
    - There are an enormous number of these, rather than get overwhelmed trying to learn them all, plan to grow your repertoire on a need-to-know basis.
    - Use `man [command]` in your terminal to review the manual page and learn more, including how to use command-line flags to access options.
    - Library of [video walk-throughs](https://web.stanford.edu/class/archive/cs/cs107/cs107.1186/unixref/) for common commands made for CS107 students
    - Bookmark this handy tool <http://explainshell.com> Enter a command and it will break it down, explain what it does and what options it uses.

- The shell includes many convenience features that can save you time and repetition. Learn to use these sooner rather than later!
    - __Tab completion__ Type the prefix of a command into the shell and stop half-way, e.g. `riscv-` and type Tab. What happens? Tab completion also applies to commands arguments such as paths. Navigate to the folder `~/cs107e_home/mycode/assign0` by starting with a partial path `cd ~/c` and using tab completion to fill in the rest.
    - __Command-line editing__  Good for those of us who make typos and need to correct them.  Type a command with a typo without hitting return and then edit to fix the typo. Move cursor with left/right arrow or movement control keys `ctrl-f`, `ctrl-b`, `ctrl-a`, `ctrl-e`. Make edits with delete key, `ctrl-d`, `ctrl-k`, `ctrl-u`, `ctrl-t`. (Emacs folk will find these combos familiar...) Hit return once finish editing to execute the command.
    - __Command history__ Try the `history` command to see a list of your previously issued commands. Up-arrow to recall a previous command from history, you can also recall commands by number.  Try out `ctrl-r` to search for a matching command in your history. You can use command-line editing on the restored command to tweak/fix/repeat it.

- Git commands to learn:
    - `git add` to add files to the staging area, `git commit` to commit contents of staging area, `git push` to sync local repo with remote
    - `git status` to see which files have changed and which of those changes are in staging area, `git diff` to see the content that has changed
    - `git log` to view past commit history


> __Share your finds__
If there are resources (tutorials, blogs, videos, books, etc.) that you are finding particularly helpful as you get oriented, please share with the class by posting on Ed forum!
{: .callout-info}

### 3. Review background topics

__Binary/hexadecimal, bitwise operators__
- Go through our course guide on [binary and hexadecimal numbers](/guides/numbers). It has a checklist of skills to practice and self-test questions to confirm your understanding.

__Electricity/circuits__
- Review our course guide on [electricity basics](/guides/electricity). We are not expecting you to have an extensive background, just enough of the basics (e.g. `V=IR`) to construct simple circuits such as one to light an LED (power->led->resistor>ground) or use a button as a switch.

### 4. Edit README.md to share your results

In the `assign0` subfolder, find the file named `README.md` and open it in your editor.

Give a short (1-2 sentences) summary of your comfort level and readiness on each of the three below:
1. Getting around in the unix environment
2. Binary/bitwise manipulation
3. Simple circuits

For the final entry in your `README.md`, we ask for a statement of understanding about use of LLMs for this course. First read the course [policy on use of LLMs](/policies/#assistance-from-generative-ai) to hear our take on the opportunities for appropriate use and prohibitions against abuse. We are looking
for confirmation that you understand these boundaries and pledge to only ethical use within the constraints of the policy. Give us a sentence or two in your own words that shows that we are all on the same page.

Save your edits to `README.md` and exit your editor.

### 5. Commit your changes locally and push to remote

Follow the workflow instructions in the section <a href="/guides/cs107e-git#assignment-commit" target="gwg">"Assignment commit"</a> to commit your changes, followed by a push to sync those changes to your remote repo.

After you do this, both the local and repo repositories should be up to date. Browse the `dev` branch of your remote repo on GitHub to confirm it is in sync.

<a name="submit"></a>
### 6. Make submit tag
Apply the tag `assign0-submit` to indicate these files are the ones to grade. Follow the workflow instructions in the section <a href="/guides/cs107e-git#assignment-tags" target="gwg">"Create a tag"</a> to create and push the tag.

Browse your remote repo on GitHub and confirm that your new tag is listed.

### 7. Add photo file and move submit tag

Oops, we forgot something in the submit. One of our cherished course tradition is posting a class roster of photos on the "Wall of Fame" in Gates B02
to introduce all members of our community to one another. We
want to include you on our wall!

Find a photo of yourself that you would like to share. Copy this photo file into your `assign0` folder. Add the photo file to the staging index and commit. Now push to sync to the remote repo.

To indicate this updated commit should replace your previous submission, you must move the submit tag from the previous commit to this one.  Follow the workflow instructions in the section <a href="/guides/cs107e-git#assignment-tags" target="gwg">"Move tag for resubmit"</a>.

Browse your remote repo on GitHub and confirm that your submit tag is placed on the commit which contains the correct content for both files (`README.md` and your photo file).

## Success!
You have now successfully navigated the full assignment workflow:
- pull starter files
- edit files
- add and commit changes
- push to remote
- make submit tag
- update submission and move tag
- confirm results on remote repo on Github

And you're ready for the further adventures to come!
