---
title: Home/user config for CS107E
---


These instructions walk you through customizations specific to CS107E. You will set up a `cs107e_home` directory and establish your git identity.

{% include callout.html type="warning" %}
Note: If you are using the VM on Windows, be sure that you are __inside__ the VM when following these instructions to set up the home/user configuration.
</div>


### Set up cs107e_home
1. Create a new directory named `cs107e_home` to store course materials. The tilde character `~` is shorthand for home directory, so the path `~/cs107e_home` creates the directory within your home. 

    ```
    $ mkdir ~/cs107e_home
    ```

2. Change to your new directory and download the courseware repository. We use this repo to distribute code and materials for lectures, labs, and assignments. 

    ```
    $ cd ~/cs107e_home
    $ git clone https://github.com/cs107e/cs107e.github.io
    Cloning into 'cs107e.github.io'...

    $ ls ~/cs107e_home/cs107e.github.io/cs107e/
    bin/     etc/     extras/  include/ lib/     src/
    ```

3. As the final step, you must edit your shell configuration to indicate where you have stored your class files. 

    When opening a new shell, the environment is initialized by reading a configuration file in your home directory. Editing this file allows you to control the initial state for a new shell. 

   The configuration file varies slightly depending on the shell. Use the command `echo $SHELL` to see which shell you are using. 

   ```
   $ echo $SHELL
   /bin/bash
   ```

   For bash, the config file is named `.bashrc`.  For zsh, the config file is named `.zshrc`.  For a shell other than bash or zsh, please contact a CA for help.

   Look in your home directory to find the appropriate config file for your shell. Open the file in a text editor, and append the following two lines:
    ```
    export CS107E=~/cs107e_home/cs107e.github.io/cs107e
    export PATH=$PATH:$CS107E/bin
    ```

    The first line sets the environment variable `CS107E` to the path to where the class files are stored. The second line adds our `bin` subdirectory to your executable path.
  
✔️__Check:__ Open a new shell and confirm the configuration with the following commands:

    ```
    $ echo $CS107E
    /Users/student/cs107e_home/cs107e.github.io/cs107e
    $ ls $CS107E/lib/libpi.a
    /Users/student/cs107e_home/cs107e.github.io/cs107e/lib/libpi.a

    $ which pinout
    /Users/student/cs107e_home/cs107e.github.io/cs107e/bin/pinout
    ```

### Configure git user
Execute the following two commands so that your identity will be properly recorded for your git actions.

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


