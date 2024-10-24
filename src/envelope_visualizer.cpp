

#include <iostream>
#include <cmath>

#include "theme.hpp"
#include "envelope_visualizer.hpp"
#include "envelope_generator.hpp"

// constructor for the envelope generator app
EnvelopeVisualizer::EnvelopeVisualizer(const Envelope& envelope)
{
    Theme& theme = theme.getInstance();

    float smallPadding = theme.getSmallPadding() / 3.f;

    m_backgroundColor = theme.getBackgroundColor();
    m_borderColor = theme.getBorderColor();
    m_rectColor = theme.getAccentColor();
    m_lineColor = theme.getPrimaryColor();
    m_indicatorColor = theme.getSecondaryColor();

    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineColor(m_borderColor);
    m_background.setOutlineThickness(smallPadding);


    // set rect sections of envelope, 
    m_attackRect.setFillColor(sf::Color::Transparent);
    m_decayRect.setFillColor(sf::Color::Transparent);
    m_sustainRect.setFillColor(sf::Color::Transparent);
    m_releaseRect.setFillColor(sf::Color::Transparent);

    positionPhaseRectangles(envelope);

    // set dividing lines of rect sections
    m_phaseDividers.setPrimitiveType(sf::Lines);
    positionPhaseDividers(envelope);

    // initialize envelope curves
    m_attackCurve.setPrimitiveType(sf::LinesStrip);
    generateAttackCurve(envelope);

    m_decayCurve.setPrimitiveType(sf::LinesStrip);
    generateDecayCurve(envelope);

    m_sustainLine.setPrimitiveType(sf::LinesStrip);
    generateSustainLine(envelope);

    m_releaseCurve.setPrimitiveType(sf::LinesStrip);
    generateReleaseCurve(envelope);    

    // Initialize envelope indicator
    m_envelopeIndicator.setRadius(smallPadding);
    m_envelopeIndicator.setOrigin(sf::Vector2f(m_envelopeIndicator.getRadius(), m_envelopeIndicator.getRadius()));
    m_envelopeIndicator.setFillColor(m_indicatorColor);    

    // initialize the gauges and their indicators
    m_xGauge.setFillColor(m_lineColor);
    
    m_xIndicator.setRadius(smallPadding);
    m_xIndicator.setOrigin(sf::Vector2f(m_xIndicator.getRadius(), m_xIndicator.getRadius()));
    m_xIndicator.setFillColor(m_indicatorColor);

    m_yGauge.setFillColor(m_lineColor);

    m_yIndicator.setRadius(smallPadding);
    m_yIndicator.setOrigin(sf::Vector2f(m_yIndicator.getRadius(), m_yIndicator.getRadius()));
    m_yIndicator.setFillColor(m_indicatorColor);    
}

