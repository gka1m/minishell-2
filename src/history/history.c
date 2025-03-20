/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:19:13 by zchan             #+#    #+#             */
/*   Updated: 2025/03/07 17:01:00 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

#define HISTORY_SIZE 100

/*
Command History Feature
    Command history allows the shell to store and recall previously executed commands.
    Let’s implement it step by step:

Key Features:
    Store commands in memory.
    Provide basic navigation using the up and down arrow keys.
    Optionally save history to a file for persistence across sessions.

Implementation Steps:
Data Structure for History:
    Use a dynamic array or linked list to store command strings.
    Keep a pointer for the current position during navigation.
Reading Arrow Keys:
    Use the read system call to capture user input, including escape sequences for arrows.
    Interpret the escape codes for up (\033[A) and down (\033[B) keys.
Adding Commands to History:
    Append a command after it is successfully executed.
    Avoid duplicating consecutive entries (optional).
History File (Optional):
    Load history from a file (e.g., .shell_history) when the shell starts.
    Save history to the file when the shell exits.
*/

// History structure
typedef struct 
{
    char *commands[HISTORY_SIZE];
    int count;  // Number of stored commands
    int current; // Current position for navigation
} History;

History history = {.count = 0, .current = -1};

// Add command to history
void    add_to_history(const char *cmd) 
{
    if (history.count == HISTORY_SIZE) 
    {
        free(history.commands[0]); // Remove the oldest entry
        ft_memmove(history.commands, history.commands + 1, (HISTORY_SIZE - 1) * sizeof(char *));
        history.count--;
    }
    history.commands[history.count++] = ft_strdup(cmd);
    history.current = history.count; // Reset navigation pointer
}

// Save history to file
/* void save_history(const char *filename) 
{
    FILE *file = fopen(filename, "w");
    if (!file)
        return;

    for (int i = 0; i < history.count; i++) {
        fprintf(file, "%s\n", history.commands[i]);
    }
    fclose(file);
} */

void save_history(const char *filename) 
{
    int fd;
    int i;

    i = 0;
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return ;
    while (i < history.count)
    {
        write(fd, history.commands[i], ft_strlen(history.commands[i]));
        write(fd, "\n", 1);
        i++;
    }
    close(fd);
}

// Enable raw mode for terminal input
void    enable_raw_mode(void) 
{
    struct termios  term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Disable raw mode
void disable_raw_mode(void) 
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Capture and handle arrow keys
void handle_input(void) 
{
    char input[1024];
    int index = 0;

    enable_raw_mode();
    printf("minishell> ");
    while (1) 
    {
        char c = getchar();
        if (c == '\n') 
        { // Enter key
            input[index] = '\0';
            printf("\n");
            add_to_history(input); // Add to history
            printf("Executing: %s\n", input);
            index = 0; // Reset input
            printf("minishell> ");
        }
        else if (c == 127) 
        { // Backspace
            if (index > 0) 
            {
                index--;
                printf("\b \b");
            }
        }
        else if (c == 27) 
        { // Escape sequence for arrows
            char seq[2];
            seq[0] = getchar();
            seq[1] = getchar();
            if (seq[0] == '[')
            {
                if (seq[1] == 'A')
                { // Up arrow
                    if (history.current > 0)
                    {
                        history.current--;
                        printf("\r\033[Kshell> %s", history.commands[history.current]);
                        ft_strlcpy(input, history.commands[history.current], ft_strlen(input));
                        index = ft_strlen(input);
                    }
                }
                else if (seq[1] == 'B')
                { // Down arrow
                    if (history.current < history.count - 1)
                    {
                        history.current++;
                        printf("\r\033[Kshell> %s", history.commands[history.current]);
                        ft_strlcpy(input, history.commands[history.current], ft_strlen(input));
                        index = ft_strlen(input);
                    }
                    else 
                    {
                        history.current = history.count; // Reset to blank
                        printf("\r\033[Kshell> ");
                        index = 0;
                    }
                }
            }
        }
        else
        {
            input[index++] = c;
            putchar(c);
        }
    }
    disable_raw_mode();
}

/* void handle_input(void)
{
    char *input;

    while (1)
    {
        // Display the prompt and read input
        input = readline("minishell> ");
        if (!input)
            break; // Exit on EOF (Ctrl+D)

        // Add non-empty input to history
        if (*input)
            add_history(input);

        // Execute the input (replace this with your execution logic)
        printf("Executing: %s\n", input);

        // Free the input buffer
        free(input);
    }
} */

// int main() {
//     load_history(".shell_history");
//     handle_input();
//     save_history(".shell_history");
//     return 0;
// }
