Project Members

	Graydon Haskins - 101080221
	Kevin Cheng - 100997440

Completed Elements

	Agents move across the map using A*. Multiple enemies spawn at random locations on the map and move towards a
	random destination on a path planned using A*. After reaching the center, the enemies begin to wander
	around the map.

	Towers shoot at enemies. The program starts with a single tower that will aim and shoot at one of the enemies on
	the map. The towers have a fixed rate of fire. Each shot fired from the tower does 100 damage and Each enemy has
	500 hit points. An enemy will disappear after taking 5 tower shots. Currently, the towers have unlimited range
	and target the last enemy that was added to the enemyObjects vector. After all enemies are killed, the towers
	and bullets stop updating and the program has "finished". Bullets will only collide with their intended target.

	The player can add towers. Towers can be placed anywhere on the map, except where a tower already exists. New
	towers will automatically target an enemy and begin shooting. Enemies will path plan around towers. Currently,
	there is only 1 type of tower, which is placed using left click.

Known Issues

	Invalid map<K, T>key once in a while when holding down left click to place towers; the program occasionally
	encounters an invalid key to the map in the graph.

	Bullets currently do not despawn when there are no targets left. Any bullets that have not hit their target will
	stay frozen and will not despawn.
