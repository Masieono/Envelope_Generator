#ifndef THEME_HPP
#define THEME_HPP

#include <SFML/Graphics.hpp>

class Theme
{
    public:

        // singleton access
        static Theme& getInstance();

        // Font
        bool loadFont(const std::string& fontFile);
        const sf::Font& getFont() const;

        unsigned int getTitleSize() const;
        unsigned int getLabelSize() const;

        // Default colors
        sf::Color getPrimaryColor() const;
        sf::Color getSecondaryColor() const;
        sf::Color getAccentColor() const;
        sf::Color getDisabledColor() const;
        sf::Color getHoverColor() const;
        sf::Color getBorderColor() const;
        sf::Color getBackgroundColor() const;
        sf::Color getShadowColor() const;

        // Other getters
        float getMargin() const;
        float getSmallPadding() const;
        float getLargePadding() const;
        float getOpacity() const;
        float getCornerRadius() const;
        float getAnimationSpeed() const;
        // sf::Vector2f getShadowOffset() const;

    private:

        // Constructor private for singleton
        Theme();

        // Font
        sf::Font m_font;

        unsigned int m_titleSize;
        unsigned int m_labelSize;

        // colors
        sf::Color m_primaryColor;
        sf::Color m_secondaryColor;
        sf::Color m_accentColor;
        sf::Color m_disabledColor;
        sf::Color m_hoverColor;
        sf::Color m_borderColor;
        sf::Color m_backgroundColor;
        sf::Color m_shadowColor;

        // other attributes
        float m_margin;
        float m_smallPadding;
        float m_largePadding;
        float m_opacity;
        float m_cornerRadius;
        float m_animationSpeed;

        // sf::Vector2f m_shadowOffset;
};

#endif //THEME_HPP