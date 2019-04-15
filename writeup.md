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
There might be some that exist but I'm not sure and don't have enough time to verify.

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
#### Towers
All towers inherit from GameObject. The flamethrower tower inherits from TowerObject but the slow tower does not. This is because the slow tower has no use for the firing behaviour in TowerObject.

While the flamethrower tower does not "shoot" projectiles, it maintains a vector of BulletObjects that are used for hit detection. The scale and position of each bullet is manipulated directly by FlameTower.

The autonomous defender inherits from TowerObject because when it target is within range, it stays still and shoots. It reuses a lot of the code in EnemyObject, but does not inherit from EnemyObject. This design decision was made for the sake of clarity.
#### Enemies
All enemies are variations of the base EnemyObject which inherits from GameObject. GameObject in turn inherits from Renderable since every item that needs to be viewed (not just game objects) shares some properties that can be grouped under a "Renderable" type.

Enemy differences include changing the texture and speed/health of individual EnemyObjects. This design decision was done for the sake of time.

#### Other
Certain towers will aim for the enemy that is closest to the goal using std::minElement with a lambda function to compute each enemy's distance from the end.

All UIElements are Renderable, and some UIElements are specialized into subclasses such as AmountView and Shop. This saves a lot of reuse of code for common properties/functions, and allows the objects to delegate tasks such as rendering.

We did not add a real font using text, but a makeshift font is used for numbers and some symbols which is used in AmountView and personally I think it worksk quite nicely.

## 7. Known bugs and limitations
The flamethrower tower uses circular hit detection with multiple circles that very roughly approximates a cone.

None of the towers have indicators to show their range.

There is no minimum movement speed, so if a player buys enough slow towers, the enemies basically stop moving.

Other missing features mentioned in this readme

## 8. Comments on individual contributions
Early work (the first assignment related to the project) worked on together.

Particle system, UI interaction/HUD and enemy types, along with general game functions (associated with these listed elements or otherwise [ex. pathfinding decision behavour - when to replan the path or not, camera pan/zoom]), writeup - Graydon

Multiple tower types, autonomous mobile defender, other general functions, some writeup (note: written by Graydon, I am not sure if I have listed everything but I hope I have) - Kevin

(this is as much information as can be given in time)

## 9. Project postmortem
Likes: coding challenges, general structure of program and technical decisions. Dislikes: some gameplay elements (not neccessarily disliked but I left them to the end and did not finish them so maybe that says something). Proud of the UI as I think it is a decent system.

No real organization system for time, group members deciced on tasks from requirements to work on and let each other know what they are doing / how it is coming along. Somewhat effective, more communication would have been nice but that is neither member's fault. 

Differently: work on the project earlier (I admit I would not do this since the deadline seemed "so far away")
