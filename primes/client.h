#pragma once
#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"
#include "imgui/imstb_rectpack.h"
#include "imgui/imstb_textedit.h"
#include "imgui/imstb_truetype.h"
#include "../VisualStudioSDL2.0.17/include/SDL.h"
#include "model.h"

#define IM_PRIu64   "I64u"
#define VERSION "0.04"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

class Client
{
private:
    std::thread taskCalculating;
    std::string windowName;
    std::vector<std::string>args;
    char buf1[64];

    unsigned animationFrame;
    unsigned windowW;
    unsigned windowH;
    int libSelection;
    int threadSlider;
    unsigned long long A;
    unsigned long long B;
    double duration;
    double animationDuration;
    double animationDurationSum;

    bool show_demo_window = true;
    bool show_another_window = false;
    bool use_work_area = true;
    bool no_titlebar;
    bool no_scrollbar;
    bool no_menu;
    bool no_move;
    bool no_resize;
    bool no_collapse;
    bool no_close;
    bool no_nav;
    bool no_background;
    bool no_bring_to_front;
    bool no_docking;
    bool unsaved_document;
    bool savePath;
    bool exitProgram;
    bool displayHelp;
    bool startProcedure;
    bool displayReadMe;
    bool startCalculating;
    bool showProgress;
    bool done;

    SDL_Event event;
    SDL_WindowFlags window_flags;
    SDL_Window* window;
    SDL_Renderer* renderer;

    //ImGuiIO & io;
    ImGuiWindowFlags window_flags_imGui;
    ImGuiViewport* viewport;
    ImVec4 clear_color;

    Model * model;

	// methods
	void HelpMarker(const char* _message) noexcept;
    void drawMenuBar(); //TODO take it to View
    void drawContent();
    void drawReadmeFrame();
    void drawComputingFrame();
    void callComputing();

public:
	Client(char ** _args, int _argc);
    ~Client();

	void init();
	void loop();
};