

#ifndef ENVELOPE_VISUALIZER_HPP
#define ENVELOPE_VISUALIZER_HPP

#include <SFML/Graphics.hpp>
#include "envelope_generator.hpp"


class EnvelopeVisualizer
{
    public: 
        EnvelopeVisualizer(const Envelope& envelope);

        EnvelopeVisualizer(const sf::Vector2f& position, const sf::Vector2f& size, const Envelope& envelope);
        ~EnvelopeVisualizer();

        void update(const Envelope& envelope);  // update envelope visualizer based on envelope
        void draw(sf::RenderWindow& window);    // draw visualizer and progress animation

        void setPosition(sf::Vector2f position, sf::Vector2f size, const Envelope& envelope);

    private:
        
        sf::Vector2f m_position;
        sf::Vector2f m_size;

        void calculateEnvelopeShape(const Envelope& envelope);
        void updateIndicators(const Envelope& envelope);
        void updateGauges(const Envelope& envelope);

        // visual elements
        sf::RectangleShape m_frame;
        sf::RectangleShape m_background;

        // Circle indicator tracks along the rectangle gauge
        sf::VertexArray m_xGauge{sf::LineStrip, 3};
        sf::VertexArray m_yGauge{sf::LineStrip, 3};
        
        sf::CircleShape m_xIndicator, m_yIndicator, m_envelopeIndicator;
        sf::Color m_indicatorColor;

        sf::RectangleShape m_envelopeContainer;

        sf::RectangleShape m_attackRect;
        sf::RectangleShape m_decayRect;
        sf::RectangleShape m_sustainRect;
        sf::RectangleShape m_releaseRect;
        void positionPhaseRectangles(const Envelope& envelope);

        sf::VertexArray m_phaseDividers{sf::Lines};
        void positionPhaseDividers(const Envelope& envelope);

        sf::VertexArray m_attackCurve{sf::LineStrip};
        sf::VertexArray m_decayCurve{sf::LineStrip};
        sf::VertexArray m_sustainLine{sf::LineStrip};
        sf::VertexArray m_releaseCurve{sf::LineStrip};

        void generateAttackCurve(const Envelope& envelope);
        void generateDecayCurve(const Envelope& envelope);
        void generateSustainLine(const Envelope& envelope);
        void generateReleaseCurve(const Envelope& envelope);

        // color parameters
        sf::Color m_backgroundColor;
        sf::Color m_borderColor;
        sf::Color m_rectColor;
        sf::Color m_lineColor;
};

#endif // ENVELOPE_VISUALIZER_HPP
