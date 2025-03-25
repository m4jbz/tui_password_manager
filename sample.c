#include <ncurses.h>

#define NUM_OPTIONS 3

const char *opciones[] = {
    "Opción 1",
    "Opción 2",
    "Salir"
};

void menu() {
    int seleccion = 0;
    int tecla;

    initscr();            // Inicializa ncurses
    noecho();             // No muestra la entrada del teclado
    curs_set(0);          // Oculta el cursor
    keypad(stdscr, TRUE); // Habilita teclas especiales como flechas

    while (1) {
        clear();
        for (int i = 0; i < NUM_OPTIONS; i++) {
            if (i == seleccion)
                attron(A_REVERSE); // Resalta la opción actual
            mvprintw(i + 1, 2, "%s", opciones[i]);
            attroff(A_REVERSE);
        }

        tecla = getch(); // Lee entrada del usuario

        switch (tecla) {
            case KEY_UP:
                if (seleccion > 0) seleccion--;
                break;
            case KEY_DOWN:
                if (seleccion < NUM_OPTIONS - 1) seleccion++;
                break;
            case '\n': // Enter
                if (seleccion == NUM_OPTIONS - 1) {
                    endwin(); // Termina ncurses antes de salir
                    return;
                }
                mvprintw(NUM_OPTIONS + 2, 2, "Seleccionaste: %s", opciones[seleccion]);
                refresh();
                getch(); // Espera una tecla para continuar
                break;
        }
    }
}

int main() {
    menu();
    return 0;
}
