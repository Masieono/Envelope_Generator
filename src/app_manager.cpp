

#include <iostream>

#include "app_manager.hpp"
#include "knob.hpp"
#include "button.hpp"

AppManager::AppManager()
: m_window(sf::VideoMode(750, 750), "Envelope Visualizer 9000")

, m_attackSlider(0.01f, 3.f, "Attack")
, m_decaySlider(0.01f, 3.f, "Decay")
, m_sustainSlider(0.01f, 1.f, "Sustain")
, m_releaseSlider(0.01f, 3.f, "Release")

, m_attackSlopeKnob(Knob::KnobType::Centered, -10.f, 10.f, 0.f, 22.f, "")
, m_decaySlopeKnob(Knob::KnobType::Centered, -10.f, 10.f, 0.f, 22.f, "")
, m_releaseSlopeKnob(Knob::KnobType::Centered, -10.f, 10.f, 0.f, 22.f, "")

, m_envelopeTypeButton(Button::ButtonShape::Rectangle, Button::ButtonType::StateCycling, Button::TitleLocation::Below, "Envelope Type", 125.f, 50.f)
, m_loopButton(Button::ButtonShape::Rectangle, Button::ButtonType::Latching, Button::TitleLocation::Below, "Loop", 125.f, 50.f)
, m_trigButton(Button::ButtonShape::Circle, Button::ButtonType::Momentary, Button::TitleLocation::Centered, "TRIG", 75.f)
, m_resetButton(Button::ButtonShape::Circle, Button::ButtonType::Momentary, Button::TitleLocation::Centered, "RESET", 75.f)

