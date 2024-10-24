

#include <iostream>
#include <sstream>
#include <cmath>

#include "theme.hpp"
#include "knob.hpp"


Knob::Knob(KnobType type, float min, float max, float init_value, float radius, const std::string& label)
: m_position(sf::Vector2f(0,0))
, m_knobType(type)
, m_radius(radius)
, m_minValue(min)
, m_maxValue(max)
, m_value(init_value)
, m_isTurning(false)
, m_increment(0.01f)
, m_sensitivity(0.15f)
, m_previousMousePosition(sf::Vector2f(0,0))
{
    Theme& theme = Theme::getInstance();

    m_padding = theme.getSmallPadding();
    m_knobColor = theme.getPrimaryColor();
    m_indicatorColor = theme.getSecondaryColor();

    // initialize the knob circle
    m_knob.setRadius(radius);
    m_knob.setOrigin(radius, radius);
    m_knob.setFillColor(m_knobColor);

    // initialize the indicator
    m_indicator.setRadius(m_knob.getRadius() / 5);
    m_indicator.setOrigin(m_indicator.getRadius(), m_indicator.getRadius());
    m_indicator.setFillColor(m_indicatorColor);

    // set indicator position and rotation
    positionIndicator(init_value);

    // initialize text elements
    m_font = theme.getFont();

    m_valueText.setCharacterSize(theme.getLabelSize());
    m_valueText.setFillColor(theme.getPrimaryColor());
    m_valueText.setFont(m_font);
    positionValueText(init_value);

    m_titleText.setString(label);
    m_titleText.setCharacterSize(theme.getTitleSize());
    m_titleText.setFillColor(theme.getPrimaryColor());
    m_titleText.setFont(m_font);
    positionTitle();
}





Knob::Knob(float x, float y, float radius, float min, float max, float init_position, const std::string& label)
: m_position(x, y)
, m_radius(radius)
, m_minValue(min)
, m_maxValue(max)
, m_value(init_position)
, m_isTurning(false)
, m_increment(0.01f)
, m_sensitivity(0.15f)
, m_previousMousePosition(x, y)
{
    Theme& theme = Theme::getInstance();

    m_padding = theme.getSmallPadding();
    m_knobColor = theme.getPrimaryColor();
    m_indicatorColor = theme.getSecondaryColor();

    // initialize the knob circle
    m_knob.setRadius(radius);
    m_knob.setOrigin(radius, radius);
    m_knob.setPosition(m_position);
    m_knob.setFillColor(m_knobColor);

    // initialize the indicator
    m_indicator.setRadius(m_knob.getRadius() / 6);
    m_indicator.setOrigin(m_indicator.getRadius(), m_indicator.getRadius());
    m_indicator.setFillColor(m_indicatorColor);

    // set indicator position and rotation
    positionIndicator(init_position);

    // initialize text elements
    m_font = theme.getFont();

    m_valueText.setCharacterSize(theme.getLabelSize());
    m_valueText.setFillColor(theme.getPrimaryColor());
    m_valueText.setFont(m_font);

    positionValueText(init_position);
    m_titleText.setString(label);
    m_titleText.setCharacterSize(theme.getTitleSize());
    m_titleText.setFillColor(theme.getPrimaryColor());
    m_titleText.setFont(m_font);

    // adjust the origin and positioning of label text
    sf::FloatRect label_bounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(label_bounds.left + label_bounds.width / 2,
                           label_bounds.top);

    sf::FloatRect knob_bounds = m_knob.getLocalBounds();

    m_titleText.setPosition(m_knob.getPosition().x, (m_knob.getPosition().y + knob_bounds.height / 2) + theme.getLargePadding());
}







void Knob::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        if (m_knob.getGlobalBounds().contains(mousePosition))
        {
            m_isTurning = true;
            m_previousMousePosition = mousePosition;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        m_isTurning = false;
    }
    else if (event.type == sf::Event::MouseMoved && m_isTurning)
    {
        // Compute the change in mouse Y movement
        float delta_y = m_previousMousePosition.y - mousePosition.y; // Positive for upward movement
        
        // Store current mouse position for next frame comparison
        m_previousMousePosition = mousePosition;

        // Map deltaY to a value change
        float range = m_maxValue - m_minValue;
        float new_value = m_value + (delta_y / m_radius) * range * m_sensitivity;

        // Snap the new value to the nearest increment
        new_value = std::round(new_value / m_increment) * m_increment;
        m_value = std::clamp(new_value, m_minValue, m_maxValue);

        setValue(m_value);

        // Update the indicator and text position
        positionIndicator(m_value);
        positionValueText(m_value);
    }
}








