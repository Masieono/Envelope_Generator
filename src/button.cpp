

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "button.hpp"
#include "theme.hpp"

// app manager constructor
Button::Button(ButtonShape shape, ButtonType type, TitleLocation location, const sf::String& title, float WidthOrRadius, float height)
: m_position(sf::Vector2f(0,0))
, m_titleLocation(location)
, m_buttonShape(shape)
, m_buttonType(type)
, m_width(WidthOrRadius)
, m_height(height)
, m_radius(0.f)
, m_currentState(0)
, m_isPressed(false)
, m_isHovered(false)
, m_isDisabled(false)
{
    Theme& theme = Theme::getInstance();

    m_padding = theme.getSmallPadding();
    m_font = theme.getFont();

    // Get and set font and styling
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(theme.getTitleSize());
    m_titleText.setFillColor(theme.getPrimaryColor());
    m_titleText.setString(title);

    // Set colors
    m_unpressedColor = theme.getPrimaryColor();
    m_pressedColor = theme.getSecondaryColor();
    m_hoverColor = theme.getHoverColor();
    m_disabledColor = theme.getDisabledColor();

    // create base shape of button

    if (m_buttonShape == ButtonShape::Rectangle)
    {
        auto rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(m_width, m_height));
        m_shape = std::move(rect); 
    }
    else if (m_buttonShape == ButtonShape::Circle)
    {
        m_radius = WidthOrRadius;
        auto circle = std::make_unique<sf::CircleShape>(m_radius);
        m_shape = std::move(circle);
    }

    // center the origin point of the button
    sf::FloatRect button_bounds = m_shape->getLocalBounds();
    m_shape->setOrigin(button_bounds.left + button_bounds.width / 2, button_bounds.top + button_bounds.height / 2);

    positionTitle(location, button_bounds);

    m_shape->setFillColor(m_unpressedColor);
}


Button::Button(float x, float y, ButtonShape shape, ButtonType type, const sf::String& title, float WidthOrRadius, float height)
: m_position(x, y)
, m_buttonShape(shape)
, m_buttonType(type)
, m_width(WidthOrRadius)
, m_height(height)
, m_radius(0.f)
, m_currentState(0)
, m_isPressed(false)
, m_isHovered(false)
, m_isDisabled(false)
{
    Theme& theme = Theme::getInstance();

    m_font = theme.getFont();

    // Get and set font and styling
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(theme.getTitleSize());
    m_titleText.setFillColor(theme.getPrimaryColor());
    m_titleText.setString(title);

    // Set colors
    m_unpressedColor = theme.getPrimaryColor();
    m_pressedColor = theme.getSecondaryColor();
    m_hoverColor = theme.getHoverColor();
    m_disabledColor = theme.getDisabledColor();

    // create base shape of button
    if (m_buttonShape == ButtonShape::Rectangle)
    {
        auto rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(m_width, m_height));
        m_shape = std::move(rect); 
    }
    else if (m_buttonShape == ButtonShape::Circle)
    {
        m_radius = WidthOrRadius;
        auto circle = std::make_unique<sf::CircleShape>(m_radius);
        m_shape = std::move(circle);
    }

    m_shape->setPosition(m_position);
    m_shape->setOutlineThickness(3);

    // center the origin point of the button
    sf::FloatRect button_bounds = m_shape->getLocalBounds();
    m_shape->setOrigin(button_bounds.left + button_bounds.width / 2, button_bounds.top + button_bounds.height / 2);

    // change the origin point of the label to center top
    sf::FloatRect title_bounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(title_bounds.width / 2, title_bounds.top);

    // center the label 25px below bottom of button
    m_titleText.setPosition(m_shape->getPosition().x, m_shape->getPosition().y + (button_bounds.height / 2) + m_padding);

    m_shape->setFillColor(m_unpressedColor);
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (m_isDisabled)
    {
        m_shape->setFillColor(m_disabledColor);
        m_shape->setOutlineThickness(0.f);
        return;
    }

    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    bool mouseOverButton = m_shape->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));

    switch (m_buttonType)
    {
        case ButtonType::Momentary:
        {
            if (mouseOverButton)
            {
                // Handle mouse button press
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    m_isPressed = true;
                    m_shape->setFillColor(m_pressedColor);  // Set to pressed (red) when clicked
                }
                // Handle mouse button release
                else if (event.type == sf::Event::MouseButtonReleased && m_isPressed)
                {
                    m_isPressed = false;
                    m_shape->setFillColor(m_unpressedColor);  // Return to hover state if still hovering
                }
                else if (!m_isPressed)
                {
                    // Hover state when not pressed
                    m_titleText.setFillColor(m_hoverColor);    // Change label color to hover color
                    m_shape->setOutlineThickness(2.f);          // Add outline for hover effect
                    m_shape->setOutlineColor(m_hoverColor);
                }
            }
            else
            {
                // Reset to default state if mouse is not over the button
                m_isPressed = false;
                m_titleText.setFillColor(m_unpressedColor);  // Reset label color
                m_shape->setFillColor(m_unpressedColor);      // Unpressed color
                m_shape->setOutlineThickness(0.f);            // Remove outline
            }
            break;
        }

        case ButtonType::Latching:
        {
            if (mouseOverButton)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    m_isPressed = !m_isPressed;  // Toggle pressed state
                    m_shape->setFillColor(m_isPressed ? m_pressedColor : m_unpressedColor);
                }
                // Hover effects while maintaining pressed state
                m_titleText.setFillColor(m_hoverColor);  // Change label to hover color
                m_shape->setOutlineThickness(2.f);        // Add hover outline
                m_shape->setOutlineColor(m_hoverColor);
            }
            else
            {
                // Keep pressed color, reset hover effects
                m_titleText.setFillColor(m_unpressedColor);
                m_shape->setOutlineThickness(0.f);  // Remove outline
            }
            break;
        }

        case ButtonType::StateCycling:
        {
            if (mouseOverButton)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    cycleState();
                }
                // Hover effects
                m_titleText.setFillColor(m_hoverColor);  // Change label to hover color
                m_shape->setOutlineThickness(2.f);        // Add hover outline
                m_shape->setOutlineColor(m_hoverColor);
            }
            else
            {
                m_titleText.setFillColor(m_unpressedColor);  // Reset label color
                m_shape->setOutlineThickness(0.f);            // Remove outline
            }
            break;
        }
    }
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(*m_shape);
    window.draw(m_titleText);

    if (m_buttonType == ButtonType::StateCycling && !m_cycleText.empty())
    {
        window.draw(m_cycleText[m_currentState]);
    }
}

