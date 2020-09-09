Dipen Kumar
2018CS50098

I have used file descriptor and execvp and forked to run different executable files.
There are five internal command which i check and call system function.
For executable I first check for pipe.
If not found then simply fork parent and run it in child process till then parent is waiting. Used file descriptor to direct output via STD_OUT in file and input STD_IN through file.
If found then fork parent and run 1st exe in child. Further fork parent and run another exe in child2, parent will wait for both child. Again here also I have used file descriptor.