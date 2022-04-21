#include "snake.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
// constructor for obstacles class
Obstacles::Obstacles(int grid_width, int grid_height, std::string file_path, bool advanced)
  : grid_width(grid_width),
    grid_height(grid_height),
    obstacle_path(file_path),
    advanced(advanced),
    _obstacles_map(grid_height, std::vector<int>(grid_width,0)){
  if(advanced){
    Obstacles::PlaceObstacles();
  }
}

void Obstacles::PlaceObstacles() {
  int x1,y1,x2,y2;
  std::string line;
  // we have to read the file and place things in the vector
  std::ifstream obstacle_file(obstacle_path);
  if(obstacle_file.is_open()){
    // read obstacles into vector
    while(std::getline(obstacle_file,line))
    {
    // at this point we have to read coordinates into the vector
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      linestream>> x1 >> y1 >> x2 >> y2;
      if(ValidPoints(x1, y1, x2, y2))
      {
        // only with valid points we can do this
        if(x2>x1){
          // let's assume this is enough to write a line
          for(int i=x1;i<x2;i++){
            _obstacles_map[i][y1] = 1;
            SDL_Point obs {static_cast<int>(i),static_cast<int>(y1)};
            _obstacles.push_back(obs);
          }
        }
        else if (y2>y1){
          for (int i=y1;i<y2;i++){
            _obstacles_map[x1][i] = 1;
            SDL_Point obs {static_cast<int>(x1),static_cast<int>(i)};
            _obstacles.push_back(obs);
          }
        }
      }
      else{
        std::cout<< "The point combination ("<< x1 << ","<< y1 << "),(" << x2 << "," << y2 << ") ";
        std::cout<<"is not valid. And will hence be ignored" << "\n" ;
      }
    }
  }
  else{
    std::cout<<"File could not be read or file does not exist, game will be switched to easy mode\n";
    advanced = false;
  } 
}

bool Obstacles::ValidPoints(int x1, int y1, int x2, int y2){
  // could clearly be written in a nicer way, especially in terms of formatting
  if(x1 >=0 && x2>=0 && x1<grid_width && x2<grid_width && y1 >=0 && y2>=0 && y1<grid_height && y2<grid_height)
  {
    // for the sake of simplicity we limit obstacles filling as a going from left to right and top to bottom
    // actually the validity check also requires additional steps --> so this is by no means a "full" solution
    if(x1<=x2 && y1<=y2){return true;}
    else{return false;}
  }
  else{return false;}
}

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
  if(obs().isObstacle(static_cast<int>(head_x),static_cast<int>(head_y))){
    alive = false;
  }

}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}