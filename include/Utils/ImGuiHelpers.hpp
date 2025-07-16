#pragma once

#include <type_traits>

#include <SDL3/SDL_rect.h>

#include <imgui.h>

namespace PongAI::CONFIG::IMGUI_HELPERS
{
    template<typename T>
    inline void ValueBox(const char* label, T& value)
    {
        ImGui::SetNextItemAllowOverlap();
        ImGui::TextUnformatted(label);
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

        if constexpr (std::is_same_v<T, bool>)
            ImGui::TextUnformatted(value ? "true" : "false");
        else
        if constexpr (std::is_integral_v<T>)
            ImGui::Text("%d", static_cast<T>(value));
        else
        if constexpr (std::is_floating_point_v<T>)
            ImGui::Text("%.2f", static_cast<float>(value));
        else
            ImGui::Text("<unsupported>");

        ImGui::PopStyleColor();
    }
    inline void FPoint(const char* label, const SDL_FPoint& point)
    {
        ImGui::TextUnformatted(label); ImGui::SameLine();
        ValueBox("x", point.x); ImGui::SameLine();
        ValueBox("y", point.y);
    }

    // Struct code "stolen" from https://github.com/epezent/implot/blob/master/implot_demo.cpp#L125 :)
    struct ScrollingBuffer
    {
        int maxSize;
        int offset = 0;
        ImVector<ImVec2> data;

        const char* label;
        ImVec4 color{ 0.0f, 0.0f, 0.0f, 0.0f };

        ScrollingBuffer(int maxSize_, const char* label_, ImVec4 color_ = { 0.0f, 0.0f, 0.0f, 0.0f }, int offset_ = 0)
             : maxSize(maxSize_), offset(offset_),
			   label(label_), color(color_)
        { data.reserve(maxSize_); };

        void addPoint(float x, float y)
        {
            if (data.size() < maxSize)
                data.push_back(ImVec2(x, y));
            else
            {
                data[offset] = ImVec2(x, y);
                offset = (offset + 1) % maxSize;
            }
        }
		void addPoint(ImVec2 point)
		{
			addPoint(point.x, point.y);
		}
        void clear()
        {
            if (data.size() > 0)
            {
                data.clear();
                offset = 0;
            }
        }
    };

	typedef struct ScrollingBuffer SB; // Alias for easier use
}