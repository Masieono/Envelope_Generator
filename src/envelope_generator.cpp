

#include <cmath>
#include <iostream>
#include "envelope_generator.hpp"

Envelope::Envelope()
: m_currentPhase(INACTIVE)
, m_attackTime(1.0f)
, m_attackCurve(1.0f)
, m_decayTime(1.0f)
, m_decayCurve(1.0f)
, m_sustainLevel(0.8f)
, m_releaseTime(1.0f)
, m_releaseCurve(1.0f)
, m_currentAmplitude(0.0f)
, m_elapsedTime(0.0f)
, m_isLooping(false)
, m_envelopeType(Envelope::EnvelopeType::ADSR)
{
}

Envelope::~Envelope() {}

// setters
void Envelope::setEnvelopeType(EnvelopeType type)
{
    m_envelopeType = type;
}
void Envelope::setAttackTime(float attack_time)
{
    m_attackTime = attack_time;
}
void Envelope::setAttackCurve(float curve)
{
    m_attackCurve = scaleCurveNumber(curve);
}
void Envelope::setDecayTime(float decay_time)
{
    m_decayTime = decay_time;
}
void Envelope::setDecayCurve(float curve)
{
    m_decayCurve = scaleCurveNumber(curve);
}
void Envelope::setSustainLevel(float sustain_level)
{
    m_sustainLevel = sustain_level;
}
void Envelope::setReleaseTime(float release_time)
{
    m_releaseTime = release_time;
}
void Envelope::setReleaseCurve(float curve)
{
    m_releaseCurve = scaleCurveNumber(curve);
}
void Envelope::setLooping(bool is_looping)
{
    m_isLooping = is_looping;
}

// getters
float Envelope::getAmplitude() const
{
    return m_currentAmplitude;
}
float Envelope::getAttackTime() const
{
    return m_attackTime;
}
float Envelope::getAttackCurve() const
{
    return m_attackCurve;
}
float Envelope::getDecayTime() const
{
    return m_decayTime;
}
float Envelope::getDecayCurve() const
{
    return m_decayCurve;
}
float Envelope::getSustainLevel() const
{
    return m_sustainLevel;
}
float Envelope::getReleaseTime() const
{
    return m_releaseTime;
}
float Envelope::getReleaseCurve() const
{
    return m_releaseCurve;
}

Envelope::EnvelopeType Envelope::getEnvelopeType() const
{
    return m_envelopeType;
}
Envelope::Phase Envelope::getPhase() const
{
    return m_currentPhase;
}

bool Envelope::isActive() const
{
    return m_currentPhase != INACTIVE;
}
bool Envelope::isLooping() const
{
    return m_isLooping;
}

// helper functions
float Envelope::calculateAttackPhase(float normalizedTime) const
{
    if(m_envelopeType == EnvelopeType::ASR)
    {
        return pow(normalizedTime, m_attackCurve) * m_sustainLevel;
        // return pow(m_elapsedTime / m_attackTime, m_attackCurve) * m_sustainLevel;
    }
    else
    {
        return pow(normalizedTime, m_attackCurve);
        // return pow(m_elapsedTime / m_attackTime, m_attackCurve);
    }
}
float Envelope::calculateDecayPhase(float normalizedTime) const
{
    if (m_envelopeType == EnvelopeType::ADSR)
    {
        return 1.f - pow(normalizedTime, m_decayCurve) * (1.f - m_sustainLevel);
    }
    else
    {
        return 1.f - pow(normalizedTime, m_decayCurve);
    }
}
float Envelope::calculateReleasePhase(float normalizedTime) const
{
    return m_sustainLevel * pow(1.0f - normalizedTime, m_releaseCurve);
}

// methods
void Envelope::trigger()
{
    m_currentPhase = ATTACK;
    m_elapsedTime = 0.0f;
}
void Envelope::release()
{
    /*
        Handle the release phase based on the envelope type.
        - If it's ADSR or ASR, transition to the release phase immediately,
          using the current amplitude as the starting point.
        - If it's AD, it plays out the entire envelope regardless of the release call.
    */

    if (m_currentPhase == RELEASE)
    {
        return;
    }

    m_currentPhase = RELEASE;

    if (m_envelopeType == EnvelopeType::ADSR || m_envelopeType == EnvelopeType::ASR)
    {
        m_sustainLevel = m_currentAmplitude;
    }

    m_elapsedTime = 0.0f;

}
void Envelope::reset()
{
    m_currentPhase = INACTIVE;
    m_elapsedTime = 0.0f;
    m_currentAmplitude = 0.0f;
}

