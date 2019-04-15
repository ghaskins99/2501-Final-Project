# Final Project Writeup COMP2804

#### Project Members

	 Graydon Haskins - 101080221
	 Kevin Cheng - 100997440

## 0. Description of the game
  This is a (partially completed) tower defense game with a space-y theme.

## 1. Explanation of how to play the game
  The game will start with no towers spawned, at the most-comfortably zoomed out level and centered on the entire map. 
  Players can zoom in or out using the Up/Down keys (↑/↓) and pan the camera with WASD. Click on a tower in the side UI menu to select it for placement, then click anywhere on the map you wish to buy the tower and, if you have enough money, the tower will place if the space is not already occupied. You can cancel a tower purchase by clicking on the 'delete' trashcan button. Pressing X for the first time will spawn the "AMD-Bot 9000", our Autonomous Mobile Defender. Try to defeat the enemies before they reach the opposite corner of the map!
  
## 2. List of Technical Requirements
1. *Written in C++ using OpenGL to render; readable code with no fatal bugs.*

    This is indeed written in C++ using OpenGL to render. No fatal bugs are known of in the release.
2. *Path planning used to determine enemy movement.*
  
    Enemies path plan using Djikstra's algo w/ A* heuristic using manhattan distance to next node. Path is replanned upon creation of an obstacle that is in the way.
3. *Finite state machine AI for tower behaviour. Visible differences in different states. (E.g., fire on cooldown.)*
    
    Regular towers have a visible cooldown on firing. Flamethrower towers are "inactive" when no enemies are nearby, but will activate and track enemies if any are within range. The autonomous defender alters between a chase state and a stationary firing state.
4. *Finite state machine controlling overall game (e.g., loading screen, pause, break from action between levels).*
  
    Unfortunately did not get this entirely working as no loading screen or pause screen exists. The sole application of an FSM in the main game logic controls whether or not the person is "shopping" and decisions based on mouse input change depending on the state.
5. *At least two instances of particle systems used.*
    
    One instance of this is present, used for the flamethrower's fire effect. It will show for as many towers are currently "throwing flame", but this is the only effect.
6. *All movement handled through transformations.*
  
    ✓ 
7. *At least one instance of hierarchical transformations for a compound object*
  
    N/A - no compund objects :(
8. *A gameworld larger than the screen, with ability to scroll around and see different parts.* 
  
    This is completed, however the world view starts zoomed out so once a player zooms in then the effect is apparent. Scrolling ability is always unlocked.

## 3. List of Gameplay Requirements
1. *At least three different enemy types.*
    
    ✓ - "Normal" enemy, Fast enemy with less health, Slow enemy with more health to compensate.
2. *At least three different tower types*
    
    ✓ - "Normal" tower, Flamethrower tower, AOE slowing debuff tower.
3. *An autonomous mobile defender.*
    
    The player can spawn an AMD that can chase after and shoot at enemies (once in range).
4. *Currency to buy new towers.*
  
    ✓ - Player starts with some amount and can use it to purchase new towers. Upon defeating an enemy the player recieves some cash.
5. *At least three levels with different layouts.*
  
    X - Only one level with a random layout
6. *A HUD showing available towers, current cash, health, other game elements of interest.*
  
    ✓ - Exists, visible at right side of screen shows: cash, health, available towers for purchase and purchase options.
7. *At least one interesting powerup with a timer.*
  
    X - No **time** for a **timed** powerup (haha)

## 4. Discussion of gameplay or technical features that go beyond the minimum

## 5. Design notes - list of entities in the game and how they behave; comments on and explanations of design decisions you made
Enemies:
- Normal enemy walks regularly
- Fast is fast, slow is slow and has more health

Design decision comments:

These enemies are regularly seen in tower defense games so they were a good base to start for more complex enemies later. However these enemies are currently all we have.

Towers:
- Default Tower - The default tower shoots 1 enemy at a time. It has a 2 second delay between shots.
- Flamethrower Tower - The flamethrower tower damages all enemies in a small cone. It does significantly less damage than the default tower but has a faster "fire rate" because it does AOE damage. It also does double damage at very close range. When no enemies are nearby, the tower will idle. If enemies are nearby, it will track the nearest enemy.
- AOE Slow Tower - The AOE slow tower reduces the speed of all enemies within range by 50%. Multiple slow towers can affect an enemy. The slow reduction is multiplicative (i.e. 50% -> 25% -> 12.5% etc). Enemies are no longer affected when they leave the tower's range.
- Autonomous Defender - The autonomous defender uses a chase behaviour to go after enemies. As soon as its target is within range, the defender stops moving and begins to shoot at the target. If the target leaves its range the defender resumes its chase behaviour. If the target dies, it chooses a new target. The defender fires significantly faster than the default tower, but does significantly less damage because it is mobile.

## 6. Implementation notes
