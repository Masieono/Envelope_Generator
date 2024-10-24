

#include <iostream>
#include <sstream>
#include <cmath>

#include "theme.hpp"
#include "slider.hpp"

// for envelope generator, still needs adjust
Slider::Slider(float minValue, float maxValue, const std::string& title)
: m_position(0.f, 0.f)
, m_width(150.f)
, m_height(10.f)
, m_minValue(minValue)
, m_maxValue(maxValue)
, m_value(minValue)
, m_dragging(false)
, m_step(0.01f)
, m_isHorizontal(true) 
{
    Theme& theme = Theme::getInstance();

    m_padding = theme.getSmallPadding();

    // Slider track
    m_background.setSize(sf::Vector2f(m_width, m_padding / 2));
    m_background.setFillColor(theme.getPrimaryColor());

    // initialize slider handle
    m_handle.setRadius(m_padding / 2);
    m_handle.setOrigin(m_handle.getRadius(), m_handle.getRadius());
    m_handle.setFillColor(theme.getSecondaryColor());
    

    // initialize text elements
    m_font = theme.getFont();

    m_titleText.setString(title);
    m_titleText.setCharacterSize(theme.getTitleSize());
    m_titleText.setFillColor(theme.getPrimaryColor());
    m_titleText.setFont(m_font);

    m_valueText.setCharacterSize(theme.getLabelSize());
    m_valueText.setFillColor(theme.getPrimaryColor());
    m_valueText.setFont(m_font);

    updateTitleText();
    updateValueText();
}

// more customizable
Slider::Slider(float x, float y, float width, float height, float minValue, float maxValue, bool isHorizontal, const std::string& title)
: m_position(x, y)
, m_width(width)
, m_height(height)
, m_minValue(minValue)
, m_maxValue(maxValue)
, m_value(minValue)
, m_dragging(false)
, m_step(0.01f)
, m_isHorizontal(isHorizontal)
{
    Theme& theme = Theme::getInstance();

    m_padding = theme.getSmallPadding();

    // Slider track
    m_background.setSize(m_isHorizontal ? sf::Vector2f(m_width, 10) : sf::Vector2f(10, m_height));
    m_background.setPosition(m_position);
    m_background.setFillColor(theme.getPrimaryColor());

    // initialize slider handle
    m_handle.setRadius(10.0f);
    m_handle.setOrigin(m_handle.getRadius(), m_handle.getRadius());
    m_handle.setFillColor(theme.getSecondaryColor());
    
    // Adjust handle position for both horizontal and vertical sliders
    if (m_isHorizontal) 
    {
        // Center the handle on the track vertically
        m_handle.setPosition(x, y + (m_background.getSize().y / 2));
    } 
    else 
    {
        // Center the handle on the track horizontally and place it at the bottom
        m_handle.setPosition(x + (m_background.getSize().x / 2), y + m_height);
    }

    // initialize text elements
    m_font = theme.getFont();

    m_titleText.setString(title);
    m_titleText.setCharacterSize(theme.getTitleSize());
    m_titleText.setFillColor(theme.getPrimaryColor());
    m_titleText.setFont(m_font);

    m_valueText.setCharacterSize(theme.getLabelSize());
    m_valueText.setFillColor(theme.getPrimaryColor());
    m_valueText.setFont(m_font);

    sf::FloatRect text_bounds = m_titleText.getLocalBounds();

    if(m_isHorizontal)
    {
        m_titleText.setOrigin(text_bounds.left, text_bounds.top + text_bounds.height);
        m_titleText.setPosition(x, y - theme.getLargePadding());
    }
    else
    {
        m_titleText.setOrigin(text_bounds.left + text_bounds.width / 2, 
                               text_bounds.top + text_bounds.height);

        m_titleText.setPosition(x + (m_background.getSize().x / 2), y - theme.getLargePadding());
    }

    updateValueText();
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Check if the knob was clicked
                if (m_handle.getGlobalBounds().contains(mousePos))
                {
                    m_dragging = true;
                }
            }
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_dragging = false;
            }
        }
        if (event.type == sf::Event::MouseMoved && m_dragging)
        {
            sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // Clamp the handle's position to be within the slider background
            float new_position = m_isHorizontal
            ? std::clamp(mouse_position.x, m_position.x, m_position.x + m_width)
            : std::clamp(mouse_position.y, m_position.y, m_position.y + m_height);
            
            m_handle.setPosition(m_isHorizontal ? new_position : m_handle.getPosition().x,
                                 m_isHorizontal ? m_handle.getPosition().y : new_position);

            // Update the value based on the knob's position
            float handle_position = m_isHorizontal
            ? (m_handle.getPosition().x - m_position.x)
            : (m_handle.getPosition().y - m_position.y);

            float normalized_position = m_isHorizontal
                ? handle_position / m_width
                : (m_height - handle_position) / m_height;

            float new_value = m_minValue + normalized_position * (m_maxValue - m_minValue);

            // Snap the new value to the nearest step
            new_value = std::round(new_value / m_step) * m_step;

            // Ensure the value stays within bounds
            new_value = std::clamp(new_value, m_minValue, m_maxValue);

            // Update the handle position again based on the new snapped value
            m_value = new_value;
            setValue(m_value);
        }
    updateValueText();
}