void Envelope::update(float deltaTime)
{
    if (m_currentPhase == INACTIVE)
    {
        m_currentAmplitude = 0.0f;
        return;  // Skip updates if the envelope is inactive
    }

    switch(m_envelopeType)
    {
        case EnvelopeType::ADSR:  // fully shaped envelope
            switch(m_currentPhase)
            {
                case ATTACK:
                {
                    m_elapsedTime += deltaTime;

                    float normalizedTime = std::min((m_elapsedTime / m_attackTime), 1.0f);

                    m_currentAmplitude = calculateAttackPhase(normalizedTime);

                    if (m_elapsedTime >= m_attackTime || 
                       (fabs(m_elapsedTime - m_attackTime) < 0.0001f))
                    {
                        m_currentPhase = DECAY;
                        m_elapsedTime = 0.0f;
                    }
                    break;
                }
                case DECAY:
                {
                    m_elapsedTime += deltaTime;

                    float normalizedTime = std::min((m_elapsedTime / m_decayTime), 1.0f);

                    m_currentAmplitude = calculateDecayPhase(normalizedTime);

                    if (m_elapsedTime >= m_decayTime || 
                       (fabs(m_elapsedTime - m_decayTime) < 0.0001f))
                    {
                        m_currentPhase = SUSTAIN;
                        m_elapsedTime = 0.0f;
                    }
                    break;
                }
                case SUSTAIN:
                {
                    m_currentAmplitude = m_sustainLevel;
                    break;
                }
                case RELEASE:
                {
                    m_elapsedTime += deltaTime;

                    float normalizedTime = std::min((m_elapsedTime / m_releaseTime), 1.0f);

                    m_currentAmplitude = calculateReleasePhase(normalizedTime);

                    if (m_elapsedTime >= m_releaseTime || 
                       (fabs(m_elapsedTime - m_releaseTime) < 0.0001f))
                    {
                        if(m_isLooping)
                        {
                            m_currentPhase = ATTACK;
                            m_elapsedTime = 0.0f;
                        }
                        else
                        {
                            m_currentPhase = INACTIVE;
                            m_currentAmplitude = 0.0f;
                        }
                    }
                    break;
                }
                default:
                    // add error handling
                    break;
            }
            break;
        case EnvelopeType::ASR:   // for gates with note ON and OFF functionality
            switch(m_currentPhase)
            {
                case ATTACK:
                {
                    m_elapsedTime += deltaTime;

                    float normalizedTime = std::min((m_elapsedTime / m_attackTime), 1.0f);

                    m_currentAmplitude = calculateAttackPhase(normalizedTime);

                    if (m_elapsedTime >= m_attackTime || 
                       (fabs(m_elapsedTime - m_attackTime) < 0.0001f))
                    {
                        m_currentPhase = SUSTAIN;
                        m_elapsedTime = 0.0f;
                    }
                break;
                }
                case SUSTAIN:
                {
                    m_currentAmplitude = m_sustainLevel;
                break;
                }
                case RELEASE:
                {
                    m_elapsedTime += deltaTime;

                    float normalizedTime = std::min((m_elapsedTime / m_releaseTime), 1.0f);

                    m_currentAmplitude = calculateReleasePhase(normalizedTime);

                    if (m_elapsedTime >= m_releaseTime || 
                       (fabs(m_elapsedTime - m_releaseTime) < 0.0001f))
                    {
                        if(m_isLooping)
                        {
                            m_currentPhase = ATTACK;
                            m_elapsedTime = 0.0f;
                        }
                        else
                        {
                            m_currentPhase = INACTIVE;
                            m_currentAmplitude = 0.0f;
                        }
                    }
                break;
                }
                default:
                    // add error handling
                    break;
            }
            break;
        case EnvelopeType::AD:    // for triggers
            switch(m_currentPhase)
            {
                case ATTACK:
                {
                    m_elapsedTime += deltaTime;

                    float normalizedTime = std::min((m_elapsedTime / m_attackTime), 1.0f);

                    m_currentAmplitude = calculateAttackPhase(normalizedTime);

                    if (m_elapsedTime >= m_attackTime || 
                       (fabs(m_elapsedTime - m_attackTime) < 0.0001f))
                    {
                        m_currentPhase = DECAY;
                        m_elapsedTime = 0.0f;
                    }
                break;
                }
                case DECAY:
                {
                    m_elapsedTime += deltaTime;

                    float normalizedTime = std::min((m_elapsedTime / m_decayTime), 1.0f);

                    m_currentAmplitude = calculateDecayPhase(normalizedTime);

                    if (m_elapsedTime >= m_decayTime || 
                       (fabs(m_elapsedTime - m_decayTime) < 0.0001f))
                    {
                        if(m_isLooping)
                        {
                            m_currentPhase = ATTACK;
                            m_elapsedTime = 0.0f;
                        }
                        else
                        {
                            m_currentPhase = INACTIVE;
                            m_currentAmplitude = 0.0f;
                        }
                    }
                    break;
                }
                default:
                    // add error handling for invalid AD type
                    break;
            }
            break;
        default:
            // error handling for invalid envelope type
            break;
    }

}

