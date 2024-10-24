#include <SFML/System.hpp>
#include <iostream>
#include "envelope_generator.hpp"  // Make sure this path matches your project structure

void printEnvelopeState(const Envelope& envelope) {
    std::cout << "Phase: " << envelope.getPhase() << "\n";
    std::cout << "Amplitude: " << envelope.getAmplitude() << "\n";
    std::cout << "Progress: " << envelope.getProgress() << "\n\n";
}

int main() {
    // Instantiate an envelope
    Envelope envelope;

    // Set some test parameters
    envelope.setAttackTime(2.0f);
    envelope.setDecayTime(1.5f);
    envelope.setSustainLevel(0.7f);
    envelope.setReleaseTime(3.0f);

    // Trigger the envelope (starts at the Attack phase)
    envelope.trigger();
    
    // Simulate an update loop for a few seconds to observe changes
    sf::Clock clock;
    float testDuration = 10.0f;  // Run for 10 seconds
    while (clock.getElapsedTime().asSeconds() < testDuration) {
        // Update the envelope state
        envelope.update();

        // Print the current state of the envelope
        printEnvelopeState(envelope);

        // Simulate a note release after 5 seconds
        if (clock.getElapsedTime().asSeconds() > 5.0f && envelope.getPhase() == Envelope::SUSTAIN) {
            envelope.release();
        }

        // Add a short delay to avoid spamming output too quickly
        sf::sleep(sf::milliseconds(100));
    }

    // Reset the envelope after testing
    envelope.reset();
    
    // Final state output
    std::cout << "Envelope reset.\n";
    printEnvelopeState(envelope);

    return 0;
}
