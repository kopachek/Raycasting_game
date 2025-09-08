#include "Raycaster.h"

void Raycaster::generateRays(Player* player, Map* map) {
	rays.clear();
	double fovRadians = (settings.fov / 180) * M_PI;
	double deltaRadians = fovRadians / (settings.numberOfRays() - 1);
	for (int i = 0; i < settings.numberOfRays(); i++) {
		double angle = player->rotationAngle - fovRadians / 2 + i * deltaRadians;
		Ray ray(player->coordinates, angle);
		ray.cast(map);
		rays.push_back(ray);
	}
}

void Raycaster::renderRays(sf::RenderWindow& window, Map* map) {
	for (int i = 0; i < rays.size(); i++) {
		rays[i].render(window, map);
	}
}