#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define Founded_As_Local 1
#define Not_Founded_As_Local 0

struct Local_Variable {
  char variable[50];
  char value[100];
};

int Local_Var_Capacity = 0;
struct Local_Variable *Local_Var_Storage = NULL;

void remove_argv_entry(char **argv, int index) {
  for (int j = index; argv[j] != NULL; j++) {
    argv[j] = argv[j + 1];
  }

}

int main(int argc, char *argv[]) {
  size_t size = 100;
  char *Buffer = (char *)malloc(size);
  if (Buffer == NULL) {
    fprintf(stderr, "Memory allocation error.\n");
    return -1;
  }

  // Save original file descriptors
  int saved_stdin = dup(STDIN_FILENO);
  int saved_stdout = dup(STDOUT_FILENO);
  int saved_stderr = dup(STDERR_FILENO);

  int Var_Founded = 0;
  int in_fd = -1;
  int out_fd = -1;
  int err_fd = -1;
  char pwd_buffer[size];
  char *Buffer_ptc;
  char *Token;
  char *dest;
  char *src;
  char *equal_sign;
  char *appended_string;
  int Local_Var_Array_Size = 0;
  extern char **environ;

  while (1) {
    // Restore original stdout before printing prompt
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);

    // My Shell's Prompt.
    printf("Mahmoud_Kaoud's Shell is ready:");
    if (fgets(Buffer, size, stdin) == NULL) {
      if (errno == ERANGE) {
        size *= 2;
        char *temp = (char *)realloc(Buffer, size);
        if (temp == NULL) {
          fprintf(stderr, "Memory reallocation error.\n");
          free(Buffer);
          return -2;
        }
        Buffer = temp;
        continue;
      } else if (feof(stdin)) {
        printf("\n");
        break;
      } else {
        perror("fgets error");
        free(Buffer);
        exit(-8);
      }
    }

    // Remove newline
    Buffer[strcspn(Buffer, "\n")] = '\0';

    if (strlen(Buffer) == 0)
      continue;

    // Tokenize input
    char *N_argv[size];
    int i = 0;
    Buffer_ptc = Buffer;

    Token = strtok(Buffer_ptc, " ");
    while (Token != NULL && i < size - 1) {
      N_argv[i++] = Token;
      Token = strtok(NULL, " ");
    }
    N_argv[i] = NULL;
    // getchar();
    // sleep(5);
    //  Process redirections
    for (int R = 0; N_argv[R] != NULL; R++) {
      if (strcmp(N_argv[R], "<") == 0) { // Input redirection with space
        if (N_argv[R + 1] == NULL) {
          fprintf(stderr, "No file provided for input redirection.\n");
          continue;
        }

        in_fd = open(N_argv[R + 1], O_RDONLY);
        if (in_fd < 0) {
          perror("Could not open file for input redirection");
          continue;
        }
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);

        // Remove redirection tokens
        remove_argv_entry(N_argv, R);
        remove_argv_entry(N_argv, R);
        R--;
      } else if (N_argv[R][0] == '<' &&
                 N_argv[R][1] != '\0') { // Input redirection without space
        in_fd = open(N_argv[R] + 1, O_RDONLY);
        if (in_fd < 0) {
          perror("Could not open file for input redirection");
          continue;
        }
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);

        remove_argv_entry(N_argv, R);
        R--;
      } else if (strcmp(N_argv[R], ">") == 0) { // Output redirection with space
        if (N_argv[R + 1] == NULL) {
          fprintf(stderr, "No file provided for output redirection.\n");
          continue;
        }

        out_fd = open(N_argv[R + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (out_fd < 0) {
          perror("Could not open file for output redirection");
          continue;
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);

        remove_argv_entry(N_argv, R);
        remove_argv_entry(N_argv, R);
        R--;
      } else if (N_argv[R][0] == '>' && N_argv[R][1] != '\0' &&
                 N_argv[R][1] != '>') { // Output redirection without space
        out_fd = open(N_argv[R] + 1, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (out_fd < 0) {
          perror("Could not open file for output redirection");
          continue;
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);

        remove_argv_entry(N_argv, R);
        R--;
      } else if (strcmp(N_argv[R], "2>") == 0) { // Error redirection with space
        if (N_argv[R + 1] == NULL) {
          fprintf(stderr, "No file provided for error redirection.\n");
          continue;
        }

        err_fd = open(N_argv[R + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (err_fd < 0) {
          perror("Could not open file for error redirection");
          continue;
        }
        dup2(err_fd, STDERR_FILENO);
        close(err_fd);

        remove_argv_entry(N_argv, R);
        remove_argv_entry(N_argv, R);
        R--;
      } else if (strncmp(N_argv[R], "2>", 2) == 0 &&
                 N_argv[R][2] != '\0') { // Error redirection without space
        err_fd = open(N_argv[R] + 2, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (err_fd < 0) {
          perror("Could not open file for error redirection");
          continue;
        }
        dup2(err_fd, STDERR_FILENO);
        close(err_fd);

        remove_argv_entry(N_argv, R);
        R--;
      }
    }
    // getchar();
    // sleep(5);
    //  Built-in commands
    if (N_argv[0] == NULL) {
      continue;
    }

    if (strcmp(N_argv[0], "pwd") == 0) {
      if (getcwd(pwd_buffer, size) == NULL) {
        perror("getcwd error");
        continue;
      }
      printf("%s\n", pwd_buffer);
      continue;
    }

    if (strcmp(N_argv[0], "echo") == 0) {
      if (N_argv[1] && N_argv[1][0] == '$') {
        char *var_name = &N_argv[1][1];
        int found_local = 0;

        // Search in local variables first
        for (int j = 0; j < Local_Var_Array_Size; j++) {
          if (strcmp(Local_Var_Storage[j].variable, var_name) == 0) {
            printf("%s\n", Local_Var_Storage[j].value);
            found_local = 1;
            break;
          }
        }

        // If not found locally, search in environment
        if (!found_local) {
          char *env_value = getenv(var_name);
          if (env_value) {
            printf("%s\n", env_value);
          } else {
            printf("\n"); // Print empty line if variable not found
          }
        }
      } else {
        for (int counter = 1; N_argv[counter] != NULL; counter++) {
          printf("%s%s", N_argv[counter], N_argv[counter + 1] ? " " : "");
        }
        printf("\n");
      }
      continue;
    }

    if (strcmp(N_argv[0], "cd") == 0) {
      char *path = N_argv[1];
      if (path == NULL || strcmp(path, "~") == 0) {
        path = getenv("HOME");
        if (path == NULL) {
          fprintf(stderr, "cd: HOME not set\n");
          continue;
        }
      }

      if (strcmp(path, "-") == 0) {
        path = getenv("OLDPWD");
        if (path == NULL) {
          fprintf(stderr, "cd: OLDPWD not set\n");
          continue;
        }
        printf("%s\n", path);
      }

      char oldpwd[size];
      if (getcwd(oldpwd, sizeof(oldpwd)) == NULL) {
        perror("getcwd");
        continue;
      }
      if (chdir(path) != 0) {
        perror("cd");
      } else {
        setenv("OLDPWD", oldpwd, 1);
        if (getcwd(pwd_buffer, size) != NULL) {
          setenv("PWD", pwd_buffer, 1);
        }
      }
      continue;
    }

    // Shell Variable Handling.
    if (strchr(N_argv[0], '=') != NULL && N_argv[1] == NULL) {
      char *equal_ptr = strchr(N_argv[0], '=');
      if (equal_ptr != NULL) {
        int pos = equal_ptr - N_argv[0];
        if (pos > 0) {
          if (Local_Var_Array_Size == Local_Var_Capacity) {
            Local_Var_Capacity =
                (Local_Var_Capacity == 0) ? 4 : Local_Var_Capacity * 2;
            struct Local_Variable *temp =
                realloc(Local_Var_Storage,
                        Local_Var_Capacity * sizeof(struct Local_Variable));
            if (temp == NULL) {
              fprintf(stderr, "Memory reallocation failed.\n");
              free(Buffer);
              free(Local_Var_Storage);
              exit(-9);
            }
            Local_Var_Storage = temp;
          }

          strncpy(Local_Var_Storage[Local_Var_Array_Size].variable, N_argv[0],
                  pos);
          Local_Var_Storage[Local_Var_Array_Size].variable[pos] = '\0';
          strcpy(Local_Var_Storage[Local_Var_Array_Size].value,
                 &N_argv[0][pos + 1]);
          Local_Var_Array_Size++;
        }
      }
      continue;
    }

    // Exporting a variable to the environment variables.
    if (strcmp(N_argv[0], "export") == 0) {
      if (N_argv[1] == NULL) {
        fprintf(stderr, "export: missing argument\n");
        continue;
      }

      char *equal_ptr = strchr(N_argv[1], '=');
      if (equal_ptr == NULL) {
        // Try to find in local variables
        for (int k = 0; k < Local_Var_Array_Size; k++) {
          if (strcmp(N_argv[1], Local_Var_Storage[k].variable) == 0) {
            char *env_var = malloc(strlen(Local_Var_Storage[k].variable) +
                                   strlen(Local_Var_Storage[k].value) + 2);
            sprintf(env_var, "%s=%s", Local_Var_Storage[k].variable,
                    Local_Var_Storage[k].value);
            if (putenv(env_var) != 0) {
              perror("export failed");
            } else {
              printf("Exported local variable %s\n",
                     Local_Var_Storage[k].variable);
            }
            break;
          }
        }
      } else {
        // Make a copy of the string for putenv
        char *env_var = strdup(N_argv[1]);
        if (env_var == NULL) {
          perror("strdup failed");
          continue;
        }
        if (putenv(env_var) != 0) {
          perror("export failed");
        }
      }
      continue;
    }
    if (strcmp(N_argv[0], "exit") == 0) {
      close(saved_stdin);
      close(saved_stdout);
      close(saved_stderr);
      printf("Good Bye :)\n");
      free(Buffer);
      free(Local_Var_Storage);
      exit(0);
    }

    // Fork and execute command
    pid_t PID = fork();

    if (PID > 0) {
      int status;
      wait(&status);
    } else if (PID == 0) {
      execvp(N_argv[0], N_argv);
      // If execvp fails, restore stderr to show error
      dup2(saved_stderr, STDERR_FILENO);
      perror("execvp failed");
      exit(-6);
    } else {
      perror("fork failed");
    }

    // Restore original file descriptors after command execution
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
  }

  close(saved_stdin);
  close(saved_stdout);
  close(saved_stderr);
  free(Buffer);
  free(Local_Var_Storage);
  return 0;
}
