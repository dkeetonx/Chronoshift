/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Callback handler to do per loop callback processing.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "callback.h"

void Call_Back()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*callback)() = reinterpret_cast<void (*)()>(0x004A765C);
    callback();
#endif
}
