---
title: "Guide: Git workflow for CS107e"
toc: true
quarterprefix: win26
attribution: Written by Maria Paula Hernandez, incorporating work of past TAs
---

In this course, we use git repositories to share code between you and us. 
These repositories are hosted on GitHub. You will use git commands such as `git pull`
to access starter code and `git push` to submit your work. You can read more about
the general use of `git` in [our git guide](/guides/git). To set up the `mycode` repo used in this course, check out [our git workflow setup guide](/guides/setup-mycode-repo).

In this document, we walk through the specific workflow to access the starter code and submit your work.

> __Note__ This guide assumes that you've already set up your `mycode` repo and development environment using the [Git workflow setup guide](/guides/setup-mycode-repo).
{: .callout-warning}

## Organization of the `mycode` repo
Your `mycode` repo is organized by lab and assignment. There will be one folder for each lab (named `lab0`, `lab1`, and so on) and one folder for each assignment (named `assign0`, `assign1` and such). Each of these folders contains the code and files specific to the named lab or assignment. There is also a folder named `cs107e` containing shared files that are used throughout.

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
placeholder. When you issue the command, be sure to replace the placeholer with the specific name you are intend to work on, e.g. `assign1` or `lab2`.

## Lab workflow

When starting a new lab, update your local `mycode` repo by checking out the `dev` branch and pulling the lab starter files from the remote (change `labX` to `lab1`, `lab2`, as appropriate):

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull code-mirror labX-starter
```

(The `git pull` command may open up your editor and display the message `Merge branch labX-starter into dev`. Confirm the merge by saving and exiting the editor.)

After these commands, your repo is on `dev` branch and all files are up to date. `cd` to the `labX` folder and use `ls` to see the new files.

You don't need to commit or submit lab code to be graded. We use in person check-in during lab to confirm participation.

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
repo.

> __Note__ We strongly encourage you to develop the habit of commit __early and often__. Any time you make a change that you'd be upset to lose, take a moment to make a commit (usually, this is at least once an hour, often more frequently). Also make regular pushes to send your latest commits to the remote. This means you have an offiste backup if anything scary should befall your laptop!
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
without losing as much work. __We recommend that you commit early and often and
that you write detailed commit messages.__

> Git command responses can sometimes look cryptic and scary. DON'T BE SCARED!
> The worst thing you can do is just ignore the responses altogether, especially when you get warnings and errors.
> Git is your friend :) Reading all the responses will help
> you understand the commands your running, and prevent you from missing
> important messages about the state of your repo. 
> {: .callout-info}

You can view the contents of your remote repo by browsing it on GitHub.
__Note:__ GitHub will show commits for the `master` branch by default. To see activity
on the `dev` branch, switch to it by selecting from the drop-down menu:

![Github change to another branch](../images/github-change-branch.png){: .zoom .w-75}

### Assignment tags

When grading your work, we'll run tests on your submission to verify its functionality. You must mark which commit you want us to test by applying a tag.
A _tag_ attaches a name to a commit. We'll
also use this tag to determine your time of your submission.

Create a tag to identify your assignment submission by doing the following (replace `assignX` with the appropriate `assign0`, `assign1`, etc.)

```console
$ git tag assignX-submit
$ git push --tags
```

A submission tag is of the format `assignX-submit`
where `X` is the particular assignment number. You can verify that your tag
was successful by browsing your repo on GitHub and
confirming that your newly created tag shows up in the __Tags__ section of the branches
dropdown menu.

#### Adding a tag to a speicfic commit 
If you say mispelled a tag, forgot to tag, or otherwise need to go back and add a tag to an older commit here is how. 

First you will need to find the commit hash of the tag you want. The commit hash is the really long sequence of letters and numbers next to each commit. 
Note you only need to copy the first seven or eight characters of the hash. To see a list of all your commits and their associated hashes run: 
```console 
$ git log
```

From there to add a tag to this commit you can run:
```console
$ git tag assignX-submit [paste hash here and remove brackets]
$ git push --tags
```

<A name="resubmit"></a>
#### Move tag for resubmit
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

### Assignment checklist

This guide contains a lot of information. Here is a checklist to follow when 
you are submitting your assignments. 

To submit an assignment:
1. Ensure all of your code is committed and pushed (see [Assignment commit](#assignment-commit))
1. Tag with `assignX-submit` and, if applicable, `assignX-extension` (see [Assignment tags](#assignment-tags)) and tags are pushed.
1. If you want to replace a previous submission, move the submit tag to the updated commit and force push.
1. You can confirm which files you submitted by browsing your remote repo on GitHub. Choose `assignX-submit` tag from the dropdown menu to review the submitted files and their contents.

After receiving our grading feedback, you will have the opportunity to submit fixes
 and request a re-test of open issues. To submit fixes for re-test:
1. Ensure all of your code is committed and pushed. 
2. Tag with `assignX-retest` and push the tag to remote.

### Assignments test results/issues

To evaluate functionality, we run your code through a sequence of automated tests 
and report any bugs found by posting 
issues on your remote Github repository. 
View the `Issues` tab on Github to see a list of your issues. Each issue corresponds to a failed
test. If the test result includes output log files, the `student` log file is the output generated when running
your code and the `staff` log file is the output generated by our staff solution.  Comparing the staff
output to your own will give you insight as to why your code failed the test. 


### Assignment retest

After receiving our grading feedback, you may use revise and resubmit to correct issues with
your previous submission.  After correcting the code and doing your own testing, commit
and push the correct code and request a retest by tagging the commit with `assignX-retest`.
We will try to run one retest run per week on all repos that have updated retest tags.
After retest, we will update the __Issues__ tab to show the updated results and close any resolved issues.

Eligible for retest:
- Core functionality for all libmango modules, assignments 2-7.

Not eligible for retest:
- Extensions 
- Quality/style
- Assignment 1 larson scanner and Assignment 2 clock application (retest __is__ available for gpio and timer modules).

Code ineligible for retest is graded on what was committed in the original submission.

