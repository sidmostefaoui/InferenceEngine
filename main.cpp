#include "App.hpp"
#include "InferenceEngine.hpp"
#include "FactsWindow.h"
#include "RulesWindow.hpp"
#include "InferenceWindow.hpp"

int main()
{
    auto app = App("System Expert", 640, 480);

    auto inference_engine = InferenceEngine();

    auto fact_wnd = FactsWindow(inference_engine);
    auto rule_wnd = RulesWindow(inference_engine);
    auto inf_wnd = InferenceWindow(inference_engine);

    app.loop([&]()
    {  
        fact_wnd.Draw();
        rule_wnd.Draw();
        inf_wnd.Draw();
    });
}

