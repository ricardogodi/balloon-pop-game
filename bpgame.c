/*
 * Project #3
 * Author: Ricardo Gonzalez
 *
 * Description:
 *      This program implements a primitive version of the game
 *      'Pop it', which can be played on the following web page:
 *      http://poppit.pogo.com/hd/PoppitHD.html
 *
 */

#include "bpgame.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/**
 BPGame is incompletely specified as far as any client program
 is concerned.
 A BPGame "instance" captures everything about a particular run
 of a balloon pop game:  the current state of the board,
 the score, any additional info for the undo feature, etc.
 
 The definition of actual struct bpgame is hidden in bboard.c
 **/
struct Node {
    char** boardPtr;
    int score;
    struct Node *next;
    
};
typedef struct Node Node;

struct bpgame {
    int nrows;
    int ncols;
    struct Node* front;
};

/**
 * Function:  bp_create
 * Parameters: number of rows and columns
 * Description: returns pointer to newly created
 *              board with given number of rows
 *              and columns.
 *
 *              Populates board with random balloons.
 *
 *              If nrows or ncols is out of range,
 *              an error message is printed to stderr
 *              and NULL is returned.
 *
 */
extern BPGame * bp_create(int nrows, int ncols) {
    srand(time(NULL));
    BPGame *gamePtr = malloc(sizeof(BPGame));
    char **m;
    int rndm;
    m = malloc(nrows * sizeof(char*)); // Create array of pointers to rows
    for (int i = 0; i < nrows; i++) {
        m[i] = malloc(sizeof(char) * ncols);  // Create rows
    }
    char colors[5] = {Red,Blue,Green,Yellow};
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            rndm = rand() % 4;
            m[i][j] = colors[rndm];
        }
    }
    gamePtr->ncols = ncols;
    gamePtr->nrows = nrows;
    Node *newNode = malloc(sizeof(Node));
    newNode->boardPtr = m;
    newNode->score = 0;
    newNode->next = NULL;
    gamePtr->front = newNode;  // Make front point to the new Node
    
    return gamePtr;
}

/**
 * Function:  bp_create_from_mtx
 * Parameters: number of rows and columns and 2D array of char
 * Description: returns pointer to newly created
 *              board with given number of rows
 *              and columns.
 *
 *              Populates board contents of given 2D matrix (assumes
 *           given dimensions).
 *
 *           Error conditions:
 *                      If nrows or ncols is out of range, an error message
 *            is printed to stderr and NULL is returned.
 *
 *            If any element in the matrix is not one of the understood
 *            color symbols (from the #define-s at top of this file),
 *            an error message is pritnted to stderr and NULL is returned.
 */
extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols) {
    //Check valid nrows and ncols
    if (nrows > MAX_ROWS || ncols > MAX_COLS) {
        fprintf(stderr, "Invalid number of rows or columns\n");
        return NULL;
    }
    BPGame *gamePtr = malloc(sizeof(BPGame));
    char **m;
    m = malloc(nrows * sizeof(char*)); // Create array of pointers to rows
    for (int i = 0; i < nrows; i++) {
        m[i] = malloc(sizeof(char) * ncols);  // Create rows
    }
    
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            if(mtx[i][j] != None
               && mtx[i][j] != Red
               && mtx[i][j] != Blue
               && mtx[i][j] != Green
               && mtx[i][j] != Yellow) {
                fprintf(stderr, "Invalid color provided\n");
                return NULL;
            } else {
                m[i][j] = mtx[i][j];
            }
        }
    }
    gamePtr->ncols = ncols;
    gamePtr->nrows = nrows;
    Node *newNode = malloc(sizeof(Node));
    newNode->boardPtr = m;
    newNode->score = 0;
    newNode->next = NULL;
    gamePtr->front = newNode;  // Make front point to the new Node
    return gamePtr;
}

/**
 * Function:  bp_destroy
 * Description:  deallocates all dynamically allocated
 *          memory associated with the board.
 *
 *        Comment:  this might start out as
 *        a one-liner.
 *
 */
