THIS TOOL IS FOR EDUCATIONAL AND RESEARCH PURPOSES ONLY                                  
MALICIOUS USE OF THIS TOOL MAY CONSTITUTE A CRIME

This is a simple keystroke logger that uses GetKeyBoardState function from WINUSER.h to get information on the status of key presses for the current user. The output is then logged to the users documents folder in a file called log.txt

keylogger [VirusTotal report](https://www.virustotal.com/gui/file/1eb7772801ae55a3cb6bab37c5b0e7990729d5e3f8680e1a2c1a9bb4593ece4b)

launcher [VirusTotal report](https://www.virustotal.com/gui/file/2d31ff1a07ecc0a57bfa1b4dbc369dc0870c90c62f6adf4de9a853cfe23bded5)

## How the keylogger works
- Uses the windows input class to record all 256 keystates
- The program then determines which keypresses are recordable
- Program then writes these keystrokes into a file that shows what the user typed
