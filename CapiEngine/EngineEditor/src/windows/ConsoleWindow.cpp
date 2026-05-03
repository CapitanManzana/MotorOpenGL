#include "ConsoleWindow.h"

namespace cme::ui {
	void ConsoleWindow::renderWindowContent() {
        if (ImGui::Button("Clear")) {
            const_cast<ConsoleWindow*>(this)->_logs.clear();
        }
        ImGui::Separator();
        ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& log : _logs) {
            if (log.find("[ERROR]") != std::string::npos) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.3f, 0.3f, 1)); // rojo
            else if (log.find("[WARN]") != std::string::npos) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0.3f, 1));    // amarillo
            else                                               ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));        // blanco

            ImGui::TextUnformatted(log.c_str());
            ImGui::PopStyleColor();
        }

        // Auto-scroll al final
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
	}
}