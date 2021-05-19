#include "Labyrinth.h"
#include "GUI/SimpleTest.h"
#include "Demos/MazeGenerator.h"
#include "set.h"
using namespace std;

bool isPathToFreedomRec(MazeCell* start, const string& moves, Set<Item>& itemsCollected) {
    Item wand = Item::WAND;
    Item potion = Item::POTION;
    Item spell = Item::SPELLBOOK;
    Item nothing = Item::NOTHING;

    if (start == nullptr) {
        return itemsCollected.contains(wand)
               && itemsCollected.contains(potion)
               && itemsCollected.contains(spell);

    } else if (moves.size() == 0) {
            if (start -> whatsHere != nothing) {
                itemsCollected += start -> whatsHere;
            }
            return itemsCollected.contains(wand)
                   && itemsCollected.contains(potion)
                   && itemsCollected.contains(spell);

    } else {
        if (start -> whatsHere != nothing) {
            itemsCollected += start -> whatsHere;
        }

        char firstMove = moves[0];
        string remaining = moves.substr(1);

        if (firstMove == 'N') {
            start = start->north;
        } else if (firstMove == 'S') {
            start = start->south;
        } else if (firstMove == 'E') {
            start = start->east;
        } else {
            start = start->west;
        }

        return isPathToFreedomRec(start, remaining, itemsCollected);
    }
}

bool isPathToFreedom(MazeCell* start, const string& moves) {
    Set<Item> itemsCollected = {};
    return isPathToFreedomRec(start, moves, itemsCollected);
}


/* * * * * * Test Cases Below This Point * * * * * */

/* Optional: Add your own custom tests here! */














/* * * * * Provided Tests Below This Point * * * * */

/* Utility function to free all memory allocated for a maze. */
void deleteMaze(const Grid<MazeCell*>& maze) {
    for (auto* elem: maze) {
        delete elem;
    }
    /* Good question to ponder: why don't we delete maze, or empty maze, or something
     * along those lines?
     */
}

PROVIDED_TEST("Checks paths in the sample maze.") {
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN"));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES"));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES"));

    /* These paths don't work, since they don't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW"));
    EXPECT(!isPathToFreedom(maze[2][2], "SWWN"));
    EXPECT(!isPathToFreedom(maze[2][2], "WNNE"));

    /* These paths don't work, since they aren't legal paths. */
    EXPECT(!isPathToFreedom(maze[2][2], "WW"));
    EXPECT(!isPathToFreedom(maze[2][2], "NN"));
    EXPECT(!isPathToFreedom(maze[2][2], "EE"));
    EXPECT(!isPathToFreedom(maze[2][2], "SS"));

    deleteMaze(maze);
}

PROVIDED_TEST("Can't walk through walls.") {
    auto maze = toMaze({"* S *",
                        "     ",
                        "W * P",
                        "     ",
                        "* * *"});

    EXPECT(!isPathToFreedom(maze[1][1], "WNEES"));
    EXPECT(!isPathToFreedom(maze[1][1], "NWSEE"));
    EXPECT(!isPathToFreedom(maze[1][1], "ENWWS"));
    EXPECT(!isPathToFreedom(maze[1][1], "SWNNEES"));

    deleteMaze(maze);
}

PROVIDED_TEST("Works when starting on an item.") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "EE"));
    EXPECT(isPathToFreedom(maze[0][1], "WEE"));
    EXPECT(isPathToFreedom(maze[0][2], "WW"));

    deleteMaze(maze);
}

