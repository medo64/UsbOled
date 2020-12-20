#include <stdbool.h>
#include <stdint.h>
#include "graph.h"
#include "ssd1306.h"
#include "buffer.h"

uint8_t GraphData[64];
uint8_t GraphStart = 128;  // there is no data so start is at the very end

void graph_push(const uint8_t value) {
    for (uint8_t i = 0; i < 63; i++) {
        GraphData[i] = (GraphData[i] << 4) | (GraphData[i + 1] & 0x0F);  // combine low nibble of current one with high nibble of the next one
    }
    GraphData[63] = (GraphData[63] << 4) | (value >> 4);  // for last byte, use low nibble and combine it with 4-bit data (don't store 8 bits to save memory))
    if (GraphStart > 0) { GraphStart--; }  // just track how many valid data points are there
}

bool graph_draw(uint8_t width, bool isLarge) {
    uint8_t startOffset = 128 - (width << 3);  // calculate where to start based on required character count
    uint8_t data[16];
    for (uint8_t i = startOffset; i < 128; i++) {
        uint8_t columnOffset = i & 0x07;  // figure out which column we're at
        uint8_t dataT, dataB;
        if (i >= GraphStart) {
            uint8_t dataOffset = i >> 1;
            uint8_t dataShift = ((i & 0x01) == 0) ? 4 : 0;
            uint8_t value = (GraphData[dataOffset] >> dataShift) & 0x0F;
            if (!isLarge) { value >>= 1; }  // divide by 2 as we're only dealing with 8 pixels
            uint8_t tip = value & 0x07;
            uint8_t opposite = 7 - tip;
            uint8_t stack = 0xFF;
            stack >>= opposite;
            stack <<= opposite;
            if (!isLarge || (value > 7)) {
                dataT = stack;
                dataB = 0xFF;
            } else {
                dataT = 0x00;
                dataB = stack;
            }
        } else {  // no graph data yet
            dataT = 0x00;
            dataB = 0x00;
        }
        data[columnOffset] = dataT;
        data[columnOffset + 8] = dataB;
        if (columnOffset == 7) {  // every 8 bytes, draw character
            if (!ssd1306_drawCharacter(data, 16, isLarge)) {
                return false;  // exit early if drawing fails (e.g. we're at the right screen edge)
            }
        }
    }
    return true;
}
