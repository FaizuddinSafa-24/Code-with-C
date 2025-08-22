#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// -------------------------
// Structure Definitions
// -------------------------
typedef struct {
    char name[50];
    int matches;
    int wins;
    int losses;
    int draws;
} PlayerStats;

typedef struct {
    char *board;   // Dynamic 1D array for NxN board
    int size;      // Board dimension (3x3 & 4x4)
    int moves;     // Number of moves made
    int status;    // 0 = ongoing, 1 = win, 2 = draw
} Game;

typedef struct {
    char player1[50];
    char player2[50];
    char winner[50];
    char timestamp[100];
    int boardSize;
    int gameMode; // 1 = PVP, 2 = PVE
} MatchRecord;

// -------------------------
// Global Variables
// -------------------------
PlayerStats gameStats[4]; // Host, Guest, Player, Bot
int currentBoardSize = 3;

// -------------------------
// Function Prototypes
// -------------------------
void initializeBoard(Game *game, int size);
void printBoard(Game *game);
int checkWinner(Game *game, char symbol);
int isDraw(Game *game);
void freeBoard(Game *game);
int isValidMove(Game *game, int move);

// Move Functions
void playerMove(Game *game, char symbol, const char *playerName);
void botMove(Game *game, char symbol);

// Menu and Game Flow
void displayMainMenu();
int selectGameMode();
int selectBoardSize();
int selectFirstPlayer(int mode);
void playGame(int mode, int firstPlayer, int boardSize);

// Statistics Functions
void loadStats(PlayerStats stats[4]);
void saveStats(PlayerStats stats[4]);
void displayStats(PlayerStats stats[4]);
void updateStats(PlayerStats stats[4], int winner, int mode);

// Match History Functions
void saveMatchResult(const char *p1, const char *p2, const char *winner, int mode, int boardSize);
void displayMatchHistory();
void displayFullStats();

// Utility Functions
void clearInputBuffer();
void getCurrentTimestamp(char *buffer);

// -------------------------
// Main Function
// -------------------------
int main() {
    srand(time(NULL));

    // Initialize player names
    strcpy(gameStats[0].name, "Host");
    strcpy(gameStats[1].name, "Guest");
    strcpy(gameStats[2].name, "Player");
    strcpy(gameStats[3].name, "Bot");

    // Load existing statistics
    loadStats(gameStats);

    int choice;
    int gameMode;
    int firstPlayer;
    int boardSize;

    printf("=== TIC-TAC-TOE GAME ===\n");
    printf("Welcome to Tic-Tac-Toe!\n\n");

    do {
        displayMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
            case 1:
                boardSize = selectBoardSize();
                if(boardSize != -1) {
                    gameMode = selectGameMode();
                    if(gameMode != -1) {
                        firstPlayer = selectFirstPlayer(gameMode);
                        if(firstPlayer != -1) {
                            currentBoardSize = boardSize;
                            playGame(gameMode, firstPlayer, boardSize);
                        }
                    }
                }
                break;

            case 2:
                displayStats(gameStats);
                break;

            case 3:
                displayMatchHistory();
                break;

            case 4:
                printf("Your progress has been successfully saved.\n");
                printf("Goodbye!\n");
                saveStats(gameStats);
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

        if(choice != 4) {
            printf("\nPress Enter to continue...");
            getchar();
        }

    } while(choice != 4);

    return 0;
}

// -------------------------
// Game Core Functions
// -------------------------
void initializeBoard(Game *game, int size) {
    game->size = size;
    game->moves = 0;
    game->status = 0;
    game->board = (char *)malloc(size * size * sizeof(char));

    // Initialize with position numbers (1, 2, 3, ...)
    for (int i = 0; i < size * size; i++) {
        if (i < 9) {
            game->board[i] = '1' + i;
        } else {
            game->board[i] = 'A' + (i - 9); // Alphabets represent Hexadecimal Values
        }
    }
}

