THIS TOOL IS FOR EDUCATIONAL AND RESEARCH PURPOSES ONLY    

MALICIOUS USE OF THIS TOOL MAY CONSTITUTE A CRIME

This is a simple keystroke logger that uses GetKeyBoardState function from WINUSER.h to get information on the status of key presses for the current user. The output is then logged to the users documents folder in a file called log.txt
![image](https://github.com/user-attachments/assets/671ab4b8-321b-4396-a03c-e1a2bed8f29d)
[VirusTotal report](https://www.virustotal.com/gui/file/4cfb138e4492bd25059b7785845f6f030c4c807bf35fc7e154c6ab79ac9fd149/detection)

## How the keylogger works
- Uses the windows input class to record all 256 keystates
- The program then determines which keypresses are recordable
- Program then writes these keystrokes into a file that shows what the user typed
