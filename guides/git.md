## Git

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
[instructions for installing git](http://git-scm.com/book/en/v2/Getting-Started-Installing-Git) for your OS.

Once you have git installed, you should configure it for yourself. In a command
prompt, type the following lines (replacing My Name and myemail with your own) 
so that it is clear your commits came from you:

$ git config --global user.name "My Name"
$ git config --global user.email myemail@stanford.edu

You may also want to change the default text editor Git will use. For example,
if you prefer Emacs, you can type the following line:

$ git config --global core.editor emacs

You can check that you set these up correctly by typing the following line. It 
should print out the settings you just configured.

$ git config --list

Your last step in the setup will be to get started with GitHub. If you do not have
an account already, [sign up now](https://github.com). Once you have an account,
please email your username to me (ibush@stanford.edu) so that I can add you to the
[CS107e organization](https://github.com/cs107e). Each of you will be the sole member 
of your own "Team" in the organization, and I will create repositories in your "Team" 
for each stand-alone assignment.

###Using Git

For each assignment, we instructors will set up a repository for you that will 
include any starter code. You will need to clone the starter code (or blank repo)
locally to your machine so that you can add your own code. To do this, you should
find the url of your assignment repo in the [CS107e organization]
(https://github.com/cs107e). From the command line on your machine, make or go to
the directory where you would like this code to live 
(something like cs107e/assignments) and type the following (replacing url with
your repository's url):

$ git clone url 

If you now type the ls command, you should now see the new repository and any starter 
code.

Now you can modify the code as usual locally on your machine. When you are ready to 
make a commit (generally a good idea after you add a new feature, fix a bug, or are 
finished with an assignment), you should first verify your changes using the following 
commands:

$ git status
$ git diff [filename or directory]

The git status command lists all the new (untracked) and modified files. You should 
ensure this matches the files you intended to fix. The git diff command shows all of 
your local changes since the last committed version. If you add a filename or 
directory to the diff command, you can view only the changes in those particular 
file(s). Lines beginning with + were added since the last commit, lines beginning with
- were removed.

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
be at the top.

$ git log

If you realize you forgot one last change you wanted in your previous commit, you can
modify the desired file, stage it (git add), and then use the following command to 
update your previous commit with the new changes:

$ git commit --amend

Once you are satisfied with all of your local changes and commit(s), you can push
the changes to the remote server. To do this, simply use the following command:

$ git push

If you get the message
"No refs in common and none specified; doing nothing.
Perhaps you should specify a branch such as 'master'.
Everything up-to-date", 
it is likely because the initial repo that you cloned was empty. If 
this is the case, you will need to specify the remote branch to which you would like to
push your code. We will not go into branch usage in this guide (checkout the Git 
documentation if you're interested), so all you need to know is that the main remote
branch is origin/master. So if you get the above error, use the following command
instead:

$ git push origin master

Now your work should be up on GitHub! Go check online to be sure it worked. 
