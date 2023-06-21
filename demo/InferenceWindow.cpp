#include "InferenceWindow.hpp"
#include <imgui/imgui.h>
#include <vector>
#include <string>

extern std::vector<std::string> facts;

InferenceWindow::InferenceWindow(InferenceEngine& ie)
	 : ie_(ie), input_()
{
}

static bool syntax_error = false;
static std::string resultat;

auto InferenceWindow::Draw() -> void
{

	ImGui::Begin("Moteur d'inference");
	{
		ImGui::InputTextWithHint(" ", "Inferer un fait", input_, IM_ARRAYSIZE(input_));

        ImGui::SameLine();
        if (ImGui::Button("Inferer"))
        {
            auto tokens = Fact::Scanner::Scan(input_);

            if (!tokens)
            {
                syntax_error = false;
                goto brk;
            }

            auto fact = Fact::Parser::Parse(*tokens);

            if (!fact)
            {
                syntax_error = true;
                goto brk;
            }

            if (ie_.Deduce(*fact))
            {
                resultat = "vrai";
                facts.push_back(input_);
            }
            else
            {
                resultat = "faux";
            }
            syntax_error = false;
        }

    brk:

        if (syntax_error)
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Erreur syntaxique");
        }

        ImGui::Separator();
        ImGui::Text("Resultat: %s", resultat.c_str());
	}
	ImGui::End();

}
