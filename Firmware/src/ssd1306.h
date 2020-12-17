#ifndef SSD1306_H
#define	SSD1306_H

#include <stdint.h>

/** Initializes Display. */
void ssd1306_init(uint8_t address, uint8_t width, uint8_t height);


/** Turns display off. */
void ssd1306_displayOff();

/** Turns display on. */
void ssd1306_displayOn();

/** Sets contrast value. */
void ssd1306_setContrast(uint8_t value);


/** Sets column and row to be used (at 8x8 resolution). */
void ssd1306_moveTo(const uint8_t row, const uint8_t column);


/** Clear display content. */
void ssd1306_clearAll();

/** Clear display content of a single row. */
void ssd1306_clearRow(uint8_t y);


/** Writes 8x8 character at the current position */
void ssd1306_writeCharacter(const uint8_t value);

/** Writes 8x8 text at the current position */
void ssd1306_writeText(const uint8_t* value);

/** Writes 8x8 text at the current position */
void ssd1306_writeTextAt(const uint8_t* value, const uint8_t row, const uint8_t column);


/** Writes 8x16 character at the current position */
void ssd1306_writeLargeCharacter(const uint8_t value);

/** Writes 8x16 text at the current position */
void ssd1306_writeLargeText(const uint8_t* value);

/** Writes 8x16 text at the current position */
void ssd1306_writeLargeTextAt(const uint8_t* value, const uint8_t row, const uint8_t column);

#endif