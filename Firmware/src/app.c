#include <stdint.h>
#include "Microchip/usb.h"
#include "Microchip/usb_device.h"
#include "Microchip/usb_device_cdc.h"
#include "buffer.h"
#include "system.h"
#include "led.h"
#include "ssd1306.h"

bool processText(uint8_t* text, const uint8_t count, const bool useLargeFont);
bool processCommand(uint8_t* text, const uint8_t count);

#define LED_TIMEOUT       2000
#define LED_TIMEOUT_NONE  65535
uint16_t LedTimeout = LED_TIMEOUT_NONE;

void main(void) {
    init();
    led_init();

    led_activity_on();

    ssd1306_init(0x3C, 128, 64);
    ssd1306_clearAll();

    ssd1306_writeLargeText("USB OLED");
    ssd1306_writeTextAt("medo64.com", 2, 0);

    led_activity_off();

#if defined(USB_INTERRUPT)
    interruptsEnable();
#endif
    
    USBDeviceInit();
    USBDeviceAttach();

    led_activity_off();
    
    while(true) {
        if (LedTimeout != LED_TIMEOUT_NONE) {
            if (LedTimeout == 0) {
                led_activity_off();
                LedTimeout = LED_TIMEOUT_NONE;
            } else {
                LedTimeout--;
            }
        }

#if defined(USB_POLLING)
        USBDeviceTasks();
#endif

        if (USBGetDeviceState() < CONFIGURED_STATE) { continue; }
        if (USBIsDeviceSuspended()) { continue; }

        CDCTxService();

        // USB receive
        uint8_t readCount = getsUSBUSART(UsbReadBuffer, USB_READ_BUFFER_MAX); //until the buffer is free.
        if (readCount > 0) {
            led_activity_on(); LedTimeout = LED_TIMEOUT;
            for (uint8_t i = 0; i < readCount; i++) {  // copy to buffer
                uint8_t value = UsbReadBuffer[i];
                if (InputBufferCount < INPUT_BUFFER_MAX) {
                    InputBuffer[InputBufferCount] = value;
                    InputBufferCount++;
                } else {
                    InputBufferCorrupted = true;  // no more buffer; darn it
                }
            }
        }

        // USB send
        if ((OutputBufferCount > 0) && USBUSARTIsTxTrfReady()) {  // send output if TX is ready
            led_activity_on(); LedTimeout = LED_TIMEOUT;
            uint8_t writeCount = 0;
            for (uint8_t i = 0; i < USB_WRITE_BUFFER_MAX; i++) {  // copy to output buffer
                if (i < OutputBufferCount) {
                    UsbWriteBuffer[i] = OutputBuffer[i];
                    writeCount++;
                } else {
                    break;
                }
            }
            if (writeCount > 0) {
                putUSBUSART(&UsbWriteBuffer[0], writeCount);  // send data
                OutputBufferCount -= writeCount;  // reduce buffer for the length used
                buffer_copy(&OutputBuffer[0], &OutputBuffer[writeCount], OutputBufferCount);  // move buffer content to beginning
            }
        }

        // Process line
        if (InputBufferCount > 0) {
            uint8_t offset = 0;
            bool potentialCrLf = false;
            for (uint8_t i = 0; i < InputBufferCount; i++) {  // find EOLs
                uint8_t value = InputBuffer[i];

                if (value == 0x0A) {  // start line processing
                    uint8_t firstChar = InputBuffer[offset];

                    if (firstChar == 0x09) {  // HT: command mode
                        uint8_t lineOffset = offset + 1;
                        uint8_t lineCount = i - offset - 1 - (potentialCrLf ? 1 : 0);
                        bool wasOk = processCommand(&InputBuffer[lineOffset], lineCount);
                        if (!wasOk) {
                            if (OutputBufferCount < OUTPUT_BUFFER_MAX) {
                                OutputBuffer[OutputBufferCount] = '!';
                                OutputBufferCount++;
                            }
                        }
                        if (potentialCrLf) {  // CR was seen before LF
                            if (OutputBufferCount < OUTPUT_BUFFER_MAX) {
                                OutputBuffer[OutputBufferCount] = 0x0D;
                                OutputBufferCount++;
                            }
                        }
                        if (OutputBufferCount < OUTPUT_BUFFER_MAX) {
                            OutputBuffer[OutputBufferCount] = 0x0A;
                            OutputBufferCount++;
                        }
                    } else {
                        uint8_t textOffset;
                        uint8_t textCount;
                        bool useLarge;
                        bool moveIfEmpty;
                        switch (firstChar) {
                            case 0x07:  // BEL: clear screen
                                ssd1306_clearAll();
                                textOffset = offset + 1;
                                textCount = i - offset - 1 - (potentialCrLf ? 1 : 0);
                                useLarge = false;
                                moveIfEmpty = false;
                                break;
                            case 0x08:  // BS: move to origin
                                ssd1306_moveTo(0, 0);
                                textOffset = offset + 1;
                                textCount = i - offset - 1 - (potentialCrLf ? 1 : 0);
                                useLarge = false;
                                moveIfEmpty = false;
                                break;
                            case 0x0B:  // VT: reserved
                                textOffset = offset + 1;
                                textCount = i - offset - 1 - (potentialCrLf ? 1 : 0);
                                useLarge = false;
                                moveIfEmpty = false;
                                break;
                            case 0x0C:  // FF: changes to double-size font
                                textOffset = offset + 1;
                                textCount = i - offset - 1 - (potentialCrLf ? 1 : 0);
                                useLarge = true;
                                moveIfEmpty = true;
                                break;
                            default:  // text
                                textOffset = offset;
                                textCount = i - offset - (potentialCrLf ? 1 : 0);
                                useLarge = false;
                                moveIfEmpty = true;
                                break;
                        }

                        bool wasOk = processText(&InputBuffer[textOffset], textCount, useLarge);
                        if ((textCount > 0) || moveIfEmpty) {
                            ssd1306_moveToNextRow();
                            if (useLarge) { ssd1306_moveToNextRow(); }  //extra move for large font
                        }

                        if (!wasOk) {
                            if (OutputBufferCount < OUTPUT_BUFFER_MAX) {
                                OutputBuffer[OutputBufferCount] = '!';
                                OutputBufferCount++;
                            }
                        }

                        if (potentialCrLf) {  // CR was seen before LF
                            if (OutputBufferCount < OUTPUT_BUFFER_MAX) {
                                OutputBuffer[OutputBufferCount] = 0x0D;
                                OutputBufferCount++;
                            }
                        }
                        if (OutputBufferCount < OUTPUT_BUFFER_MAX) {
                            OutputBuffer[OutputBufferCount] = 0x0A;
                            OutputBufferCount++;
                        }
                    }

                    offset = i + 1;  // set the next start

                }

                potentialCrLf = (value == 0x0D);  // checked when LF is matched
            }

            InputBufferCount =- offset;
            buffer_copy(&InputBuffer[0], &InputBuffer[offset], InputBufferCount);  // move unused portion of buffer to the start
        }
    }
}


void __interrupt() SYS_InterruptHigh(void) {
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif
}


bool processText(uint8_t* text, const uint8_t count, const bool useLargeFont) {
    bool ok = true;
    for (uint8_t i = 0; i < count; i++) {
        uint8_t value = *text;
        if (value >= 32) {
            if (useLargeFont) {
                ok = ok && ssd1306_writeLargeCharacter(value);
            } else {
                ok = ok && ssd1306_writeCharacter(value);
            }
        } else {
            ok = false;
        }
        text++;
    }
    return ok;
}

bool processCommand(uint8_t* text, const uint8_t count) {
    return false;
}
