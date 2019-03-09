/*
GOALS - graydon
done: Player ( = TowerObject) doesnt move and it targets the random wander boi ( = EnemyObject)
done: when right click, set path, wander boi/enemy now paths to it
done: left click spawn another tower
done: other stuff with bullets and fuckery
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Shader.h"
#include "Window.h"
#include "TowerObject.h"
#include "BulletObject.h"
#include "EnemyObject.h"
#include "Graph.h"
#include "Node.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Assignment 3";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
//const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
const glm::vec3 viewport_background_color_g(0.15, 0.17, 0.21);

// Global texture info
GLuint tex[5];


// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		   //  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}


void setthisTexture(GLuint w, char *fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;

	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

}

void setallTexture(void)
{
	//	tex = new GLuint[4];
	glGenTextures(5, tex);
	setthisTexture(tex[0], "Assets/blueship.png");
	setthisTexture(tex[1], "Assets/rock.png");
	setthisTexture(tex[2], "Assets/laser2.png");
	setthisTexture(tex[3], "Assets/saw.png");
	setthisTexture(tex[4], "Assets/orb.png");

	glBindTexture(GL_TEXTURE_2D, tex[0]);
}

// random float between LO and HI
float getRand(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

// gives the given game object a path where the start is defined by the object's location and the end is either found by updating the graph on click, or set manually beforehand
void doPath(Graph &g, EnemyObject* obj, bool shouldUpdate) {
	// get the node that the game object is on top of
	int nodeId = g.getNodeIdFromCoords(obj->getPosition().x, -obj->getPosition().y);

	// set the start of the path to this node
	g.setStart(nodeId);

	// set end through the graph's update method (clicks) if needed
	if (shouldUpdate) {
		g.update();
	}

	g.pathfind();

	obj->setPath(g.getPath());
}

// rand int between hi and lo
int getRand(int LO, int HI) {
	return rand() % (HI - LO + 1);
}

// Main function that builds and runs the game
int main(void) {
	try {
		// Seed for generating random numbers with rand()
		srand((unsigned int)time(0));

		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Create geometry of the square
		int size = CreateSquare();

		// Set up shaders
		Shader shader("Shader/shader.vert", "Shader/shader.frag");

		// Set up the textures
		setallTexture();

		// setup cameraZoom outside now
		float cameraZoom = 0.2f;

		/*------------------ GAME SPECIFIC STUFF --------------------------*/

		//Setup 40x30 graph
		Graph g = Graph(40, 30, GameObject(glm::vec3(0.0f), tex[4], size));

		// random start location for the player
		int p = getRand(0, 1199);
		int q;

		// make sure the npc does not start where the player does (yes they might start beside each other lol)
		do {
			q = getRand(0, 1199);
		} while (p == q);

		Node* start = g.getNode(p);

		Node* npcStart = g.getNode(q);

		// for towers
		std::vector<TowerObject*> towerObjects;
		// Setup the first tower object (for now, we will start with a tower) : position, texture, vertex count, some bool im using for now
		TowerObject* tower = new TowerObject(glm::vec3(start->getX(), start->getY(), 0.0f), tex[0], size);
		towerObjects.push_back(tower);

		// enemy object info
		std::vector<EnemyObject*> enemyObjects;
		// first enemy, wanders
		EnemyObject* target = new EnemyObject(glm::vec3(npcStart->getX(), npcStart->getY(), 0.0f), tex[3], size, 2.3f);
		enemyObjects.push_back(target);

		// bullet info
		std::vector<BulletObject*> bulletObjects;
		// first bullet already exists lel
		bulletObjects.push_back(new BulletObject(tower->getPosition(), tex[2], size));

		// so it will start with a pathfind
		g.setStart(p);
		g.setEnd(1198); // just a default end
		g.pathfind();


		// Run the main loop
		double lastTime = glfwGetTime();
		while (!glfwWindowShouldClose(window.getWindow())) {
			// Clear background
			window.clear(viewport_background_color_g);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// Select proper shader program to use
			shader.enable();

			// Setup camera to focus on (0, 0)
			glm::vec3 cameraTranslatePos(glm::vec3(0.0f));
			float cameraZoom = 0.2f;
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			shader.setUniformMat4("viewMatrix", viewMatrix);

			/*--------------- INPUT --------------------*/

			// left or right click does the same thing here: replans the path
			if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS || glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				double xpos, ypos;
				glfwGetCursorPos(Window::getWindow(), &xpos, &ypos);

				// dont let them click outside the graph area
				if ((xpos > 30 && xpos < 740 && ypos > 25 && ypos < 570)) {
					// add a new tower if it's left click
					// TODO: a tower is much larger area than one node on the graph, will want to do this first, then set all x amount of nodes
					//		 "where the tower is" to obstacles
					if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
						int n = g.selectNode(xpos, ypos);
						Node* node = g.getNode(n);

						// add a new tower and its bullet
						if (!node->isObstacle()) { // dont add a million towers due to one click being registered in multiple frames
							towerObjects.push_back(new TowerObject(glm::vec3(node->getX(), node->getY(), 0.0f), tex[0], size));
							bulletObjects.push_back(new BulletObject(glm::vec3(node->getX(), node->getY(), 0.0f), tex[2], size));
						}
					}


					bool trueOnlyOnce = true;
					// gotta replan path since either new destination set or obstacle added
					for (int i = enemyObjects.size() - 1; i > -1; --i) {
						// the main event
						doPath(g, enemyObjects[i], trueOnlyOnce);

						// pls dont kill me
						if (trueOnlyOnce) {
							trueOnlyOnce = false;
						}
					}
				}
			}

			/*--------------- UPDATE --------------------*/

			//Update and render "all" targets
			for (int i = 0; i < enemyObjects.size(); ++i) {
				// Get the current object
				EnemyObject* enemy = enemyObjects[i];

				// Updates enemy
				enemy->update(deltaTime);

				// the object is doing a path -> see if it can move on to the next node
				if (!enemy->getFinished()) {
					Node* n = enemy->getCurrNode();

					// getting distance between the object and the node it is heading towards
					float d = glm::length(enemy->getPosition() - glm::vec3(n->getX(), n->getY(), 0.0f));

					if (d < 0.05) {
						enemy->nextNode();
					}
				}
				// the path is finished -> it goes back to wander
				else {
					// pick new node to wander to
					do {
						q = getRand(0, 1199);
					} while (g.getEndId() == q); // I'd rather it not go to the same node, not that it really matters most likely

					// end of new path
					g.setEnd(q);

					// actually gives enemy the path
					doPath(g, enemy, false);
				}
			}

			//TODO: target selection, the above code is ok for multiple targets, the below code uses the single target declared at the start

			for (TowerObject* t : towerObjects) {
				t->update(target);
			}

			for (int i = 0; i < bulletObjects.size(); ++i) {
				bulletObjects[i]->update(deltaTime, target);
				
				// if it has collided, replace it with a new bullet, and delete the old one since it is dynamically allocated (new)
				// this is allowed ONLY because bullets and towers are currently 1:1, and each tower is added with its bullet
				if (bulletObjects[i]->hitsTarget(target)) {
					BulletObject* oldBullet = bulletObjects[i];
					delete oldBullet;

					bulletObjects[i] = new BulletObject(towerObjects[i]->getPosition(), tex[2], size);
				}
			}

			/*--------------- RENDER --------------------*/

			for (EnemyObject* e : enemyObjects) {
				e->render(shader);
			}

			for (TowerObject* t : towerObjects) {
				t->render(shader);
			}


			for (BulletObject* b : bulletObjects) {
				b->render(shader);
			}

			//render graph
			//g.render(shader);

			// Update other events like input handling
			glfwPollEvents();

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window.getWindow());
		}
	}
	catch (std::exception &e) {
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
}
