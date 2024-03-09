#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <map>
#include <numeric>

// Represents a point in a two-dimensional space with additional visual properties.
// The point is defined by its coordinates (x, y), a color, and a size, which
// can be used for rendering on a graphical interface. This struct is designed
// to be used within the context of a grid or game board, where each point
// can represent a cell or an entity with a specific location and visual appearance.
struct Point {
    int x, y;
    sf::Color color = sf::Color(118, 171, 174, 255);
    float size = 50;

    // Constructs a Point with specified coordinates, defaulting the color and size.
    // This constructor allows for the creation of a point without specifying visual properties,
    // making it suitable for cases where only the location is of concern.
    Point(int x, int y) : x(x), y(y), color(sf::Color(118, 171, 174, 255)), size(50) {}

    // Constructs a Point with specified coordinates, color, and size.
    // This overloaded constructor provides a way to fully specify a point's properties,
    // enabling the customization of its appearance in addition to its location.
    Point(int x, int y, sf::Color color, float size) : x(x), y(y), color(color), size(size) {}

    // Compares this point with another for equality based on coordinates.
    // Two points are considered equal if they have the same x and y coordinates,
    // ignoring their visual properties. This is useful for identifying the same locations
    // on a grid regardless of how they are rendered.
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Defines a strict weak ordering of points based on their coordinates.
    // This operator allows Points to be used in sorted containers and algorithms
    // that require an ordering criterion. Points are ordered primarily by their x coordinate,
    // and then by their y coordinate if their x coordinates are equal. This ordering
    // is useful for organizing points in a consistent manner.
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

// Patterns made to have fun, last one is a special one !
std::unordered_map<std::string, std::vector<std::pair<int, int>>> patterns = {
	{"Pulsar", {
		{2,4},{3,4},{4,4},{8,4},{9,4},{10,4},
		{2,6},{7,6},{12,6},
		{2,7},{7,7},{12,7},
		{2,8},{7,8},{12,8},
		{2,10},{3,10},{4,10},{8,10},{9,10},{10,10},
		{2,11},{7,11},{12,11},
		{2,12},{7,12},{12,12},
		{2,13},{7,13},{12,13},
		{2,15},{3,15},{4,15},{8,15},{9,15},{10,15}
	}},
	{"LLWS", {
		{1,0},{4,0},
		{0,1},
		{0,2},{4,2},
		{0,3},{1,3},{2,3},{3,3}
	}},
	{"Gosper Glider Gun", {
		{1,5},{1,6},{2,5},{2,6},
		{11,5},{11,6},{11,7},{12,4},{12,8},{13,3},{13,9},{14,3},{14,9},{15,6},
		{16,4},{16,8},{17,5},{17,6},{17,7},{18,6},
		{21,3},{21,4},{21,5},{22,3},{22,4},{22,5},{23,2},{23,6},{25,1},{25,2},{25,6},{25,7},
		{35,3},{35,4},{36,3},{36,4}
	}},
	{"Ben Special", {
		{24,22},  {22,23},  {24,23},  {12,24},  {13,24},  {20,24},  {21,24},  {34,24},
		{35,24},  {11,25},  {15,25},  {20,25},  {21,25},  {34,25},  {35,25},  {0,26},
		{1,26},   {10,26},  {16,26},  {20,26},  {21,26},  {0,27},   {1,27},   {10,27},
        {14,27},  {16,27},  {17,27},  {22,27},  {24,27},  {10,28},  {16,28},  {24,28},
        {11,29},  {15,29},  {12,30},  {13,30},  {54,52},  {55,52},  {56,52},  {60,52},
        {61,52},  {62,52},  {52,54},  {57,54},  {59,54},  {64,54},  {52,55},  {57,55},
        {59,55},  {64,55},  {52,56},  {57,56},  {59,56},  {64,56},  {54,57},  {55,57},
        {56,57},  {60,57},  {61,57},  {62,57},  {75,75},  {78,75},  {74,76},  {74,77},
        {74,78},  {75,78},  {76,78},  {77,78},  {78,77},  {105,32}, {103,33}, {105,33},
        {93,34},  {94,34},  {101,34}, {102,34}, {115,34}, {116,34}, {92,35},  {96,35},
		{101,35}, {102,35}, {115,35}, {116,35}, {81,36},  {82,36},  {91,36},  {97,36},
        {101,36}, {102,36}, {81,37},  {82,37},  {91,37},  {95,37},  {97,37},  {98,37},
        {103,37}, {105,37}, {91,38},  {97,38},  {105,38}, {92,39},  {96,39},  {93,40},
		{94,40},  {40,40},  {41,40},  {39,41},  {42,41},  {40,42},  {41,42},  {60,60},
		{61,60},  {59,61},  {62,61},  {60,62},  {61,62},  {80,100}, {81,100}, {82,100},
        {85,100}, {86,100}, {87,100}, {90,100}, {91,100}, {92,100}

	}}
};

namespace GolEngine {

