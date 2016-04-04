#include <vector>
#include "GameObject.h"
#include "Vector.h"

int main() {
	std::vector<Vec3*> pts1;	//Contains all points of shape one
	std::vector<Vec3*> pts2;	//Contains all points of shape two
	std::vector<Vec3*> result;	//Contains all possible points for collision
	for (GameObject* Vec3 : pts1) {	//Points 1
		for (GameObject* Vec3 : pts2) {	//Points 2
			result.push_back(pts1 - pts2);
		}
	}
}