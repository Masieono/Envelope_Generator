
#include <iostream>
#include <SFML/graphics.hpp>

#include "slider.hpp"
#include "knob.hpp"
#include "button.hpp"

#include "envelope_generator.hpp"
#include "envelope_visualizer.hpp"


/*
    Meant for testing UI components
*/

int main()
{
    std::cout << "Program starting..." << std::endl;

    // create a window
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Slider Test");

    // Sliders

//    Slider slider1(75.0f, 50.0f, 300.0f, 10.0f, 0.0f, 1.0f, true, "Horizontal");

    Slider slider1(0.0f, 1.f, "Horizontal");
    slider1.setPosition(sf::Vector2f(75.f, 50.f), sf::Vector2f(375.f, 50.f));

    Slider slider2(0.0f, 10.f, "Vertical");
    slider2.setPosition(sf::Vector2f(550.f, 50.f), sf::Vector2f(550.f, 350.f));

    // Slider slider2(550.0f, 50.0f, 10.0f, 300.0f, 0.0f, 10.0f, false, "Vertical");

    // Knobs

    Knob linear_knob(175.0f, 125.0f, 40.0f, 0.0f, 10.0f, 0.0f, "Linear");


    Knob centered_knob(Knob::KnobType::Centered, -5.f, 5.f, 0.f, 40.f, "Centered");
    centered_knob.setPosition(sf::Vector2f(300.f, 125.f));

    // Buttons

    Button momentary_button(100.0f, 250.0f, Button::ButtonShape::Rectangle, Button::ButtonType::Momentary, "Momentary", 60.0f, 60.0f);
    Button latching_button(200.0f, 250.0f, Button::ButtonShape::Rectangle, Button::ButtonType::Latching, "Latching", 60.0f, 60.0f);
    Button cycling_button(300.0f, 250.0f, Button::ButtonShape::Circle, Button::ButtonType::StateCycling, "Cycling", 30.0f);
    cycling_button.setStates({"ADSR", "ASR", "AD"}, {sf::Color(23, 155, 174), sf::Color(233, 184, 36), sf::Color(229, 155, 223)});

    Button disabled_button(Button::ButtonShape::Circle, Button::ButtonType::Momentary, Button::TitleLocation::Below, "Disabled", 30.f);
    disabled_button.setPosition(sf::Vector2f(400.f, 250.f));
    disabled_button.setDisabled(true);

    // Instantiate a default envelope
    Envelope envelope;

    // Instantiate a visualizer window
    EnvelopeVisualizer viz_window(sf::Vector2f(100, 350), sf::Vector2f(325, 250), envelope);

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            slider1.handleEvent(event, window);
            slider2.handleEvent(event, window);

            linear_knob.handleEvent(event, window);
            centered_knob.handleEvent(event, window);

            momentary_button.handleEvent(event, window);
            latching_button.handleEvent(event, window);
            cycling_button.handleEvent(event, window);
            disabled_button.handleEvent(event, window);

            envelope.setDecayCurve(centered_knob.getValue());
            envelope.setSustainLevel(slider1.getValue());
            envelope.setReleaseTime(linear_knob.getValue());
            envelope.setEnvelopeType(static_cast<Envelope::EnvelopeType>(cycling_button.getButtonState()));

            viz_window.update(envelope);
        }

        // clear the window
        window.clear();

        // draw to window
        slider1.draw(window);
        slider2.draw(window);

        linear_knob.draw(window);
        centered_knob.draw(window);

        momentary_button.draw(window);
        latching_button.draw(window);
        cycling_button.draw(window);
        disabled_button.draw(window);

        viz_window.draw(window);

        // display window contents
        window.display();
    }

    std::cout << "Shutting down..." << std::endl;

    return 0;
}