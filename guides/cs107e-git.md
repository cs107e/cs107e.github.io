---
title: "Guide: Git workflow for CS107e"
toc: true
quarterprefix: winter23
---

_Written by Maria Paula Hernandez, incorporating material from past documents_. 

In this course, we use git repositories to share code between you and us. 
These repositories are hosted on GitHub. You will use git commands such as `git pull`
to access starter code and `git push` to submit your work. You can read more about
the general use of `git` in [our git guide](/guides/git). In this document, we go through the
details of the `mycode` repo and the specific workflow to access starter code and submit your work.

## Configure local `mycode` repo
Each student has their own `mycode` repo, which manages all of the code for the coures assignments and labs.

On GitHub, we create a personal repository for each student within our CS107e organization. This repo will be named `https://github.com/cs107e/{{page.quarterprefix}}-[YOUR-GITHUB-USERNAME]`, where [YOUR-GITHUB-USERNAME] is replaced with your actual github username.

Your personal repository that resides on GitHub 
is your __remote__ mycode repo. It will be connected to 
the __local__ mycode repo on your computer where you will work on your code.

After we have set up your remote repo on GitHub, you will connect it to the local repo on your computer where you will do your work.

The following instructions are used to configure your local `mycode` repo.  Do these steps __once__
 at the beginning of the course. Be sure to ask for help if you run into any snags.

### Step 1: Accept GitHub invitations

- You should have received two email invitations from GitHub: an invitation for read-only access to the starter code repo <https://github.com/cs107e/code-mirror.git> and another invitation for read-write access to your personal repo `{{page.quarterprefix}}-[YOUR-GITHUB-USERNAME]`. Once you receive and accept both invitations, you're ready to proceed.

### Step 2: Create SSH key and add to GitHub account