, m_envelope()
, m_envelopeVisualizer(m_envelope)
{
    // get window dimensions
    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2f windowSizeF(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

    // position sliders
    float sliderStartX = (2.f / 29.f) * windowSizeF.x; 
    float sliderEndX = (20.f / 29.f) * windowSizeF.x;

    float attackY = (3.f / 29.f) * windowSizeF.y;
    m_attackSlider.setPosition(sf::Vector2f(sliderStartX, attackY), sf::Vector2f(sliderEndX, attackY));
    m_attackSlider.setValue(m_envelope.getAttackTime());

    float decayY = (6.f / 29.f) * windowSizeF.y;
    m_decaySlider.setPosition(sf::Vector2f(sliderStartX, decayY), sf::Vector2f(sliderEndX, decayY));
    m_decaySlider.setValue(m_envelope.getDecayTime());

    float sustainY = (9.f / 29.f) * windowSizeF.y;
    m_sustainSlider.setPosition(sf::Vector2f(sliderStartX, sustainY), sf::Vector2f(sliderEndX, sustainY));
    m_sustainSlider.setValue(m_envelope.getSustainLevel());

    float releaseY = (12.f / 29.f) * windowSizeF.y;
    m_releaseSlider.setPosition(sf::Vector2f(sliderStartX, releaseY), sf::Vector2f(sliderEndX, releaseY));
    m_releaseSlider.setValue(m_envelope.getReleaseTime());

    // initialize knobs

    float knobX = (23.f / 29.f) * windowSizeF.x;

    m_attackSlopeKnob.setPosition(sf::Vector2f(knobX, attackY));
    m_decaySlopeKnob.setPosition(sf::Vector2f(knobX, decayY));
    m_releaseSlopeKnob.setPosition(sf::Vector2f(knobX, releaseY));

    // initialize buttons

    // envelope type button
    float envButtonX = (5.5f / 29.f) * windowSizeF.x;
    float envButtonY = (14.5f / 29.f) * windowSizeF.y;
    m_envelopeTypeButton.setPosition(sf::Vector2f(envButtonX, envButtonY));

    std::vector<sf::String> labels = {"ADSR", "ASR", "AD"};
    // std::vector<sf::String> colors = {sf::Color(r, g, b), sf::Color(r, g, b), sf::Color(r, g, b)};
    std::vector<sf::Color> colors = {sf::Color::Magenta, sf::Color::Yellow, sf::Color::Green};
    m_envelopeTypeButton.setStates(labels, colors);

    // loop button
    float loopButtonX = (13.5f / 29.f) * windowSizeF.x;
    float loopButtonY = (14.5f / 29.f) * windowSizeF.y;
    m_loopButton.setPosition(sf::Vector2f(loopButtonX, loopButtonY));

    // trig button
    float trigButtonX = (23.f / 29.f) * windowSizeF.x;
    float trigButtonY = (18.f / 29.f) * windowSizeF.y;

    m_trigButton.setPosition(sf::Vector2f(trigButtonX, trigButtonY));
    m_trigButton.setUnpressedColor(sf::Color::Green);

    // reset button
    float resetButtonX = (23.f / 29.f) * windowSizeF.x;
    float resetButtonY = (24.f / 29.f) * windowSizeF.y;

    m_resetButton.setPosition(sf::Vector2f(resetButtonX, resetButtonY));
    m_resetButton.setUnpressedColor(sf::Color::Red);

    // initialize visualizer window
    float vizX = (1.f / 29.f) * windowSizeF.x;
    float vizY = (17.f / 29.f) * windowSizeF.y;

    float vizWidth = ((19.f / 29.f) * windowSizeF.x) - vizX;
    float vizHeight = ((28.f / 29.f) * windowSizeF.y) - vizY;

    m_envelopeVisualizer.setPosition(sf::Vector2f(vizX, vizY), sf::Vector2f(vizWidth, vizHeight), m_envelope);
}

AppManager::~AppManager()
{
}

void AppManager::run()
{
    while(m_window.isOpen())
    {
        handleEvents();    // handle user inputs
        update();           // Update application state
        render();           // Render everything to the screen
    }
}

void AppManager::handleEvents()
{
    sf::Event event;
    while(m_window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        // sliders
        m_attackSlider.handleEvent(event, m_window);
        m_decaySlider.handleEvent(event, m_window);
        m_sustainSlider.handleEvent(event, m_window);
        m_releaseSlider.handleEvent(event, m_window);

        // knobs
        m_attackSlopeKnob.handleEvent(event, m_window);
        m_decaySlopeKnob.handleEvent(event, m_window);
        m_releaseSlopeKnob.handleEvent(event, m_window);

        // buttons
        m_envelopeTypeButton.handleEvent(event, m_window);
        m_loopButton.handleEvent(event, m_window);
        m_trigButton.handleEvent(event, m_window);
        m_resetButton.handleEvent(event, m_window);
    }
}

void AppManager::update()
{
    // sliders
    m_envelope.setAttackTime(m_attackSlider.getValue());
    m_envelope.setDecayTime(m_decaySlider.getValue());
    m_envelope.setSustainLevel(m_sustainSlider.getValue());
    m_envelope.setReleaseTime(m_releaseSlider.getValue());

    // knobs
    m_envelope.setAttackCurve(m_attackSlopeKnob.getValue());
    m_envelope.setDecayCurve(m_decaySlopeKnob.getValue());
    m_envelope.setReleaseCurve(m_releaseSlopeKnob.getValue());

    // buttons
    m_envelope.setEnvelopeType(static_cast<Envelope::EnvelopeType>(m_envelopeTypeButton.getButtonState()));
    m_envelope.setLooping(m_loopButton.isPressed());





    // if the trigger button is pressed,
    if (m_trigButton.isPressed()) 
    {
        // if the envelope is NOT active
        if (!m_envelope.isActive())
        {
            // start an envelope
            m_envelope.trigger();
        }
    }
    // if the trigger button is NOT pressed,
    else
    {
        // If trigger is NOT pressed, handle the release
        // Check if envelope is in an active phase that allows for release
        if (m_envelope.isActive() &&
        (m_envelope.getPhase() == Envelope::Phase::ATTACK || 
         m_envelope.getPhase() == Envelope::Phase::DECAY ||
         m_envelope.getPhase() == Envelope::Phase::SUSTAIN))
        {
            m_envelope.release();
        }

    }



    if (m_resetButton.isPressed()) 
    {
        m_envelope.reset();
    }

    float deltaTime = m_clock.restart().asSeconds();
    m_envelope.update(deltaTime);
    m_envelopeVisualizer.update(m_envelope);

}

void AppManager::render()
{
    m_window.clear();

    // draw sliders
    m_attackSlider.draw(m_window);
    m_decaySlider.draw(m_window);
    m_sustainSlider.draw(m_window);
    m_releaseSlider.draw(m_window);

    // draw knobs
    m_attackSlopeKnob.draw(m_window);
    m_decaySlopeKnob.draw(m_window);
    m_releaseSlopeKnob.draw(m_window);

    // draw buttons
    m_loopButton.draw(m_window);
    m_envelopeTypeButton.draw(m_window);
    m_trigButton.draw(m_window);
    m_resetButton.draw(m_window);

    // draw envelope visualizer area
    m_envelopeVisualizer.draw(m_window);

    m_window.display();
}

