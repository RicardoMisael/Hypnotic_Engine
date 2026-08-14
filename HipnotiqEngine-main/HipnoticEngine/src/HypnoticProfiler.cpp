#include "HypnoticProfiler.h"

#include <algorithm>

HypnoticProfiler::HypnoticProfiler()
    : m_enabled(true),
    m_showWindow(true),
    m_fps(0.0f),
    m_frameTime(0.0f)
{
    m_frameHistory.reserve(HISTORY_SIZE);
    m_sections.reserve(16);
    m_activeSections.reserve(8);
}

void
HypnoticProfiler::beginFrame()
{
    if (!m_enabled)
        return;

    static auto lastFrame =
        std::chrono::high_resolution_clock::now();

    auto currentTime =
        std::chrono::high_resolution_clock::now();

    m_frameTime =
        std::chrono::duration<float, std::milli>(
            currentTime - lastFrame
        ).count();

    lastFrame = currentTime;

    if (m_frameTime > 0.0f)
    {
        m_fps = 1000.0f / m_frameTime;
    }

    m_frameHistory.push_back(m_frameTime);

    if (m_frameHistory.size() > HISTORY_SIZE)
    {
        m_frameHistory.erase(m_frameHistory.begin());
    }

    m_activeSections.clear();
}

void
HypnoticProfiler::endFrame()
{
    if (!m_enabled)
        return;

    m_activeSections.clear();
}

void
HypnoticProfiler::beginSection(const char* name)
{
    if (!m_enabled || name == nullptr)
        return;

    ActiveSection section;

    section.name = name;

    section.start =
        std::chrono::high_resolution_clock::now();

    m_activeSections.push_back(section);
}

void
HypnoticProfiler::endSection(const char* name)
{
    if (!m_enabled || name == nullptr)
        return;

    auto currentTime =
        std::chrono::high_resolution_clock::now();

    for (auto it = m_activeSections.rbegin();
        it != m_activeSections.rend();
        ++it)
    {
        if (it->name != name)
            continue;

        float elapsed =
            std::chrono::duration<float, std::milli>(
                currentTime - it->start
            ).count();

        auto section =
            std::find_if(
                m_sections.begin(),
                m_sections.end(),
                [name](const SectionData& data)
                {
                    return data.name == name;
                }
            );

        if (section == m_sections.end())
        {
            SectionData newSection;

            newSection.name = name;
            newSection.lastTime = elapsed;
            newSection.averageTime = elapsed;
            newSection.maxTime = elapsed;

            m_sections.push_back(newSection);
        }
        else
        {
            section->lastTime = elapsed;

            // Promedio suavizado
            section->averageTime =
                section->averageTime * 0.90f +
                elapsed * 0.10f;

            section->maxTime =
                std::max(
                    section->maxTime,
                    elapsed
                );
        }

        m_activeSections.erase(
            std::next(it).base()
        );

        return;
    }
}

void
HypnoticProfiler::draw()
{
    if (!m_enabled || !m_showWindow)
        return;

    ImGui::Begin(
        "Hypnotic Profiler",
        &m_showWindow
    );

    ImGui::Text("Render Performance");

    ImGui::Separator();

    ImGui::Text(
        "FPS: %.1f",
        m_fps
    );

    ImGui::Text(
        "Frame Time: %.3f ms",
        m_frameTime
    );

    if (m_fps < 30.0f)
    {
        ImGui::Text(
            "WARNING: Low FPS"
        );
    }
    else if (m_fps < 60.0f)
    {
        ImGui::Text(
            "Performance could be improved"
        );
    }
    else
    {
        ImGui::Text(
            "Performance OK"
        );
    }

    ImGui::Separator();

    if (!m_frameHistory.empty())
    {
        ImGui::PlotLines(
            "Frame Time",
            m_frameHistory.data(),
            static_cast<int>(
                m_frameHistory.size()
                ),
            0,
            nullptr,
            0.0f,
            40.0f,
            ImVec2(0.0f, 100.0f)
        );
    }

    ImGui::Separator();

    ImGui::Text("CPU Sections");

    if (ImGui::BeginTable(
        "ProfilerTable",
        4,
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_SizingStretchProp
    ))
    {
        ImGui::TableSetupColumn("Section");
        ImGui::TableSetupColumn("Last");
        ImGui::TableSetupColumn("Average");
        ImGui::TableSetupColumn("Max");

        ImGui::TableHeadersRow();

        for (const SectionData& section : m_sections)
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);

            ImGui::Text(
                "%s",
                section.name.c_str()
            );

            ImGui::TableSetColumnIndex(1);

            ImGui::Text(
                "%.3f ms",
                section.lastTime
            );

            ImGui::TableSetColumnIndex(2);

            ImGui::Text(
                "%.3f ms",
                section.averageTime
            );

            ImGui::TableSetColumnIndex(3);

            ImGui::Text(
                "%.3f ms",
                section.maxTime
            );
        }

        ImGui::EndTable();
    }

    ImGui::Separator();

    if (ImGui::Button("Clear History"))
    {
        m_frameHistory.clear();

        for (SectionData& section : m_sections)
        {
            section.lastTime = 0.0f;
            section.averageTime = 0.0f;
            section.maxTime = 0.0f;
        }
    }

    ImGui::SameLine();

    ImGui::Checkbox(
        "Enabled",
        &m_enabled
    );

    ImGui::End();
}

void
HypnoticProfiler::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool
HypnoticProfiler::isEnabled() const
{
    return m_enabled;
}

float
HypnoticProfiler::getFPS() const
{
    return m_fps;
}

float
HypnoticProfiler::getFrameTime() const
{
    return m_frameTime;
}