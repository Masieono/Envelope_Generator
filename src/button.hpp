

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Button
{
    public:

        enum class ButtonShape
        {
            Rectangle,
            Circle
        };

        enum class ButtonType
        {
            Momentary,
            Latching,
            StateCycling
        };

        enum class TitleLocation
        {
            Above,
            Centered,
            Below
        };

        // app manager constructor
        Button(ButtonShape shape, ButtonType type, TitleLocation location, const sf::String& title, float WidthOrRadius, float height = 0);

        Button(float x, float y, ButtonShape shape, ButtonType type, const sf::String& title, float WidthOrRadius, float height = 0);

        ~Button() = default;

        void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);

        // setters
        void setUnpressedColor(sf::Color color);
        void setPressedColor(sf::Color color);
        void setHoverColor(sf::Color color);
        void setDisabledColor(sf::Color color);
        void setBorderColor(sf::Color color);
        void setDisabled(bool disabled);        
        void setTextAlignment(sf::Vector2f position);
        void setPosition(sf::Vector2f position);
        void setStates(const std::vector<sf::String>& labels, const std::vector<sf::Color>& colors);

        // getters
        ButtonType getButtonType() const;
        sf::Color getPressedColor() const;
        sf::Color getUnpressedColor() const;
        sf::Color getHoverColor() const;
        sf::Color getDisabledColor() const;

        bool isPressed() const;
        bool isHovered() const;
        bool isDisabled() const;
        int getButtonState() const;

    private: 

        sf::Vector2f m_position;

        ButtonShape m_buttonShape;      // rectangular or circular
        ButtonType m_buttonType;        // momentary, latching, or state cycling   
        TitleLocation m_titleLocation;  // where does the button label display

        float m_width;
        float m_height;
        float m_radius;
        float m_padding;

        sf::Font m_font;
        sf::Text m_titleText;      // displayed on or below the button
        std::vector<sf::Text> m_cycleText;     // descriptors for each button state

        sf::Color m_unpressedColor;
        sf::Color m_pressedColor;
        sf::Color m_hoverColor;
        sf::Color m_disabledColor = sf::Color(100, 100, 100);
        sf::Color m_borderColor = sf::Color::Transparent;
        std::vector<sf::Color> m_stateColors;

        int m_currentState = 0;

        std::unique_ptr<sf::Shape> m_shape;

        bool m_isPressed = false;
        bool m_isHovered = false;
        bool m_isDisabled = false;

        void positionTitle(Button::TitleLocation location, sf::FloatRect button_bounds);
        void updateButtonState();
        void cycleState();
};

#endif // BUTTON_HPP