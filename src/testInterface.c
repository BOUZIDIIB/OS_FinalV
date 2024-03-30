#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];  // Buffer de texte
    memset(buffer, 0, sizeof(buffer));  // Initialise le buffer à zéro

    initscr(); // Initialise ncurses
    cbreak(); // Active le mode caractère (pas de mise en mémoire tampon de ligne)
    noecho(); // Désactive l'affichage des caractères saisis

    // Crée une nouvelle fenêtre pour le buffer
    WINDOW *win = newwin(LINES - 1, COLS, 0, 0);
    box(win, 0, 0); // Affiche une bordure autour de la fenêtre
    keypad(win, TRUE); // Active la prise en charge des touches spéciales

    // Affiche un message dans la fenêtre
    mvwprintw(win, 1, 1, "Press F1 to save and exit");

    // Affiche le contenu initial du buffer
    mvwprintw(win, 2, 1, buffer);
    wrefresh(win); // Actualise la fenêtre

    int ch;
    int pos = 0; // Position actuelle dans le buffer

    // Boucle pour la saisie du texte
    while ((ch = wgetch(win)) != KEY_F(1)) { // Sort de la boucle lorsque la touche F1 est pressée
        switch(ch) {
            case KEY_LEFT:
                if (pos > 0) {
                    pos--; // Déplace le curseur vers la gauche
                }
                break;
            case KEY_RIGHT:
                if (pos < strlen(buffer)) {
                    pos++; // Déplace le curseur vers la droite
                }
                break;
            case KEY_BACKSPACE:
                if (pos > 0) {
                    // Supprime un caractère à gauche du curseur
                    memmove(buffer + pos - 1, buffer + pos, strlen(buffer) - pos + 1);
                    pos--; // Déplace le curseur vers la gauche
                }
                break;
            default:
                if (pos < BUFFER_SIZE - 1) {
                    // Insère un caractère à la position actuelle du curseur
                    memmove(buffer + pos + 1, buffer + pos, strlen(buffer) - pos + 1);
                    buffer[pos] = ch;
                    pos++; // Déplace le curseur vers la droite
                }
                break;
        }

        // Efface la fenêtre et réaffiche le contenu du buffer avec le curseur à la bonne position
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 1, 1, "Press F1 to save and exit");
        mvwprintw(win, 2, 1, buffer);
        wmove(win, 2, pos + 1); // Positionne le curseur
        wrefresh(win);
    }

    // Sauvegarde le contenu du buffer dans une variable
    char* contenu = strdup(buffer);
    if (contenu == NULL) {
        mvprintw(LINES - 1, 0, "Failed to allocate memory");
    } else {
        mvprintw(LINES - 1, 0, "Content copied to variable: %s", contenu);
    }

    // Nettoie et ferme ncurses
    delwin(win);
    endwin();
    refresh();

    return 0;
}
