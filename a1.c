#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#define NEGATIVE -1
int HIDDEN_FLAG = 0;


/*This helper function takes the current directory as a string
and the directory you seek*/
void printDir (const char *cwd, char *argVal) {
    //OPEN DIRECTORY
    DIR *dir = opendir(cwd);
    struct dirent *dirPointer;
    //NULL CHECK
    //if the full path name cannot open the directory this if statement executes
    if(!dir){
        printf("Directory not Found: %s\n", argVal); //need argVal to print name of directory
        return;
    }
    //PRINT DIRECTORY NAME
    //Special edge case check, when -h is last arg, a period is seen, or the argc == 1
    //we just want the name of the current directory that we are in
    if(strcmp(argVal, "myls") ==0 || strcmp(argVal, "-h") == 0 || strcmp(argVal, "./myls") == 0 || strcmp(argVal, ".") == 0){
       //since no valid argv[] is provided in these cases I had to refer
       //to basename to grab the users current directory name
         char *curName = basename(cwd); 
        printf("\n%s\n",curName);
    }
    //Otherwise print the name of the directory the user wants for their convenience
    //We have the name becuase of the argv[] and argVal
    else{printf("\n%s\n", argVal);}

    //READ AND PRINT DIRECTORY FILES
    //Our pointer will read the directory until there are no files left to be read
    while ((dirPointer = readdir(dir)) != NULL)
        {
            //compares the current file name to . or ..
             if (strcmp(dirPointer->d_name, ".") == 0 || strcmp(dirPointer->d_name, "..") == 0)
             {
                 //We found either . or ..
                 if(HIDDEN_FLAG > 0) //If the hidden flag exists then we will print out the hidden files
                 {
                    printf("%s\n", dirPointer->d_name);
                 }
                 //SKIP THE HIDDEN FILES
                 else{
                 continue;
                 }
                continue;
        } 
        //print the file name and a newline character
        printf("%s\n", dirPointer->d_name);
    }
    //CLOSE DIRECTORY
    closedir(dir);
    return;
}

//Main function call utilizing our argc and argv array
int main(int argc, char *argv[]) {
    
    char cwd[1024];
    int opt;
    //grab our current working directory, was used for testing purposes
    getcwd(cwd, sizeof(cwd));

    //FLAG THAT INDICATES WE FOUND THE -h COMMAND
    while((opt = (argc, argv, "-h")) != NEGATIVE){
        HIDDEN_FLAG++; //USES getopt to find our special flag
    }
    
    if(argc == 1){
        //PROGRAM IS RUN WITHOUT ANY ARGUMENTS, PRINT OUT CURRENT DIRECTORY
        //This is the "./myls" case
        printDir(cwd, argv[0]);
    } 

    //ARGUMENT SUPPLIED AFTER PROGRAM CALLED
    else{
        
            //LOOP THROUGH argv ARRAY AND PERFORM FUNCTIONALITY
            //notice we start at i = 1 so we can skip the ./myls command
            int i =1;
            for (i ; i < argc; i++){
            char cwd[1024];
            //ESTABLISH OUR CURRENT WORKING DIRECTORY
            getcwd(cwd, sizeof(cwd));  //this needs to be refreshed for each loop
                //checks if current arg is the special flag
                if(strcmp(argv[i], "-h") == 0 ){
                
                    if(i+1 == argc) //Checks if this is the last iteration in the loop
                    {
                        //Current iteration is the last in the loop and is the h flag, so we will print
                        printDir(cwd, argv[i]);
                    }
                    //This continue allows us to have multiple -h so we just pass over them until we get to the last one
                    //Allows us to do './myls -h -h -h -h -h dir1'
                    continue;
                }
            
            if(strcmp(argv[i], ".") == 0){ //checks if current arg is the period
                //We have the current path so lets just print it
                printDir(cwd, argv[i]);
            }
            //we dont want periods to come into here or the cat methods will screw us up
            else{
            //ADD SLASH FOR PATH
            strcat(cwd, "/");
            //SUPPLY PATH
            strcat(cwd, argv[i]); //PUTs the whole path together

            //execute our print function
            printDir(cwd, argv[i]); //param 1 = direcotry path we want, param2 = name of that directory for printing purposes
        }}
    }
}

