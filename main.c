#include <ncurses.h>
#include <string.h>

const char* password = "admin";

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

int authenticate_user()
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

void menu()
{
    int width, height;
    initscr(); // Start curses mode
    getmaxyx(stdscr, height, width); // It gets the width and heigth of the terminal or display idk
    noecho();
    cbreak();

    if (!authenticate_user()) {
        return;
    }

    curs_set(0); // Hide the cursor
    keypad(stdscr, TRUE);
    int user_input_key;
    char* message;
    message = "Password Manager";

    // X and Y are going to be in the middle of the screen (width related)
    int x = (width - strlen(message)) / 2;
    int y = height / 13;

    while (1) {
        clear();

        mvprintw(y, x, "%s", message);
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

int main(void)
{
    menu();

    return 0;
}
