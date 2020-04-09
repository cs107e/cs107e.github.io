---
title: Home/user config for CS107E
toc: true
---


After you have installed the development tools on your laptop, follow the steps below to configure your user environment. Take care to validate at each ✔️__Check__ point before moving on to the next step.  If you hit a snag, reach out on Piazza or come to office hours for help!

{% include callout.html type="warning" %}
Note: For those running WSL on Windows, be sure that you are in the Ubuntu shell when following these instructions.
</div>

### Set up cs107e_home
1. Create a new directory named `cs107e_home` to store course materials. The tilde character `~` is shorthand for home directory, so the path `~/cs107e_home` creates the directory within your home. 

    ```
    $ mkdir ~/cs107e_home
    ```

2. Change to the new directory and clone the courseware repository here. This repo is used to distribute code and materials for lectures, labs, and assignments. 

    ```
    $ cd ~/cs107e_home
    $ git clone https://github.com/cs107e/cs107e.github.io
    Cloning into 'cs107e.github.io'...
    ```

✔️__Check:__ confirm your cs107e_home:

```
$ ls ~/cs107e_home/cs107e.github.io/cs107e/
bin/    examples/   extras/     include/    lib/    sample_build/   src/
```

Note: If you're using WSL, now is a good time to open File Explorer on your new cS107e_home and and "Pin to Quick Access" to add it the sidebar for future use. See [accessing WSL files from Windows](../wsl/#files).

<A name="env"></A>
### Edit shell configuration file
Next, configure your shell environment to match where you stored cs107e_home.

When opening a new shell, the environment is initialized by reading a configuration file in your home directory. Editing the configuration file allows you to set the initial state of your shell. 

The name of the configuration file depends on which shell you are using. Use the command `echo $SHELL` to see your shell. Your shell is likely `bash`, although it might be `zsh` if on a newer Mac.

```
$ echo $SHELL
/bin/bash
```

If your shell is `bash`, the file is named `.bashrc`.  If your shell is `zsh`, the file is named `.zshrc`.  For any other shell, please contact a CA for help.

Change to your home directory and look for the configuration file for your shell. Note that filenames starting with a dot are hidden in the directory listing by default. Use the command `ls -a` to list all files, including hidden ones:

```
$ cd ~
$ ls -a
.    .bash_history   .bashrc     cs107e_home     .python_history 
..   .bash_logout    .config     .profile        .viminfo
```

If your home directory does not yet have the configuration file, you can create an empty file with the appropriate name:
```
$ touch .bashrc
```

Open the configuration file in a text editor and append the following two lines verbatim:

```
export CS107E=~/cs107e_home/cs107e.github.io/cs107e
export PATH=$PATH:$CS107E/bin
```

The first line sets the environment variable `CS107E` to the path to where the class files are stored. The second line adds our `bin` subdirectory to your executable path. 

The configuration file will be automatically read when creating a new shell in the future. Use the `source` command to re-read the configuration right now in your current shell: 

```
$ source ~/.bashrc
```

  
✔️__Check:__ confirm your current shell configuration:

```
$ echo $CS107E
/Users/student/cs107e_home/cs107e.github.io/cs107e
$ ls $CS107E/lib/libpi.a
/Users/student/cs107e_home/cs107e.github.io/cs107e/lib/libpi.a

$ which pinout
/Users/student/cs107e_home/cs107e.github.io/cs107e/bin/pinout
```


If your shell is `bash` and you are using the Terminal app on Mac OS, you will need an additional customization to make the shell configuration persistent. __This step is NOT necessary if your shell is `zsh` or you are using WSL__.

+ Find the file named `.bash_profile` in your home directory. If you do not yet have such a file, use the `touch` command to create an empty file with that name. Open that file in a text editor and append the following line verbatim:

    ```
    source ~/.bashrc
    ```

✔️__Check:__ confirm your shell customization is persistent:

Close your current shell and open a new one. Try the check step above in the new shell and confirm the new shell is also properly configured.

### Configure git identity
We distribute course materials as git repos and you will use git to access, manage, and submit your work. The commands below will properly set the identity recorded with your git actions.

(Replace `My Name` and `myemail@stanford.edu` with your own.)

```
$ git config --global user.name "My Name"
$ git config --global user.email myemail@stanford.edu
```

✔️__Check:__ confirm your git identity:

```
$ git config --get-regexp user
user.name My Name
user.email myemail@stanford.edu
```

### Finished
After following the above steps, you've configured your user account. Continue on to the [Final Checklist](../#steps) to confirm your entire environment.