// dusty old constructor 
EnvelopeVisualizer::EnvelopeVisualizer(const sf::Vector2f& position, const sf::Vector2f& size, const Envelope& envelope)
: m_position(position)
, m_size(size)
{
    Theme& theme = theme.getInstance();

    float smallPadding = theme.getSmallPadding() / 3.f;

    m_backgroundColor = theme.getBackgroundColor();
    m_borderColor = theme.getBorderColor();
    m_rectColor = theme.getAccentColor();
    m_lineColor = theme.getPrimaryColor();
    m_indicatorColor = theme.getSecondaryColor();

    // initialize background
    m_background.setSize(size);
    m_background.setPosition(m_position);
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineColor(m_borderColor);
    m_background.setOutlineThickness(smallPadding);

    // initialize the envelope container

    float rightSide = m_background.getSize().x * (1.f / 13.f);
    float leftSide = m_background.getSize().x * (11.f / 13.f);

    float topSide = m_background.getSize().y * 0.1f;
    float bottomSide = m_background.getSize().y * 0.8f;

    m_envelopeContainer.setSize(sf::Vector2f(leftSide - rightSide, bottomSide - topSide));
    m_envelopeContainer.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + topSide));

    // set rect sections of envelope, 
    m_attackRect.setFillColor(sf::Color::Transparent);
    m_decayRect.setFillColor(sf::Color::Transparent);
    m_sustainRect.setFillColor(sf::Color::Transparent);
    m_releaseRect.setFillColor(sf::Color::Transparent);

    positionPhaseRectangles(envelope);

    // set dividing lines of rect sections
    m_phaseDividers.setPrimitiveType(sf::Lines);
    positionPhaseDividers(envelope);

    // initialize envelope curve(s)
        // Attack
    m_attackCurve.setPrimitiveType(sf::LinesStrip);
    generateAttackCurve(envelope);

        // Decay
    m_decayCurve.setPrimitiveType(sf::LinesStrip);
    generateDecayCurve(envelope);

        // Sustain
    m_sustainLine.setPrimitiveType(sf::LinesStrip);
    generateSustainLine(envelope);

        // Release
    m_releaseCurve.setPrimitiveType(sf::LinesStrip);
    generateReleaseCurve(envelope);    

    // Initialize envelope indicator
    m_envelopeIndicator.setRadius(smallPadding);
    m_envelopeIndicator.setOrigin(sf::Vector2f(m_envelopeIndicator.getRadius(), m_envelopeIndicator.getRadius()));
    m_envelopeIndicator.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + bottomSide));
    m_envelopeIndicator.setFillColor(m_indicatorColor);    

    // initialize the gauges and their indicators

        // x gauge and indicator

    float yCoord = m_background.getSize().y * 0.9f;

    m_xGauge.setSize(sf::Vector2f(m_envelopeContainer.getSize().x, smallPadding / 2.f));
    m_xGauge.setOrigin(sf::Vector2f(0.0f, m_xGauge.getSize().y / 2.f ));
    m_xGauge.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + yCoord));
    m_xGauge.setFillColor(m_lineColor);
    
    m_xIndicator.setRadius(smallPadding);
    m_xIndicator.setOrigin(sf::Vector2f(m_xIndicator.getRadius(), m_xIndicator.getRadius()));
    m_xIndicator.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + yCoord));
    m_xIndicator.setFillColor(m_indicatorColor);

        // y gauge and indicator

    float xCoord = m_background.getSize().x * (12.f / 13.f);

    m_yGauge.setSize(sf::Vector2f(smallPadding / 2.f, m_envelopeContainer.getSize().y));
    m_yGauge.setOrigin(sf::Vector2f(m_yGauge.getSize().x / 2, 0.0f));
    m_yGauge.setPosition(sf::Vector2f(m_background.getPosition().x + xCoord, m_background.getPosition().y + topSide));
    m_yGauge.setFillColor(m_lineColor);

    m_yIndicator.setRadius(smallPadding);
    m_yIndicator.setOrigin(sf::Vector2f(m_yIndicator.getRadius(), m_yIndicator.getRadius()));
    m_yIndicator.setPosition(sf::Vector2f(m_background.getPosition().x + xCoord, m_background.getPosition().y + bottomSide));
    m_yIndicator.setFillColor(m_indicatorColor);

}

EnvelopeVisualizer::~EnvelopeVisualizer() {}

void EnvelopeVisualizer::update(const Envelope& envelope)
{
    // Recalculate envelope shape based on updated parameters
    calculateEnvelopeShape(envelope);

    // Position indicators
    updateIndicators(envelope);

    // Position envelope sections
    positionPhaseRectangles(envelope);
    positionPhaseDividers(envelope);

    // Color animation to show the current envelope phase
    sf::Color color = sf::Color::Magenta;

    switch (envelope.getPhase())
    {
        case Envelope::Phase::INACTIVE:
        {
                m_attackRect.setFillColor(sf::Color::Transparent);
                m_decayRect.setFillColor(sf::Color::Transparent);
                m_sustainRect.setFillColor(sf::Color::Transparent);
                m_releaseRect.setFillColor(sf::Color::Transparent);
            break;
        }
        case Envelope::Phase::ATTACK:
        {
                m_attackRect.setFillColor(color);
                m_decayRect.setFillColor(sf::Color::Transparent);
                m_sustainRect.setFillColor(sf::Color::Transparent);
                m_releaseRect.setFillColor(sf::Color::Transparent);
            break;
        }
        case Envelope::Phase::DECAY:
        {
                m_attackRect.setFillColor(sf::Color::Transparent);
                m_decayRect.setFillColor(color);
                m_sustainRect.setFillColor(sf::Color::Transparent);
                m_releaseRect.setFillColor(sf::Color::Transparent);
            break;
        }
        case Envelope::Phase::SUSTAIN:
        {
                m_attackRect.setFillColor(sf::Color::Transparent);
                m_decayRect.setFillColor(sf::Color::Transparent);
                m_sustainRect.setFillColor(color);
                m_releaseRect.setFillColor(sf::Color::Transparent);
            break;
        }
        case Envelope::Phase::RELEASE:
        {
                m_attackRect.setFillColor(sf::Color::Transparent);
                m_decayRect.setFillColor(sf::Color::Transparent);
                m_sustainRect.setFillColor(sf::Color::Transparent);
                m_releaseRect.setFillColor(color);
            break;
        }
        default:
        {
            std::cerr << "Invalid envelope phase" << std::endl;
            break;
        }
    }
}

