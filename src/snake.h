#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <string>
#include "SDL.h"

// for simplicity I will simply create the class here, but of course
// it would also be possible to do things differently and create a separate header and
// source file and then include them accordinglu
class Obstacles{
  public:
  Obstacles(int grid_width, int grid_height, std::string file_path, bool advanced);
  bool isAdvanced() const {return advanced;}; // get whether we are in advanced mode
  std::vector<SDL_Point> getObstacles() const {return _obstacles;}; //getter for obstacles
  int isObstacle(int x, int y) const {return _obstacles_map[x][y];}; //check for obstacles with getter
  private:
  // function to place obstacles
  void PlaceObstacles();
  // simple function to check if obstacles are valid while being inserted
  bool ValidPoints(int x1,int y1,int x2, int y2);
  // this is used for checking, to have access to an indexable grid
  std::vector<std::vector<int>> _obstacles_map;
  // this is for rendering --> more efficient than iterating through 2D vector
  std::vector<SDL_Point> _obstacles;
  // path to file containing the obstacles
  std::string obstacle_path;

  int grid_width;
  int grid_height;
  bool advanced;

};

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height, std::string file_path, bool advanced)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2),
        _obstacles(grid_width, grid_height, file_path, advanced) {}
  // not sure if this is the best way of doing things overall, but it should technically just work
  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  // create element of type obstacles
  Obstacles obs() const{return _obstacles;};
  void changeSpeed(float delta){speed+=delta;};
 private:
  float speed{0.1f};
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  Obstacles _obstacles;
  bool growing{false};
  int grid_width;
  int grid_height;

  
};

#endif