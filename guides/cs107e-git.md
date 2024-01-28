---
title: "Guide: Git workflow for CS107e"
toc: true
quarterprefix: winter24
attribution: Written by Maria Paula Hernandez, incorporating work of past TAs
---

In this course, we use git repositories to share code between you and us. 
These repositories are hosted on GitHub. You will use git commands such as `git pull`
to access starter code and `git push` to submit your work. You can read more about
the general use of `git` in [our git guide](/guides/git). To set up the `mycode` repo used in this course, check out [our git workflow setup guide](/guides/setup-mycode-repo).

In this document, we walk through the specific workflow to access the starter code and submit your work.

> **Note** This guide assumes that you've already set up your `mycode` repo and development environment using the [Git workflow setup guide](cs107e-git-setup).
{: .callout-warning}

## Organization of the `mycode` repo
Your `mycode` repo is organized by lab and assignment. There will be one folder for each lab (named `lab0`, `lab1`, and so on) and one folder for each assignment (named `assign0`, `assign1`, ...). Each of these folders contains the code and files specific to the named lab or assignment. There is also a folder named `cs107e` containing tools, header and library files that are used throughout the course.

```console
$ cd ~/cs107e_home/mycode
$ ls
assign0/   assign2/    assign4/    lab1/       lab3/       cs107e/
assign1/   assign3/    lab0/       lab2/       lab4/
$ ls assign1
Makefile    larson.s
```

Your typical workflow will be to change to the subfolder for the particular lab or assignment 
you are currently working on. Within that subfolder, you edit files, build and run your code, make git commits, and so on.

In the instructions for the workflows we give below, we use `assignX` or `labX` as a generic
placeholder. When you issue the command, be sure to use the specific name for the assign or lab you intend to work on, e.g. `assign1` or `lab2`.

## Lab workflow

When starting a new lab, update your local `mycode` repo by checking out the `dev` branch and pulling the lab starter files from the remote (change `labX` to `lab1`, `lab2`, as appropriate):

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror labX-starter
```

(The `git pull` command may open up your editor and display the message `Merge branch labX-starter into dev`. Confirm the merge by saving and exiting the editor.)

After these commands, your repo is on `dev` branch and all files are up to date. `cd` to the `labX` folder and use `ls` to see the new files.

You don't need to commit or submit lab code to be graded. The staff will review your work during lab and check off your participation.

<a name="assign-workflow"></a>

## Assignment workflow
Here are the steps to start, work on, and submit an assignment.

### Pull assignment starter code

When starting a new assignment, update your local `mycode` repo by checking out the `dev` branch and pulling the assignment starter files from the remote (change `assignX` to `assign1`, `assign2`, etc. as appropriate):

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror assignX-starter
```

(The `git pull` command may open up your editor and display the message `Merge branch assignX-starter into dev`. Confirm the merge by saving and exiting the editor.)

After these commands, your repo is on `dev` branch and all files are up to date. `cd` to the `assignX` folder and use `ls` to see the new files.

### Assignment commit
You will work on your assignments in your local `mycode` repo. As you edit, we recommend that you
make regular git commits to record a snapshot of your work and track your progress. Each commit is
saved in your local repo. You can follow up with a `git push` to sync those changes with your remote
repo. You can consider your remote repo as a sort of "offsite backup".

> **Note** Everyone commits and pushes their code at different rates, but we highly encourage you to commit __early and often__. Any time you make a change that you'd be upset to lose, make sure you commit your code (usually, this is at least once an hour, often more frequently). Any time you put away your laptop, we highly encourage you to push your code as well, since you'll be grateful to have a backup if something happens to your computer!
{: .callout-warning}

To see the current state of your repo, use the `git status` command. It will indicate which files have been modified, which changes are "staged" (ready to be commmitted) and whether the local and remote repos are currently in sync.
```console
$ git status
```

The commands below show how to add a changed file to the staging index, commit staged changes (save snapshot to local repo), and push the commit to GitHub (update remote repo):

```console
$ git add filename.c
$ git commit -m "Short but descriptive message about the changes you are committing"
$ git push
```

The command `git add .` can be used as a
shorthand for adding all changed files to the staging index rather than adding individual files by name.

You can use `git log` to view the history of commits in your local repo. When
you enter the log, you can exit out by pressing "q" on your keyboard.

```console
$ git log
```

Each commit that you make records a snapshot of your work. You can use these
snapshots to review your progress, compare versions, or identify and undo an
unfortunate change. Establishing good commit practices gives you the ability to
navigate the history of code changes, as well as providing the opportunity to
revert to previous versions. Sometimes you may find that you want to revert to
a commit before some code changes that introduced a bug. The more frequently you
commit, the more opportunities you have to restore previous working states
without losing as much work. **We recommend that you commit early and often and
that you write detailed commit messages.**

> Git command responses can sometimes look cryptic and scary. DON'T BE SCARED!
> The worst thing you can do is just ignore the responses altogether, especially when you get warnings and errors.
> Git is your friend :) Reading all the responses will help
> you understand the commands your running, and prevent you from missing
> important messages about the state of your repo. 
> {: .callout-info}

You can view the contents of your remote repo by browsing it on GitHub.
**Note:** GitHub will show commits for the `master` branch by default. To see activity
on the `dev` branch, switch to it by selecting from the drop-down menu:

![Github change to another branch](../images/github-change-branch.png){: .zoom .w-75}

### Assignment tags

When grading your work, we'll run tests on your submission to verify its functionality. You indicate which commit you want us to test by applying a tag.
A _tag_ is simply a way of giving a name to a particular commit. We'll
also use this tag to determine your submission time. The submission time recorded
will correspond to the time you pushed the tag commit.

