#ifndef KNOB_HPP
#define KNOB_HPP

#include <SFML/Graphics.hpp>

class Knob
{
    public:

        enum class KnobType
        {
            Linear,
            Centered
        };

        // app manager constructor
        Knob(KnobType type, float min, float max, float init_value, float radius, const std::string& label);

        // old constructor
        Knob(float x, float y, float radius, float min, float max, float init_value, const std::string& label);

        void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);

        // getters
        float getValue() const;
        float getMinValue() const;
        float getMaxValue() const;
        bool isTurning() const;

        // setters
        void setValue(float value);
        void setIncrement(float increment);
        void setSensitivity(float sensitivity);
        void setPosition(sf::Vector2f position);

        void setHoverColor(sf::Color color);
        void setDisabledColor(sf::Color color);
        void setDisabled(bool disabled);

        bool isHovered() const;
        bool isDisabled() const;

    private:

        KnobType m_knobType;
        sf::Vector2f m_position;    // positioning
        float m_radius;             // size
        float m_minValue;
        float m_maxValue;
        float m_value;
        float m_sensitivity;
        float m_increment;
        float m_padding;

        sf::CircleShape m_knob;
        sf::CircleShape m_indicator;

        sf::Vector2f m_previousMousePosition;

        sf::Font m_font;            // for displaying the text
        sf::Text m_valueText;      // current value text
        sf::Text m_titleText;      // title/label for knob

        sf::Color m_knobColor;
        sf::Color m_indicatorColor;
        sf::Color m_textColor;
        sf::Color m_hoverColor;
        sf::Color m_disabledColor;
        sf::Color m_borderColor;

        bool m_isTurning;
        bool m_isHovered;
        bool m_isDisabled;

        void positionTitle();
        void positionIndicator(float value);
        void positionValueText(float value);

        float normalize_value(float value) const;   // helper functions
        float calculate_angle(float value);
        float calculate_radian(float value);

        static constexpr float START_ANGLE = 240.0f;    // 7 o'clock position
        static constexpr float END_ANGLE = -60.0f;      // 5 o'clock position
};

#endif //KNOB_HPP