extern void bp_destroy(BPGame * b) {
    char **m;
    Node *temp;
    while (b->front != NULL) {
        //printf("Freeing node...\n");
        m = b->front->boardPtr;
        int n = b->ncols;
        for(int i = 0; i < n; i++) {
            free(m[i]);     // Free rows
        }
        free(m);
        temp = b->front;
        b->front = b->front->next;
        free(temp);
    }
}

/**
 * Function:  bp_display
 * Description:  pretty-prints the current board.
 *
 *        uses '|', '+' and '-' to "draw" the boundaries
 *               of the board.
 *
 *        For full credit rows and columns are labeled with
 *         their indices.  Column indices displayed "vertically"
 *
 *        Examples:
 *
 *  This board is full of balloons (like at the beginning of a game).
 *
 *       +-----------------------+
 *          0 | + + + = + ^ ^ o + = = |
 *          1 | ^ = = o + o + + + ^ = |
 *          2 | ^ + ^ o + o + = = = = |
 *          3 | = ^ o o = o + + + = = |
 *            +-----------------------+
 *             0 0 0 0 0 0 0 0 0 0 1
 *              0 1 2 3 4 5 6 7 8 9 0
 *
 *
 *  This board has some empty slots represented by  '.'
 *       +-----------------------+
 *          0 | + + + = + ^ ^ o + = = |
 *          1 | ^ = o o + o + + + ^ = |
 *          2 | ^ + . o + . + = . . = |
 *          3 | . . . o . . + + . . = |
 *            +-----------------------+
 *             0 0 0 0 0 0 0 0 0 0 1
 *              0 1 2 3 4 5 6 7 8 9 0
 *
 *
 *  Here is the same board using a space ' ' to
 *  represent an empty slot.
 *
 *       +-----------------------+
 *          0 | + + + = + ^ ^ o + = = |
 *          1 | ^ = o o + o + + + ^ = |
 *          2 | ^ +   o +   + =     = |
 *          3 |       o     + +     = |
 *            +-----------------------+
 *             0 0 0 0 0 0 0 0 0 0 1
 *              0 1 2 3 4 5 6 7 8 9 0
 *
 */
extern void bp_display(BPGame * b) {
    int m = b->nrows;
    int n = b->ncols;
    
    char **board = b->front->boardPtr;
    printf("    +");
    for (int i = 0; i < n*2+1; i++) {
        printf("-");
    }
    printf("+\n");
    for (int i = 0; i < m; i++) {
        printf("%3d | ", i);
        for (int j = 0; j < n; j++) {
            printf("%c ", board[i][j]);
        }
        printf("|\n");
    }
    printf("    +");
    for (int i = 0; i < n*2+1; i++) {
        printf("-");
    }
    printf("+\n");
    printf("      ");
    for (int i = 0; i < n; i++) {
        printf("%d ", i/10);
    }
    printf("\n      ");
    for (int i = 0; i < n; i++) {
        printf("%d ", i%10);
    }
    printf("\n");
}

/*
 * Helper Function hasFriend
 * Returns 1 if the coordinate provided has at least one friend
 * Returns 0 otherwise
 */
int hasFriend(BPGame * b, int r, int c, char colorTarget) {
    char **board = b->front->boardPtr;
    int m = b->nrows;
    int n = b->ncols;
    if (c != 0) {  // Check for valid c
        if (board[r][c - 1] == colorTarget){  // LEFT
            return 1;
        }
    }
    if (r != 0) {  // Check for valid r
        if (board[r - 1][c] == colorTarget){  // UP
            return 1;
        }
    }
    if (c < n - 1) {  // Check for valid c
        if (board[r][c + 1] == colorTarget){  // RIGHT
            return 1;
        }
    }
    if (r < m - 1) {  // Check for valid r
        if (board[r + 1][c] == colorTarget){  // DOWN
            return 1;
        }
    }
    return 0;
}

