#include "alias.h"
void selfmade(char *buffer2, char *buffer3, int file, char *p, char *c, char *g, int k, int flg) {
    FILE *file2 = fopen("/home/viratgarg/Documents/Sem3/OSN/projects/project_1B/alias.myshrc", "r");
    if (file2 == NULL) {
        perror("Error opening file");
        return;
    }

    trim(buffer2);
//   printf("buffer 2 is %s\n",buffer2);

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file2) != NULL) {
        if (strncmp(buffer, "alias", 5) == 0) {
            char new[4096];
            strcpy(new, buffer);
            char *token = strtok(new, " ");
            char *alias = strtok(NULL, " ");
            trim(alias);
           // printf("alias is %s\n",alias);
            if (strcmp(alias, buffer2) == 0) {
                char *command = strtok(NULL, "=");
                trim(command);
                //  printf("command is %s\n",command);
                char* save;
                input2(command, buffer3, file, p, c, g, k, flg);
            //  input(0,p,c,g,k,flg,save,command);
                fclose(file2);  // Close file before returning
                return;

            }
        }
// else {
//     char *buffer3 = strtok(buffer2, " ");
//     char *arguments[100];
//     int i = 0;

//     printf("buffer3 is %s\n", buffer3);

//     // Parse the arguments from buffer2
//     while (buffer3 != NULL) {
//         buffer3 = strtok(NULL, " ");
//         if (buffer3 != NULL) {
//             arguments[i] = strdup(buffer3);  // Use strdup to allocate memory for each argument
//             printf("argument[%d] is %s\n", i, arguments[i]);
//             i++;
//         }
//     }

//     // Process lines within the curly braces
//     while (fgets(buffer, sizeof(buffer), file2) != NULL) {
//         trim(buffer);
//         if (strcmp(buffer, "{") == 0) {
//             // Start of the function body
//             while (fgets(buffer, sizeof(buffer), file2) != NULL) {
//                 trim(buffer);
//                 if (strcmp(buffer, "}") == 0) {
//                     break;  // End of function body
//                 }

//                 // Substitute arguments like $1, $2, etc., with corresponding values
//                 char *substituted_command = strdup(buffer);
//                 for (int j = 0; j < i; j++) {
//                     char *argument = arguments[j];
//                 if (argument[0] == '"' && argument[strlen(argument) - 1] == '"') {
//                     argument[strlen(argument) - 1] = '\0';  // Remove the trailing quote
//                     argument++;  // Skip the leading quote
//                     }
//                     char arg_placeholder[10];
//                     snprintf(arg_placeholder, sizeof(arg_placeholder), "$%d", j + 1);
                    
//                     // Replace "$j+1" with corresponding argument 
//                     char *pos = strstr(substituted_command, arg_placeholder);
//                     if (pos != NULL) {
//                         char new_command[4096];
//                         strncpy(new_command, substituted_command, pos - substituted_command);
//                         // printf("new_command is %s",new_command);
//                         new_command[pos - substituted_command] = '\0';
//                         strcat(new_command, arguments[j]);
//                         // printf("new_command is %s",new_command);
//                         strcat(new_command, pos + strlen(arg_placeholder));
//                         strcpy(substituted_command, new_command);
//                     }
//                 }

//                 // Now, `substituted_command` has the line with arguments replaced
//                 char *save;
//                 printf("Calling input with: %s\n", substituted_command);
//                 input(0, p, c, g, k, flg, save, substituted_command);

//                 free(substituted_command);  // Free the dynamically allocated memory
//             }
//         }
    // }

    // // Free the arguments array
    // for (int j = 0; j < i; j++) {
    //     free(arguments[j]);
    // }


    }

    char* save;
    // printf("You are correct");
    input(0,p,c,g,k,flg,save,buffer2);

    fclose(file2);  // Close file before returning
}