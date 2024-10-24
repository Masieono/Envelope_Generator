

#ifndef ENVELOPE_GENERATOR_HPP
#define ENVELOPE_GENERATOR_HPP

/*
    Functionality for an envelope generator

    To be used in sound synthesis, envelope visualization,
    anything that has to do with a musical envelope.
*/

class Envelope
{
    public:

        enum class EnvelopeType
        {
            ADSR,       // fully shaped envelope
            ASR,        // for gates (note ON and OFF functionality)
            AD          // for triggers (no sustain functionality)
        };

        Envelope();     // constructor
        ~Envelope();    // destructor

        enum Phase
        {
            INACTIVE,
            ATTACK,
            DECAY,
            SUSTAIN,
            RELEASE, 
        };

        // setters
        void setAttackTime(float attackTime);
        void setAttackCurve(float curve);
        void setDecayTime(float decayTime);
        void setDecayCurve(float curve);
        void setSustainLevel(float sustainLevel);
        void setReleaseTime(float releaseTime);
        void setReleaseCurve(float curve);
        void setLooping(bool isLooping);
        void setEnvelopeType(EnvelopeType type);

        // getters
        float getAttackTime() const;
        float getAttackCurve() const;
        float getDecayTime() const;
        float getDecayCurve() const;
        float getSustainLevel() const;
        float getReleaseTime() const;
        float getReleaseCurve() const;

        EnvelopeType getEnvelopeType() const;
        Phase getPhase() const;

        float getAmplitude() const;
        float getAmplitudeAtTime(Envelope::Phase phase, float normalizedTime) const;
        float getDuration(float sustainTime) const;   // returns total duration of envelope
        float getProgress() const;   // returns envelope progress from 0 to 1

        bool isActive() const;
        bool isLooping() const;

        // helper functions to reduce redundancy in update()
        float calculateAttackPhase(float normalizedTime) const;
        float calculateDecayPhase(float normalizedTime) const;
        float calculateReleasePhase(float normalizedTime) const;

        // methods
        void trigger();             // start an envelope (note = ON)
        void release();             // finish an envelope (note = OFF)
        void reset();               // reset envelope back to init state
        void update(float deltaTime);

    private:

        float scaleCurveNumber(float input);

        EnvelopeType m_envelopeType;
        Phase m_currentPhase;

        float m_attackTime;
        float m_attackCurve;

        float m_decayTime;
        float m_decayCurve;
        
        float m_sustainLevel;
        
        float m_releaseTime;
        float m_releaseCurve;

        float m_currentAmplitude;
        float m_elapsedTime;
        
        bool m_isLooping;
};

#endif // ENVELOPE_GENERATOR_HPP