/*
 * This is the recursive function that actually performs the 'pop' operation
 * Returns the total number of balloons popped
 */
int popRecurse(BPGame * b, int r, int c) {
    char **board = b->front->boardPtr;
    char color = b->front->boardPtr[r][c];
    int ballonsPopped = 0;
    int m = b->nrows;
    int n = b->ncols;
    board[r][c] = '.'; // POP!
    ballonsPopped = 1;
    if (c != 0) {  // Check for valid c
        if (board[r][c - 1] == color){  // LEFT
            ballonsPopped = ballonsPopped + popRecurse(b, r, c - 1);
        }
    }
    if (r != 0) {  // Check for valid r
        if (board[r - 1][c] == color){  // UP
            ballonsPopped = ballonsPopped + popRecurse(b, r - 1, c);
        }
    }
    if (c < n - 1) {  // Check for valid c
        if (board[r][c + 1] == color){  // RIGHT
            ballonsPopped = ballonsPopped + popRecurse(b, r, c + 1);
        }
    }
    if (r < m - 1) {  // Check for valid r
        if (board[r + 1][c] == color){  // DOWN
            ballonsPopped = ballonsPopped + popRecurse(b, r + 1, c);
        }
    }
    return ballonsPopped;
}

/*
 * Helper function that copies and returns a board
 */
char** copyBoard(char** oldBoard, int nrows, int ncols) {
    char **m;
    m = malloc(nrows * sizeof(char*)); // Create array of pointers to rows
    for (int i = 0; i < nrows; i++) {
        m[i] = malloc(sizeof(char) * ncols);  // Create rows
    }
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            m[i][j] = oldBoard[i][j];
        }
    }
    return m;
}

/**
 * Function:  bp_pop
 * Description:  (attempts to) "pop" the balloon at (r,c) (if any)
 *               and its associated "color cluster"
 *
 *               Cluster must be of AT LEASE SIZE 2 (a balloon that
 *                   is its own cluster will not be popped).
 *
 * Returns:  number of balloons popped (which may be zero).
 *
 * Side effects:
 *        locations of popped balloons become None (as in the "color"
 *            None in the #define at the top of this file).
 *
 *        adds n*(n-1) to score if n balloons were popped.
 *
 * Notes:  does not require that the board be "compact".  But presumably,
 *    most client applicationw would only call on a compact board.
 *
 */
extern int bp_pop(BPGame * b, int r, int c) {
    char color = b->front->boardPtr[r][c];
    int hasfriend = hasFriend(b, r, c, color);
    int m = b->nrows;
    int n = b->ncols;
    if (hasfriend == 0 || color == '.') {  // Filter
        return 0;
    }
    char **boardCopy = copyBoard(b->front->boardPtr, m, n);
    Node *newNode = malloc(sizeof(Node));
    newNode->boardPtr = boardCopy;
    newNode->score = b->front->score;    // Prepend node
    newNode->next = b->front;
    b->front = newNode;
    int ballonsPopped = popRecurse(b, r, c);
    b->front->score = b->front->score + ballonsPopped*(ballonsPopped - 1);
    return ballonsPopped;
}

/**
 * Function:  bp_is_compact
 * Description:  determines if all balloons are as "high" as possible.
 *               Returns 1 if this is the case
 *               Returns 0 otherwise.
 *
 *               Note:  a balloon is as high as possible if and only
 *             if all slots in its column ABOVE it are also balloons
 *        (non-empty)
 *
 */
