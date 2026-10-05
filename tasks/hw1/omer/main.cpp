#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

struct Position {
    int x{0};
    int y{0};

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
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

enum class Action {Forward, TurnLeft, TurnRight};

class Robot {
public:
        Robot(std::shared_ptr<const Grid> grid, double battery) 
            : grid_(std::move(grid)), battery_(battery){
            if (grid_)  {
                pos_ = grid_->start();
            }
        }

        virtual ~Robot() = default;

        void tick() {
            if (!grid_ || battery_ <= 0.0 || atGoal()) return;

            Action act = decide();

            switch (act) {
                case Action::Forward: {
                    battery_ -= 1.0;
                    Position nextPos = moveForward(pos_, dir_);
                    if(grid_->isFree(nextPos)) {
                        pos_ = nextPos;
                        steps_++;
                    }
                    break;
                }
                case Action::TurnLeft: {
                    battery_ -= 0.5;
                    dir_ = turnLeft(dir_);
                    break;
                }
                case Action::TurnRight: {
                    battery_ -= 0.5;
                    dir_ = turnRight(dir_);
                    break;
                }
            }

        }

        bool atGoal() const {
            return grid_ && (pos_ == grid_->goal());
        }

        double battery() const { return battery_; }
        int steps() const { return steps_;}
        Position position() const { return pos_; }

protected:
    virtual Action decide() = 0;       // Saf sanal fonksiyon: Türetilen robotlar dolduracak

    std::shared_ptr<const Grid> grid_; // Salt okunur paylaşılan harita
    Position  pos_{0, 0};
    Direction dir_{Direction::East};

private:
    double battery_{200.0};
    int    steps_{0};
};

class WallFollowerRobot : public Robot {
public: 
    WallFollowerRobot(std::shared_ptr<const Grid> grid, double battery)
        : Robot(std::move(grid), battery) {}

protected:
    Action decide() override {
        if (justTurnedRight_) {
            justTurnedRight_ = false;
            return Action::Forward;
        }

        Direction rightDir = turnRight(dir_);
        Position rightPos = moveForward(pos_, rightDir);

        if (grid_->isFree(rightPos)) {
            justTurnedRight_ = true;
            return Action::TurnRight;
        }

        Position frontPos = moveForward(pos_, dir_);
        if (grid_->isFree(frontPos)) {
            return Action::Forward;
        }

        return Action::TurnLeft;
    }

private:
    bool justTurnedRight_{false};
};

int main () {
    auto grid = std::make_shared<Grid>();

    if (!grid->load("map1.txt")){
        std::cerr << "Harita yuklenemedi\n";
        return 1;
    }

    std::unique_ptr<Robot> robot = std::make_unique<WallFollowerRobot>(grid, 200.0);

    for (int t = 0; t < 500; ++t) {
        if (robot->atGoal() || robot ->battery() <= 0.0) {
            break;
        }
        robot -> tick();
    }

    if (robot->atGoal()) {
        std::cout << "Hedefe ulasildi!\n";
    }else {
        std::cout << "Hedefe ulasilamadi!\n";
    }

    std::cout << "Toplam atilan adim: " << robot->steps() << "\n";
    std::cout << "Kalan batarya: " << robot->battery() << "\n";

    return 0;
}