- To streamline interacting with GitHub from the command line, you'll need to add an SSH
    key on your GitHub account. An SSH key is a way to authenticate
    that you are who you say you are. To create an SSH key, enter the following
    command in your shell:

    ```console
    $ ssh-keygen -t rsa -b 4096 -C "<your_email>"
    ```

    After you press enter, you'll be prompted to choose an alternate name for your
    key. Skip this by pressing enter again. Next, you'll be prompted to enter a
    passphrase for a key. If you want no passphrase, press enter. Otherwise, enter
    your passphrase. If you choose to add a passphrase, you must enter that passphrase each time you push to or pull from GitHub .

    Confirm the key has been created by looking for the key files in your `.ssh` directory:

    ```console
    $ ls ~/.ssh/
    ```

    You should see two files: `id_rsa` and `id_rsa.pub`. SSH uses public-key
    cryptography, which requires a private key and a public key. `id_rsa` is your
    private key and should never be shared. `id_rsa.pub` is your public key and can
    (and should) be shared.

    Now follow [these instructions](https://docs.github.com/en/github/authenticating-to-github/adding-a-new-ssh-key-to-your-github-account) to add your SSH key to your GitHub account.


### Step 3: Clone repo

> **Note** In any commands below that reference `[YOUR-GITHUB-USERNAME]`, be sure replace with your __actual GitHub username__.
{: .callout-warning}

In your browser, visit the page
`https://github.com/cs107e/{{page.quarterprefix}}-[YOUR-GITHUB-USERNAME]` to see the contents of your remote repo.
It should have only a single file: `README.md`, which lists the name of your
repo and nothing more.

- After accepting the GitHub invitation and setting up your SSH key, you can make a local clone of your repo. 
   This is a copy of the remote repo that lives locally on your computer.
   We recommend that you store your repo in the parent directory `cs107e_home` that you made during the install process.
   Execute the following terminal commands to make your local `mycode` repo.

    ```console
    $ cd ~/cs107e_home
    $ git clone git@github.com:cs107e/{{page.quarterprefix}}-[YOUR-GITHUB-USERNAME].git mycode
    ```

    Change to your repo and use `ls` confirm the files match those you see when browsing your remote repo on GitHub.

    ```console
    $ cd mycode
    $ ls
    $ cat README.md
    ```

### Step 4: Create `dev` branch (local and remote)

- The `master` branch in your repo is write-protected which means
    that you will not be able to directly modify this branch on GitHub. Instead, you'll do your work
    on a separate `dev` branch. To create this
    branch, change to your repo and execute the
    following commands:

    ```console
    $ cd ~/cs107e_home/mycode
    $ git branch
    $ git checkout -b dev
    $ git branch
    ```

    When you execute the first `git branch` command, notice how there is only a single
    branch listed: `master` and there is an asterisk next to `master`. This asterisk
    identifies which is the currently checked out branch. When you run the second
    `git branch` command, you should have two branches (`master` and `dev`) and the
    asterisk is now next to `dev`.

    The new `dev` branch you created only exists in your local repo; next you will 
    connect it to a new remote branch of the same name. Use `git branch` to confirm that you are on the
    `dev` branch and execute the following command:

    ```console
    $ git push --set-upstream origin dev
    ```

    If you return to your GitHub repo in your browser, you should now find a `dev` branch in the branches dropdown menu.

### Step 5: Add starter-code remote

- Now you will configure your local repo to have an additional remote connection to the starter code repo so that it can synchronize with it. Execute the following commands to add a `starter-code` on the `code-mirror` repository for which you early accepted the invitation.

    ```console
    $ git remote -v
    $ git remote add starter-code git@github.com:cs107e/code-mirror.git
    $ git remote -v
    ```

    When executing the first `git remote -v` command, you should have only a single
    remote: `origin`. `origin` is a shorthand way of referring to your remote repo
    on GitHub. The `git remote add` command adds a second remote. This second remote
    is `starter-code`, which is a shorthand way of referring to the starter code
    repo on GitHub. The second `git remote -v` should show you both remotes:
    `origin` and `starter-code` and the URLs that they represent.

    To get the starter files for `lab0`, verify that you're on the `dev` branch and execute the
    following commands:

    ```console
    $ git pull --allow-unrelated-histories start-code lab0-starter
    $ ls
    ```

    When pulling new files, the `git pull` command may open your editor and display the message 
    "Merge branch 'labX-starter' into dev". 
    Confirm the merge by saving and exiting the editor.

## Organization of the `mycode` repo
Your `mycode` repo is organized by lab and assignment. Pulling the starter files for
a lab or assignment will add a subdirectory named `labX` or `assignX` that contains the
associated starter code. 

```console
$ cd ~/cs107e_home/mycode
$ ls
assign0/   assign2/    assign4/    lab1/       lab3/       mylib/
assign1/   assign3/    lab0/       lab2/       lab4/
$ ls assign1
Makefile    larson.s
```

Your typical workflow will be to change to the subfolder for the particular lab or assignment 
you are currently working on. Within that subfolder, you edit files, build and run your code, make git commits, and so on.

## Lab workflow

When starting a new lab, do a `git pull` in your `cs107e.github.io` repository to ensure your courseware files are up to date.

```console
$ cd $CS107E
$ git pull
```
Update your local `mycode` repo by checking out the `dev` branch and pulling the lab starter files from the remote (change labX to lab1, lab2, as appropriate):

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull --allow-unrelated-histories starter-code labX-starter
```

After these commands, your repo is on `dev` branch and all files are up to date. `cd` to the `labX` folder and use `ls` to see the new files.

(Note: the `git pull` command may open up your editor and display the message "Merge branch 'labX-starter' into dev". Confirm the merge by saving and exiting the editor.)

You don't need to commit or submit lab code to be graded. The staff will review your code during lab and check off your participation. 

<a name="assign-workflow"></a>
## Assignment workflow
### Get assignment starter code

When starting a new assignment, do a `git pull` in your `cs107e.github.io` repository to ensure your courseware files are up to date.

```console
$ cd $CS107E
$ git pull
```

Update your local `mycode` repo by checking out the `dev` branch and pulling the assignment starter files from the remote (change assignX to assign1, assign2, as appropriate):

```console
$ cd ~/cs107e_home/mycode
$ git checkout dev
$ git pull --allow-unrelated-histories starter-code assignX-starter
```

After these commands, your repo is on `dev` branch and all files are up to date. `cd` to the `assignX` folder and use `ls` to see the new files.

(Note: the `git pull` command may open up your editor and display the message "Merge branch 'assignX-starter' into dev". Confirm the merge by saving and exiting the editor.)

### Assignment commit
You will work on your assignments in your local `mycode` repo. As you edit, we recommend that you
make regular git commits to record a snapshot of your work and track your progress. Each commit is
saved in your local repo. You can follow up with a `git push` to synch those changes with your remote
repo. You can consider your remote repo as a sort of "offsite backup".

The commands below add a changed file to the staging index, commit staged changes (save to local repo), and push the commit to GitHub (update remote repo):

```console
$ git status
```
The `git status` command will show you the files you have edited locally that have 
not yet been pushed to your remote repo. Now, let's add two edited files to the staging index
using `git add`, then commit and push those changes.

```console
$ git add filename.c other.c
$ git commit -m "Short but descriptive message about the changes you are committing"
$ git push
```

Note: using the command `git add .` will add all changed files in the repo to the staging index.

You can use `git log` to view the history of commits in your local repo. When
you enter the log, you can exit out by pressing "q" on your keyboard.

```console
$ git log
```

Also note that you can add multiple files (also called staging) and commit their
changes as a single commit. This is important because you don't ever want your code
to be in an inconsistent or bad state. For example, if you add a new file that
another file references, you want to put those two changes together in a single
commit; the file shouldn't be there if it isn't referenced, and you don't want
code to reference a file that doesn't exist.

Each commit that you make records a snapshot of your work. You can use these
snapshots to review your progress, compare versions, or identify and undo an
unfortunate change. Establishing good commit practices gives you the ability to
navigate the history of code changes, as well as providing the opportunity to
revert to previous versions. Sometimes you may find that you want to revert to
a commit before a set a changes that introduced a bug in your code. The more you
commit, the more opportunities you will have to restore previous working states
without losing as much work. **We recommend that you commit early and often and
that you write detailed commit messages.**

> Git command responses can sometimes look cryptic and scary. DON'T BE SCARED!!!
> Git is your friend :). Reading all the responses from git messages will help
> you understand the commands your running, and prevent you from missing
> important messages about the state of your repo.
> {: .callout-info}

**Note:** GitHub will show commits for the `master` branch by default. To see activity
on the `dev` branch, switch to it by selecting from the drop-down menu :

![Github change to another branch](../images/github-change-branch.png){: .zoom .w-75}

### Assignment tags

In order to grade your assignments, we'll run tests on your submission to verify that it works as
expected. You indicate which commit you want us to test by applying a tag.  
In Git, a _tag_ is simply a way of giving a name to a particular commit. We'll
also use this tag to determine your submission time. The submission time recorded
will correspond to the time you pushed the tag commit.
Create a tag to identify which commit if your submission by doing the following:

```console
$ git tag assignX-submit
$ git push --tags
```

A submission tag should always be of the format `assignX-submit`
where X is the particular assignment number. Now, go to your repo on GitHub and
verify that your newly created tag shows up in the "Tags" section of the branches
dropdown menu.

If after making your initial tag, you need to commit additional changes to include with
your submission, you can move the tag to a later commit with the following command 
(note that you need to push `--force` when moving a tag that already exists).

```console
$ git tag -f assignx-submit
$ git push --tags --force
```

Additionally, if you complete the assignment extension, mark the tag `assignX-extension`
on the commit containing the code for the extension. Your extension commit can be the 
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
practices when working in a distributed development environment. The typical
flow starts with a base branch (often called _master_) that reflects the master
copy of the code. Feature development or bug fixing occurs by creating a new
_branch_ where changes are made. Once you're confident that the code in your
branch is polished, tested, and working well, you can ask other people to merge
the commits in your branch into _master_ by making a _pull request_. You've
pushed commits to your branch, you're now asking other people to pull those
commits into master. Team members use the pull request to review the
proposed changes and provide comments. When a team member approves the pull request,
GitHub automatically merges the commits back into the base branch.

You will only have to make a pull request once. After you confirm you have done 
it in this assignment, it will remain open throughout the quarter and allow us to 
grade your assignments. If you accidentally close the pull request, just make 
sure to open a new one following the steps below. 

View the contents of your remote repository on the web at `https://github.com/cs107e/{{page.quarterprefix}}-[YOUR-GITHUB-USERNAME]`.
Switch to the branch `dev` by selecting it from the "Branch" drop-down
menu.

Click 'Contribute', then 'Open pull request'. 

![Github open pull request](../images/github-open-pr.png){: .zoom .w-75}

Now you will see text entry boxes for describing the pull request, followed by
a list of the commits that you have made and a line-by-line comparison (`diff`)
of the changed files.

In the title field, enter "Submission pull request" and leave the comment description blank. 
![Github create pull request](../images/github-create-pr.png){: .zoom .w-75}

Click 'Create pull request', which will submit the pull request and take you to
a page to view it:

![Github view pull request](../images/github-view-pr.png){: .zoom .w-75}

The pull request page will update to reflect any changes that are pushed after
the initial submission. This page will also be used by the grader to provide feedback
on your submissions.

If you need to update your submission, simply edit your files, commit, re-tag with `assignX-submit`, and push to add
it to the pull request. There is no need to make another pull request.

### Assignment submission

This guide contains a lot of information. Here is a checklist to follow when 
you are submitting your assignments. 

To submit an assignment:
1. Ensure all of your code is committed and pushed (see [Assignment commit](#assignment-commit))
1. Tag with `assignX-submit` and, if applicable, `assignX-extension` (see [Assignment tags](#assignment-tags))
1. Ensure you have an open Pull Request (see [Assignment pull request](#assignment-pull-request)) 
1. If you have already made a submission, but wish to make another,
    you may replace your previous submission. Edit your code, commit your changes, move your
    tags to this new commit, and push. You do not need to change your open Pull Request.
1. You can confirm what you have submitted by browsing your remote repo on GitHub. Choose `assignX-submit` tag from the dropdown menu to review the submitted files and their contents.

After receiving our grading feedback, you will have the opportunity to submit fixes
 and request a re-test of open issues. To submit fixes for re-test:
1. Ensure all of your code is committed and pushed. 
2. Tag with `assignX-retest`
3. Ensure you have an open Pull Request.  

### Assignment grading feedback

Feedback on __functionality__

To evaluate functionality, we run your code through a sequence of automated tests 
and report any bugs found by posting 
issues on your remote Github repository. 
See the `Issues` tab for a list of your open issues (bugs). Each issue has
a link to the log files. The 'student' log file is the output generated when running
your code and the 'staff' log file is the output generated by our staff solution.  Comparing the staff 
output to your own will give you insight as to why your code failed the test. 
As you fix your bugs and submit your debugged code, the issues will be closed in the 'Issues' 
tab and the logs will disappear from your logs folder. Your goal will be to close all 
high priority issues. Below we discuss the process for submitting your code for re-testing after 
fixing bugs. 

Feedback on __quality__ (code style and tests)

To evaluate quality, the grader reviews your code and test coverage. To read the grader comments,
view your submission pull request on the 'Pull Requests' tab of your remote Github repo. The grader
comments will be added on the 'Conversation' tab.

### Assignment bug fixes/re-test

After receiving our grading feedback, you may resubmit bug fixes to resolve 
open issues. To request a re-test after making fixes, tag the commit with `assignX-retest`.
You may do this as multiple times as you make further changes, simply keep moving the
`assignX-retest` tag to the commit that you want re-tested. 
We will do one re-test run per week on all repos with updated retest tags.
We communicate the re-test results by updating your repo 'Issues' and logs folder
to show which issues are now closed by your fixes. 

Note that not all code is eligible for re-test. 

Code that is eligible for re-test: 
- Core functionality for assignments 2-7. 

Code that is not eligible for re-test: 
- Extensions 
- Quality (code style/tests)
- Assignment 1 larson scanner and Assignment 2 clock application (you may re-test bug fixes for assign2 gpio and timer modules).

Code ineligible for re-test is graded on what was committed at the time of the original submission.