    // Generates a list of all neighboring points around a given point in the grid.
    // Neighbors are determined based on the Moore neighborhood, which includes
    // the eight cells surrounding a central cell in a two-dimensional square lattice.
    std::vector<Point> getNeighbors(const Point& p) {
        return {
            {p.x - 1, p.y - 1}, {p.x, p.y - 1}, {p.x + 1, p.y - 1},
            {p.x - 1, p.y},                     {p.x + 1, p.y},
            {p.x - 1, p.y + 1}, {p.x, p.y + 1}, {p.x + 1, p.y + 1}
        };
    }

    // Counts the number of neighbors each cell has by iterating over all alive cells
    // and incrementing a count for each of their neighbors. This function is crucial
    // for determining the next state of each cell based on Conway's Game of Life rules.
    std::map<Point, int> countNeighbors(const std::vector<Point>& alive) {
        std::map<Point, int> neighborCount;
        for (const auto& cell : alive) {
            auto neighbors = GolEngine::getNeighbors(cell);
            std::accumulate(neighbors.begin(), neighbors.end(), std::ref(neighborCount),
                            [](auto& countMap, const Point& neighbor) {
                countMap.get()[neighbor]++;
                return countMap;
            });
        }
        return neighborCount;
    }

    // Identifies potential candidates for the next generation by collecting all unique
    // points that are either currently alive or are neighbors of alive cells. This set
    // of candidates will be filtered to determine which cells remain or become alive.
    std::set<Point> getCandidates(const std::vector<Point>& alive) {
        std::set<Point> candidates;
        for (const auto& cell : alive) {
            auto neighbors = getNeighbors(cell);
            candidates.insert(neighbors.begin(), neighbors.end());
            candidates.insert(cell); // Include the cell itself as a candidate
        }
        return candidates;
    }

    // Filters through the set of candidate cells to determine which will be alive
    // in the next generation. This function applies the rules of Conway's Game of Life
    // to decide the fate of each cell based on its current state and the number of live neighbors.
    std::vector<Point> filterNextGen(const std::set<Point>& candidates, const std::map<Point, int>& neighborCount, const std::vector<Point>& alive) {
        return std::accumulate(candidates.begin(), candidates.end(), std::vector<Point>{},
                               [&](std::vector<Point>& nextGen, const Point& candidate) {
            const auto it = neighborCount.find(candidate);
            const int count = it != neighborCount.end() ? it->second : 0;
            const bool isAlive = std::find(alive.begin(), alive.end(), candidate) != alive.end();
            if ((isAlive && (count == 2 || count == 3)) || (!isAlive && count == 3)) {
                nextGen.push_back(candidate);
            }
            return nextGen;
        });
    }

    // Calculates the next generation of cells based on the current state of the grid.
    // This function orchestrates the process by counting neighbors, determining candidates,
    // and applying the game's rules to filter the candidates into the next generation.
    std::vector<Point> nextGeneration(const std::vector<Point>& alive) {
        auto neighborCount = countNeighbors(alive);
        auto candidates = getCandidates(alive);
        return filterNextGen(candidates, neighborCount, alive);
    }

    // Places a predefined pattern onto the grid at a specified starting position.
    // This allows for the initialization of the grid with complex structures that
    // can evolve in interesting ways according to the game's rules.
    void placePattern(const std::string& patternName, const int startX, const int startY, std::vector<Point>& points) {
        auto it = patterns.find(patternName);
        if (it != patterns.end()) {
            std::transform(it->second.begin(), it->second.end(), std::back_inserter(points),
                           [&](const std::pair<int, int>& relPos) {
                return Point(startX + relPos.first, startY + relPos.second);
            });
        }
    }

} // namespace GolEngine