void Knob::positionIndicator(float value)
{
    float indicator_radian = calculate_angle(value) * (M_PI / 180.0f);

    // set indicator position
    m_indicator.setPosition(
        m_position.x + std::cos(indicator_radian) * (m_radius - m_indicator.getRadius()),
        m_position.y - std::sin(indicator_radian) * (m_radius - m_indicator.getRadius())); // Invert Y-axis

    m_indicator.setRotation(calculate_angle(value));
}
void Knob::positionValueText(float value)
{
    float value_radian = calculate_radian(value);

    // Distance from the knob center to where the value should be rendered
    float text_distance = m_radius + (m_padding / 2);

    // Calculate the position of the text along the circular path
    sf::Vector2f text_position(
        m_position.x + std::cos(value_radian) * text_distance,
        m_position.y - std::sin(value_radian) * text_distance // Invert Y-axis
    );

    // Set the value text to display the current knob value
    std::stringstream ss;
    ss << value;
    m_valueText.setString(ss.str());

    // Get text bounds for positioning adjustment
    sf::FloatRect text_bounds = m_valueText.getLocalBounds();

    // Dynamically calculate the origin based on the angle
    float cos_angle = std::cos(value_radian);
    float sin_angle = std::sin(value_radian);

    // Adjust the origin to ensure the text stays 25px from the edge of the knob
    float origin_x = (text_bounds.width / 2.0f) * (1.0f - cos_angle);  // Shift left or right
    float origin_y = (text_bounds.height / 2.0f) * (1.0f + sin_angle); // Shift up or down

    // Set the calculated origin dynamically
    m_valueText.setOrigin(origin_x, origin_y);

    // Set the calculated position of the value text
    m_valueText.setPosition(text_position);
}

void Knob::draw(sf::RenderWindow& window)
{
    window.draw(m_knob);
    window.draw(m_indicator);

    window.draw(m_titleText);
    window.draw(m_valueText);
}
void Knob::setValue(float value)
{
    if(value >= m_minValue && value <= m_maxValue)
        m_value = value;
}
void Knob::setIncrement(float increment)
{
    if(increment > 0.0f)
        m_increment = increment;
}
void Knob::setSensitivity(float sensitivity)
{
    if(sensitivity < 1.0f && sensitivity > m_increment)
        m_sensitivity = sensitivity;
}

float Knob::getValue() const
{
    return m_value;
}
float Knob::getMinValue() const
{
    return m_minValue;
}
float Knob::getMaxValue() const
{
    return m_maxValue;
}
bool Knob::isTurning() const
{
    return m_isTurning;
}

float Knob::normalize_value(float value) const
{
    // returns a value from 0 to 1 based on knob range and current value
    return (value - m_minValue) / (m_maxValue - m_minValue);
}
float Knob::calculate_angle(float value)
{
    return START_ANGLE + normalize_value(value) * (END_ANGLE - START_ANGLE);
}
float Knob::calculate_radian(float value)
{
    return calculate_angle(value) * (M_PI / 180.0f);
}

void Knob::setHoverColor(sf::Color color)
{
    m_hoverColor = color;
}
void Knob::setDisabledColor(sf::Color color)
{
    m_disabledColor = color;
}

void Knob::setDisabled(bool disabled)
{
    m_isDisabled = disabled;
    m_knob.setFillColor(m_isDisabled ? m_disabledColor : m_knobColor);
}
bool Knob::isHovered() const
{
    return m_isHovered;
}
bool Knob::isDisabled() const
{
    return m_isDisabled;
}

void Knob::setPosition(sf::Vector2f position)
{
    m_position = position;

    m_knob.setPosition(position);
    positionTitle();

    positionIndicator(m_value);
    positionValueText(m_value);
}

void Knob::positionTitle()
{
    // adjust the origin and positioning of label text
    sf::FloatRect title_bounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(title_bounds.left + title_bounds.width / 2,
                           title_bounds.top);

    sf::FloatRect knob_bounds = m_knob.getLocalBounds();

    m_titleText.setPosition(m_knob.getPosition().x, (m_knob.getPosition().y + knob_bounds.height / 2) + (m_padding));
}
