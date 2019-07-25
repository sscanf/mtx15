
I developed this project in the early 2000s.

It is a video booth system, each booth carries a firmware to control the button panel (changing channels, volume up / down, etc.) and also controls the wallet and card reading.

An OSD chip is responsible for inserting information such as the channel number into the video.

All booths are controlled by a Windows application on a centralized PC. It manages all the button events of each booth, orders the booths the channel switching, volume up/down, finish a session, etc.


The booths firmware is written in C for MC68HC705C8/A microcontroller.
The Windows App is written in Microsoft C (MFC).

