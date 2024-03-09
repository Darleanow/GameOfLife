#pragma once
#include <SFML/Graphics.hpp>

class UIManager {
private:
    sf::RectangleShape checkbox;
    sf::Text checkboxText;
    sf::RectangleShape checkmark;

    bool checkboxChecked;
    sf::RectangleShape startButton;
    sf::Text startButtonText;
    sf::RectangleShape controlPanel;
    sf::Text controlPanelText;

    bool isRunning;

    sf::RectangleShape dropdownButton;
    sf::Text dropdownButtonText;
    bool dropdownExpanded;
    std::vector<std::string> dropdownOptions;
    std::vector<sf::Text> dropdownOptionsText;

    std::string selectedPattern;

    sf::RectangleShape clearButton;
    sf::Text clearButtonText;
public:
    UIManager(sf::Font& font, sf::RenderWindow& window) {
        // Control Panel
        controlPanel.setSize(sf::Vector2f(window.getSize().x, window.getSize().y * 0.25f));
        controlPanel.setFillColor(sf::Color(49, 54, 63, 255));
        controlPanel.setPosition(0, window.getSize().y - controlPanel.getSize().y);
        controlPanelText.setFont(font);
        controlPanelText.setString("Control Panel");
        controlPanelText.setCharacterSize(20);
        controlPanelText.setFillColor(sf::Color::Color(238, 238, 238));
        controlPanelText.setPosition(20, controlPanel.getPosition().y + 10);

        // Checkbox
        checkbox.setSize(sf::Vector2f(20, 20));
        checkbox.setPosition(10, controlPanel.getPosition().y + 47);
        checkbox.setFillColor(sf::Color::Color(238, 238, 238));
        checkboxChecked = true;

        // Checkbox Text
        checkboxText.setFont(font);
        checkboxText.setString(checkboxChecked ? "Hide Grid" : "Display Grid");
        checkboxText.setCharacterSize(20);
        checkboxText.setFillColor(sf::Color::Color(238, 238, 238));
        checkboxText.setPosition(checkbox.getPosition().x + checkbox.getSize().x + 10, controlPanel.getPosition().y + 45);

        // Checkmark
        checkmark.setSize(sf::Vector2f(10, 10));
        checkmark.setPosition(checkbox.getPosition().x + 5, checkbox.getPosition().y + 5);
        checkmark.setFillColor(sf::Color::Color(176, 197, 164));

        // Start Button
        startButton.setSize(sf::Vector2f(150, 30));
        startButton.setPosition(200, controlPanel.getPosition().y + 40);
        startButton.setFillColor(sf::Color::Color(176, 197, 164));

        // Start Button Text
        startButtonText.setFont(font);
        startButtonText.setString("Start Game");
        startButtonText.setCharacterSize(20);
        startButtonText.setFillColor(sf::Color::Color(238, 238, 238));
        // Center text within the start button
        sf::FloatRect textRect = startButtonText.getLocalBounds();
        startButtonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        startButtonText.setPosition(sf::Vector2f(startButton.getPosition().x + startButton.getSize().x / 2.0f, startButton.getPosition().y + startButton.getSize().y / 2.0f));

        isRunning = false;

        // Initialize the dropdown button
        dropdownButton.setSize(sf::Vector2f(180, 30));
        dropdownButton.setPosition(380, controlPanel.getPosition().y + 40); 
        dropdownButton.setFillColor(sf::Color::Color(34, 40, 49));
        dropdownButtonText.setFont(font);
        dropdownButtonText.setString("Select Pattern");
        dropdownButtonText.setCharacterSize(20);
        dropdownButtonText.setFillColor(sf::Color::Color(238, 238, 238));
        sf::FloatRect dropdownTextBounds = dropdownButtonText.getLocalBounds();
        dropdownButtonText.setPosition(dropdownButton.getPosition().x + (dropdownButton.getSize().x - dropdownTextBounds.width) / 2, dropdownButton.getPosition().y + (dropdownButton.getSize().y - dropdownTextBounds.height) / 2 - dropdownTextBounds.top);

        dropdownExpanded = false;

        // Initialize dropdown options
        dropdownOptions = { "Pulsar", "LLWS", "Gosper Glider Gun", "Ben Special" };
        for (const auto& option : dropdownOptions) {
            sf::Text optionText;
            optionText.setFont(font);
            optionText.setString(option);
            optionText.setCharacterSize(16);
            optionText.setFillColor(sf::Color::Color(238, 238, 238));
            dropdownOptionsText.push_back(optionText);
        }

        // Clear Button
        clearButton.setSize(sf::Vector2f(150, 30));
        clearButton.setPosition(580, controlPanel.getPosition().y + 40);
        clearButton.setFillColor(sf::Color::Color(211, 118, 118));

        clearButtonText.setFont(font);
        clearButtonText.setString("Clear Points");
        clearButtonText.setCharacterSize(20);
        clearButtonText.setFillColor(sf::Color::Color(238, 238, 238));
        sf::FloatRect clearTextBounds = clearButtonText.getLocalBounds();
        clearButtonText.setPosition(
            clearButton.getPosition().x + (clearButton.getSize().x - clearTextBounds.width) / 2.0f,
            clearButton.getPosition().y + (clearButton.getSize().y - clearTextBounds.height) / 2.0f - clearTextBounds.top
        );

    }

