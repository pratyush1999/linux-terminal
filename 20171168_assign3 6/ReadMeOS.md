#ReadMe
To execute:
make
./shell
Features:
  - builtin commands like ls,pwd,echo,cd,fg,bg,jobs,setenv,unsetenv
  implemented
  - kjob and overkill, two other commands also implemented
  - Background and Foreground Process creation possible
  - quit only when quit is typed
  - signal handling of contro+c,control+d,
  SIGCHLD,control+z also done
  -I/O redirectiona and piping also possible

Files:
    -parse.c - code to parse the input and call the required command
    -commands.c - all the commands 
    -getDirectory.c - code to give current dirctory
    -getInput.c - code to get Input 
    -main.c - the main loop 
    globals.h - all the global headers
    
