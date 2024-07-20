THIS TOOL IS FOR EDUCATIONAL AND RESEARCH PURPOSES ONLY                                  
MALICIOUS USE OF THIS TOOL MAY CONSTITUTE A CRIME

This is a simple keystroke logger that uses GetKeyBoardState function from WINUSER.h to get information on the status of key presses for the current user. The output is then logged to the users documents folder in a file called log.txt

##How the keylogger works
- Uses the windows input class to record all 256 keystates
- The program then determines which keypresses are recordable
- Program then writes these keystrokes into a file that (hopefully) shows what the user typed

##How the launcher works
1. The program reads the main resource file "IDR_RCDATA" then it's contents "RCDATA.bin" then it's file header file "resource.h"
   - Note you must include the full path from resource.h everything else is automatically included by the compiler
2. The program then proceeds to create a folder and file depending on whether or not it has user or admin permission
  - User: Creates `C:\Users\<username>\AppData\Roaming\<HKCU_folder_name>` and `C:\Users\<username>\AppData\Roaming\<HKCU_folder_name>\<malware>`
  - Admin: Created C:\Windows\<malware>
3. The program then decodes the data in the resource file and writes it to the the malware file created in step 2
4. The program then establishes persistence in HKCU or HKLM depending on whether it is run as admin or user
  - User: `HKey_Current_User\Software\Microsoft\Windows\CurrentVersion\Run`
          `<HKCU_subkey_name>    REG_SZ    C:\Users\<username>\AppData\Roaming\<HKCU_folder_name>\<malware>`
  - Admin: `HKey_Local_Machine\Software\Microsoft\Windows\CurrentVersion\Run`
          `<HKLM_subkey_name>    REG_SZ    C:\Windows\<malware>`
5. The program finishes by running the dropped file
