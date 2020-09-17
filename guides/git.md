# Git

_Written by CS 107E TA Peter McEvoy_

## What is Git? 
Git is a distributed version control system (DVCS), which allows multiple
people to work on the same software at the same time. Git is predominantly used 
by software engineers and developers to coordinate work on software, but the project
need not involve software to take advantage of Git. Software is mainly composed
of text files, which Git is especially good at managing because it can track small
changes made to text files. While Git can also manage more complex data files like
images, it lacks the ability to detect the same kind of small changes as in text
files.

## Why Git? 
Suppose that you're working on a project in C that will compute all powers of an
integer `x` between `0` and a positive integer `n`. The project consists of only
a single file, powers.c, which contains the following function signatures.

```
// return x^n
int power(int x, int n) {}

// print x^0, ... x^n
void print_powers(int x, int n) {}

// parse argv and delegate to print_powers
int main(int argc, char *argv[]) {}
```

You implement `main` first, then `print_powers`, and then `power`. You then compile 
and run your code. It works, but it's on the slow side, so you decide to optimize 
`power`. You finish optimizing and decide to go back through and comment your code
thoroughly to justify each of your steps. 

Now you recompile and rerun your code. Unfortunately, your optimizations introduced 
a bug, and so you want to return to your previous working version. If you use standard 
undo functionality, you'll revert `power` back to its slow yet correct form, but
you'll also lose all of your carefully crafted comments. You could try to
recreate the correct version of `power` from memory, but that's error prone.
What you really want is to cherry pick the `power` code from your editing
history and leave everything else as is. With Git, you can do exactly this. 

Once you have a working version of a function, you can create a checkpoint in
Git (called a commit) so that you can return to it at any time. Git, then, gives
you complete peace of mind during the development process since you know that
anything that you've committed can still be easily accessed.

## How does Git work?
We've established that Git gives you access to the editing history of your
project. But how does it do this? Well, the first step is to create what's
called a _repository_ (repo for short), which is a fancy name for a project managed by Git. As a
way of illustrating this, let's create a project (represented by the directory
`dummy`) and initialize a Git repository inside of it using `git init`. 

```
$ cd ~
$ mkdir dummy
$ cd dummy
$ ls -al
$ git init
$ ls -al
```

You'll notice that the first `ls -al` shows that the `dummy` directory is empty
aside from the `.` and `..` entries. However, the second `ls -al`, which is run
after `git init`, shows us the presence of a `.git` directory. Don't worry too
much about what's in the `.git` directory. For now, all you need to know is that
Git stores all of the information about your repo within the `.git` directory. 

Now that we've set Git up to track the changes in our repo, we can now edit some
files. 

```
$ echo "1" > file1.txt
$ echo "2" > file2.txt
$ ls 
$ cat file1.txt
$ cat file2.txt
```

We now have two files, `file1.txt` and `file2.txt` in our repo. Let's see if Git
is tracking our newly created files.

```
$ git status
```

Git tells us that we have two untracked files, which is expected since we just
created two new files. Git also tells us that we can use `git add <file>` to
include new files in what will be committed. At this point, it's time to talk
about how Git "saves" files. 

