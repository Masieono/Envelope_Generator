

#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SFML/graphics.hpp>

class Slider
{
    public:
        Slider(float minValue, float maxValue, const std::string& title);

        Slider(float x, float y, float width, float height, float minValue, float maxValue, bool isHorizontal, const std::string& title);
        
        void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);

        void setPosition(sf::Vector2f startPoint, sf::Vector2f endPoint);
        void setValue(float value);
        void setStep(float increment);
        void setDisabled(bool disabled);

        float getValue() const;
        float getMinValue() const;
        float getMaxValue() const;
        bool isDragging() const;
        bool isDisabled() const;

    private:
        sf::Vector2f m_position;
        float m_width;              // width of slider module
        float m_height;             // for vertical sliders
        float m_minValue;          // min value of slider
        float m_maxValue;          // max value of slider
        float m_value;              // value setting of slider
        float m_step;               // for setting precision of slider
        float m_padding;            // distance between elements, taken from theme

        sf::RectangleShape m_background;
        sf::CircleShape m_handle;   // the sliding thing you click and drag
        
        sf::Font m_font;            // for displaying the text
        sf::Text m_valueText;    // current value text
        sf::Text m_titleText;

        sf::Color m_handleColor;
        sf::Color m_backgroundColor;
        sf::Color m_textColor;
        sf::Color m_disabledColor;
        sf::Color m_hoverColor;

        bool m_dragging;
        bool m_isHorizontal;
        bool m_isDisabled;

        void updateValueText();
        void updateTitleText();
};

#endif // SLIDER_HPP