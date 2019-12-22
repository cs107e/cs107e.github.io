---
title: Home/user configuration
---

{% include callout.html type="warning" %}
Note: If you are using the VM on Windows, be sure that you are __inside__ the VM when following these instructions to set up the home/user configuration.
</div>

### cs107e_home and shell environment
1. Create a `cs107e_home` directory to hold all your course materials. You can create this directory anywhere, but this guide
assumes that you are doing it in your home directory. 

    ```
    $ cd
    $ pwd
    /Users/student
    $ mkdir cs107e_home
    $ cd cs107e_home
    ```

2. Download the courseware repository into `cs107e_home`.  The courseware repo
contains the lecture material as well as common files that we distribute for
labs and assignments.

    ```
    $ pwd
    /Users/student/cs107e_home
    $ git clone https://github.com/cs107e/cs107e.github.io
    Cloning into 'cs107e.github.io'...
    $ cd cs107e.github.io/cs107e
    $ pwd
    /Users/student/cs107e_home/cs107e.github.io/cs107e
    ```

3. Edit your shell configuration file to identify the path to the courseware repo.  

    When a shell starts, it initializes the environment by reading a configuration file from your home directory. Editing this configuration file will change the initial state that applies to all shells.
    The shell configuration file is named `.bashrc` (for bash) or `.zshrc` (for zsh). 

    Use this command to determine which shell you are using:
    ```
    $ echo $SHELL
    /bin/bash
    ```

    If you are using a shell other than bash or zsh, please contact a CA to get help.

    Find the .bashrc or .zshrc in your home directory and add these two lines at the end of the file. The value for CS107E should match the path to the cs107e subdirectory of the courseware repo shown above. 
    ```
    export CS107E=/Users/student/cs107e_home/cs107e.github.io/cs107e
    export PATH=$PATH:$CS107E/bin
    ```


    ✔️__Check:__ confirm environment variables CS107E and PATH are properly set with the following commands:

    ```
    $ echo $CS107E
    /Users/student/cs107e_home/cs107e.github.io/cs107e
    $ ls $CS107E/lib/libpi.a
    /Users/student/cs107e_home/cs107e.github.io/cs107e/lib/libpi.a

    $ which pinout
    /Users/student/cs107e_home/cs107e.github.io/cs107e/bin/pinout
    ```

If you have troubles completing this task, reach out for help before going on, as setting up a proper
environment is crucial for later steps.

### Configure git user
These commands configure Git with your identity, which will be
associated with the git actions that you perform.

(Replace `My Name` and `myemail@stanford.edu` with your own.)

```
$ git config --global user.name "My Name"
$ git config --global user.email myemail@stanford.edu
```

✔️__Check:__ confirm your git configuration with the following command:

```
$ git config --get-regexp user
user.name My Name
user.email myemail@stanford.edu
```


