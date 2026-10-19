#ifndef SHIBAENGINE_TIMESTEP_H
#define SHIBAENGINE_TIMESTEP_H

class Timestep {
public:
    explicit Timestep(float time = 0.0f);

    [[nodiscard]] float GetSeconds() const { return m_time; }
    [[nodiscard]] float GetMilliseconds() const { return m_time * 1000.0f; }

private:
    float m_time = 0.0f;
};

#endif //SHIBAENGINE_TIMESTEP_H