extern int bp_is_compact(BPGame * b) {
    char **board = b->front->boardPtr;
    int m = b->nrows;
    int n = b->ncols;
    int firstAir;
    for (int j = 0; j < n; j++) {
        firstAir = -1;
        for (int i = 0; i < m; i++) {  // Find the 'air' index
            if (board[i][j] == '.') {
                firstAir = i;
                break;
            }
        }
        if(firstAir != -1) {
            for (int i = firstAir + 1; i < m; i++) {  // Find balloon
                if(board[i][j] != '.') {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/**
 * Function:  bp_float_one_step
 * Description:  moves all balloons that that are NOT AS HIGH AS POSSIBLE
 *               up one spot.
 *
 *   Example:
 *
 *        before
 *
 *       +-----------------------+
 *          0 | + + + = + ^     + = = |
 *          1 | ^       + o + +   ^ = |
 *          2 | ^ +   o +   + =     = |
 *          3 |       o     + + =   = |
 *            +-----------------------+
 *             0 0 0 0 0 0 0 0 0 0 1
 *              0 1 2 3 4 5 6 7 8 9 0
 *
 *        after
 *
 *       +-----------------------+
 *          0 | + + + = + ^ + + + = = |
 *          1 | ^ +   o + o + =   ^ = |
 *          2 | ^     o +   + + =   = |
 *          3 |                     = |
 *            +-----------------------+
 *             0 0 0 0 0 0 0 0 0 0 1
 *              0 1 2 3 4 5 6 7 8 9 0
 *
 *
 * Note:  think animation by repeating this until it
 *   is compact (perhaps displaying each intermediate
 *   configuration).
 */
extern void bp_float_one_step(BPGame * b) {
    char **board = b->front->boardPtr;
    int m = b->nrows;
    int n = b->ncols;
    int firstAir;
    for (int j = 0; j < n; j++) {
        firstAir = -1;
        for (int i = 0; i < m; i++) {  // Find the 'air' index
            if (board[i][j] == '.') {
                firstAir = i;
                break;
            }
        }
        if(firstAir != -1) {
            for (int i = firstAir + 1; i < m; i++) {  // Find block
                if(board[i][j] != '.') {
                    board[i - 1][j] = board[i][j];
                    board[i][j] = '.';
                }
            }
        }
    }
}

/**
 * Function:  bp_score
 * Description:  returns the current score
 *
 */
extern int bp_score(BPGame * b) {
    return b->front->score;
}

/**
 * Function:   bp_get_balloon
 * Description:  returns color of balloon at given location as a char.
 *        if (r,c) is out of range, -1 is returned.
 *
 */
extern int bp_get_balloon(BPGame * b, int r, int c) {
    int m = b->nrows;
    int n = b->ncols;
    char **boardPtr = b->front->boardPtr;
    if ((r > -1 && r < m)&&(c > -1 && c < n)) {
        return boardPtr[r][c];
    } else {
        return -1;
    }
    return 0;
}

/**
 * Function:   bp_can_pop
 * Description:  returns 1 if the player can pop some balloons;
 *                0 otherwise.
 *
 *        If the board is empty, of course nothing can
 *            be popped.
 *        If there are no adjacent balloons of the same
 *        color, there also are no moves.
 *
 * Notes:  probable usage is to determine when game is over.
 */
extern int bp_can_pop(BPGame * b) {
    char **board = b->front->boardPtr;
    int m = b->nrows;
    int n = b->ncols;
    char nextColor;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            nextColor = board[i][j];
            if (board[i][j] != '.' && hasFriend(b, i, j, nextColor)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Function:  bp_undo
 * Description:  "undoes" most recent successful pop (if any),
 *        restoring the board to the exact state it was
 *        in immediately prior to that pop.
 *
 *               Also restores score to corresponding previous value.
 *
 *               Returns 1 if successful; 0 otherwise (i.e., when in the initial state --
 *        no moves have been made to be undone).
 *
 *        NOTE:  bp_undo can be called repeatedly to back up all the way to the beginning
 *        of the game.
 *
 */
extern int bp_undo(BPGame * b) {
    char **m;
    int n = b->ncols;
    Node *temp = b->front;
    if (b->front->next != NULL) {
        m = temp->boardPtr;
        for(int i = 0; i < n; i++) {
            free(m[i]);     // Free rows
        }
        free(m);
        b->front = b->front->next;
        free(temp);
        return 1;
    } else {
        return 0;
    }
}
