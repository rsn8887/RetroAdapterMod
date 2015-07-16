/* Updated mapping used in all current subroutines:
bit 0	button 1: bottom button (SNES B, PSX Cross, NegCon B, NES B, NeoGeo A, Genesis A, Gamecube A, N64 A. PCE II), 
bit 1	button 2: right button (SNES A, PSX Circle, NegCon A, NES A, Neogeo B, Genesis B, Gamecube X, N64 Yellow Down, PCE I)
bit 2	button 3: left button (SNES Y, PSX Square, NeoGeo C, Genesis C, Gamecube B, N64 B), 
bit 3	button 4: upper button (SNES X, PSX Triangle, NeoGeo D, Genesis X, Gamecube Y, N64 Yellow Up)
bit 4	button 5: Misc 1 (PSX L2, N64 Yellow Left), 
bit 5	button 6: Misc 2 (PSX R2, N64 Yellow Right),
bit 6	button 7: Left Shoulder (PSX L1, Genesis Y, Gamecube L, N64 L, Saturn L)
bit 7	button 8: Right Shoulder (PSX R1 Genesis Z, Gamecube R, N64 R, Saturn R)
bit 0	button 9: Select
bit 1	button 10: Start (PC Engine Play, Genesis Mode)
bit 2	button 11: Misc 3 (PSX L3, N64 Z, Gamecube Z) 
bit 3	button 12: Misc 4 (PSX R3)
*/

/* Mapping required by Android
   buttons in parentheses are non-standard, but seem to be supported
	button 1: Android A (bottom)
	button 2: Android B (right)
	button 3: (Android C)
	button 4: Android X (left)
	button 5: Android Y (top)
	button 6: (Android Z)
	button 7: Android L1 
	button 8: Android R1 
	button 9: Android L2 
	button 10: Android R2
	button 11: (Android Select) (Select)
	button 12: (Android Start) (Start)

	button 15: Android Left Stick Press
	button 16: Android Right Stick Press
*/

#define x 1
#define  2
#define Android 