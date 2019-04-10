#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <tuple>

#include "CONSTS.h"

using namespace std;
using namespace sf;

const int height = 256;
const int width = 256;
const int threshold = 400;
bool mark[MAXN][MAXN];

Image src, disp;
queue<pair<int, int>> q;

/*

    Description: This function invokes the recursive call for the DFS algorithm, it crawls through
    the pixel array until all the pixels have been visited. This algorithm was implemented
    for a crawling effect while the image is being displayed.

    Arguments:
        - int i: contains width coordinate value of the current pixel
        - int j: contains the height coordinate value of the current pixel

    Returns:
        - None

*/
void dfs(int i, int j, Color& col) {

    Color cur = src.getPixel(i, j);
    if (sqrsum(col.r - cur.r, col.g - cur.g, col.b - cur.b) > threshold) return;
    mark[i][j] = true;
    disp.setPixel(i, j, cur);

    // performs the recursive call to the DFS algorithm
    for (int di : {-1, 1}) if (i + di > 0 && i + di < width)
        for (int dj : {-1, 1}) if (j + dj > 0 && j + dj < height)
            if (!mark[i+di][j+dj]) dfs(i + di, j + dj, col);
}

/*

    Description: Contains the initial call for the DFS algorithm. It triggers the call to DFS. This will keep calling until all the width(i) coordinates
    at the current height coordinate(j) have been explored.

    Arguments:
        - int& i: contains the width coordinate value of the current pixel
        - int& j: contains the height coordinate value of the current pixel

    returns:
        - none

*/
void dfsCaller(int &i, int &j) {
    // this loop iterates through the width and height coordinates until all pixels have been visited using the DFS algorithm
    for (; i < width; ++i) {
        for (; j < height; ++j) {
            if (mark[i][j]) continue;
            Color colour = src.getPixel(i, j);
            dfs(i, j, colour);
            return;
        }
        j = 0;
    }

}

/*

    Description: contains the BFS algorithm which will give the zooming effect on the image.

    Arguments:
        - None

    returns:
        - None

*/
void bfs() {
    if (q.empty()) {
        for (int i = 0; i < width; ++i) {
            // puts the top and bottom border in the queue
            q.emplace(i, 0);
            q.emplace(i, height-1);
            // keeps track of those pixels as visited
            mark[i][0] = mark[i][height-1] = true;
            //grabs the associated pixel at the given coordinate from the source image object (only pixels from the top)
            Color cur = src.getPixel(i, 0);
            // displays the associated pixel
            disp.setPixel(i, 0, cur);
            // gets the associated pixel at the given coordinate from the source image object (only pixels from the bottom)
            cur = src.getPixel(i, height-1);
            // displays the associated pixel
            disp.setPixel(i, height-1, cur);
        }
        for (int j = 0; j < height; ++j) {
            // puts the left and right in the queue
            q.emplace(0, j); // pass coordinates pair
            q.emplace(width-1, j);
            // keeps track of those pixels as visited
            mark[0][j] = mark[width-1][j] = true;

            Color cur = src.getPixel(0, j);
            disp.setPixel(0, j, cur); // left column
            cur = src.getPixel(width-1, j); // right
            disp.setPixel(width-1, j, cur);
        }
    }

    int i, j, updt = 0;
    while (!q.empty()) { // BFS implementation
        tie(i, j) = q.front(); q.pop(); // assign coordinates

        // grabs the pixel coordinates and then displays it
        Color cur = src.getPixel(i, j);
        disp.setPixel(i, j, cur);

        // now keep iterating through each pixel until we displayed all of them
        for (int di : {-1, 1}) if (i + di > 0 && i + di < width)
            for (int dj : {-1, 1}) if (j + dj > 0 && j + dj < height)
                if (!mark[i+di][j+dj]) {
                    mark[i+di][j+dj] = true;
                    q.emplace(i+di, j+dj);
                }

        ++updt;
        if (4*updt == threshold) break;
    }
}

/*

    Description: This makes the initial window in the program and calls either DFS or BFS depending on user choice

    Arguments:
        - string title: contains the window title
        - int mode: contains the user choice of either BFS or DFS

    Returns:
        - None

*/
void setImage(string title, int mode) {
    auto size = disp.getSize(); // grabs the display size
    RenderWindow window(VideoMode(size.x, size.y), title); // creates a window

    Texture texture;
    texture.loadFromImage(disp); // load the texture from an image
    Event event; // checks for a click event on the window

    Clock clock;
    int i = 0, j = 0;
    while (window.isOpen()) {

        while (window.pollEvent(event)) { // checks if the user closes the window
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        if (clock.getElapsedTime().asSeconds() > 0.01f) {

            if (mode == 1) { // checks the mode entered by the user
                bfs();
            } else {
               dfsCaller(i, j);
            }

            texture.loadFromImage(disp);
            Sprite pic(texture);
            clock.restart();

            // displays the image
            window.clear();
            window.draw(pic);
            window.display();
        }
    }

}

/*

    Description: This is where the GUI program will start and does the initial call and some file opening

    Arguments:
        - None

    Returns:
        - 0: returns 0 for convention

*/
int main() {

    if (!src.loadFromFile("ans.jpg")) {
        cout << "error unable to open source image file" << endl;
        exit(1);
    }

    int mode;

    cout << "Press 1 for BFS effect or 2 for DFS effect: ";
    cin >> mode;

    disp.create(256, 256, Color::Black);

    setImage("Animating", mode);

    return 0;
}