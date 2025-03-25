#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 20
#define COLS 2
#define MAX_LINE_LENGTH 256

typedef struct {
    char* account;
    char* password;
} Register;

const char* headers[] = {
    "Account",
    "Password"
};

Register registers[ROWS];

const char* password = "admin";

// Declaration of functions
void get_password(char* user_input_password);
int authenticate_user(void);
void draw_table(int start_y, int start_x, int row_count, int col_count, int col_width);
void print_passwords(int start_y, int start_x, int col_width);
void menu(void);
int get_amount_of_registers(const char* file_name);
void get_register_content(void);

void get_password(char* user_input_password)
{
    int ch;
    int i = 0;

    while ((ch = getch()) != '\n') {
        if ((ch == 127 || ch == KEY_BACKSPACE) && i > 0) { 
            i--;
            move(getcury(stdscr), getcurx(stdscr) - 1);
            printw(" ");
            move(getcury(stdscr), getcurx(stdscr) - 1);
            refresh();
        } 
        else if (ch != 127 && ch != KEY_BACKSPACE && i < 15) {  
            user_input_password[i++] = ch;
            printw("*");
            refresh();
        }
    }
    user_input_password[i] = '\0';
}

int authenticate_user(void)
{
    char user_input_password[16];
    int keep_going = 1;

    do {
        clear();
        printw("Write the password: ");
        refresh();
        get_password(user_input_password);
        int is_the_correct_password = strcmp(user_input_password, password);
        memset(user_input_password, 0, 16);
        if (!is_the_correct_password) keep_going = 0;
    } while (keep_going);

    return 1;
}

void draw_table(int start_y, int start_x, int row_count, int col_count, int col_width)
{
    for (int row = 0; row <= row_count; row++) {
        for (int col = 0; col <= col_count; col++) {
            int y = start_y + row;
            int x = start_x + col * col_width;

            if (row < get_amount_of_registers("register.csv") + 2) mvprintw(y, x, "|");

            if (row == 1) {
                if (col < col_count) {
                    for (int i = 1; i < col_width; i++) mvprintw(y, x+i, "-");
                }
            }
        }
    }
}

void print_passwords(int start_y, int start_x, int col_width)
{
    // Headers
    for (int col = 0; col < COLS; col++)
        mvprintw(start_y, start_x + col * col_width + 2, "%s", headers[col]);

    // Data
    for (int row = 0; row < ROWS - 1; row++) {
        if (registers[row].account == NULL) continue;
        for (int col = 0; col < COLS; col++)
            mvprintw(start_y + 2 + row, start_x + col * col_width + 2, "%s", (col % 2 == 0) ? registers[row].account : registers[row].password);
    }
}

void menu(void)
{
    int width, height;
    initscr(); // Start curses mode
    getmaxyx(stdscr, height, width); // It gets the width and heigth of the terminal or display idk
    noecho();
    cbreak();

    if (!authenticate_user()) {
        return;
    }

    int user_input_key;
    char* message;
    message = "Password Manager";

    // X and Y are going to be in the middle of the screen (width related)
    int x = (width - strlen(message)) / 2;
    int y = height / 13;

    int col_width = 30;
    int start_y = y+3, start_x = (width / 2) - ((col_width * COLS) / 2);

    while (1) {
        clear();
        mvprintw(y, x, "%s", message);

        draw_table(start_y, start_x, ROWS, COLS, col_width);
        print_passwords(start_y, start_x, col_width);

        refresh(); // Print it on to the real screen
        user_input_key = getch();

        // Handle the user input
        switch (user_input_key)
        {
            // Quits if q is pressed
            case 'q':
                endwin();
                return;
        }
    }
}

int get_amount_of_registers(const char* file_name)
{
    FILE* file_ptr = fopen(file_name, "r");

    if (file_ptr == NULL) {
        perror("Error opening file.");
        return -1;
    }

    int count = 0;
    char c;

    while ((c = fgetc(file_ptr)) != EOF) {
        if (c == '\n') count++;
    }

    fseek(file_ptr, -1, SEEK_END);
    if (fgetc(file_ptr) != '\n') {
        count++;
    }

    fclose(file_ptr);

    return count;
}

void get_register_content(void)
{
    int amount_of_registers = get_amount_of_registers("register.csv");
    FILE* file_ptr = fopen("register.csv", "r");

    char line[MAX_LINE_LENGTH];
    int index = 0;

    while (fgets(line, sizeof(line), file_ptr) && index < amount_of_registers) {
        line[strcspn(line, "\n")] = '\0';

        char *comma = strchr(line, ',');
        if (!comma) {
            registers[index].account = strdup(line);
            registers[index].password = strdup("");
        } else {
            *comma = '\0'; 
            registers[index].account = strdup(line);
            registers[index].password = strdup(comma + 1);
        }

        index++;
    }

    fclose(file_ptr);
}

int main(void)
{
    get_register_content();
    menu();

    return 0;
}
