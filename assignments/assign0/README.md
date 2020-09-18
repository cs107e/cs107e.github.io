---
released: true
permalink: /assignments/assign0/
title: 'Assignment 0: Choose Lab Section, Learn Git'
toc: true
duedate: 2020-09-22 11:59 PM
---

{% comment %}
Task list to copy/paste when creating PR for this assign:

__Before releasing assign0:__
- [ ] Review writeup/starter files/process (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #221)

__To prep for assign0:__
- [ ] 

{% endcomment %}

*Written by Omar Rizwan, updated by Peter McEvoy for autumn 2020*

{% include duedate.html n=0 %}

<small>This assignment is not for a grade, but we do need you to complete it on-time so that you're set up to work on the rest of the assignments. If you run into troubles, please reach out on our [Piazza forum]({{ site.data.quarter.forum }}).</small>

## Goals

We have a single goal for this assignment: walk through the assignment workflow.
This will include initializing your assignments repo, downloading the starter
code, checking in your solution, and submitting it for grading.

This assignment will introduce you to the basics of the
[git](https://en.wikipedia.org/wiki/Git_(software)) version control
system and the [GitHub](https://github.com) repository hosting service that we will be using in this course.

## Prerequisites
Read over the following course guides before continuing:

- [Git](/guides/git)
- [Unix command line](/guides/unix)
- [Electricity](/guides/electricity)
- [Binary numbers](/guides/numbers)

We also assume that you have followed our [installation guide](/guides/install) 
to configure your laptop with the development tools and set up your `cs107e_home`
directory and git user identity.

{% include callout.html type="danger" %}
If you were not able to successfully complete all installation steps, please reach
out to the course staff for help before starting the assignment.
</div>

## Steps

### 1. Accept GitHub invitation

Our course repositories will be hosted on GitHub. You will download assignment 
starter files from GitHub and upload your completed work to GitHub for grading.

When enrolling in the course, you provided the username for your GitHub
account.  Each student will be given a private assignments repository inside our
[CS107e GitHub organization](https://github.com/cs107e). At this point, you
should have received two email invitations from GitHub: an invitation for
read-only access to the [starter code
repo](https://github.com/cs107e/assignments-mirror.git) and another invitation
for read-write access to your personal assignments repo. Once you receive and
accept both invitations, you're ready to proceed.


### 2. Set up SSH key

In order to access the starter code, you'll need set up an SSH key on your
GitHub account. An SSH key is simply a way for GitHub to authenticate that you
are who you say you are. To create an SSH key, enter the following at the
command line:

```
$ ssh-keygen -t rsa -b 4096 -C "<your_email>"
```

After you press enter, you'll be prompted to choose an alternate name for your
key. Skip this by pressing enter again. Next, you'll be prompted to enter a
passphrase for a key. If you want no passphrase, press enter. Otherwise, enter
your passphrase. Keep in mind that you'll need to enter your passphrase
everytime you push to or pull from GitHub if you choose to add a passphrase.

Let's verify that you created the key correctly:

```
$ ls ~/.ssh/
```

You should see two files: `id_rsa` and `id_rsa.pub`. SSH uses public-key
cryptography, which requires a private key and a public key. `id_rsa` is your
private key and should never be shared. `id_rsa.pub` is your public key and can
(and should) be shared.

Now that you've created your SSH key, you need to add it to GitHub. To do so,
follow [these
instructions](https://docs.github.com/en/github/authenticating-to-github/adding-a-new-ssh-key-to-your-github-account).
Note that if you're using macOS, you can replace step 1 with the following
command to copy your public key to your clipboard:

```
$ cat ~/.ssh/id_rsa.pub | pbcopy
```


### 3. Get assignment starter files

**Note**: For the rest of this writeup, **where you see
`[YOUR-GITHUB-USERNAME]`, replace with your actual GitHub username**.

Confirm that your personal assignments repository is available on GitHub by visiting 
in a browser: `https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments`.
You'll notice that it's empty. To fix that, we'll create a local repo, populate
it with the assignment starter code, and push to the remote repo.

Change to your `cs107e_home` directory and enter the following commands:

```
$ cd ~/cs107e_home
$ mkdir assignments
$ cd assignments
$ git init
$ git remote add starter-code git@github.com:cs107e/assignments-mirror.git
$ git remote -v
$ ls
$ git pull starter-code master
$ ls
$ git remote add origin git@github.com:cs107e/[YOUR-GITHUB-USERNAME]-assignments.git
$ git remote -v
$ git push --set-upstream origin master
```

Refresh your repo on GitHub, and you should see that the starter code that you
pulled from the `assignments-mirror` repo is now in your remote repo. What we
just did was pull the starter code from the `assignments-mirror` repo into your local
assignments repo and then push the starter code into your remote assignments 
repo on GitHub. You now have all of starter code for all of the assignments in
the local and remote copies of your repo. We did so by hooking up your local
repo to `assignments-mirror`, which we aliased (think, nicknamed) to `starter-code`, and to your
assignments repo, which we aliased to `origin`.

{% include callout.html type="info" %}
You can always get the latest tweaks to the starter code via `git
pull starter-code master`.
</div>


### 4. Create an assignment branch

To begin work on an assignment, you must first switch to the appropriate branch. 
We name all assignment branches using the same convention: `assignN` where N is 
the assignment number followed by the suffix `-basic` or `-extension` for basic 
and extension submissions, respectively. For example, an assignment 7 extension 
is done on the `assign7-extension` branch.

Since you now plan to work on the basic portion of assign0, you would switch to 
the branch named `assign0-basic`. But first, we need to create it.

```
$ git checkout -b assign0-basic
$ git branch
```

`git checkout -b` creates a new branch and switches us to it. `git branch` lists
the branches in our local repo and adds a star next to the branch that we're
currently on (`assign0-basic` in this case).

### 5. Record your lab preference

To get practice with adding and commiting a file with Git, go ahead and open up
`lab.txt` and edit its contents so that it displays the following information:

```
<assigned_lab_day> <assigned_lab_start_time>-<assigned_lab_end_time>
```

Save and quit the file. 

### 6. Commit your change locally and push it to remote

Once you confirm that Git has detected that you changed `lab.txt`, go ahead and 
add, commit (save to local repo), and push it (update remote repo):

```
$ git status
$ git add lab.txt
$ git commit -m "recorded my assigned lab time"
$ git push --set-upstream origin assign0-basic
```

You can use `git log` to view the history of commits in your local repo. Also
verify that your changed `lab.txt` file shows up in your remote repo on GitHub.

```
$ git log
$ open https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments
```

Also note that you can add multiple files (also called staging) and commit their
changes as a single commit. This is important because you don't every want your code 
to be in an inconsistent or bad state. For example, if you add a new file that 
another file references, you want to put those two changes together in a single 
commit; the file shouldn't be there if it isn't referenced, and you don't want 
code to reference a file that doesn't exist.

Each commit that you make records a snapshot of your work. You can use these 
snapshots to review your progress, compare versions, or identify and undo an 
unfortunate change.  Establishing good commit practices gives you the ability to
navigate the history of code changes, as well as providing the opportunity to 
revert to previous versions.  Sometimes you may find that you want to revert to 
a commit before a set a changes that introduced a bug in your code.  The more you 
commit, the more opportunities you will have to restore previous working states 
without losing as much work.  **We recommend that you commit early and often and 
that you write detailed commit messages.**


**Note:**  GitHub will show commits for `Branch: master` by default. To see the code and
changes for a specific assignment branch, switch to the branch by selecting it from  `Branch` drop-down menu :

<img title="Changing to another branch." src="images/03-change-branch.png" width="400">

### 7. Answer background questions
Create a file called `background.txt` that contains answers to the following
questions. Please ensure that your responses are clearly labeled (i.e. we know 
what question it correspond too) and reasonably organized.

1. What is the result of performing a bitwise OR of the following two
   hexadecimal numbers (include the result in decimal): 0x0f OR 0xff. Now left
   shift that number by 24; what is the result in hexadecimal? Assume numbers are 32-bit.

2. How many milliamperes of current will flow through a 1.5K resistor connected
   to 3V on one end and 0V (Ground) on the other end?

3. Find a file in the directory `/etc` on your computer; send us the name of the
   file and the number of bytes contained in that file. How many files and
   directories are contained in the first level of directory `/etc`? Include the commands you used to
   answer these questions.

Use the `git` commands you learned in the previous steps to stage this file and
commit it to your local repository. Then, push your changes to GitHub.

{% include callout.html type="info" %}
When you push this second time, there's no need to use `git push --set-upstream
origin assign0-basic` since you've already created the `assign0-basic` branch in
the remote repo; `git push` will suffice.
</div>

### 8. Create a pull request (submission)

In this course, assignment submission is done using GitHub's _pull request_
feature.  Pull requests provide a summary view of changes made to the code as
well as a section for comments where course instructors will be providing
feedback.

The pull request model is used by many modern software projects to promote good
practices when working in a distributed development environment. The typical
flow starts with a base branch (often called *master*) that reflects the master
copy of the code. Feature development or bug fixing occurs by creating a new
_branch_ where changes are made. Once you're confident that the code in your 
branch is polished, tested, and working well, you can ask other people to merge
the commits in your branch into *master* by making a _pull request_. You've
pushed commits to your branch, you're now asking other people to pull those
commits into master. Team members use the pull request to review the
proposed changes and provide comments. When a team member approves the pull request,
GitHub automatically merges the commits back into the base branch.

In this class, we use this same workflow that is so common in software development 
to review and grade your code. You write each assignment on a branch. To submit an 
assignment, you make a pull request to merge it with the base branch, signifying
your code is ready to be added to your slowly growing library of code you'll use
to build your shell. The grader reviews your code and gives comments and bug reports.

View the contents of your remote repository on the web at `https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments`. 
Switch to the branch `assign0-basic` by selecting it from the "Branch" drop-down
menu.

Click the 'New pull request' button.

<img title="We're on the right branch, so let's make a new pull request." src="images/04-new-pull-request.png" width="300">

Now you will see text entry boxes for describing the pull request, followed by
a list of the commits that you have made and a line-by-line comparison (`diff`)
of the changed files.

Set the Title to "Submit assign0 basic" and leave the description blank (for
future assignments you can include comments for your grader in the
description).
<img title="Ready to create the pull request." src="images/06-create-pull-request.png" width="500">

Click 'Create pull request', which will submit the pull request and take you to
a page to view it:

<img title="Pull request page." src="images/07-pull-request.png" width="500">

The pull request page will update to reflect any changes that are pushed after
the initial submission. This page will also be used by graders to provide feedback
on your submissions.

You make one pull request per assignment. If you need to update your submission 
after your initial pull request, simply edit your files, commit, and push to add
it the assign0-basic branch. There is no need to make another pull request. We will 
grade the last commit pushed to the branch, and that commit determines the submission 
time for deadlines and lateness. To repeat: submission time is determined by the
**time of the last commit you push**, not the time you clicked 'Create pull request'. 
If the deadline is 11:59 and you create a pull request at 11:55, and push an additional 
commit at 12:02, the submission is one day late.

## Afterward: Grader's code review

Graders will review code by including line-specific and general comments on the
pull request page:

<img title="Comments." src="images/08-comments.png" width="500">

Click the 'Files changed' tab to see your whole solution from top to bottom,
along with any inline comments that the grader has added.

<img title="Files changed." src="images/09-files-changed.png" width="500">

When finished with their review, the grader will submit it so that you can view
the feedback. Once you've received a completed review, you can click the Merge 
button to merge the `assign0-basic` branch into `master`. At this point, you can
safely delete the `assign0-basic` branch using the Delete button.
