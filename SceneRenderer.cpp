#include "SceneRenderer.h"

void SceneRenderer::renderWalls(Raycaster* raycaster, Player* player, Map* map, sf::RenderWindow& window) {
	float segmentWidth = settings.resolution;
	float centerHeight = settings.projectionWindowSize().y / 2 - player->deltaHeight; //The lower the height the higher the horizon

	centerHeight = centerHeight < settings.projectionWindowSize().y ? centerHeight : settings.projectionWindowSize().y;
	centerHeight = centerHeight > 0 ? centerHeight : 0;

	float alphaStep = 255.f / (settings.tileSize * settings.viewingDist);
	std::map<int, sf::VertexArray> wallBatches;

	for (int i = 0; i < settings.numberOfRays(); i++) {
		int texId = raycaster->rays[i].wallNum;

		if (wallBatches.count(texId) == 0)
			wallBatches[texId] = sf::VertexArray(sf::PrimitiveType::Triangles);

		sf::VertexArray& batch = wallBatches[texId];

		float perpLength = raycaster->rays[i].length * std::cosf(player->rotationAngle - raycaster->rays[i].rotationAngle);
		float segmentHeight = settings.tileSize * settings.planeDist() / perpLength;

		int textureId = raycaster->rays[i].wallNum;
		sf::Vector2u wallTextureSize = TextureManager::getTexture(textureId)->getSize();
		int textureX;
		int relativeTexX;
		if (raycaster->rays[i].isHorizontalWallHit) {
			if(raycaster->rays[i].rotationAngle > 0 && raycaster->rays[i].rotationAngle < M_PI)
				relativeTexX = settings.tileSize - (int)raycaster->rays[i].wallHitCoordinates.x % settings.tileSize - 1;
			else 
				relativeTexX = (int)raycaster->rays[i].wallHitCoordinates.x % settings.tileSize;
		}
		else {
			if(raycaster->rays[i].rotationAngle > 3 * M_PI_2 || raycaster->rays[i].rotationAngle < M_PI_2)
				relativeTexX = (int)raycaster->rays[i].wallHitCoordinates.y % settings.tileSize;
			else
				relativeTexX = settings.tileSize - (int)raycaster->rays[i].wallHitCoordinates.y % settings.tileSize - 1;
		}
		textureX = wallTextureSize.x * (float)relativeTexX / settings.tileSize;

		float segmentTop = centerHeight - segmentHeight / 2;
		int alpha = alphaStep * perpLength;

		sf::Vertex vertexArr[6];

		vertexArr[0].position = sf::Vector2f(i * segmentWidth, segmentTop);
		vertexArr[1].position = sf::Vector2f((i+1) * segmentWidth, segmentTop);
		vertexArr[2].position = sf::Vector2f(i * segmentWidth, segmentTop + segmentHeight);
		vertexArr[3].position = sf::Vector2f(i * segmentWidth, segmentTop + segmentHeight);
		vertexArr[4].position = sf::Vector2f((i + 1) * segmentWidth, segmentTop);
		vertexArr[5].position = sf::Vector2f((i+1) * segmentWidth, segmentTop + segmentHeight);

		vertexArr[0].texCoords = sf::Vector2f(textureX, 0);
		vertexArr[1].texCoords = sf::Vector2f(textureX + 1, 0);
		vertexArr[2].texCoords = sf::Vector2f(textureX, wallTextureSize.y);
		vertexArr[3].texCoords = sf::Vector2f(textureX, wallTextureSize.y);
		vertexArr[4].texCoords = sf::Vector2f(textureX + 1, 0);
		vertexArr[5].texCoords = sf::Vector2f(textureX + 1, wallTextureSize.y);

		sf::Color dimColor(255, 255, 255, 255 - std::min(alpha, 255));
		for (int j = 0; j < 6; j++) {
			vertexArr[j].color = dimColor;
			batch.append(vertexArr[j]);
		}
	}
	for (const auto& [texId, vertexArray] : wallBatches) {
		sf::RenderStates states;
		states.texture = TextureManager::getTexture(texId);
		window.draw(vertexArray, states);
	}
}

void SceneRenderer::renderFloor(Player* player, sf::RenderWindow& window) {
	float centerHeight = settings.projectionWindowSize().y / 2 - player->deltaHeight;

	centerHeight = centerHeight < settings.projectionWindowSize().y ? centerHeight : settings.projectionWindowSize().y;
	centerHeight = centerHeight > 0 ? centerHeight : 0;
	
	float invisibleH = settings.tileSize * settings.planeDist() * settings.planeDist() / (2 * settings.tileSize * settings.viewingDist * (settings.planeDist() + settings.viewingDist * settings.tileSize));
	sf::VertexArray floor(sf::PrimitiveType::TriangleStrip, 6);

	floor[0].position = sf::Vector2f(0, centerHeight + invisibleH);
	floor[1].position = sf::Vector2f(settings.projectionWindowSize().x, centerHeight + invisibleH);
	floor[2].position = sf::Vector2f(0, settings.projectionWindowSize().y);
	floor[3].position = sf::Vector2f(0, settings.projectionWindowSize().y);
	floor[4].position = sf::Vector2f(settings.projectionWindowSize().x, centerHeight + invisibleH);
	floor[5].position = sf::Vector2f(settings.projectionWindowSize().x, settings.projectionWindowSize().y);

	floor[0].color = sf::Color::Black;
	floor[1].color = sf::Color::Black;
	floor[2].color = sf::Color(61, 65, 80);
	floor[3].color = sf::Color(61, 65, 80);
	floor[4].color = sf::Color::Black;
	floor[5].color = sf::Color(61, 65, 80);

	sf::RectangleShape rect(sf::Vector2f(settings.projectionWindowSize().x, invisibleH + 1));
	rect.setPosition(sf::Vector2f(0, centerHeight));
	rect.setFillColor(sf::Color::Black);
	window.draw(rect);
	window.draw(floor);
}

