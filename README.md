# Smegtris
A simple Tetris clone for MS-DOS
(C)1994, Cheese Freak Software

Smegtris was created in 1994 while I was experimenting with Microsoft C 7.0, which I was using on my university work placement at BT Laboratories. I didn't own a PC at the time, so most of the code was written on an Atari ST at home, then transferred to floppy and compiled/debugged on my office 386 during my lunch hours.

Perhaps surprisingly, this resulted in a 'not half bad' version of the game, loosely based on the Gameboy version which I owned and loved.

The resulting game and source were stored on floppies, which over the years somehow got lost. I recovered a version of the source from an old floppy sometime in the mid-2000s, but the executable was long gone so I had no way to actually play it.

Fast forward to 2022(!) and I stumbled across the source again on my hard drive. Thinking it over, I wondered if I could reassemble my original toolset and actually compile a working version. 

It didn't take long to find all the software I needed (MS-DOS 6.22, Microsoft Windows for Workgroups 3.11 and a copy of Microsoft C 7.0) and get it all installed on a VM. Transferring the files over to a DOS-based VM proved a little more challenging than I expected, but I eventually managed it using a floppy disk image file and a copy of DOSBOX.

With everything in place, I then realised that I had no idea how to compile - the MAKEFILE on the disk didn't seem to be compatible with Microsoft C 7.0, which leads me to believe I was using something third party that I have long forgotten. After a few failed attempts I eventually took things back to basics and interpreted the MAKEFILE into a simple batch file to do the compiling and linking for me - not very elegant, but.... it worked!

So, for the first time in a really long time, Smegtris is actually playable - I recommend running it in DOSBOX, where it runs very nicely indeed. :-)

It's a real kick to see it running again after all these years, so I figured it would be nice to get all of this onto Github so it didn't get lost again.

Have fun!


@Blerkotron, 01/06/2022
