#pragma once

#include <regex>

constexpr auto PLUGIN_NAME = "Pushback Florian";
constexpr auto PLUGIN_VERSION = "1.1.1";
constexpr auto PLUGIN_AUTHOR = "Nick Mueller";
constexpr auto PLUGIN_LICENSE = "(c) 2021-2022, MIT License";

constexpr auto SETTINGS_DELIMITER = '|';

const int TAG_ITEM_FP_DIRECTION = 1;
const int TAG_ITEM_FP_LINE = 2;

const int TAG_FUNC_DIRECTION_MENU = 100;
const int TAG_FUNC_DIRECTION_NORTH = 101;
const int TAG_FUNC_DIRECTION_SOUTH = 102;
const int TAG_FUNC_DIRECTION_EAST = 103;
const int TAG_FUNC_DIRECTION_WEST = 104;
const int TAG_FUNC_DIRECTION_CLEAR = 105;
const int TAG_FUNC_LINE_MENU = 110;
const int TAG_FUNC_LINE_BLUE = 111;
const int TAG_FUNC_LINE_ORANGE = 112;
const int TAG_FUNC_LINE_CLEAR = 113;

const COLORREF TAG_COLOR_LINE_BLUE = 12671488;
const COLORREF TAG_COLOR_LINE_ORANGE = 24252;

constexpr auto SCRATCH_PAD_DELIMITER = '/';
constexpr auto SCRATCH_PAD_PREFIX = "PBF";
constexpr auto SCRATCH_PAD_DIRECTION = "DIR";
constexpr auto SCRATCH_PAD_LINE = "LINE";
constexpr auto SCRATCH_PAD_CLEAR = "CLEAR";