void SceneRenderer::renderCeiling(Player* player, sf::RenderWindow& window) {
	float centerHeight = settings.projectionWindowSize().y / 2 - player->deltaHeight;

	centerHeight = centerHeight < settings.projectionWindowSize().y ? centerHeight : settings.projectionWindowSize().y;
	centerHeight = centerHeight > 0 ? centerHeight : 0;

	float invisibleH = settings.tileSize * settings.planeDist() * settings.planeDist() / (2 * settings.tileSize * settings.viewingDist * (settings.planeDist() + settings.viewingDist * settings.tileSize));
	sf::VertexArray ceiling(sf::PrimitiveType::TriangleStrip, 6);

	ceiling[0].position = sf::Vector2f(0, 0);
	ceiling[1].position = sf::Vector2f(settings.projectionWindowSize().x, 0);
	ceiling[2].position = sf::Vector2f(0, centerHeight - invisibleH);
	ceiling[3].position = sf::Vector2f(0, centerHeight - invisibleH);
	ceiling[4].position = sf::Vector2f(settings.projectionWindowSize().x, 0);
	ceiling[5].position = sf::Vector2f(settings.projectionWindowSize().x, centerHeight - invisibleH);

	ceiling[0].color = sf::Color(61, 65, 80);
	ceiling[1].color = sf::Color(61, 65, 80);
	ceiling[2].color = sf::Color::Black;
	ceiling[3].color = sf::Color::Black;
	ceiling[4].color = sf::Color(61, 65, 80);
	ceiling[5].color = sf::Color::Black;

	sf::RectangleShape rect(sf::Vector2f(settings.projectionWindowSize().x, invisibleH + 1));
	rect.setPosition(sf::Vector2f(0, centerHeight - invisibleH));
	rect.setFillColor(sf::Color::Black);
	window.draw(rect);
	window.draw(ceiling);
}

void SceneRenderer::renderCompass(Player* player, sf::RenderWindow& window) {
	float compassRadius = settings.projectionWindowSize().y / (8 * settings.resolution);
	float posX = settings.projectionWindowSize().x - compassRadius * 5 * settings.resolution / 2;
	float posY = settings.projectionWindowSize().y - compassRadius * 5 * settings.resolution / 2;
	float center = compassRadius + settings.resolution;

	sf::RenderTexture compass(sf::Vector2u(center * 2, center * 2));
	compass.clear(sf::Color::Transparent);
	sf::CircleShape frame(compassRadius);
	frame.setOrigin(sf::Vector2f(compassRadius, compassRadius));
	frame.setPosition(sf::Vector2f(center, center));
	frame.setFillColor(sf::Color(19, 23, 28));
	frame.setOutlineThickness(1);
	frame.setOutlineColor(sf::Color(158, 173, 219));

	float glareRadius = compassRadius * 0.8;
	sf::CircleShape glare(glareRadius);
	glare.setOrigin(sf::Vector2f(glareRadius, glareRadius));
	glare.setPosition(sf::Vector2f(center, center));
	glare.setFillColor(sf::Color::Transparent);
	glare.setOutlineThickness(compassRadius * 0.1);
	glare.setOutlineColor(sf::Color(110, 140, 170));

	float faceRadius = compassRadius * 0.7;
	sf::CircleShape face(faceRadius);
	face.setOrigin(sf::Vector2f(faceRadius, faceRadius));
	face.setPosition(sf::Vector2f(center, center));
	face.setFillColor(sf::Color::White);

	float markupRadius = compassRadius * 0.6;
	sf::CircleShape markup(markupRadius);
	markup.setOrigin(sf::Vector2f(markupRadius, markupRadius));
	markup.setPosition(sf::Vector2f(center, center));
	markup.setFillColor(sf::Color::Transparent);
	markup.setOutlineThickness(1);
	markup.setOutlineColor(sf::Color(190, 190, 190));

	sf::VertexArray needle(sf::PrimitiveType::Triangles, 6);
	float needleL = compassRadius * 0.5;
	float needleW = compassRadius * 0.1;
	needle[0].position = sf::Vector2f(0, -needleL);
	needle[1].position = sf::Vector2f(-needleW, 0);
	needle[2].position = -needle[1].position;
	needle[3].position = needle[2].position;
	needle[4].position = needle[1].position;
	needle[5].position = -needle[0].position;

	needle[0].color = sf::Color::Red;
	needle[1].color = sf::Color::Red;
	needle[2].color = sf::Color::Red;
	needle[3].color = sf::Color(120, 120, 120);
	needle[4].color = sf::Color(120, 120, 120);
	needle[5].color = sf::Color(120, 120, 120);

	sf::Transform transform;
	transform.translate(sf::Vector2f(center, center));
	transform.rotate(sf::Angle(sf::radians(3 * M_PI_2 - player->rotationAngle)));
	compass.draw(frame);
	compass.draw(glare);
	compass.draw(face);
	compass.draw(markup);
	compass.draw(needle, transform);
	compass.display();

	sf::Sprite toRender(compass.getTexture());
	toRender.setScale(sf::Vector2f(settings.resolution, settings.resolution));
	toRender.setPosition(sf::Vector2f(posX, posY));
	window.draw(toRender);
}