// setters

void Button::setUnpressedColor(sf::Color color)
{
    m_unpressedColor = color;
}
void Button::setPressedColor(sf::Color color)
{
    m_pressedColor = color;
}
void Button::setHoverColor(sf::Color color)
{
    m_hoverColor = color;
}
void Button::setDisabledColor(sf::Color color)
{
    m_disabledColor = color;
}
void Button::setBorderColor(sf::Color color)
{
    m_borderColor = color;
}
void Button::setDisabled(bool disabled)
{
    m_isDisabled = disabled;
}
void Button::setTextAlignment(sf::Vector2f position)
{
    m_titleText.setPosition(position);
}
void Button::setPosition(sf::Vector2f position)
{
    m_position = position;

    m_shape->setPosition(position);
    positionTitle(m_titleLocation, m_shape->getLocalBounds());
}

void Button::setStates(const std::vector<sf::String>& labels, const std::vector<sf::Color>& colors)
{
    if (labels.size() != colors.size())
    {
        std::cerr << "Error: number of labels and colors must match" << std::endl;
        return;
    }

    m_stateColors = colors;
    m_cycleText.clear();

    const Theme& theme = Theme::getInstance();

    for (int i = 0; i < labels.size(); i++)
    {
        sf::Text text;
        text.setFont(theme.getFont());
        text.setCharacterSize(theme.getLabelSize());
        text.setFillColor(m_unpressedColor);
        text.setString(labels[i]);
        text.setStyle(sf::Text::Bold);

        // center text on the button
        sf::FloatRect text_bounds = text.getLocalBounds();
        text.setOrigin(text_bounds.left + text_bounds.width / 2, text_bounds.top + text_bounds.height / 2);
        text.setPosition(m_shape->getPosition());

        m_cycleText.push_back(text);
    }

    if (!m_stateColors.empty())
    {
        m_shape->setFillColor(m_stateColors[0]);
    }
}

// getters

Button::ButtonType Button::getButtonType() const
{
    return m_buttonType;
}
sf::Color Button::getPressedColor() const
{
    return m_pressedColor;
}
sf::Color Button::getUnpressedColor() const
{
    return m_unpressedColor;
}
sf::Color Button::getDisabledColor() const
{
    return m_disabledColor;
}
sf::Color Button::getHoverColor() const
{
    return m_hoverColor;
}

bool Button::isPressed() const
{
    return m_isPressed;
}
bool Button::isHovered() const
{
    return m_isHovered;
}
bool Button::isDisabled() const
{
    return m_isDisabled;
}
int Button::getButtonState() const
{
    return m_currentState;
}


void Button::positionTitle(Button::TitleLocation location, sf::FloatRect button_bounds)
{
    // position title text
    sf::FloatRect title_bounds = m_titleText.getLocalBounds();

    switch (location)
    {
        case TitleLocation::Above:
        {
            // Set origin centered at bottom
            m_titleText.setOrigin(title_bounds.left + title_bounds.width / 2, title_bounds.top + title_bounds.height);

            // Set position above the button
            m_titleText.setPosition(m_shape->getPosition().x, m_shape->getPosition().y - (button_bounds.height / 2) - m_padding);

            break;
        }
        case TitleLocation::Centered:
        {
            // Set Origin to absolute center
            m_titleText.setOrigin(title_bounds.left + title_bounds.width / 2, title_bounds.top + title_bounds.height / 2);

            // Set Position
            m_titleText.setPosition(m_shape->getPosition().x, m_shape->getPosition().y);

            break;
        }
        case TitleLocation::Below:
        {
            // Set origin centered at top
            m_titleText.setOrigin(title_bounds.width / 2, title_bounds.top);

            // Set position below the button
            m_titleText.setPosition(m_shape->getPosition().x, m_shape->getPosition().y + (button_bounds.height / 2) + m_padding);

            break;
        }
        default:
        {
            std::cerr << "Invalid title location." << std::endl;
            break;
        }
    }
}

// for latching and state-cycling buttons
void Button::updateButtonState()
{
    if (m_buttonType == ButtonType::Latching)
    {
        m_isPressed = !m_isPressed;
        m_shape->setFillColor(m_isPressed ? m_pressedColor : m_unpressedColor);
    }
    else if (m_buttonType == ButtonType::StateCycling)
    {
        cycleState();
    }
}

// cycle through states for state cycling buttons
void Button::cycleState() 
{
    if (!m_stateColors.empty()) 
    {
        m_currentState = (m_currentState + 1) % m_stateColors.size();
        m_shape->setFillColor(m_stateColors[m_currentState]);
    }
}

