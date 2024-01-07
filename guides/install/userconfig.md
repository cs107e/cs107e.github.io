
After you have installed the development tools,  follow the steps below to configure your user environment. 

## Create `cs107e_home` directory
1. Make a new directory named `cs107e_home` to store course material within your home directory. The tilde character `~` is shorthand for the user's home directory. 

    ```console
    $ mkdir ~/cs107e_home
    ```

2. Change to this directory and clone the courseware repository. The courseware repo is used to distribute code and materials for lectures, labs, and assignments. 

    ```console
    $ cd ~/cs107e_home
    $ git clone https://github.com/cs107e/cs107e.github.io
    Cloning into 'cs107e.github.io'...
    ```

{% include checkstep.html content="confirm directory contents" %}
```console
$ ls ~/cs107e_home/cs107e.github.io/cs107e/bin
pinout     rpi-run.py
```

Note: If you're using WSL, now is a good time to open File Explorer on your `cs107e_home` directory and "Pin to Quick Access" to add it the sidebar for future use. See [accessing WSL files from Windows](../wsl/#files).

<A name="env"></A>
## Edit shell configuration file
Next, configure your shell environment to match the location of your `cs107e_home` directory.

When opening a new shell, the environment is initialized by reading a configuration file in your home directory. Editing the configuration file allows you to set the initial state of your shell. 

1. Determine the name of the configuration file for your shell. The name depends on which shell you are using. Use the command `echo $SHELL` to see your shell. Your shell is likely `bash`, although it might be `zsh` if using a more recent macOS.
    ```console
    $ echo $SHELL
    /bin/bash
    ```

    If your shell is `bash`, the configuration file is named `.bashrc`.  If your shell is `zsh`, the configuration file is named `.zshrc`.  For any other shell, please contact a CA for help.

2. Find out if you already have an existing configuration file or create it if needed. Change to your home directory and list the files. Filenames starting with a dot are hidden in a directory listing by default. Use the command `ls -a` to list all files, including hidden ones:
    ```console
    $ cd ~
    $ ls -a
    .    .bash_history   .bashrc     cs107e_home     .python_history 
    ..   .bash_logout    .config     .profile        .viminfo
    ```
(The filenames listed in your directory may be somewhat different, don't worry!) Look through list to see if there is already a configuration file for your shell. 
    If not listed, use `touch` to create an empty file with the appropriate name:
    ```console
    $ touch .bashrc
    ```

3. Open the configuration file in a text editor and append the following two lines verbatim:
    ```
    export CS107E=~/cs107e_home/cs107e.github.io/cs107e
    export PATH=$PATH:$CS107E/bin
    ```

    The first line sets the environment variable `CS107E` to the path to where the class files are stored. The second line adds our `bin` subdirectory to your executable path. 

4. Use the `source` command to read the updated configuration file into your current shell:
    ```console
    $ source ~/.bashrc
    ```

{% include checkstep.html content="confirm current shell is properly configured" %}
```console
$ echo $CS107E
/Users/student/cs107e_home/cs107e.github.io/cs107e
$ ls $CS107E/lib/libpi.a
/Users/student/cs107e_home/cs107e.github.io/cs107e/lib/libpi.a
$ which pinout
/Users/student/cs107e_home/cs107e.github.io/cs107e/bin/pinout
```

The configuration file is persistent and should be automatically read when creating a new shell in the future. 

{% include checkstep.html content="confirm shell configuration is persistent and future shells properly configured" %}
Close your current shell and open a new one. Repeat the check step above in the new shell and confirm the new shell is also properly configured.

If you confirm your configuration is persistent, skip Step 5 below. If it is not persistent and you are using  `bash` and macOS Terminal, use the additional customization in Step 5 to add persistence.

{: start="5"}
5. (only if needed) Find the file named `.bash_profile` in your home directory. If no such file exists, use the `touch` command to create an empty file with that name. Open that file in a text editor and append the following line verbatim:
    ```
    source ~/.bashrc
    ```

Repeat the previous check step with a new shell to confirm your configuration is now persistent.

## Configure git identity
We distribute course materials as git repos and you will use git to access, manage, and submit your work. 

1. The commands below will properly set the identity recorded with your git actions. Be sure to replace `My Name` and `myemail@stanford.edu` with your own name and address.

    ```console
    $ git config --global user.name "My Name"
    $ git config --global user.email myemail@stanford.edu
    ```

{% include checkstep.html content="confirm your git identity" %}
```console
$ git config --get-regexp user
user.name My Name
user.email myemail@stanford.edu
```

## Installation complete
Use the [Final check steps](../#installchecklist) to confirm your entire environment.
