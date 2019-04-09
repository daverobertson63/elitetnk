Elite - The New Kind   Release 1.2
--------------------   -----------

Original Revision date: 22 July 2001
Original Revision date: 09 April 2019

This is based on the release 1.0 of Elite - The New Kind.
For changes since previous releases see below.

I've updated a bunch of things to cope with aLlegro 5 - the original was based on Allegro 4.

For the original info - refer back to github and search for the original version.  This is a work in progress

To run the supplied executable you will need DirectX installed.  Windows 98/ME/2000 and NT 4 (with latest service pack) come with this.
If you are using Windows 95 and you haven't already installed DirectX you will need to do so.
The DirectX runtime can be downloaded from the Microsoft website. (www.microsoft.com/directx).

Two pieces of music are included in Elite - The New Kind.  The Elite Theme and The Blue Danube.
The Elite Theme was composed by Aidan Bell and the Blue Danube was composed by Strauss.

Keys you can use...
Press Y or N on the intro screen.
Press Space on the ship parade screen.

F1  - Launch when docked, Front View when in flight.
F2  - Rear View
F3  - Left View
F4  - Right View when in flight.
	When docked, Buy Equipment for ship.
      Use up and down arrow keys to select item, return/enter key to buy.
F5  - Display Galactic Chart.
F6  - Short Range Chart.
F7  - Show information on selected planet.
F8  - Buy and sell items on the stock market.
      Use up and down arrow keys to select item, right arrow key to buy, left arrow key to sell.
F9  - Commander information.
F10 - Inventory.
F11 - Options screen (Save Game, Load Game, Game Settings, Quit).
      Use up and down arrows keys to select option, return/enter to select.

 A - Fire.
 S - Dive.
 X - Climb.
 < - Roll Left
 > - Roll Right
 / - Slow Down
Space - Speed up.

 C  - Activate docking computer, if fitted.
 D  - De-activate docking computer if switched on.
 E  - Active ECM, if fitted.
 H  - Hyperspace.
 J  - Warp Jump.
 M  - Fire missile.
 T  - Target a missile.
 U  - Un-target missile.
TAB - Detonate energy bomb, if fitted.
CTRL+H - Galactic Hyperspace, if fitted.
ESC - Launch escape capsule, if fitted.
 P  - Pause game.
 R  - Resume game when paused.

On The Chart Screens
--------------------
D - Select a planet and show distance to it.
F - Find planet by name.
O - Return cursor to current planet.
Cursor Keys - Move cross hairs around.


On The Game Settings Screen
---------------------------
From the Options Screen (F11) you can enter the Game Settings Screen. From here you can change
a number of settings that control how the game looks and plays.  Use the cursor keys to select an option
and the Enter/Return key to change it. The options can be saved as default for future games by pressing Enter
while on the Save Settings option (NB this is not necessary if you want to change the settings just for
the current game).  Game settings are held in the newkind.cfg file which should be in the same directory
as the newkind.exe file.





Work in progress
================

- The Allegro 5 lib is quite different from the previous version so still converting things.  Biggest issue so far was having to organise the vertices in counter-clockwise (CCW) direction for 
polygons.  Not sure this is the best way.  

- Colour pallete uses some web colours - this could be better

- Music is not working

- Red cross on charts doesnt work on XOR mode properly

- Lots of other things




Known Problems
==============

- Bits of hidden surfaces on ships sometimes show through.

- Buying more than 255gs of Gold/Platinum doesn't work.
  It didn't in the original Elite either.  Broken as designed.


Have fun!

Original work - Christian Pinder.
<christian@newkind.co.uk>
http://www.newkind.co.uk
