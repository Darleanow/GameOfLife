#include "GameOfLife.h"

void drawLine(int left, int y, sf::Color gridColor, int right, sf::RenderWindow& window)
{
	const sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(left, y), gridColor),
			sf::Vertex(sf::Vector2f(right, y), gridColor)
	};
	window.draw(line, 2, sf::Lines);
}

void drawGrid(sf::RenderWindow& window, const sf::View& view, float gridSpacing) {
	const sf::Vector2f viewSize = view.getSize();
	const sf::Vector2f viewCenter = view.getCenter();

	const float left = viewCenter.x - viewSize.x / 2;
	const float right = viewCenter.x + viewSize.x / 2;
	const float top = viewCenter.y - viewSize.y / 2;
	const float bottom = viewCenter.y + viewSize.y / 2;

	const sf::Color gridColor = sf::Color(100, 100, 100, 50);

	auto drawLine = [&](float startX, float startY, float endX, float endY) {
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(startX, startY);
		line[0].color = gridColor;
		line[1].position = sf::Vector2f(endX, endY);
		line[1].color = gridColor;
		window.draw(line);
	};

	for (float x = left - std::fmod(left, gridSpacing); x < right; x += gridSpacing) {
		drawLine(x, top, x, bottom);
	}

	for (float y = top - std::fmod(top, gridSpacing); y < bottom; y += gridSpacing) {
		drawLine(left, y, right, y);
	}
}

void addPoint(std::vector<Point>& points, int x, int y, const int gridSpacing, const sf::Color& color, const float size = 50) {
	points.emplace_back(x, y, color, size);
}

void removePoint(std::vector<Point>& points, const std::vector<Point>::iterator& it) {

	points.erase(it);
}

void redrawPoints(sf::RenderWindow& window, const std::vector<Point>& points, const int gridSpacing) {
	for (const auto& point : points) {
		sf::RectangleShape square(sf::Vector2f(point.size, point.size));
		const float offset = (gridSpacing - point.size) / 2.0f;
		square.setPosition(point.x * gridSpacing + offset, point.y * gridSpacing + offset);
		square.setFillColor(point.color);
		window.draw(square);
	}
}

void setupPannel(sf::Text& text, sf::Font font, std::string title, uint16_t size, sf::Color color)
{
	text.setFont(font);
	text.setString("Panneau de Controle");
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Color(238, 238, 238));
}

int main() {
	sf::Clock clock;
	const float updateInterval = 0.05;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Game Of Life");
	window.setFramerateLimit(60);

	sf::Font font;
	if (!font.loadFromFile("C:/Users/ehugo/Downloads/warden-regular-font/Warden.otf"))
	{
		std::cerr << "Couldn't load font \n";
		return -1;
	}

	UIManager uiManager(font, window);

	sf::View mainView(window.getView());
	float zoomFactor = 1.0f;

	const float gridSpacing = 50.0f;
	std::vector<Point> points;

	bool panning = false;
	sf::Vector2f panStart;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized) {
				float aspectRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
				sf::View resizedView(sf::FloatRect(0, 0, event.size.width, event.size.height));

				window.setView(resizedView);
				mainView.setSize(event.size.width * zoomFactor, event.size.height * zoomFactor); // Adjust mainView as well
				mainView.setCenter(event.size.width / 2, event.size.height / 2); // Center the view
			}


			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta > 0) {
					zoomFactor *= 0.9f; // Zoom in
				}
				else if (event.mouseWheelScroll.delta < 0) {
					zoomFactor *= 1.1f; // Zoom out
				}

				// Calculate the new size based on zoomFactor
				sf::Vector2f newSize = window.getDefaultView().getSize() * zoomFactor;

				// Set the new size to the mainView
				mainView.setSize(newSize);
			}

			// Wheel movement when click
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) {
				panning = true;
				panStart = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			}

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle) {
				panning = false;
			}

			if (event.type == sf::Event::MouseMoved && panning) {
				const sf::Vector2f panEnd = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				const sf::Vector2f panDelta = panStart - panEnd;
				mainView.move(panDelta * zoomFactor);
				panStart = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			}

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);

				if (uiManager.isClearButtonClicked(mousePos)) {
					points.clear();
				}

				// Toggle grid display if the checkbox is clicked
				if (uiManager.isCheckboxClicked(mousePos)) {
					uiManager.toggleCheckbox(window);
					break;
				}

				// Start/Stop Game of Life if the button is clicked
				if (uiManager.isStartButtonClicked(mousePos)) {
					break;
				}

				if (uiManager.isRestrainedClick(mousePos)) {
					uiManager.handleEvent(window, event);
					break;
				}

				sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), mainView);
				const int gridX = std::floor(worldPos.x / gridSpacing);
				const int gridY = std::floor(worldPos.y / gridSpacing);

				auto it = std::find_if(points.begin(), points.end(), [gridX, gridY](const Point& point) {
					return point.x == gridX && point.y == gridY;
				});

				if (it != points.end())
				{
					removePoint(points, it);
				}
				else
				{
					addPoint(points, gridX, gridY, gridSpacing, sf::Color::Color(118, 171, 174, 255));
				}
			}
		}

		window.clear(sf::Color::Color(34, 40, 49));
		window.setView(mainView);

		// Grid draw
		if (uiManager.isCheckboxChecked())
		{
			drawGrid(window, mainView, gridSpacing);
		}

		// Points
		if (uiManager.isGameRunning()) {
			if (clock.getElapsedTime().asSeconds() >= updateInterval) {
				points = GolEngine::nextGeneration(points);
				clock.restart(); // Reset the clock after updating
			}
		}

		redrawPoints(window, points, gridSpacing);

		std::string selectedPattern = uiManager.getSelectedPattern();
		if (!selectedPattern.empty()) {
			const int startX = 0;
			const int startY = 0;
			GolEngine::placePattern(selectedPattern, startX, startY, points);
			uiManager.clearSelectedPattern();
		}

		// Draw UI
		window.setView(window.getDefaultView());
		uiManager.draw(window);

		window.display();
	}

	return 0;
}
