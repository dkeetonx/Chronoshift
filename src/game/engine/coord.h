/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling the different distance and position representations in the engine.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef COORD_H
#define COORD_H

#include "always.h"
#include "facing.h"
#include "gametypes.h"

#define CELL_PIXELS 24
#define CELL_LEPTONS 256
#define CELL_MIN 0
#define CELL_MAX 128
#define CELL_MAX_X 128
#define COORD_MIN 0
#define COORD_MAX (CELL_LEPTONS * CELL_MAX)
#define MAP_MAX_WIDTH 128
#define MAP_MAX_HEIGHT 128
#define MAPTD_MAX_WIDTH 64
#define MAPTD_MAX_HEIGHT 64
#define MAP_MAX_AREA MAP_MAX_WIDTH * MAP_MAX_HEIGHT

extern const coord_t AdjacentCoord[FACING_COUNT];
extern const cell_t AdjacentCell[FACING_COUNT];

inline uint16_t Coord_Lepton_X(coord_t coord)
{
    return coord & 0x0000FFFF;
}

inline uint16_t Coord_Lepton_Y(coord_t coord)
{
    return (coord & 0xFFFF0000) >> 16;
}

inline int16_t Coord_Sub_Cell_X(uint32_t coord)
{
    return coord & 0xFF;
}

inline int16_t Coord_Sub_Cell_Y(uint32_t coord)
{
    return (coord >> 16) & 0xFF;
}

inline coord_t Coord_From_Lepton_XY(int16_t x, int16_t y)
{
    return (x & 0xFFFF) | (y << 16);
}

inline coord_t Coord_Add(coord_t coord1, coord_t coord2)
{
    uint16_t lx = Coord_Lepton_X(coord1) + Coord_Lepton_X(coord2);
    uint16_t ly = Coord_Lepton_Y(coord1) + Coord_Lepton_Y(coord2);

    return Coord_From_Lepton_XY(lx, ly);
}

inline coord_t Coord_Centered(coord_t coord)
{
    return coord & 0xFF80FF80;
}

inline coord_t Coord_Top_Left(coord_t coord)
{
    return coord & 0xFF00FF00;
}

inline coord_t Coord_Get_Adjacent(coord_t coord, FacingType facing)
{
    return Coord_Centered(Coord_Add(coord, AdjacentCoord[(unsigned)facing % FACING_COUNT]));
}

inline uint8_t Coord_Cell_X(coord_t coord)
{
    return (coord & 0x00007F00) >> 8;
}

inline uint8_t Coord_Cell_Y(coord_t coord)
{
    return (coord & 0x7F000000) >> 24;
}

inline BOOL Coord_Is_Negative(uint32_t coord)
{
    return (coord & 0x80008000) != 0;
}

inline cell_t Cell_From_XY(uint8_t x, uint8_t y)
{
    return ((y * MAP_MAX_WIDTH) + x);
}

inline cell_t Coord_To_Cell(coord_t coord)
{
    return Cell_From_XY(Coord_Cell_X(coord), Coord_Cell_Y(coord));
}

inline uint8_t Cell_Get_X(cell_t cellnum)
{
    return (unsigned)cellnum % MAP_MAX_WIDTH;
}

inline uint8_t Cell_Get_Y(cell_t cellnum)
{
    return (unsigned)cellnum / MAP_MAX_HEIGHT;
}

inline coord_t Cell_To_Coord(cell_t cellnum)
{
    return 0x00800080 | (Cell_Get_X(cellnum) << 8) | (Cell_Get_Y(cellnum) << 24);
}

inline int16_t Coord_Cell_To_Lepton(int cellcoord)
{
    return cellcoord * 256;
}

inline int16_t Pixel_To_Lepton(int pixel)
{
    return ((unsigned(pixel) * 256) + 12) / 24;
}

inline int Lepton_To_Pixel(int16_t lepton)
{
    return (24 * lepton + 128) / 256;
}

inline int16_t Lepton_Round_To_Pixel(int16_t lepton)
{
    return Pixel_To_Lepton(Lepton_To_Pixel(lepton));
}

inline coord_t Coord_From_Pixel_XY(int x, int y)
{
    return Coord_From_Lepton_XY(Pixel_To_Lepton(x), Pixel_To_Lepton(y));
}

inline uint8_t Lepton_To_Cell_Coord(int16_t lepton)
{
    return ((uint16_t)lepton >> 8) & 0xFF;
}

inline uint8_t Lepton_Sub_Cell(int16_t lepton)
{
    return lepton & 0xFF;
}

int Distance(uint32_t coord1, uint32_t coord2);
void Move_Point(int16_t &x, int16_t &y, DirType dir, uint16_t distance);
uint32_t Coord_Move(uint32_t coord, DirType dir, uint16_t distance);
BOOL __cdecl Confine_Rect(int &x_pos, int &y_pos, int x, int y, int w, int h);

int Distance(coord_t coord1, coord_t coord2);

#endif // COORD_H
