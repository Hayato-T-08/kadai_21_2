gcc -c bitmap.c 
gcc -c kadai_IIR.c 
gcc bitmap.o kadai_FIR.o -o kadai_IIR
./kadai_IIR < input.txt