void EnvelopeVisualizer::draw(sf::RenderWindow& window)
{
    window.draw(m_background);
    
    window.draw(m_xGauge);
    window.draw(m_xIndicator);

    window.draw(m_yGauge);
    window.draw(m_yIndicator);

    window.draw(m_attackRect);
    window.draw(m_decayRect);
    window.draw(m_sustainRect);
    window.draw(m_releaseRect);

    window.draw(m_phaseDividers);
    
    window.draw(m_attackCurve);
    window.draw(m_decayCurve);
    window.draw(m_sustainLine);
    window.draw(m_releaseCurve);

    window.draw(m_envelopeIndicator);
}

void EnvelopeVisualizer::setPosition(sf::Vector2f position, sf::Vector2f size, const Envelope& envelope)
{
    m_position = position;
    m_size = size;

    m_background.setSize(size);
    m_background.setPosition(m_position);


    Theme& theme = theme.getInstance();
    float smallPadding = theme.getSmallPadding();

    // establish envelope container
    float rightSide = m_background.getSize().x * (1.f / 13.f);
    float leftSide = m_background.getSize().x * (11.f / 13.f);
    float topSide = m_background.getSize().y * 0.1f;
    float bottomSide = m_background.getSize().y * 0.8f;

    m_envelopeContainer.setSize(sf::Vector2f(leftSide - rightSide, bottomSide - topSide));
    m_envelopeContainer.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + topSide));

    positionPhaseRectangles(envelope);
        
    positionPhaseDividers(envelope);
    
    m_envelopeIndicator.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + bottomSide));

    // x gauge positioning
    float yCoord = m_background.getSize().y * 0.9f;
    m_xGauge.setSize(sf::Vector2f(m_envelopeContainer.getSize().x, smallPadding / 2.f));
    m_xGauge.setOrigin(sf::Vector2f(0.0f, m_xGauge.getSize().y / 2.f));
    m_xGauge.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + yCoord));

    m_xIndicator.setPosition(sf::Vector2f(m_background.getPosition().x + rightSide, m_background.getPosition().y + yCoord));
    
    // y gauge positioning
    float xCoord = m_background.getSize().x * (12.f / 13.f);
    m_yGauge.setSize(sf::Vector2f(smallPadding / 2.f, m_envelopeContainer.getSize().y));
    m_yGauge.setOrigin(sf::Vector2f(m_yGauge.getSize().x / 2.f, 0.0f));
    m_yGauge.setPosition(sf::Vector2f(m_background.getPosition().x + xCoord, m_background.getPosition().y + topSide));

    m_yIndicator.setPosition(sf::Vector2f(m_background.getPosition().x + xCoord, m_background.getPosition().y + bottomSide));

}
void EnvelopeVisualizer::calculateEnvelopeShape(const Envelope& envelope)
{
    m_attackCurve.clear();
    m_decayCurve.clear();
    m_sustainLine.clear();
    m_releaseCurve.clear();

    switch(envelope.getEnvelopeType())
    {
        case Envelope::EnvelopeType::ADSR:
        {
            generateAttackCurve(envelope);
            generateDecayCurve(envelope);
            generateSustainLine(envelope);
            generateReleaseCurve(envelope);

            break;
        }
        case Envelope::EnvelopeType::ASR:
        {
            generateAttackCurve(envelope);
            generateSustainLine(envelope);
            generateReleaseCurve(envelope);

            break;
        }
        case Envelope::EnvelopeType::AD:
        {
            generateAttackCurve(envelope);
            generateDecayCurve(envelope);

            break;
        }
        default:
            std::cerr << "Cannot calculate envelope shape, invalid envelope type" << std::endl;
            break;
    }
}
void EnvelopeVisualizer::updateIndicators(const Envelope& envelope)
{
    // get current progress of envelope (0.0 - 1.0)
    float progress = envelope.getProgress();

    // map the x indicator to the xGauge based on progress
    float xX = m_xGauge.getPosition().x + (progress * m_xGauge.getSize().x);
    float yX = m_xGauge.getPosition().y;
    
    m_xIndicator.setPosition(sf::Vector2f(xX, yX));

    // get current amplitude of envelope (0.0 - 1.0)
    float amplitude = envelope.getAmplitude();

    // map the y indicator to the yGauge based on amplitude
    float xY = m_yGauge.getPosition().x;
    float yY = m_yGauge.getPosition().y + ((1 - amplitude) * m_yGauge.getSize().y);

    m_yIndicator.setPosition(sf::Vector2f(xY, yY));

    // map the envelope indicator to both to map the envelope shape
    m_envelopeIndicator.setPosition(sf::Vector2f(xX, yY));
}

