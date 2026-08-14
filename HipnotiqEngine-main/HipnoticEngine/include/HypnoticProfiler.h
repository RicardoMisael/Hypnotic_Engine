#pragma once

#include "Prerequisites.h"
#include <chrono>
#include <vector>
#include <string>

class HypnoticProfiler
{
public:

    struct SectionData
    {
        std::string name;
        float lastTime = 0.0f;
        float averageTime = 0.0f;
        float maxTime = 0.0f;
    };

    HypnoticProfiler();
    ~HypnoticProfiler() = default;

    // Inicia la medición de un frame
    void beginFrame();

    // Termina la medición de un frame
    void endFrame();

    // Inicia una sección específica
    void beginSection(const char* name);

    // Termina una sección específica
    void endSection(const char* name);

    // Dibuja la ventana del profiler
    void draw();

    void setEnabled(bool enabled);

    bool isEnabled() const;

    float getFPS() const;

    float getFrameTime() const;

private:

    struct ActiveSection
    {
        std::string name;

        std::chrono::high_resolution_clock::time_point start;
    };

    static const int HISTORY_SIZE = 120;

    bool m_enabled;
    bool m_showWindow;

    float m_fps;
    float m_frameTime;

    std::vector<float> m_frameHistory;

    std::vector<SectionData> m_sections;

    std::vector<ActiveSection> m_activeSections;
};