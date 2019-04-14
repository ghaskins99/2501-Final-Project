#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Shader.h"
#include "FirePartShader.h"
#include "Window.h"
#include "TowerObject.h"
#include "BulletObject.h"
#include "EnemyObject.h"
#include "Graph.h"
#include "Node.h"
#include "UIController.h"
#include "Renderable.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Assignment 3";
const unsigned int window_width_g = 1000;
const unsigned int window_height_g = 600;
//const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
const glm::vec3 viewport_background_color_g(0.15, 0.17, 0.21);

// Global texture info
GLuint tex[6];
std::array<GLuint, 14> fakefont;

GLuint sprite_vbo;
GLuint sprite_ebo;
GLuint partic_vbo;
GLuint partic_ebo;

void drawParticles(GLuint particleprogram, int particlesize, TowerObject* t, float cameraZoom)
{

	// Select proper shader program to use
	glUseProgram(particleprogram);

	//set displacement
	int matrixLocation = glGetUniformLocation(particleprogram, "x");
	int timeLocation = glGetUniformLocation(particleprogram, "time");

	glm::mat4 rot = glm::mat4();
	glm::mat4 world = glm::mat4();

	float k = glfwGetTime();
	//rot = glm::translate(rot, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3 tPos = t->getPosition() * cameraZoom; // scale factor
	tPos.x += 0.6f * cameraZoom * cos(glm::radians(t->getOrientation())); //move out from the tower a bit
	tPos.y += 0.6f * cameraZoom * sin(glm::radians(t->getOrientation()));
	rot = glm::translate(rot, tPos);
	//rot = glm::rotate(rot, -k * 360 / 6.283f, glm::vec3(0, 0, 1));
	rot = glm::rotate(rot, t->getOrientation() - 90.0f, glm::vec3(0, 0, 1));
	
	rot = glm::scale(rot, glm::vec3(0.05, 0.05, 0.05));
	// get ready to draw, load matrix
	glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &rot[0][0]);
	glUniform1f(timeLocation, k);
	glBindTexture(GL_TEXTURE_2D, tex[4]);
	
	// Draw 
	glDrawElements(GL_TRIANGLES, 6*particlesize, GL_UNSIGNED_INT, 0);

}

int CreateParticleArray(void) {

	// Each particle is a square with four vertices and two triangles

	// Number of attributes for vertices and faces
	const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), direction (2), 2D texture coordinates (2), time (1)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
		//  square (two triangles)
		//  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};

	GLfloat particleatt[1000 * vertex_attr];
	float theta, r, tmod;

	for (int i = 0; i < 1000; i++)
	{
		if (i % 4 == 0)
		{
			theta = (6.28*(rand() % 1000) / 1000.0f);//(2*(rand() % 10000) / 10000.0f -1.0f)*0.13f;
			r = 0.1f + 0.25*(rand() % 10000) / 10000.0f;
			tmod = (rand() % 10000) / 10000.0f;
		}
		// position	
		particleatt[i*vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
		particleatt[i*vertex_attr + 1] = vertex[(i % 4) * 7 + 1];

		// velocity
		particleatt[i*vertex_attr + 2] = sin(theta)*r;
		particleatt[i*vertex_attr + 3] = abs(cos(theta)*r);

		// phase
		particleatt[i*vertex_attr + 4] = tmod;

		// texture coordinate
		particleatt[i*vertex_attr + 5] = vertex[(i % 4) * 7 + 5];
		particleatt[i*vertex_attr + 6] = vertex[(i % 4) * 7 + 6];


	}


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint manyface[1000 * 6];

	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 6; j++)
			manyface[i * 6 + j] = face[j] + i * 4;
	}

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &partic_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, partic_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleatt), particleatt, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &partic_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partic_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyface), manyface, GL_STATIC_DRAW);

	// Return number of elements in array buffer
	return sizeof(manyface);

}

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

	// Create buffer for vertices
	glGenBuffers(1, &sprite_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &sprite_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
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
	glGenTextures(6, tex);
	setthisTexture(tex[0], "Assets/blueship.png");
	setthisTexture(tex[1], "Assets/rock.png");
	setthisTexture(tex[2], "Assets/laser2.png");
	setthisTexture(tex[3], "Assets/saw.png");
	setthisTexture(tex[4], "Assets/fire.png");
	setthisTexture(tex[5], "Assets/sawFast.png");

	glBindTexture(GL_TEXTURE_2D, tex[0]);

	//fakefont
	glGenTextures(fakefont.max_size(), fakefont.data());
	for (int i = 0; i < fakefont.max_size(); ++i) {
		std::string str = "Assets/fakefont/" + std::to_string(i) + ".png";
		char* ch = &str[0];
		setthisTexture(fakefont[i], ch);
	}
	glBindTexture(GL_TEXTURE_2D, fakefont[0]);
}