void EnvelopeVisualizer::generateAttackCurve(const Envelope& envelope)
{
    float attackCurve = envelope.getAttackCurve();

    float rightX = m_attackRect.getPosition().x;
    float topY = m_attackRect.getPosition().y;

    float width = m_attackRect.getSize().x;
    float height = m_attackRect.getSize().y;

    float sustainLevel = envelope.getSustainLevel();

    int numSegments = 100;  // higher number = smoother curve

    for (int i = 0; i < numSegments; i++)
    {
        // t goes from 0 to 1 as i progresses
        float t = static_cast<float>(i) / numSegments;

        float y = envelope.getAmplitudeAtTime(Envelope::Phase::ATTACK, t);

        // plot points
        sf::Vector2f point(rightX + t * width, topY + (1 - y) * height);    
        m_attackCurve.append(sf::Vertex(point, m_lineColor));
    }
}
void EnvelopeVisualizer::generateDecayCurve(const Envelope& envelope)
{
    float decayTime = envelope.getDecayTime();
    float decayCurve = envelope.getDecayCurve();

    float rightX = m_decayRect.getPosition().x;
    float topY = m_decayRect.getPosition().y;

    float width = m_decayRect.getSize().x;
    float height = m_decayRect.getSize().y;

    float sustainLevel = envelope.getSustainLevel();

    int numSegments = 100;  // higher number = smoother curve

    for (int i = 0; i < numSegments; i++)
    {
        // t goes from 0 to 1 as i progresses
        float t = static_cast<float>(i) / numSegments;

        float y = envelope.getAmplitudeAtTime(Envelope::Phase::DECAY, t);

        sf::Vector2f point(rightX + t * width, topY + (1 - y) * height);
        m_decayCurve.append(sf::Vertex(point, m_lineColor));
    }
}
void EnvelopeVisualizer::generateSustainLine(const Envelope& envelope)
{
    // already normalized from 0.0 - 1.0
    float sustainLevel = envelope.getSustainLevel();

    float width = m_sustainRect.getSize().x;
    float height = m_sustainRect.getSize().y;

    float xStart = m_sustainRect.getPosition().x;
    float xEnd = xStart + width;

    float yLevel = m_sustainRect.getPosition().y + height - (sustainLevel * height);

    sf::Vector2f startPoint(xStart, yLevel);
    sf::Vector2f endPoint(xEnd, yLevel);

    m_sustainLine.append(sf::Vertex(startPoint, m_lineColor));
    m_sustainLine.append(sf::Vertex(endPoint, m_lineColor));
}
void EnvelopeVisualizer::generateReleaseCurve(const Envelope& envelope)
{
    float releaseCurve = envelope.getReleaseCurve();

    float rightX = m_releaseRect.getPosition().x;
    float topY = m_releaseRect.getPosition().y;

    float width = m_releaseRect.getSize().x;
    float height = m_releaseRect.getSize().y;

    float sustainLevel = envelope.getSustainLevel();

    int numSegments = 100;  // higher number = smoother curve

    for (int i = 0; i < numSegments; i++)
    {
        // t goes from 0 to 1 as i progresses
        float t = static_cast<float>(i) / numSegments;

        float y = envelope.getAmplitudeAtTime(Envelope::Phase::RELEASE, t);

        sf::Vector2f point(rightX + t * width, topY + (1 - y) * height);
        m_releaseCurve.append(sf::Vertex(point, m_lineColor));
    }
}

