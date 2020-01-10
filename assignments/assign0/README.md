---
released: true
permalink: /assignments/assign0/
title: 'Assignment 0: Choose Lab Section, Learn Git'
toc: true
duedate: 2020-01-12 9:00 PM
---

{% comment %}
Task list to copy/paste when creating PR for this assign:

__Before releasing assign0:__
- [ ] Review writeup/starter files/process (TA)
- [ ] Followup on issues from previous quarter postmortem (issue #221)

__To prep for assign0:__
- [ ] 

{% endcomment %}

*Written by Omar Rizwan*

{% include duedate.html n=0 %}

<small>This assignment is not for a grade, but we do need you to complete it on-time to complete your enrollment and assign you to a lab section. If you run into troubles, please reach out on our [Piazza forum]({{ site.data.quarter.forum }}).</small>

## Goals

We have two goals for this assignment:

1. You'll tell us which lab section you prefer.
   (This'll be your 'solution' to the assignment.)

1. Have you go through the assignment workflow (download starter, check
   in your solution, and submit for grading) and ensure you can successfully complete these tasks.

This assignment will introduce you to the basics of the
[git](https://en.wikipedia.org/wiki/Git_(software)) version control
system and the [GitHub](https://github.com) repository hosting service that we will be using in this course.

Version control systems are widely used for software development in industry as
well as in the open source and academic communities.  These tools enable teams
of developers to write code in a distributed setting and send it to a central
repository for review and use.  Alongside keeping a log of changes to the code,
version control systems support the resolution of conflicts introduced by
multiple changesets (e.g. two people trying to edit the same lines of the same
file).  Web interfaces like GitHub promote the practices of code reviews, where
other team members will review proposed changes to check for bugs and provide
feedback on the design and implementation.  The version control skills that you
will learn in this course will be invaluable for any future software projects
that you decide to work on.

## Prerequisites
All students should read over our [course
guides](/guides) on the Review topics of electricity, binary numbers, and the unix command line.

The assignment assumes you have followed our [installation guide](/guides/install) to configure your laptop with the development tools and set up your `cs107e_home` directory and git user identity.

{% include callout.html type="danger" %}
If you were not able to successfully complete all installation steps, please reach out to the course staff for help before starting the assignment.
</div>

## Steps

### 1. Accept GitHub invitation
Our course repositories will be hosted on GitHub. You will download assignment starter files from GitHub and upload your completed work to GitHub for grading.

When enrolling in the course, you provided the username for your GitHub
account.  Each student will be given a private assignments repository inside our [CS107e GitHub organization](https://github.com/cs107e).  When the TAs set up the repository for your account, you will receive an
email invitation from GitHub. Accept this invitation and you're ready to proceed.

### 2. Get assignment starter files

**Note**: For the rest of this writeup, **where you see
`[YOUR-GITHUB-USERNAME]`, replace with your actual GitHub username**.

Confirm that your personal assignments repository is available on GitHub by visiting in a browser:
`https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments`

<img title="Sample assignment repo page" src="images/01-assignment-repo.png" height="200">

On your laptop, change to your `cs107e_home` directory and use git to clone your assignments repository:

```
$ cs ~/cs107e_home
$ git clone https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments assignments
remote: Counting objects: 3, done.
remote: Total 3 (delta 0), reused 0 (delta 0), pack-reused 3
Unpacking objects: 100% (3/3), done.
Checking connectivity... done.
```

{% include callout.html type="warning" %}
**NOTE**: You may have to enter your GitHub username and password to authorize
access to your GitHub account. To avoid having to supply these credentials every
time, see [instructions for connecting to GitHub using SSH](https://help.github.com/articles/connecting-to-github-with-ssh/).
</div>

You should now have a new folder named
`assignments` containing a copy of your GitHub assignments repository. We will refer to this copy as your __local__ repository; the repository on Github is the __remote__ repository. You will do all of your assignment work in the local repository and synchronize that work with your remote repository using git push and pull operations (more on that later).

Change to your local repository and confirm it is properly connected to your Github remote with the following command:
```
$ cd assignments
$ git remote -v
origin  https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments.git (fetch)
origin  https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments.git (push)
```

To begin work on an assignment, you must first switch to the appropriate branch. We name all assignment branches using the same convention: `assignN` where N is the assignment number followed by the suffix `-basic` or `-extension` for basic and
extension submissions, respectively. For example, an assignment 7 extension is done on the `assign7-extension` branch.

Since you now plan to work on the basic portion of assign0, you would switch to the branch named `assign0-basic`.

The `git checkout` command switches to a named branch. Use to switch to the `assign0-basic` branch and then list the directory contents.  There should be just one starter file: `lab.txt`

```
$ git checkout assign0-basic
Switched to branch 'assign0-basic'
$ ls
lab.txt
```

### 3. Indicate your lab preference

There are two lab sections that meet every week of the quarter: {{ site.data.quarter.labs }}. You will be assigned to attend one of the two labs, based on your preference/constraint. We must split the cohort fairly evenly to fit each half in our lab room, so we appreciate flexibility if you are able to provide it.  

Use the `lab.txt` file to indicate your lab preference.  Open the file in a text editor and replace the file contents with a single line containing one of the four options listed below.  For example, if you have preference for Tuesday but could go on Wednesday,
`lab.txt` should have one line that says `Tuesday`. If you can only make Wednesday,
then the file should hold a single line that says `Wednesday, can't make Tuesday`. Your
submission will be processed by an automated tool so **be sure to use the text exactly as it appears below**:

- `Tuesday`
- `Tuesday, can't make Wednesday`
- `Wednesday`
- `Wednesday, can't make Tuesday`

Save the file.

### 4. Commit your change

Git will recognize that you have edited a file in the repository.
Verify this by running `git status`:

```
$ git status
On branch assign0-basic
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   lab.txt

no changes added to commit (use "git add" and/or "git commit -a")
```

You *commit* your changes to record them into the repository history.
The log of commits provides a history of changes to the code,
akin to different versions.

In order to commit a changed file, you must first *stage* the file. This
fine-grained control allows you to selectively record changes.  For example, if 
two files have been edited, you may want to only commit changes to one file.
In this case, you would stage only the file of interest before committing,
and changes to the other file will not be included.

Use `git add` to add a changed file to the staging area:

```
$ git add lab.txt
```

`git status` reports what files are currently staged:

```
$ git status
On branch assign0-basic
Your branch is up-to-date with 'origin/assign0-basic'.
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	modified:   lab.txt
```

Once the desired files are staged, follow with `git commit` to record these changes:

```
$ git commit -m "Entered my lab preference."
[assign0-basic 019f726] Entered my lab preference.
 1 file changed, 1 insertion(+), 1 deletion(-)
```

The `-m` flag indicates the string argument to be
used as the commit message. Alternatively, the command `git commit` without the `-m` flag, causes git to open a text editor where you can enter a more detailed commit message. In practice, good commit messages contain a short (under 50 character) summary of
the changes on the first line, followed by a more thorough description of the
changes.  Detailed commit messages enable programmers to look back in the
history of changes to better understand when and why parts of the code were
changed.

Try out `git log` to view the history of commits in your current repository. 

```
$ git log
commit d1b60b978dbd59ec8fdb3377220bf9f6fce52a90 (HEAD -> assign0-basic)
Author: Julie Zelenski <zelenski@cs.stanford.edu>
Date:   Thu Jan 9 13:05:11 2020 -0800

    Entered my lab preference.
```


Each commit that you make records a snapshot of your work. You can use these snapshots to review your progress, compare versions, or identify and undo an unfortunate change.  Establishing good commit practices will give you the ability to navigate the history of
code changes, as well as providing the opportunity to revert to previous
versions.  Sometimes you may find that you want to revert to a commit before a
set a changes that introduced a bug in your code.  The more you commit, the
more opportunities you will have to restore previous working states without
losing as much work.  **We recommend that you commit early and often and that you write
detailed commit messages.**


### 5. Push to GitHub
The commits you have made so far are stored only in your local repository.  The next step is to push those changes to GitHub, which synchronizes the remote repository to match your local copy. Pushing to GitHub is also necessary to share your work with the course staff for grading.
```
$ git push
Counting objects: 3, done.
Delta compression using up to 2 threads.
Compressing objects: 100% (1/1), done.
Writing objects: 100% (3/3), 265 bytes | 0 bytes/s, done.
Total 3 (delta 0), reused 0 (delta 0)
To https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments.git
 * f1610d4a..00a3466f  assign0-basic -> assign0-basic
```

To confirm that your changes were properly received into your remote repository, view it on the web at
`https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments`.

**Note:**  GitHub will show commits for `Branch: master` by default. To see the code and
changes for a specific assignment branch, switch to the branch by selecting it from  `Branch` drop-down menu :

<img title="Changing to another branch." src="images/03-change-branch.png" width="400">

![After pushing.](images/02-show-commit.png)

### 6. Answer background questions
Create a file called `background.txt` that contains answers to the following
questions.  Responses to these questions will be graded by a human, so no need
to follow strict formatting requirements.  However, please ensure that your
responses are clearly labeled (i.e. we know what question it correspond too)
and reasonably organized.

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
commit it to your local repository. Then, push your changes to your GitHub.

### 7. Create a pull request (submission)

In this course, assignment submission is done using GitHub's _pull request_
feature.  Pull requests provide a summary view of changes made to the code as
well as a section for comments where course instructors will be providing
feedback.

The pull request model is used by many modern software projects to promote good
practices when working in a distributed development environment. The typical
flow starts with a base branch (often called *master*) that reflects the master
copy of the code. Feature development or bug fixing occurs by creating a new
_branch_ where changes are made, followed by the submission of a _pull request_
to the original base branch. Team members use the pull request to review the
proposed changes and provide comments. Once the changes have been approved,
they are merged back into the base branch. In this class, we use a similar
model, except a grader will be reviewing your code and the base branch is the
starter code.

View the contents of your remote repository on the web at `https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments`. Switch to the branch `assign0-basic` by selecting it from the "Branch" drop-down menu.

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

There is a section indicating whether our automated checks have passed.  You
may need to wait a minute or two and refresh if have not finished.  If the
checks have passed, you will see a green checkmark and a message: "All checks
have passed". If the checks did not pass, make sure you have inserted a valid
choice in `lab.txt` (one of the four choices we provided) and added a 
`background.txt` with answers to the background questions.

Each assignment has a set of automated checks that will be run upon submission. For this
assignment, the check just verifies that `lab.txt` contains one of the four
valid choices and that the `background.txt` file exists. Note the initial starter code will not pass the check until both criteria are met. To see more about the results and why the check is failing, click on the 'Details' link (you may be prompted to register for Travis and sync your Github).

The pull request page will update to reflect any changes that are pushed after
the initial submission (e.g. to fix any errors the checker detected).  This
page will also be used by graders to provide feedback on your submissions.

You make one pull request per assignment. If you need to update your submission after your initial pull request, simply edit your files, commit, and push to add it the assign0-basic branch. There is no need to make another pull request. We will grade the last commit pushed to the branch, and that commit determines the submission time for deadlines and lateness. To repeat: submission time is determined by the **time of the last commit you push**, not
the time you clicked 'Create pull request'. If the deadline is 9:00 and you create a pull request at
8:58, and push an additional commit at 9:02, the submission is one day late.

## Afterward: Grader's code review

Graders will review code by including line-specific and general comments on the
pull request page:

<img title="Comments." src="images/08-comments.png" width="500">

Click the 'Files changed' tab to see your whole solution from top to bottom,
along with any inline comments that the grader has added.

<img title="Files changed." src="images/09-files-changed.png" width="500">

When finished with their review, the grader will click the Merge button
themselves. This is similar to how a coworker would merge a pull request when
they are satisfied with the quality of the code changes that you have proposed.