    void updateCheckboxText() {
        checkboxText.setString(checkboxChecked ? "Hide Grid" : "Display Grid");
    }

    void draw(sf::RenderWindow& window) {
        window.draw(controlPanel);
        window.draw(controlPanelText);
        window.draw(checkbox);
        if (checkboxChecked) window.draw(checkmark);
        window.draw(checkboxText);
        window.draw(startButton);
        window.draw(startButtonText);

        window.draw(dropdownButton);
        window.draw(dropdownButtonText);

        if (dropdownExpanded) {
            for (size_t i = 0; i < dropdownOptionsText.size(); ++i) {
                float offsetY = 15 + (i + 1) * 15;
                dropdownOptionsText[i].setPosition(dropdownButton.getPosition().x, dropdownButton.getPosition().y + offsetY);
                window.draw(dropdownOptionsText[i]);
            }
        }

        window.draw(clearButton);
        window.draw(clearButtonText);
    }

    void toggleDropdown(sf::RenderWindow& window, sf::Vector2i mousePos) {
        if (dropdownButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            dropdownExpanded = !dropdownExpanded;
        }
        else if (dropdownExpanded) {
            for (size_t i = 0; i < dropdownOptionsText.size(); ++i) {
                if (dropdownOptionsText[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    dropdownExpanded = false;
                    selectedPattern = dropdownOptions.at(i);
                    break;
                }
            }
        }
    }

    void handleEvent(sf::RenderWindow& window, sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            toggleDropdown(window, mousePos);
        }
    }

    void toggleCheckbox(sf::RenderWindow& window) {
        checkboxChecked = !checkboxChecked;
        checkboxChecked ? checkmark.setFillColor(sf::Color::Color(176, 197, 164)) : checkmark.setFillColor(sf::Color::Color(238, 238, 238));
        updateCheckboxText();
    }

    const bool isCheckboxChecked() const {
        return checkboxChecked ? true : false;
    }

	const sf::RectangleShape& getCheckBox()
	{
		return checkbox;
	}

	sf::RectangleShape& getStartButton() {
		return startButton;
	}

	bool isStartButtonClicked(sf::Vector2i mousePos) {
		if (startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
		{
			if (isGameRunning())
			{
				startButton.setFillColor(sf::Color::Color(176, 197, 164));
				setStartButtonText("Start Game");
			}
			else
			{
				startButton.setFillColor(sf::Color::Color(211, 118, 118));
				setStartButtonText("Stop Game");
			}
			isRunning = !isRunning;

			return true;
		}

		return false;
	}

	bool isGameRunning()
	{
		return isRunning;
	}

	void setStartButtonText(const std::string& text) {
		startButtonText.setString(text);
	}

	bool isCheckboxClicked(sf::Vector2i mousePos) const {
		return checkbox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
	}

    std::string getSelectedPattern() {
        return selectedPattern;
    }

    void clearSelectedPattern() {
        selectedPattern.clear();
    }

    bool isClearButtonClicked(sf::Vector2i mousePos) const {
        return clearButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    bool isRestrainedClick(sf::Vector2i mousePos) const {
        return controlPanel.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }
};