float Envelope::getDuration(float sustain_time) const  // total duration of the envelope
{
    // needs to take the sustained note as an argument for ADSR and ASR
    if (m_currentPhase == INACTIVE) {
        return 0.0f;  // No duration in the inactive phase
    }

    switch(m_envelopeType)
    {
        case EnvelopeType::ADSR:
            if(m_currentPhase == ATTACK)
            {
                return m_elapsedTime;
            }
            else if(m_currentPhase == DECAY)
            {
                return m_attackTime + m_elapsedTime;
            }
            else if(m_currentPhase == SUSTAIN)
            {
                return m_attackTime + m_decayTime + sustain_time;
            }
            else if(m_currentPhase == RELEASE)
            {
                return m_attackTime + m_decayTime + sustain_time + m_elapsedTime;
            }
            break;
        case EnvelopeType::ASR:
            if(m_currentPhase == ATTACK)
            {
                return m_elapsedTime;
            }
            else if(m_currentPhase == SUSTAIN)
            {
                return m_attackTime + sustain_time;
            }
            else if(m_currentPhase == RELEASE)
            {
                return m_attackTime + sustain_time + m_elapsedTime;
            }
            break;
        case EnvelopeType::AD:
            return m_attackTime + m_decayTime;
        default:
            // error handling for invalid envelope type
            return 0.0f;
    }
    return 0.0f;
}
float Envelope::getProgress() const
{
    //    Calculates the progress through the envelope,
    //    normalizes it as a float from 0 to 1.

    //    In the case of sustained notes, the progress 'freezes'
    //    and continues once the note is released. 

    if (m_currentPhase == INACTIVE)
    {
        return 0.0f;  // No progress if inactive
    }

    float total_duration = 0.0f;    // placeholder for total time across phases
    float progress = 0.0f;          // progress accumulator

    switch(m_envelopeType)
    {
        case EnvelopeType::ADSR:
            total_duration = m_attackTime + m_decayTime + m_releaseTime;

            if(m_currentPhase == ATTACK)
            {
                progress = m_elapsedTime / total_duration;
            }
            else if(m_currentPhase == DECAY)
            {
                progress = (m_elapsedTime + m_attackTime) / total_duration;
            }
            else if(m_currentPhase == SUSTAIN)
            {
                progress = (m_attackTime + m_decayTime) / total_duration;
            }
            else if(m_currentPhase == RELEASE)
            {
                progress = (m_elapsedTime + m_attackTime + m_decayTime) / total_duration;
            }
            break;
        case EnvelopeType::ASR:
            total_duration = m_attackTime + m_releaseTime;

            if(m_currentPhase == ATTACK)
            {
                progress = m_elapsedTime / total_duration;
            }
            else if(m_currentPhase == SUSTAIN)
            {
                progress = m_attackTime / total_duration;
            }
            else if(m_currentPhase == RELEASE)
            {
                progress = (m_elapsedTime + m_attackTime) / total_duration;
            }
            break;
        case EnvelopeType::AD:
            total_duration = m_attackTime + m_decayTime;

            if(m_currentPhase == ATTACK)
            {
                progress = m_elapsedTime / total_duration;                
            }
            else if(m_currentPhase ==  DECAY)
            {
                progress = (m_elapsedTime + m_attackTime) / total_duration;
            }
            break;
        default:
            // error handling for invalid envelope type
            return 0.0f;
    }

    return std::min(progress, 1.0f);
}

float Envelope::getAmplitudeAtTime(Envelope::Phase phase, float normalizedTime) const
{
    switch (phase)
    {
        case Phase::INACTIVE:
        {
            return 0.0f;
        }
        case Phase::ATTACK:
        {
            return calculateAttackPhase(normalizedTime);
        }
        case Phase::DECAY:
        {
            return calculateDecayPhase(normalizedTime);
        }
        case Phase::SUSTAIN:
        {
            return m_sustainLevel;
        }
        case Phase::RELEASE:
        {
            return calculateReleasePhase(normalizedTime);
        }
        default:
        {
            std::cerr << "Can not calculate amplitude, invalid envelope phase" << std::endl;
            break;
        }
    }
    return 0.0f;
}

float Envelope::scaleCurveNumber(float input)
{
    if (input > 0)
    {
        return 1.f + input;
    }
    else if (input == 0)
    {
        return 1.f;
    }
    else
    {
        return 1.f / (fabs(input) + 1.f);
    }
}

