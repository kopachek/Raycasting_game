#include "StatesRenderer.h"

void StatesRenderer::renderMainMenu(sf::RenderWindow& window) {
	sf::VertexArray bg(sf::PrimitiveType::TriangleStrip, 4);
	bg[0].position = sf::Vector2f(0, 0);
	bg[1].position = sf::Vector2f(settings.projectionWindowSize().x / settings.resolution, 0);
	bg[2].position = sf::Vector2f(0, settings.projectionWindowSize().y / settings.resolution);
	bg[3].position = sf::Vector2f(settings.projectionWindowSize().x / settings.resolution, settings.projectionWindowSize().y / settings.resolution);

	sf::Texture* tex = TextureManager::getTexture(10);
	sf::Vector2u texSize = tex->getSize();
	float scaleX = (float)texSize.x / settings.projectionWindowSize().x;
	float scaleY = (float)texSize.y / settings.projectionWindowSize().y;
	float scale = std::min(scaleX, scaleY);
	bg[0].texCoords = sf::Vector2f(0, 0);
	bg[1].texCoords = sf::Vector2f(settings.projectionWindowSize().x * scale, 0);
	bg[2].texCoords = sf::Vector2f(0, settings.projectionWindowSize().y * scale);
	bg[3].texCoords = sf::Vector2f(settings.projectionWindowSize().x * scale, settings.projectionWindowSize().y * scale);

	sf::RectangleShape dim(sf::Vector2f(settings.projectionWindowSize().x / settings.resolution, settings.projectionWindowSize().y / settings.resolution));
	dim.setPosition(sf::Vector2f(0, 0));
	dim.setFillColor(sf::Color(0, 0, 0, 125));

	sf::Font font;
	font.openFromFile("Fonts\\VT323-Regular.ttf");
	sf::Text text(font, "Press any key to start", settings.projectionWindowSize().y / 10);
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));
	text.setPosition(sf::Vector2f(settings.projectionWindowSize().x / 2, settings.projectionWindowSize().y / 2));
	text.setFillColor(sf::Color::White);

	sf::RenderTexture menu(sf::Vector2u(settings.projectionWindowSize().x / settings.resolution, settings.projectionWindowSize().y));
	menu.draw(bg, tex);
	menu.draw(dim);
	menu.display();
	sf::Sprite toDraw(menu.getTexture());
	toDraw.setScale(sf::Vector2f(settings.resolution, settings.resolution));
	window.draw(toDraw);
	window.draw(text);
}

void StatesRenderer::renderWinScreen(sf::RenderWindow& window) {
	sf::RectangleShape bg(sf::Vector2f(settings.projectionWindowSize().x, settings.projectionWindowSize().y));
	bg.setFillColor(sf::Color::Black);
	bg.setPosition(sf::Vector2f(0, 0));

	sf::Font font;
	font.openFromFile("Fonts\\VT323-Regular.ttf");
	sf::Text text(font, "FIN", settings.projectionWindowSize().y / 2);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));
	text.setPosition(sf::Vector2f(settings.projectionWindowSize().x / 2, settings.projectionWindowSize().y / 2));
	window.draw(bg);
	window.draw(text);
}