Instead of saving changes, Git commits them. The idea is similar to saving, but it
gives us the ability to choose which subset of the files that were changed to
include in the commit. (We can also selectively choose which parts of a given
file to commit, but that's beyond the scope of this guide.) To save/commit a file
in Git, we need to first tell Git what it is that we want to save (this is the 
`git add` step) and then tell Git that we want to save what we added (this is the
`git commit` step). It's worth pointing out that each commit requires an associated 
message, which helps us remember the reason for the commit. If the choice of the
word "commit" is unintuitive, then think of `git commit` as committing the
change to Git's memory.

Let's go ahead and commit the first file that we created. (As an aside, we can
use `git commit` instead of `git commit -m "<some message>". `git commit` will
simply send us into vim or some other text editor to enter the commit message.)

```
$ git add file1.txt
$ git commit -m "create file1.txt"
```

Now we can see our brand new commit using `git log`.

```
$ git log
```

`git log` shows us that "create file1.txt" was our most recent commit. Success! 

Let's commit the second file and verify that it was in fact committed.

```
$ git add file2.txt
$ git commit -m "create file2.txt"
$ git log
```

Now our most recent commit is, as expected, "create file2.txt". Our two commits
comprise the editing history of our project, which we can go back and examine at
any time. But what if we want to share our work with a friend? Thankfully, Git
(in conjunction with GitHub) can help us with this too. 

## Sharing work
First, let's distinguish Git from GitHub. GitHub is a Git server, meaning that
it stores Git repositories and serves them upon request to clients. Think of it
as a giant USB flash drive that stores your Git repositories and allows you and
your friends to access them from any computer in the world. We'll use GitHub to
store the repository that we just created. For demonstration purposes, we'll
assume that your GitHub account already has a repo named `dummy` created, which
can be found at `git@github.com:<your-username>/dummy.git`.

In Git terminology, the version of the repo living on your computer (the one you 
created with `git init`) is called the local copy, and the version living on GitHub's
servers is called the remote copy. 

In order to add the work in our local repo to our remote repo, we first need to 
assign a shorter name to the URL of the remote repo, the one that is hosted by 
GitHub. We'll use the name `origin`, since this remote repo will be the "origin" 
of your code. 

```
$ git remote add origin git@github.com:<your-username>/dummy.git
```

Now that we've set up a shorter name for our URL, we can create a new branch on
our remote repo and push our local changes to it.

```
$ git push --set-upstream origin master
```

What we're doing here is pushing our local commits (the ones that we made with
`git commit ...`) onto the branch `master` in the remote repo represented by the 
name `origin` (`git@github.com:<your-username>/dummy.git` in this case). In future
pushes to the `master` branch of `origin`, we will be able to simply do `git
push` instead of `git push --set-upstream origin master`. The latter, longer
version is necessary when the branch that we're pushing from our local repo doesn't
yet exist on our remote repo.

That was a lot, but fortunately `file1.txt` and `file2.txt` should now both show 
up in GitHub under your `dummy` repo. We started with just a local copy, and we 
created a remote copy by specifying the URL of the remote (using `git remote add 
origin`). We then _pushed_ all of the work that we've done on our local copy to 
the remote copy using `git push`. 

Getting back to our friend who wants to see our work: she can _pull_ our work
from the remote copy of the repo, the one hosted on GitHub. To do so, she'll use
`git clone` to create a local copy on her computer.

```
$ git clone git@github.com:<your-username>/dummy.git
```

This will create a repo named `dummy` on her computer, which will contain all of
the work that has been pushed to the remote copy up to that point. 

Now that our friend has a local copy of the repo, she can add a new change and
push it to the remote copy.

```
$ echo "3" > file3.txt
$ cat file3.txt
$ git add file3.txt
$ git commit -m "create file3.txt"
$ git push
```

She tells us that she just pushed a new change, but when we look at our local
copy, we see that we don't have the new file. To get it, we must _pull_ the
change from the remote copy of the repo to our local copy.

```
$ git pull
```

Now we have `file3.txt`. `git log` will also show that the new latest commit is
"create file3.txt".

## What next?
There are many bells and whistles to Git, but the two ideas of recording the
editing history and sharing changes with others are at the heart of Git. Git
takes a long time to understand; the longer I use it, the more I learn about it.
If you're interested in learning more, here are some useful resources:
- (Hacker Noon)[https://hackernoon.com/understanding-git-fcffd87c15a3]: another
  introductory guide to Git.
- Git man (short for manual) pages: the official documentation for Git. They can be accessed
  through the command line via `man git-<command>` or `git <command> --help`.
  For example, to see the man page for `git add`, you can use either `man
  git-add` or `git add --help`. The man pages can be difficult to understand at
  first, so it's normal to be confused. 
- (Pro Git)[https://git-scm.com/book/en/v2]: the comprehensive book on Git. If
  you want to understand Git, not just use it, then I highly recommend this book. 

Also, please please please ask the teaching staff questions as they come up. 
It's often times much easier to explain a Git concept verbally than it is to write
down an explanation. Finally, take solace in the fact that the rest of the
teaching staff and I have all faced the daunting learning curve of Git before.
We've managed just fine, and so will you. (This is coming from someone who
struggled in CS 106A.) 
