#include <iostream>

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

int main () {
    Direction test = turnRight(Direction::West);
    if (test == Direction::North){
        std::cout << "Test basarili: turnRİght(west) -> NOrth" <<std::endl; 
    }else{
        std::cout << "Test basarisiz yanlis yon donusu." <<std::endl;
    }
    
    return 0;
}