void printBoard(Game *game) {
    int size = game->size;

    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("   ");
        for (int j = 0; j < size; j++) {
            printf(" %c ", game->board[i * size + j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");

        if (i < size - 1) {
            printf("   ");
            for (int k = 0; k < size; k++) {
                printf("---");
                if (k < size - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

int checkWinner(Game *game, char symbol) {
    int size = game->size;

    // Check rows
    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = 0; j < size; j++) {
            if (game->board[i * size + j] == symbol)
                count++;
        }
        if (count == size) return 1;
    }

    // Check columns
    for (int j = 0; j < size; j++) {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (game->board[i * size + j] == symbol)
                count++;
        }
        if (count == size) return 1;
    }

    // Check main diagonal
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (game->board[i * size + i] == symbol)
            count++;
    }
    if (count == size) return 1;

    // Check anti-diagonal
    count = 0;
    for (int i = 0; i < size; i++) {
        if (game->board[i * size + (size - 1 - i)] == symbol)
            count++;
    }
    if (count == size) return 1;

    return 0;
}

int isDraw(Game *game) {
    return (game->moves == game->size * game->size);
}

void freeBoard(Game *game) {
    free(game->board);
    game->board = NULL;
}

int isValidMove(Game *game, int move) {
    int maxPos = game->size * game->size;
    if (move < 1 || move > maxPos) return 0;

    char pos = game->board[move - 1];
    return (pos != 'X' && pos != 'O');
}

// -------------------------
// Move Functions
// -------------------------
void playerMove(Game *game, char symbol, const char *playerName) {
    int move;
    int validMove = 0;
    int maxPos = game->size * game->size;

    do {
        printf("%s's turn (%c)\n", playerName, symbol);
        printf("Enter position (1-%d): ", maxPos);
        scanf("%d", &move);
        clearInputBuffer();

        if (isValidMove(game, move)) {
            game->board[move - 1] = symbol;
            game->moves++;
            validMove = 1;
        } else {
            printf("Invalid move! Position must be between 1-%d and not already taken.\n", maxPos);
        }
    } while (!validMove);
}

void botMove(Game *game, char symbol) {
    int move;
    int maxPos = game->size * game->size;

    printf("Bot is thinking");
    for(int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        for(int j = 0; j < 100000000; j++);
    }
    printf("\n");

    // Simple random AI
    do {
        move = rand() % maxPos + 1;
    } while (!isValidMove(game, move));

    game->board[move - 1] = symbol;
    game->moves++;

    printf("Bot chose position %d\n", move);
}

// -------------------------
// Menu and Game Flow Functions
// -------------------------
void displayMainMenu() {
    printf("\n=== TIC-TAC-TOE MENU ===\n");
    printf("1. Start New Game\n");
    printf("2. View Game Statistics\n");
    printf("3. View Match History\n");
    printf("4. Exit\n");
}

int selectBoardSize() {
    int size;
    printf("\nSelect Board Size:\n");
    printf("3. Classic 3x3\n");
    printf("4. Standard 4x4\n");
    printf("Enter board size (3-4): ");
    scanf("%d", &size);
    clearInputBuffer();

    if (size >= 3 && size <= 4) {
        return size;
    } else {
        printf("Invalid size! Please choose between 3 and 4.\n");
        return -1;
    }
}

int selectGameMode() {
    int mode;
    printf("\nSelect Game Mode:\n");
    printf("1. PVP (Host vs Guest)\n");
    printf("2. PVE (Player vs Bot)\n");
    printf("Enter choice (1-2): ");
    scanf("%d", &mode);
    clearInputBuffer();

    if (mode == 1 || mode == 2) {
        return mode;
    } else {
        printf("Invalid choice!\n");
        return -1;
    }
}

int selectFirstPlayer(int mode) {
    int choice;

    if (mode == 1) {
        printf("\nWho goes first?\n");
        printf("1. Host (X)\n");
        printf("2. Guest (O)\n");
        printf("Enter choice (1-2): ");
    } else {
        printf("\nWho goes first?\n");
        printf("1. Player (X)\n");
        printf("2. Bot (O)\n");
        printf("Enter choice (1-2): ");
    }

    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1 || choice == 2) {
        return choice;
    } else {
        printf("Invalid choice!\n");
        return -1;
    }
}

void playGame(int mode, int firstPlayer, int boardSize) {
    Game game;
    int currentPlayer = firstPlayer;
    int winner = 0;

    initializeBoard(&game, boardSize);

    printf("\n=== GAME STARTED ===\n");
    printf("Board Size: %dx%d\n", boardSize, boardSize);

    if (mode == 1) {
        printf("Mode: PVP - Host (X) vs Guest (O)\n");
    } else {
        printf("Mode: PVE - Player (X) vs Bot (O)\n");
    }

    // Game loop
    while (game.status == 0) {
        printBoard(&game);

        if (mode == 1) { // PVP mode
            if (currentPlayer == 1) {
                playerMove(&game, 'X', "Host");
            } else {
                playerMove(&game, 'O', "Guest");
            }
        } else { // PVE mode
            if (currentPlayer == 1) {
                playerMove(&game, 'X', "Player");
            } else {
                botMove(&game, 'O');
            }
        }

        // Check for winner
        char currentSymbol = (currentPlayer == 1) ? 'X' : 'O';
        if (checkWinner(&game, currentSymbol)) {
            game.status = 1;
            winner = currentPlayer;
            printBoard(&game);

            if (mode == 1) {
                printf("Congratulations! %s wins!\n", (winner == 1) ? "Host" : "Guest");
                saveMatchResult("Host", "Guest", (winner == 1) ? "Host" : "Guest", mode, boardSize);
            } else {
                printf("Congratulations! %s wins!\n", (winner == 1) ? "Player" : "Bot");
                saveMatchResult("Player", "Bot", (winner == 1) ? "Player" : "Bot", mode, boardSize);
            }
        } else if (isDraw(&game)) {
            game.status = 2;
            winner = 0;
            printBoard(&game);
            printf("It's a draw!\n");

            if (mode == 1) {
                saveMatchResult("Host", "Guest", "Draw", mode, boardSize);
            } else {
                saveMatchResult("Player", "Bot", "Draw", mode, boardSize);
            }
        }

        // Switch player
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    // Update statistics
    updateStats(gameStats, winner, mode);
    saveStats(gameStats);

    freeBoard(&game);
}

// -------------------------
// Statistics Functions
// -------------------------
void loadStats(PlayerStats stats[4]) {
    FILE *file = fopen("game_stats.txt", "r");

    if (file == NULL) {
        for (int i = 0; i < 4; i++) {
            stats[i].matches = 0;
            stats[i].wins = 0;
            stats[i].losses = 0;
            stats[i].draws = 0;
        }
        return;
    }

    char line[200];
    int lineCount = 0;
    int playerIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL && playerIndex < 4) {
        lineCount++;
        if (lineCount <= 3) continue;

        sscanf(line, "%s %d %d %d %d",
               stats[playerIndex].name, &stats[playerIndex].matches,
               &stats[playerIndex].wins, &stats[playerIndex].losses,
               &stats[playerIndex].draws);
        playerIndex++;
    }

    while (playerIndex < 4) {
        stats[playerIndex].matches = 0;
        stats[playerIndex].wins = 0;
        stats[playerIndex].losses = 0;
        stats[playerIndex].draws = 0;
        playerIndex++;
    }

    fclose(file);
}

void saveStats(PlayerStats stats[4]) {
    // First read existing match history if it exists
    FILE *existingFile = fopen("game_data.txt", "r");
    FILE *tempFile = fopen("temp_history_backup.txt", "w");

    if (existingFile != NULL && tempFile != NULL) {
        char line[200];
        int foundHistorySection = 0;

        // Look for match history section and save it
        while (fgets(line, sizeof(line), existingFile) != NULL) {
            if (strstr(line, "=== MATCH HISTORY ===") != NULL) {
                foundHistorySection = 1;
            }
            if (foundHistorySection) {
                fprintf(tempFile, "%s", line);
            }
        }
        fclose(existingFile);
        fclose(tempFile);
    } else if (tempFile != NULL) {
        fclose(tempFile);
    }
    if (existingFile != NULL) {
        fclose(existingFile);
    }

    // Now write the complete file
    FILE *file = fopen("game_data.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to save statistics!\n");
        return;
    }

    // Write Game Statistics
    fprintf(file, "=== GAME STATISTICS ===\n");
    fprintf(file, "%-10s %-8s %-6s %-8s %-7s %-8s\n", "Player", "Matches", "Wins", "Losses", "Draws", "Win Rate");
    fprintf(file, "--------------------------------------------------------\n");

    for (int i = 0; i < 4; i++) {
        float winRate = (stats[i].matches > 0) ?
                       ((float)stats[i].wins / stats[i].matches * 100) : 0.0;
        fprintf(file, "%-10s %-8d %-6d %-8d %-7d %.1f%%\n",
                stats[i].name, stats[i].matches,
                stats[i].wins, stats[i].losses, stats[i].draws, winRate);
    }

    fprintf(file, "\n");

    // Append existing match history
    FILE *historyFile = fopen("temp_history_backup.txt", "r");
    if (historyFile != NULL) {
        char line[200];
        while (fgets(line, sizeof(line), historyFile) != NULL) {
            fprintf(file, "%s", line);
        }
        fclose(historyFile);
    } else {
        // If no existing history, create the section
        fprintf(file, "=== MATCH HISTORY ===\n");
    }

    fclose(file);

    // Remove temporary file
    remove("temp_history_backup.txt");
}

void displayStats(PlayerStats stats[4]) {
    printf("\n=== GAME STATISTICS ===\n");
    printf("%-10s %-8s %-6s %-8s %-7s %-8s\n", "Player", "Matches", "Wins", "Losses", "Draws", "Win Rate");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < 4; i++) {
        float winRate = (stats[i].matches > 0) ?
                       ((float)stats[i].wins / stats[i].matches * 100) : 0.0;
        printf("%-10s %-8d %-6d %-8d %-7d %.1f%%\n",
               stats[i].name, stats[i].matches,
               stats[i].wins, stats[i].losses, stats[i].draws, winRate);
    }
}

