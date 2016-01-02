---
layout: page
title: Guide to Git
---

Git is a version control system commonly used in the software industry.
Git tracks changes between versions so that you can easily revert to old versions
of your code. Git also allows multiple developers to work from the same
codebase and then merge their work together. If you have never worked with version 
control before or want to understand how Git handles it, I recommend reading a little 
about it from the [Git tutorial]
(http://git-scm.com/book/en/v2/Getting-Started-About-Version-Control).

GitHub provides a web-based graphical interface for Git. In this class, you will 
use Git and GitHub for your assignments. You will clone any starter code from
GitHub, add your own code locally on your machine to complete the assignment,
and then push your code back to GitHub for evaluation.

This guide walks you through some of the most useful Git commands. If you want to 
learn more commands or read more explanation on these commands, check out the 
[Git documentation](http://git-scm.com/doc).

###Git setup

Your first step will be to install Git on your machine. Please see the
[instructions for installing Git](http://git-scm.com/book/en/v2/Getting-Started-Installing-Git) for your OS.

Once you have Git installed, you should configure it for yourself. In a command
prompt, type the following lines (replacing My Name and myemail with your own) 
so that it is clear your commits came from you:

$ git config --global user.name "My Name"  
$ git config --global user.email myemail@stanford.edu

You may also want to change the default text editor Git will use. For example,
if you prefer Emacs, you can type the following line:

$ git config --global core.editor emacs

You can check that you set these up correctly by typing the following line. It 
should print out the settings you just configured:

$ git config --list

Your last step in the setup will be to get started with GitHub. If you do not have
an account already, [sign up now](https://github.com). Once you have an account,
please email your username to Isabel (ibush@stanford.edu) so that she can add you to the
[CS107e organization](https://github.com/cs107e). Each of you will be the sole member 
of your own "Team" in the organization, and your "Team" will have one private repo.
This is where you will push all of your assignments for grading.

###Using Git

The first step when working with git is to initalize a git repo locally on your machine
or to clone an existing git project (or empty repo). This tutorial will teach the latter.
For the first assignment, you will start by cloning your repository from GitHub.
Go to the [CS107e organization](https://github.com/cs107e) and click on your username in the 
Teams section, and then click on the repository for your Team. Copy the url (https) shown in
the Quick setup box. From the command line on your machine, make or go to
the directory where you would like this code to live and type the following (replacing url with
your repository's url):

$ git clone url 

If you now type the ls command, you should see the new repository (directory).

Now you can cd into the new directory and add code locally on your machine. For assignments in this class,
you will have a subdirectory within your repo for each assignment. But for now you may want to test
out the git steps below by adding a simple test file. 

When you are ready to 
make a commit (generally a good idea after you add a new feature, fix a bug, or are 
finished with an assignment), you should first verify your changes using the following 
commands:

$ git status  
$ git diff [filename or directory]

The git status command lists all the new (untracked) and modified files. You should 
ensure this matches the files you intended to fix. The git diff command shows all of 
your local changes since the last committed version. Adding a filename or 
directory to the diff command allows you to view only the changes in those particular 
file(s). Lines beginning with + were added since the last commit, lines beginning 
with - were removed.

Once you are satisfied with your changes, you must stage any file you want in 
this commit using the following command:

$ git add filename

If you would like to include changes from all files in a particular directory, you can 
replace filename above with the directory name. If you would like to commit all of 
your local changes, you can use the following:

$ git add --all

If you type git status now, you should see your desired files staged for commit.
To do the commit, use the following command:

$ git commit -m "Description of my modifications"

The -m option allows you to specify a message for your commit, which will help you to 
find it later if you need to review your changes or revert a commit.

To view all your commits, you can use the following command. Your latest commit should
be at the top:

$ git log

If you realize you forgot one last change you wanted in your previous commit, you can
modify the desired file, stage it (git add), and then use the following command to 
update your previous commit with the new changes:

$ git commit --amend

Once you are satisfied with all of your local changes and commit(s), you can push
the changes to the remote server. To do this, simply use:

$ git push

The first time you do this, you may get the message  
"No refs in common and none specified; doing nothing.  
Perhaps you should specify a branch such as 'master'.  
Everything up-to-date".   
This happens if the initial repo that you cloned was empty. In this case, 
you will need to specify the remote branch to which you would like to
push your code. We will not go into branch usage in this guide (checkout the Git 
documentation if you're interested), so all you need to know is that the main remote
branch is origin/master. So if you get the above error, use the following command
instead:

$ git push origin master

Now your work should be up on GitHub! Go check online to be sure it worked. 
