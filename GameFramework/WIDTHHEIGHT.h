#pragma once

// 맵 전체 크기
static const int LEVEL_WIDTH = 1920;
static const int LEVEL_HEIGHT = 1080;
// 화면 크기
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

// 중력값
static const float GRAVITY = 0.5f;

// 타일 사이즈
static const int TILE_SIZE = 32;
// 타일 맵 크기
static const int MAP_MAX_WIDTH = LEVEL_WIDTH / TILE_SIZE;
static const int MAP_MAX_HEIGHT = LEVEL_HEIGHT / TILE_SIZE;