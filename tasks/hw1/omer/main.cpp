#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Position {
    int x{0};
    int y{0};
};

enum class Direction {North, East , South, West};

Direction turnLeft(Direction d) {
    switch (d) {
        case Direction::North: return Direction::West;
        case Direction::West: return Direction::South;
        case Direction::South: return Direction::East;
        case Direction::East: return Direction::North; 
    }
    return Direction::North;
}

Direction turnRight(Direction d) {
    switch (d) {
        case Direction::North: return Direction::East;
        case Direction::East: return Direction::South;
        case Direction::South: return Direction::West;
        case Direction::West: return Direction::North;
    }
    return Direction::North;
}

Position moveForward(Position p, Direction d) {
    switch (d) {
        case Direction::North: p.y -= 1; break;
        case Direction::South: p.y +=1; break;
        case Direction::East: p.x +=1; break;
        case Direction::West: p.x -= 1; break;
    }
    return p;
}

class Grid {
public:
    bool load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Harita acilamadi: " << path << "\n";
            return false;
        }

        cells_.clear();
        std::string line;
        while (std::getline(file, line)) {
            cells_.push_back(line);
        }
        bool foundS =false;
        bool foundG = false;

        for (int y=0; y < static_cast<int>(cells_.size()); ++y){
            for (int x = 0; x < static_cast<int>(cells_[y].size()); ++x){
                if (cells_[y][x] == 'S') {
                    start_ = {x, y};
                    foundS = true;
                }else if (cells_[y][x] == 'G') {
                    goal_ = {x, y};
                    foundG = true;
                }
            }
        }
        if (!foundS || !foundG) {
            std::cerr << "Haritada 's' veya 'g' bulunamadi! \n"; 
            return false;
        }
        return true;
    }

    bool isFree(Position p) const {
        if (p.y < 0 || p.y >= static_cast<int>(cells_.size())) return false;
        if (p.x < 0 || p.x >= static_cast<int>(cells_[p.y].size())) return false;
        if (cells_[p.y][p.x] == '#') return false;
        return true;
    }

    Position start() const { return start_; }
    Position goal()  const { return goal_; }

    void print (Position robotPos) const {
        for (int y = 0; y < static_cast<int>(cells_.size()); ++y) {
            for (int x = 0; x < static_cast<int>(cells_[y].size()); ++x) {
                if (x == robotPos.x && y == robotPos.y) {
                    std::cout << 'R'; // Robotun bulunduğu konum
                } else {
                    std::cout << cells_[y][x];
                }
            }
            std::cout << "\n";
        }
    }
private:
    std::vector<std::string> cells_;
    Position start_{0,0};
    Position goal_{0,0};
};

int main () {
    Grid grid;

    if (!grid.load("map1.txt")) {
        std::cerr << "Harita yuklenirken hata olustu.\n";
        return 1;
    }

    std::cout << "Harita yuklendi!\n";
    std::cout << "Baslangic (s): (" <<grid.start().x << ", " << grid.start().y << ")\n";
    std::cout << "Hedef (G): (" << grid.goal().x << ", " << grid.goal().y << ")\n\n";

    std::cout << "--- Harita ve Robot Konumu (S noktasinda 'R' gorunmeli) ---\n";
    grid.print(grid.start());

    return 0;
}