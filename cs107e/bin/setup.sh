if [ ! -d "cs107e.github.io" ]; then
    echo "This script should be run in your cs107e_home directory that contains the cs107e.github.io repository"
    exit 1
fi

echo export PATH=\"$PWD/cs107e.github.io/cs107e/bin:\$PATH\" | tee -a ~/.bashrc ~/.bash_profile
echo export CS107E=\"$PWD/cs107e.github.io/cs107e\" | tee -a ~/.bashrc ~/.bash_profile

source ~/.bashrc
