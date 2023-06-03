#include <iostream>

#include "Fact.hpp"
#include "InferenceEngine.hpp"
#include "Rule.hpp"
#include "App.hpp"
#include "FactsWindow.h"
#include "RulesWindow.hpp"
#include "InferenceWindow.hpp"

int main()
{
    App::Create("", 640, 480);


    std::vector<std::string> rules = {
        "Toux chronique & Fievre & SueurNocturne & Amaigrissement & Crachat -> Maladie = \"Tuberculose\""
    };

    auto inference_engine = InferenceEngine();

   // Add rules to inference engine

    auto fact_wnd = FactsWindow(inference_engine);
    auto rule_wnd = RulesWindow(inference_engine, rules);
    auto inf_wnd = InferenceWindow(inference_engine);

    while (App::IsRunning())
    {
        App::StartFrame();

        fact_wnd.Draw();
        rule_wnd.Draw();
        inf_wnd.Draw();

        App::EndFrame();
    }

    App::Shutdown();
}

