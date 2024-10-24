

#ifndef APP_MANAGER_HPP
#define APP_MANAGER_HPP

#include <SFML/Graphics.hpp>

#include "slider.hpp"
#include "knob.hpp"
#include "button.hpp"
#include "envelope_visualizer.hpp"
#include "envelope_generator.hpp"

class AppManager
{
    public:
        AppManager();
        ~AppManager();

        void run();

    private:
        void handleEvents();
        void update();
        void render();

        sf::RenderWindow m_window;
        sf::Clock m_clock;

        Envelope m_envelope;

        Slider m_attackSlider;
        Slider m_decaySlider;
        Slider m_sustainSlider;
        Slider m_releaseSlider;

        Knob m_attackSlopeKnob;
        Knob m_decaySlopeKnob;
        Knob m_releaseSlopeKnob;

        Button m_loopButton;
        Button m_envelopeTypeButton;
        Button m_trigButton;
        Button m_resetButton;

        EnvelopeVisualizer m_envelopeVisualizer;
};

#endif