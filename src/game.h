#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

// #include <array>
#include <vector>
#include <string>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::string path_to_file, bool advanced);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  // path to file
  std::string file_path;
  // apparently making a pointer doesn't really make sense.. Not sure if this is the 
  // best, but I guess we will see as we go
  std::vector<std::vector<int>> _obstacles;
  // std::unique_ptr<std::vector<std::vector<int>>> _obstacles;
  int grid_width;
  int grid_height;
  bool advanced;
  
  int score{0};

  void PlaceObstacles();
  void PlaceFood();
  void Update();
  bool ValidPoints(int x1,int y1,int x2, int y2);
};

#endif