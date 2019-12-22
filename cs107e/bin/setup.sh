# I am not particularly found of doing things behind student's 

EXPECTED=~/cs107e_home/cs107e.github.io/cs107e
if [ ! -d $EXPECTED ]; then
    echo "Expected directory $EXPECTED. Not found. Exiting"
    exit 1
fi
case $SHELL in
*/zsh) 
   SHELL_CONFIG="$HOME/.zshrc"
   ;;
*/bash)
   SHELL_CONFIG="$HOME/.bashrc"
   ;;
*)
   echo "Sorry, cannot configure shell $SHELL"
   exit 1
esac

echo "# Below two lines added by cs107e automated setup script" >> $SHELL_CONFIG
echo "export CS107E=$EXPECTED" >> $SHELL_CONFIG
echo "export PATH=\$PATH:\$CS107E/bin" >> $SHELL_CONFIG
