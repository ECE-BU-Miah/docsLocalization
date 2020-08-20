PROG=$1
echo -n "Compiling "
echo -n $PROG
echo -n " ..."
gcc $PROG -o TestProg.a -lm
echo "Done."
./TestProg.a
