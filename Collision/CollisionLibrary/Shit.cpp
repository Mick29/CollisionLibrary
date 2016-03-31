#include <vector>
#include "GameObject.h"

int main() {
	std::vector<GameObject*> objects;	//Contains all objects
	for (GameObject* obj1 : objects) {	//Object 1
		for (GameObject* obj1 : objects) {	//Object 2
			//Handle Collision
		}
	}
}