Create a tag to identify your assignment submission by doing the following (be sure to replace `assignX` with `assign0`, `assign1`, etc. as appropriate)

```console
$ git tag assignX-submit
$ git push --tags
```

A submission tag is of the format `assignX-submit`
where `X` is the particular assignment number. You can verify that your tag
was successful by browsing your repo on GitHub and
confirming that your newly created tag shows up in the __Tags__ section of the branches
dropdown menu.

If you need to update your previously tagged submission with additional changes, you can move the tag to a different commit with the following command
(note that you need to push `--force` when moving a tag that already exists).

```console
$ git tag -f assignX-submit
$ git push --tags --force
```

If you complete one of the extensions for the assignment, add the tag `assignX-extension`
on the commit containing the code for the extension. Your extension commit can be tagged on
same commit as your basic functionality, or it can be a different one. We use the extension
tag to identify which submissions have attempted the extension. If there is no extension
tag, we will not test/grade it and will assume the extension was not attempted.

See the section below entitled 'Assignment Submission' for a checklist of steps to 
complete when you submit your assignment. 

### Assignment pull request

In this course, assignment submission is done using GitHub's _pull request_
feature. Pull requests provide a summary view of changes made to the code as
well as a section for comments where course instructors will be providing
feedback.

The pull request model is used by many modern software projects to promote good
practices when working in a distributed development environment. The usual
flow starts with a base branch (typically named _master_ or _main_) that reflects the mainline
version of the code. Feature development or bug fixing occurs by creating a new
_branch_ where changes are made. After you have confirmed that the code in your
branch is working correctly and have finished testing and polishing, you ask other people to approve
the commits from your branch by making a _pull request_.  Team members respond on the pull request to review the
proposed changes and provide comments. When a team member approves the pull request,
GitHub automatically merges the commits back into the base branch.

__You will only need to make one pull request__. You will open a pull request for your
first submission and it will remain open throughout the quarter. You will not need
to make an other pull request. If you accidentally close your previous pull request, just open a new one following the steps below.

To open a new pull request: View the contents of your remote repository on the web at `https://github.com/cs107e/{{page.quarterprefix}}-[YOUR-GITHUB-USERNAME]`.
Switch to the branch `dev` by selecting it from the __Branch__ drop-down
menu. (Reminder: there will not be an option to open another pull request if there is already one open for this branch)

Click __Contribute__, then __Open pull request__.
![Github open pull request](../images/github-open-pr.png){: .zoom .w-75}

In the title field, enter "Submission pull request" and leave the comment description text area blank.
![Github create pull request](../images/github-create-pr.png){: .zoom .w-75}

Click the button __Create pull request__, which will submit the pull request and take you to
a page to view it:

![Github view pull request](../images/github-view-pr.png){: .zoom .w-75}

The pull request page will update to reflect any changes that are pushed after
the initial submission. This page will also be used by the grader to provide feedback
on your submissions.

If you need to update your submission, simply edit your files, commit, re-tag with `assignX-submit`, and push to add
it to the pull request. You do not need to make another pull request.

### Assignment checklist

This guide contains a lot of information. Here is a checklist to follow when 
you are submitting your assignments. 

To submit an assignment:
1. Ensure all of your code is committed and pushed (see [Assignment commit](#assignment-commit))
1. Tag with `assignX-submit` and, if applicable, `assignX-extension` (see [Assignment tags](#assignment-tags)) and tags are pushed.
1. Confirm your Pull Request is open (see [Assignment pull request](#assignment-pull-request))
1. If you want to replace a previous submission, move the submit tag to the updated commit and force push. You do not need to change your open Pull Request.
1. You can confirm which files you submitted by browsing your remote repo on GitHub. Choose `assignX-submit` tag from the dropdown menu to review the submitted files and their contents.

After receiving our grading feedback, you will have the opportunity to submit fixes
 and request a re-test of open issues. To submit fixes for re-test:
1. Ensure all of your code is committed and pushed. 
2. Tag with `assignX-retest`
3. Confirm your Pull Request is open.

### Assignment grading feedback

Feedback on __functionality__

To evaluate functionality, we run your code through a sequence of automated tests 
and report any bugs found by posting 
issues on your remote Github repository. 
See the `Issues` tab for a list of your open issues (bugs). Each issue has
a link to the log files. The `student` log file is the output generated when running
your code and the `staff` log file is the output generated by our staff solution.  Comparing the staff
output to your own will give you insight as to why your code failed the test. 
As you fix your bugs and submit your debugged code, the issues will be closed in the __Issues__
tab and the logs will disappear from your logs folder. Your goal will be to close all 
high priority issues. Below we discuss the process for submitting your code for re-testing after 
fixing bugs. 

Feedback on __quality__ (code style and tests)

To evaluate quality, the grader reviews your code and test coverage. To read the grader comments,
view your submission pull request on the __Pull Requests__ tab of your remote Github repo. The grader
comments will be added on the __Conversation__ tab.

### Assignment bug fixes/re-test

After receiving our grading feedback, you may resubmit bug fixes to resolve 
open issues. To request a re-test after making fixes, tag the commit with `assignX-retest`.
You may do this as multiple times as you make further changes, simply keep moving the
`assignX-retest` tag to the commit that you want re-tested. 
We will try to run one re-test run per week on all repos with updated retest tags.
We communicate the re-test results by updating your repo __Issues__ and logs folder
to show which issues are now closed by your fixes. 

**Note that not all code is eligible for re-test.**

Code that is eligible for re-test: 
- Core functionality for assignments 2-7. 

Code that is not eligible for re-test: 
- Extensions 
- Quality (code style/tests)
- Assignment 1 larson scanner and Assignment 2 clock application (you may re-test bug fixes for assign2 gpio and timer modules).

Code ineligible for re-test is graded on what was committed at the time of the original submission.