void Slider::updateValueText()
{
    // format as string
    std::stringstream ss_current;
    ss_current << m_value;
    m_valueText.setString(ss_current.str());

    // current value positioning
    sf::FloatRect text_bounds = m_valueText.getLocalBounds();

    if (m_isHorizontal)
    {
        m_valueText.setOrigin(text_bounds.left + text_bounds.width / 2, text_bounds.top);

        // Position text centered below the slider handle
        float text_x = m_handle.getPosition().x;
        float text_y = m_position.y + m_padding;
        m_valueText.setPosition(text_x, text_y);
    }
    else
    {
        // adjust the horizontal positioning of the text origin.
        m_valueText.setOrigin(text_bounds.left, text_bounds.top + text_bounds.height / 2);

        // Position text centered to the right of the slider handle
        float text_x = m_position.x + m_padding;
        float text_y = m_handle.getPosition().y;
        
        // Adjust for the origin of the text
        m_valueText.setPosition(text_x, text_y);
    }
}
void Slider::updateTitleText()
{
    sf::FloatRect text_bounds = m_titleText.getLocalBounds();

    if (m_isHorizontal)
    {
        m_titleText.setOrigin(text_bounds.left, text_bounds.top + text_bounds.height);
        m_titleText.setPosition(m_position.x, m_position.y - m_padding);
    }
    else
    {
        m_titleText.setOrigin(text_bounds.left + text_bounds.width / 2, 
                               text_bounds.top + text_bounds.height);

        m_titleText.setPosition(m_position.x + (m_background.getSize().x / 2), m_position.y - m_padding);
    }

}


void Slider::draw(sf::RenderWindow& window)
{
    window.draw(m_background);
    window.draw(m_handle);

    window.draw(m_titleText);
    window.draw(m_valueText);
}
void Slider::setValue(float value)
{
    if(value > m_maxValue) 
    {
        m_value = m_maxValue;
    }
    else if (value < m_minValue)
    {
        m_value = m_minValue;
    } 
    else
    {
        m_value = value;
    }

    // update handle position
    if (m_isHorizontal)
    {
        float handle_x = m_position.x + ((m_value - m_minValue) / (m_maxValue - m_minValue)) * m_width;
        m_handle.setPosition(handle_x, m_handle.getPosition().y);
    }
    else
    {
        float handle_y = m_position.y + (m_height - ((m_value - m_minValue) / (m_maxValue - m_minValue)) * m_height);
        m_handle.setPosition(m_handle.getPosition().x, handle_y);
    }

    updateValueText();
}
void Slider::setStep(float increment)
{
    if(increment > 0.0f)
        m_step = increment;
}

void Slider::setPosition(sf::Vector2f startPoint, sf::Vector2f endPoint)
{
    m_position = startPoint;

    float newWidth = endPoint.x - startPoint.x;
    float newHeight = endPoint.y - startPoint.y;

    // reset slider components to new position
    // background, handle, title text, and value text

    if(m_isHorizontal)
    {
        m_width = newWidth;
        m_background.setSize(sf::Vector2f(newWidth, m_padding / 2));

        // origin point centered on the far left side
        m_background.setOrigin(0.f, m_background.getSize().y / 2);

        m_background.setPosition(m_position);
    
        m_handle.setPosition(m_position.x, m_position.y);

    }
    else
    {
        m_height = newHeight;
        m_background.setSize(sf::Vector2f(m_padding / 2, m_height));

        // origin point centered on the top side
        m_background.setOrigin(m_background.getSize().x / 2, 0.f);

        m_background.setPosition(m_position.x, m_position.y);

        m_handle.setPosition(m_position.x, m_position.y);

        std::cout << m_background.getPosition().x << "  " << m_background.getPosition().y << std::endl;

//        m_handle.setPosition(m_position.x + (m_background.getSize().x / 2), m_position.y + m_height);
    }

    updateTitleText();
    updateValueText();
}


float Slider::getValue() const
{
    return m_value;
}
float Slider::getMinValue() const
{
    return m_minValue;
}
float Slider::getMaxValue() const
{
    return m_maxValue;
}
bool Slider::isDragging() const
{
    return m_dragging;
}