void updateStats(PlayerStats stats[4], int winner, int mode) {
    if (mode == 1) { // PVP mode
        stats[0].matches++; // Host
        stats[1].matches++; // Guest

        if (winner == 1) {
            stats[0].wins++;
            stats[1].losses++;
        } else if (winner == 2) {
            stats[1].wins++;
            stats[0].losses++;
        } else {
            stats[0].draws++;
            stats[1].draws++;
        }
    } else if (mode == 2) { // PVE mode
        stats[2].matches++; // Player
        stats[3].matches++; // Bot

        if (winner == 1) {
            stats[2].wins++;
            stats[3].losses++;
        } else if (winner == 2) {
            stats[3].wins++;
            stats[2].losses++;
        } else {
            stats[2].draws++;
            stats[3].draws++;
        }
    }
}

// -------------------------
// Match History Functions
// -------------------------
void saveMatchResult(const char *p1, const char *p2, const char *winner, int mode, int boardSize) {
    // Read existing file content
    FILE *existingFile = fopen("game_data.txt", "r");
    FILE *tempFile = fopen("temp_complete_file.txt", "a");

    if (tempFile == NULL) {
        printf("Error: Unable to save match history!\n");
        return;
    }

    char timestamp[100];
    getCurrentTimestamp(timestamp);

    // If file exists, copy everything up to and including match history header
    if (existingFile != NULL) {
        char line[200];
        while (fgets(line, sizeof(line), existingFile) != NULL) {
            fprintf(tempFile, "%s", line);
            // Stop after the match history header to append new match
            if (strstr(line, "=== MATCH HISTORY ===") != NULL) {
                break;
            }
        }

        // Copy existing match history entries
        while (fgets(line, sizeof(line), existingFile) != NULL) {
            fprintf(tempFile, "%s", line);
        }

        fclose(existingFile);
    } else {
        // If file does not exist, create basic structure
        fprintf(tempFile, "=== GAME STATISTICS ===\n");
        fprintf(tempFile, "%-10s %-8s %-6s %-8s %-7s %-8s\n", "Player", "Matches", "Wins", "Losses", "Draws", "Win Rate");
        fprintf(tempFile, "--------------------------------------------------------\n");
        fprintf(tempFile, "%-10s %-8d %-6d %-8d %-7d %.1f%%\n", "Host", 0, 0, 0, 0, 0.0);
        fprintf(tempFile, "%-10s %-8d %-6d %-8d %-7d %.1f%%\n", "Guest", 0, 0, 0, 0, 0.0);
        fprintf(tempFile, "%-10s %-8d %-6d %-8d %-7d %.1f%%\n", "Player", 0, 0, 0, 0, 0.0);
        fprintf(tempFile, "%-10s %-8d %-6d %-8d %-7d %.1f%%\n", "Bot", 0, 0, 0, 0, 0.0);
        fprintf(tempFile, "\n=== MATCH HISTORY ===\n");
    }

    // Add new match entry
    fprintf(tempFile, "Date & Time: %s", timestamp);
    fprintf(tempFile, "Board Size: %dx%d\n", boardSize, boardSize);
    fprintf(tempFile, "Game Mode: %s\n", (mode == 1) ? "PVP" : "PVE");
    fprintf(tempFile, "Player 1: %s (X)\n", p1);
    fprintf(tempFile, "Player 2: %s (O)\n", p2);
    fprintf(tempFile, "Winner: %s\n", winner);
    fprintf(tempFile, "=====================================\n");

    fclose(tempFile);

    // Replace original file
    remove("game_data.txt");
    rename("temp_complete_file.txt", "game_data.txt");
}

void displayMatchHistory() {
    FILE *file = fopen("game_data.txt", "r");
    if (file == NULL) {
        printf("\nNo match history found!\n");
        return;
    }

    printf("\n=== MATCH HISTORY ===\n");
    char line[200];
    int foundHistorySection = 0;

    // Skip to match history section
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "=== MATCH HISTORY ===") != NULL) {
            foundHistorySection = 1;
            continue;
        }
        if (foundHistorySection) {
            printf("%s", line);
        }
    }

    if (!foundHistorySection) {
        printf("No match history available yet!\n");
    }

    fclose(file);
}

void displayFullStats() {
    FILE *file = fopen("game_data.txt", "r");
    if (file == NULL) {
        printf("\nNo game data found!\n");
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

// -------------------------
// Utility Functions
// -------------------------
void clearInputBuffer() {
    while (getchar() != '\n');
}

void getCurrentTimestamp(char *buffer) {
    time_t now;
    time(&now);
    strcpy(buffer, ctime(&now));
}