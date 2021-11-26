#pragma once

static const int LEVEL_WIDTH = 1280;
static const int LEVEL_HEIGHT = 720;
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

static const int TILE_SIZE = 32;
static const int MAP_MAX_WIDTH = LEVEL_WIDTH / TILE_SIZE; // 1280/32 = 40
static const int MAP_MAX_HEIGHT = LEVEL_HEIGHT / TILE_SIZE; // 720/32 = 22 (int)