void EnvelopeVisualizer::positionPhaseRectangles(const Envelope& envelope)
{
    float rightX = m_envelopeContainer.getPosition().x;
    float topY = m_envelopeContainer.getPosition().y;
    float width = m_envelopeContainer.getSize().x;
    float height = m_envelopeContainer.getSize().y;

    // total duration of envelope phases
    float sustainTime = 0.05f;   // magic number for sustainRect width

    // Reset all rectangles to size (0,0) initially
    m_attackRect.setSize(sf::Vector2f(0, 0));
    m_decayRect.setSize(sf::Vector2f(0, 0));
    m_sustainRect.setSize(sf::Vector2f(0, 0));
    m_releaseRect.setSize(sf::Vector2f(0, 0));

    switch (envelope.getEnvelopeType())
    {
        case Envelope::EnvelopeType::ADSR:
        {
            float totalDuration = envelope.getAttackTime() + envelope.getDecayTime() + sustainTime + envelope.getReleaseTime();

            // Attack
            float attackWidth = envelope.getAttackTime() / totalDuration * width;

            m_attackRect.setSize(sf::Vector2f(attackWidth, height));
            m_attackRect.setPosition(rightX, topY);

            // Decay
            float decayWidth = envelope.getDecayTime() / totalDuration * width;
            float decayX = rightX + attackWidth;
            m_decayRect.setSize(sf::Vector2f(decayWidth, height));
            m_decayRect.setPosition(decayX, topY);

            // Sustain
            float sustainWidth = sustainTime / totalDuration * width;
            float sustainX = decayX + decayWidth;
            m_sustainRect.setSize(sf::Vector2f(sustainWidth, height));
            m_sustainRect.setPosition(sustainX, topY);

            // Release
            float releaseWidth = envelope.getReleaseTime() / totalDuration * width;
            float releaseX = sustainX + sustainWidth;
            m_releaseRect.setSize(sf::Vector2f(releaseWidth, height));
            m_releaseRect.setPosition(releaseX, topY);

            break;
        }

        case Envelope::EnvelopeType::ASR:
        {
            float totalDuration = envelope.getAttackTime() + sustainTime + envelope.getReleaseTime();

            // Attack
            float attackWidth = envelope.getAttackTime() / totalDuration * width;
            m_attackRect.setSize(sf::Vector2f(attackWidth, height));
            m_attackRect.setPosition(rightX, topY);

            // Sustain
            float sustainWidth = sustainTime / totalDuration * width;
            float sustainX = rightX + attackWidth;

            m_sustainRect.setSize(sf::Vector2f(sustainWidth, height));
            m_sustainRect.setPosition(sustainX, topY);

            // Release
            float releaseWidth = envelope.getReleaseTime() / totalDuration * width;
            float releaseX = sustainX + sustainWidth;

            m_releaseRect.setSize(sf::Vector2f(releaseWidth, height));
            m_releaseRect.setPosition(releaseX, topY);

            m_decayRect.setSize(sf::Vector2f(0,0));

            break;
        }

        case Envelope::EnvelopeType::AD:
        {
            float totalDuration = envelope.getAttackTime() + envelope.getDecayTime();

            // Attack
            float attackWidth = envelope.getAttackTime() / totalDuration * width;

            m_attackRect.setSize(sf::Vector2f(attackWidth, height));
            m_attackRect.setPosition(rightX, topY);

            // Decay
            float decayWidth = envelope.getDecayTime() / totalDuration * width;
            float decayX = rightX + attackWidth;

            m_decayRect.setSize(sf::Vector2f(decayWidth, height));
            m_decayRect.setPosition(decayX, topY);

            m_sustainRect.setSize(sf::Vector2f(0,0));
            m_releaseRect.setSize(sf::Vector2f(0,0));

            break;
        }

        default:
        {
            std::cerr << "Cannot position phase rectangles. Invalid envelope type." << std::endl;
            break;
        }
    }
}
void EnvelopeVisualizer::positionPhaseDividers(const Envelope& envelope)
{
    float topY = m_envelopeContainer.getPosition().y;
    float bottomY = m_envelopeContainer.getPosition().y + m_envelopeContainer.getSize().y;

    float leftX = m_envelopeContainer.getPosition().x;
    float rightX = m_envelopeContainer.getPosition().x + m_envelopeContainer.getSize().x;

    switch(envelope.getEnvelopeType())
    {        
        case Envelope::EnvelopeType::ADSR:
        {
            m_phaseDividers.clear();
            m_phaseDividers.resize(10); // five lines total

            // Right side
            m_phaseDividers[0].position = sf::Vector2f(leftX, topY);
            m_phaseDividers[1].position = sf::Vector2f(leftX, bottomY);

            // AD divider
            float arX = leftX + m_attackRect.getSize().x;

            m_phaseDividers[2].position = sf::Vector2f(arX, topY);
            m_phaseDividers[3].position = sf::Vector2f(arX, bottomY);

            // DS divider
            float drX = arX + m_decayRect.getSize().x;

            m_phaseDividers[4].position = sf::Vector2f(drX, topY);
            m_phaseDividers[5].position = sf::Vector2f(drX, bottomY);

            // SR divider
            float srX = drX + m_sustainRect.getSize().x;

            m_phaseDividers[6].position = sf::Vector2f(srX, topY);
            m_phaseDividers[7].position = sf::Vector2f(srX, bottomY);

            // Left side
            m_phaseDividers[8].position = sf::Vector2f(rightX, topY);
            m_phaseDividers[9].position = sf::Vector2f(rightX, bottomY);

            break;               
        }

        case Envelope::EnvelopeType::ASR:
        {
            m_phaseDividers.resize(8); // four lines total

            // Right side
            m_phaseDividers[0].position = sf::Vector2f(leftX, topY);
            m_phaseDividers[1].position = sf::Vector2f(leftX, bottomY);

            // AS divider
            float asX = m_sustainRect.getPosition().x;

            m_phaseDividers[2].position = sf::Vector2f(asX, topY);
            m_phaseDividers[3].position = sf::Vector2f(asX, bottomY);

            // SR divider
            float srX = m_releaseRect.getPosition().x;

            m_phaseDividers[4].position = sf::Vector2f(srX, topY);
            m_phaseDividers[5].position = sf::Vector2f(srX, bottomY);

            // Left side
            m_phaseDividers[6].position = sf::Vector2f(rightX, topY);
            m_phaseDividers[7].position = sf::Vector2f(rightX, bottomY);

            break;               
        }

        case Envelope::EnvelopeType::AD:
        {
            m_phaseDividers.resize(6); // three lines total

            // Right side
            m_phaseDividers[0].position = sf::Vector2f(leftX, topY);
            m_phaseDividers[1].position = sf::Vector2f(leftX, bottomY);

            // AD divider
            float adX = m_decayRect.getPosition().x;

            m_phaseDividers[2].position = sf::Vector2f(adX, topY);
            m_phaseDividers[3].position = sf::Vector2f(adX, bottomY);

            // Left side
            m_phaseDividers[4].position = sf::Vector2f(rightX, topY);
            m_phaseDividers[5].position = sf::Vector2f(rightX, bottomY);

            break;                
        }

        default:
            std::cerr << "Cannot position phase dividers. Invalid envelope type" << std::endl;
            // invalid envelope type error handling
            break;
    }

    // set colors of lines
    for(int i = 0; i < m_phaseDividers.getVertexCount(); i++)
    {
        m_phaseDividers[i].color = m_lineColor;
    }
}