// random float between LO and HI
float getRand(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

// gives the given game object a path where the start is defined by the object's location and the end is either found by updating the graph on click, or set manually beforehand
void doPath(Graph &g, EnemyObject* obj, bool shouldUpdate) {
	// get the node that the game object is on top of
	int nodeId = 0;
	if (obj->getCurrNode() != nullptr) {
		nodeId = g.getNodeIdFromCoords(obj->getCurrNode()->getX(), -obj->getCurrNode()->getY());
	}
	

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
		Shader spriteShader("Shader/shader.vert", "Shader/shader.frag");
	
		CreateParticleArray();
		FirePartShader* fireShader = new FirePartShader();

		// Set up the textures
		setallTexture();

		// setup cameraZoom outside now
		float cameraZoom = 0.2f;

		/*------------------ GAME SPECIFIC STUFF --------------------------*/

		//Setup 40x30 graph
		Graph g = Graph(40, 30, GameObject(glm::vec3(0.0f), tex[4], size));

		//// random start location for the player
		//int p = getRand(0, 1199);
		//int q;

		//// make sure the npc does not start where the player does (yes they might start beside each other lol)
		//do {
		//	q = getRand(0, 1199);
		//} while (p == q);

		//Node* start = g.getNode(p);

		//Node* npcStart = g.getNode(q);

		// for towers
		std::vector<TowerObject*> towerObjects;
		// Setup the first tower object (for now, we will start with a tower) : position, texture, vertex count, texture for bullets
		//TowerObject* tower = new TowerObject(glm::vec3(start->getX(), start->getY(), 0.0f), tex[0], size, tex[2]);
		//towerObjects.push_back(tower);

		// enemy object info
		std::vector<EnemyObject*> enemyObjects;
		std::queue<EnemyObject*> spawnWaitingRoom;

		// different spawn point & path for each enemy
		Node* npcStart = g.getNode(0); // ^ not anymore
		g.setEnd(1199);
		glm::vec3 endNodeLoc(g.getNode(g.getEndId())->getX(), g.getNode(g.getEndId())->getY(), 0.f);
		
		// enemies
		EnemyObject* tempEnemy = nullptr;
		for (int i = 0; i < 12; ++i) {
			if (i > 7) {
				tempEnemy = new EnemyObject(glm::vec3(npcStart->getX(), npcStart->getY(), 0.0f), tex[5], size, 3.3f);
				tempEnemy->setHP(200.f);
			}
			else if (i > 3) {
				tempEnemy = new EnemyObject(glm::vec3(npcStart->getX(), npcStart->getY(), 0.0f), tex[1], size, 1.1f);
				tempEnemy->setHP(1000.f);
			}
			else {
				tempEnemy = new EnemyObject(glm::vec3(npcStart->getX(), npcStart->getY(), 0.0f), tex[3], size, 2.2f);	
			}

			if (i == 0) {
				doPath(g, tempEnemy, false);
				enemyObjects.push_back(tempEnemy);
			}
			else {
				tempEnemy->setPath(g.getPath());
				spawnWaitingRoom.push(tempEnemy);
			}
			
			tempEnemy->setPosition(tempEnemy->getPosition() - glm::vec3(1.f, 0.f, 0.f));
		}

		// uuuu iiiii
		std::vector<GLuint> towerTextures;
		for (int i = 0; i < 3; ++i) {
			towerTextures.push_back(tex[0]);
		}
		UIController ui(fakefont.data(), size, towerTextures);

		Renderable cursor(glm::vec3(20.0f, 20.0f, 0.0f), towerTextures[0], size);
		cursor.setScale(glm::vec3(5.f, 4.f, 0.f));

		bool shopping = false;

		double spawnTime = 1.25;

		double timeSinceLastSpawn = 0;

		// Run the main loop
		double lastTime = glfwGetTime();
		while (!glfwWindowShouldClose(window.getWindow())) {
			// Clear background
			window.clear(viewport_background_color_g);
			glDepthMask(GL_TRUE);

			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// spawning
			if (spawnWaitingRoom.size() > 0) {
				timeSinceLastSpawn += deltaTime;
				if (timeSinceLastSpawn > spawnTime) {
					enemyObjects.insert(enemyObjects.begin(), spawnWaitingRoom.front());
					timeSinceLastSpawn = 0;
					spawnWaitingRoom.pop();

					if (spawnWaitingRoom.size() == 8) spawnTime = 0.75;
					if (spawnWaitingRoom.size() == 4) spawnTime = 2.5;
				}
			}

			/*------------------------------ INPUT ------------------------------*/

			if (shopping) { // maybe place a new tower, or cancel
				double xpos, ypos;
				glfwGetCursorPos(Window::getWindow(), &xpos, &ypos);

				Node* node = nullptr;
				glm::vec3 cursorPos;
				// draw the tower at the right spot
				if ((xpos > 30 && xpos < 740 && ypos > 25 && ypos < 570)) {
					int n = g.selectNode(xpos, ypos);
					node = (n == -1) ? nullptr : g.getNode(n);

					cursorPos = (node == nullptr) ? glm::vec3(20.f) : glm::vec3(node->getX(), node->getY(), 0.f);

					cursor.setPos(cursorPos);
				}

				// if they click either place the tower or dont
				if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT)) {
					cursor.setPos(glm::vec3(20.f, 20.f, 0.f));

					if (node != nullptr && !node->isObstacle()) {
						towerObjects.push_back(new TowerObject(glm::vec3(node->getX(), node->getY(), 0.0f), tex[0], size, tex[2]));

						bool onPath = false;
						for (Node* p : enemyObjects.back()->getPath()) {
							if (p == node) {
								onPath = true;
								break;
							}
						}

						if (onPath) {
							bool trueOnlyOnce = true;
							// gotta replan path since obstacle added
							for (int i = enemyObjects.size() - 1; i > -1; --i) {
								// the main event
								doPath(g, enemyObjects[i], trueOnlyOnce);

								trueOnlyOnce = false; // compiler pls
							}
						}
						else {
							g.update();
						}

						shopping = false;

						ui.changeMoney(-ui.getShopCost(0));
					}
					else {
						float cursor_x_pos = (xpos / (float)(window_width_g / 2)) - 1.0f;	//transforms cursor position to screen coordinates
						float cursor_y_pos = (ypos / (float)(window_height_g / 2)) - 1.0f;

						cursor_x_pos /= 0.2f;
						cursor_y_pos /= 0.2f;

						// "delete button"
						if (glm::length(glm::vec3(cursor_x_pos, -cursor_y_pos, 0.f) - glm::vec3(4.7f, 3.7f, 0.0f)) < .15f) {
							shopping = false;
						}
					}
				}
			} // set to "shopping" if needed
			else if (glfwGetMouseButton(Window::getWindow(), GLFW_MOUSE_BUTTON_LEFT)) {
				double xpos, ypos;
				glfwGetCursorPos(Window::getWindow(), &xpos, &ypos);

				float cx_pos = (xpos / (float)(window_width_g / 2)) - 1.0f;	//transforms cursor position to screen coordinates
				float cy_pos = (ypos / (float)(window_height_g / 2)) - 1.0f;

				cx_pos /= 0.2f;
				cy_pos /= 0.2f;

				glm::vec3 cPos(cx_pos, cy_pos, 0.f);

				std::vector<glm::vec3> ships{ glm::vec3(3.7f, -2.7f, 0.f), glm::vec3(3.7f, .3f, 0.f), glm::vec3(3.7f, 3.3f, 0.f) };

				for (int i = 0; i < 3; ++i) {
					if (glm::length(cPos - ships[i]) < .65f && ui.getCurrentMoney() >= ui.getShopCost(i)) {
						shopping = true;
						cursor.setTex(towerTextures[i]);
						break;
					}
				}
			}

			/*------------------------------ UPDATE ------------------------------*/

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
					enemy->kill = true;
					ui.changeHealth(-1);
				}
			}

			// towers, this also handles update of bullets
			for (TowerObject* t : towerObjects) {
				if (enemyObjects.empty())
					break;
				auto min_e = std::min_element(enemyObjects.begin(), enemyObjects.end(), [endNodeLoc](EnemyObject*& e1, EnemyObject*& e2) {
					return glm::length(e1->getPosition() - endNodeLoc) < glm::length(e2->getPosition() - endNodeLoc);
				});
				t->update(deltaTime, (*min_e));
			}

			// removing elements from the game array while iterating, don't think could do in "for i=x" loop
			for (std::vector<EnemyObject*>::iterator it = enemyObjects.begin(); it != enemyObjects.end();) {
				if ((*it)->kill) {
					if ((*it)->merked) ui.changeMoney((*it)->value);
					delete (*it);
					it = enemyObjects.erase(it);
				}
				else {
					++it;
				}
			}

			/*------------------------------ RENDER ------------------------------*/

			// Select proper shader program to use
			spriteShader.enable();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_ebo);
			spriteShader.attributeBinding();

			// big yeet for UI
			glm::mat4 viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
			spriteShader.setUniformMat4("viewMatrix", viewMatrix);

			glViewport(0, 0, 1000, 600);
			ui.render(spriteShader);

			// best function ever
			glViewport(0, 0, 800, 600);

			// setup camera to zoom/pan for regular game rendering
			glm::vec3 cameraTranslatePos(glm::vec3(0.0f));
			cameraZoom = 0.2f;
			g.setScreenScale(cameraZoom);
			viewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::translate(glm::mat4(1.0f), cameraTranslatePos);
			spriteShader.setUniformMat4("viewMatrix", viewMatrix);
			
			for (EnemyObject* e : enemyObjects) {
				e->render(spriteShader);
			}

			// render towers & bullets
			for (TowerObject* t : towerObjects) {
				t->render(spriteShader);
			}

			if (shopping) { // ok to not reset colorMod because this is the last thing the spriteShader deals with and the first of the loop will reset it
				spriteShader.setUniform4f("colorMod", glm::vec4(0.1f, 0.1f, 0.1f, -.5f));
				cursor.render(spriteShader);
			}

			//g.render(spriteShader);

			// flame throwaaaa
			if (!enemyObjects.empty() && !towerObjects.empty()) {
				//get ready to draw particles
				//glBlendFunc(GL_ONE, GL_ONE);
				glDepthMask(GL_FALSE); // draw particles without writing to depth buffer

				glBindBuffer(GL_ARRAY_BUFFER, partic_vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, partic_ebo);
				fireShader->attributeBinding();

				for (TowerObject* t : towerObjects) {
					float fireDistance = glm::length(t->getPosition() - enemyObjects.back()->getPosition());
					if (fireDistance < 0.5f / cameraZoom) {
						drawParticles(fireShader->getShaderID(), 1000, t, cameraZoom);
					}
				}
			}

			// Update other events like input handling
			glfwPollEvents();

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window.getWindow());
		} // while loop

		// clean up memory
		for (EnemyObject* e : enemyObjects) {
			delete e;
		}
		for (TowerObject* t : towerObjects) {
			delete t;
		}
	}
	catch (std::exception &e) {
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	return 0;
}
