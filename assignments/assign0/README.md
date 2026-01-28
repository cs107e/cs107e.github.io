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

Your goal for this assignment is two-fold:

1. Walk through and understand the git workflow we use in this course
2. Practice with your tools and refresh on background concepts

This assignment will give you practice with the standard tasks of getting starter code, editing files, 
committing changes, and submitting your work.

Have our <a href="/guides/cs107e-git#assignment-workflow" target="gwg">Git Workflow Guide</a> open in another browser tab. You will be following along with the section "Assignment Workflow" of the guide.

## Steps

### 0. Choose your editor and configure for use with git

In Lab 0, we asked you to explore text editors like vim and emacs. You
will be making heavy use of your chosen editor, so now is a good time to invest in
becoming comfortable and efficient with it.  This can be in the form of 
watching tutorial videos, reading manuals, bringing questions to OH, sharing tips with
each other, and practicing on your own.

Some git commands automatically open a text editor on your behalf. If you have not set an editor
in your git config, it uses a default choice.  Which editor does git use as its default?
Is this your preferred editor? If it is not, you can configure git to use the editor you prefer with the following command:

```console
$ git config --global core.editor [NAME-OF-YOUR-PREFERRED-EDITOR]
```

### 1. Pull starter code

To get the starter files, follow the instructions "Pull assignment starter code" in the <a href="/guides/cs107e-git#pull-assignment-starter-code" target="gwg">Git Workflow Guide</a>.

Pulling the starter code adds a new subfolder named `assign0` to  your `mycode` repo.
This folder contain the starter files for the assignment.

### 2. Answer background questions

In the `assign0` subfolder, find the file named `background.txt` and open it in your editor.

Add answers to the following questions.  Please try to keep the file reasonably organized and label your responses by question number.

> __Feeling stuck?__ Check out the guides here on
>  course website, come by office hours, post on Ed, discuss with a peer, do a web search, consult an LLM, ask for help!
{: .callout-question}

1. What is the result of a bitwise OR of these two
   hexadecimal numbers: `0x23 OR 0x4a`?  Now left
   shift that number by 5; what is the result?  Express your answers in both hex and decimal.

1. How many milliamperes of current will flow through a 1.5K resistor connected
   to 3V on one end and 0V (Ground) on the other end?

1. Explain difference between an absolute and relative path. Which path is referred to by the shorthand `~`? the shorthand `.`? the shorthand `..`?

1. Navigate to your `mycode` repo and find the directory named `cs107e`. What is
   the absolute path to this directory? How can you quickly change to this directory using the
   `CS107E` environment variable you created during your environment setup? Poke around
  in the `cs107e` folder to familiarize yourself with the files it contains. Try out the `pinout.py` command.

1. What editor will you be using? Confirm you have configured git to use your chosen editor. What is your plan for learning
   how to best use thus editor?

1. Start thinking through an arrangement of windows and tools that sets you up for an efficient workflow.
   It can be a real timesaver to keep multiple terminals open, i.e keep your editor open in one window and use a separate window to compile and execute rather than trying to do all in one window and repeatedly exit and re-enter the editor. Learn the key bindings so you can quickly switch between your code and your shell, without reaching for your mouse.
    Tell us what your setup you are using and the commands to switch between windows/tabs/panes.

1. Unix has an extensive collection of commands, but don't let this overwhelm you, plan to grow your repertoire on a need-to-know basis.
As a start, we recommend learning these key commands for interacting with the filesystem: `cd`, `pwd`, `ls`, `cat`, `mkdir`, `rmdir`, `touch`, `rm`, `echo`.
    Use `man [command]` in your terminal to review the manual page and learn more.
    For each command in our list, explain what it does in your own words and give an example of how you would use it.
    If there are options for the command, try out a few and tell us about options you think will be useful.

1. The shell includes many convenience features that are handy time savers. Below are three we want to be sure you are aware of. Try these out and tell us how you plan to incoporate them into your practice.
   - Tab completion. Type the prefix of a command into the shell and stop half-way, e.g. `riscv-` and type Tab. What happens? Tab completion also applies to commands arguments such as paths. Navigate to the folder `~/cs107e_home/mycode/assign0` by starting with a partial path `cd ~/c` and using tab completion to fill in the rest.

   - Command-line editing.  Good for those of us who make typos and need to correct them.  Type a command without hitting return and then edit. Move cursor with left/right arrow or movement control keys `ctrl-f`, `ctrl-b`, `ctrl-a`, `ctrl-e`. Make edits with delete key, `ctrl-d`, `ctrl-k`, `ctrl-u`, `ctrl-t`. (Emacs folk will find these combos familiar...)

   - Command history. Try the `history` command to see a list of your previously issued commands. Up-arrow to recall a previous command from history, you can also recall commands by number.  Try out `ctrl-r`, what does that do?

1. If there are resources (tutorials, blogs, videos, books, etc.) that you are finding particularly helpful as you get oriented with the environment and tools, tell us about them here and please share with everyone on our Ed forum!

Save your edits to `background.txt` and exit your editor.

### 3. Commit your changes locally and push to remote

Follow the instructions in "Assignment commit" of the <a href="/guides/cs107e-git#assignment-commit" target="gwg">Git Workflow Guide</a> to `git commit` a local snapshot, followed by `git push` to sync those changes to your remote repo.

After you do this, both the local and repo repositories should be up to date. Browse the `dev` branch of your remote repo on GitHub to confirm it is in sync.

<a name="submit"></a>
### 4. Make submit tag
Apply the tag `assign0-submit` to indicate these files are the ones to grade. Follow the instructions in the section "Assignment Tags"
in <a href="/guides/cs107e-git#assignment-tags" target="gwg">Git Workflow Guide</a> to create and push a tag.

Browse your remote repo on GitHub <https://github.com> and confirm that your new tag is listed.

### 5. Add photo file and edit README with your info

One of our cherished course tradition is posting the class roster on the "Wall of Fame" in Gates B02
to introduce all members of our community to one another. We
want to include you on our wall!

Find a photo of yourself that you would like to share. Copy this photo file into your `assign0` folder.

Edit the `README.md` file in the `assign0` folder to include the following information:
1. Your preferred name and pronouns 
2. Your hometown 
3. Your year and major

Add both the photo file and README.md to the staging index. Commit and push to your remote repo.

### 6. Move submit tag

To indicate this updated commit should replace your previous submission, move the submit tag.  Read the section "Assignment Tags" in the <a href="/guides/cs107e-git#assignment-tags" target="gwg">Git Workflow Guide</a> for instructions on how to move a tag.

Browse your remote repo on GitHub and confirm that your submit tag is placed on the commit which contains the correct content for all three files (`background.txt`, `README.md`, and your photo file).

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
