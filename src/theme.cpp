

#include <iostream>

#include "theme.hpp"

// Singleton instance
Theme& Theme::getInstance()
{
    static Theme instance;
    return instance;
}

// font
bool Theme::loadFont(const std::string& fontFile)
{
    return m_font.loadFromFile(fontFile);
}

const sf::Font& Theme::getFont() const 
{ 
    return m_font; 
}

unsigned int Theme::getTitleSize() const 
{ 
    return m_titleSize; 
}
unsigned int Theme::getLabelSize() const
{
    return m_labelSize;
}

// colors
sf::Color Theme::getPrimaryColor() const 
{ 
    return m_primaryColor; 
}
sf::Color Theme::getSecondaryColor() const 
{ 
    return m_secondaryColor; 
}
sf::Color Theme::getAccentColor() const
{
    return m_accentColor;
}
sf::Color Theme::getDisabledColor() const 
{ 
    return m_disabledColor; 
}
sf::Color Theme::getHoverColor() const 
{ 
    return m_hoverColor;
}
sf::Color Theme::getBorderColor() const
{
    return m_borderColor;
}
sf::Color Theme::getBackgroundColor() const
{
    return m_backgroundColor;
}
sf::Color Theme::getShadowColor() const
{
    return m_shadowColor;
}

float Theme::getMargin() const
{
    return m_margin;
}
float Theme::getSmallPadding() const 
{ 
    return m_smallPadding; 
}
float Theme::getLargePadding() const
{
    return m_largePadding;
}
float Theme::getOpacity() const
{
    return m_opacity;
}
float Theme::getCornerRadius() const
{
    return m_cornerRadius;
}
float Theme::getAnimationSpeed() const
{
    return m_animationSpeed;
}

// Private constructor initializing defaults
Theme::Theme()
: m_titleSize(20)
, m_labelSize(14)
, m_primaryColor(sf::Color(249, 242, 237))      // white-ish
, m_secondaryColor(sf::Color(248, 116, 116))    // red-ish
, m_accentColor(sf::Color(255, 181, 98))        // orange-ish
, m_disabledColor(sf::Color(100, 100, 100))     // gray
, m_hoverColor(sf::Color(255, 201, 60))    // yellow
, m_borderColor(sf::Color())
, m_backgroundColor(sf::Color(22, 28, 31))
, m_shadowColor(sf::Color(50, 50, 50, 128)) // semi-transparent
, m_margin(5.0f)
, m_smallPadding(20.0f)
, m_largePadding(25.0f)
, m_opacity(1.0f)
, m_cornerRadius(5.0f)
, m_animationSpeed(0.2f)
// , m_shadowOffset = sf::Vector2f(3.0f, 3.0f)
{
    // load font
    if (!Theme::loadFont("../assets/MusticaPro-SemiBold.otf"))
    {
        std::cerr << "Failed to load font." << std::endl;
    }

}
