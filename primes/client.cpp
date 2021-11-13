#include "client.h"

const ImU64   u64_zero = 0, u64_one = 1, u64_fifty = 50, u64_min = 0, u64_max = ULLONG_MAX / 2, u64_hi_a = ULLONG_MAX / 2 - 100, u64_hi_b = ULLONG_MAX / 2;

void Client::HelpMarker(const char* _message) noexcept
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(_message);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

Client::Client(char** _args, int _argc)
{
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //myWindow
    buf1[64];
    //for (size_t i = 0; i < 64; i++)
    //    buf1[i] = ' ';
    windowName = "Find primes ";
    windowName += VERSION;
    windowW = 800;
    windowH = 600;
    libSelection = 0;
    threadSlider = 1;
    A = 0;
    B = 100;
    animationFrame = 0;
    animationDuration = 0.0;
    animationDurationSum = 0.0;
    duration = 0.0;

    // Demonstrate the various window flags. Typically you would just use the default!
    no_titlebar         = true;
    no_scrollbar        = true;
    no_menu             = true;
    no_move             = true;
    no_resize           = true;
    no_collapse         = true;
    no_close            = false;
    no_nav              = false;
    no_background       = false;
    no_bring_to_front   = false;
    no_docking          = false;
    unsaved_document    = false;
    savePath            = false;
    exitProgram         = false;
    displayHelp         = false;
    displayReadMe       = false;
    startProcedure      = false;
    startCalculating    = false;
    showProgress        = false;

    window_flags_imGui = 0;
    if (no_titlebar)        window_flags_imGui |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags_imGui |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags_imGui |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags_imGui |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags_imGui |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags_imGui |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags_imGui |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags_imGui |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags_imGui |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_docking)         window_flags_imGui |= ImGuiWindowFlags_NoDocking;
    if (unsaved_document)   window_flags_imGui |= ImGuiWindowFlags_UnsavedDocument;

    // Main loop
    done = false;

    model = new Model();
}

void Client::init()
{
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        throw std::exception(SDL_GetError());

    // Setup window
    window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
     window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)windowW, (int)windowH, window_flags);

    // Setup Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        throw std::exception("SDL cannot create renderer!");


    // Init Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);
}

void Client::drawMenuBar()
{
    // Menu
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            //ShowExampleMenuFile();
            ImGui::MenuItem("Write output path", "", &savePath);
            ImGui::MenuItem("Exit", "", &exitProgram);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Readme", "", &displayReadMe)) {}
            if (ImGui::MenuItem("Author", "Michal Jackowski")) {};
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Client::drawContent()
{
    // Windows contents
    ImGui::Text("Select library:");                 ImGui::SameLine();
    ImGui::RadioButton("ASM", &libSelection, 0);    ImGui::SameLine();
    ImGui::RadioButton("C/C++", &libSelection, 1);
    ImGui::Separator();

    ImGui::Text("Select number of threads:");
    ImGui::SliderInt("slider int", &threadSlider, 1, model->getMaximumNumberOfThreads());
    ImGui::SameLine(); HelpMarker("CTRL+click to input value. TODO dodaj ograniczenia!");
    ImGui::Separator();

    ImGui::Text("Select Interval:");
    ImGui::InputScalar("Begin", ImGuiDataType_U64, &A, true ? &u64_one : NULL);
    ImGui::InputScalar("End", ImGuiDataType_U64, &B, true ? &u64_one : NULL); ImGui::SameLine(); HelpMarker("End must be greater than begin!");
    ImGui::Separator();
}

void Client::drawReadmeFrame()
{
    viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    ImGui::Begin("Readme", NULL, window_flags_imGui);

    std::string projectInfo("Version : ");
    projectInfo += VERSION;
    projectInfo += "\nAuthor : Michal Jackowski\n";
    projectInfo += "Date of compilation : ";
    projectInfo += __DATE__;
    projectInfo += "\nProject for the subject assembly languages";

    ImGui::Text(projectInfo.c_str());
    ImGui::Separator();

    if (ImGui::Button("Return"))
        displayReadMe = false;

    ImGui::End();
}

void Client::callComputing()
{
    this->duration = 0.0;

    auto t1 = std::chrono::high_resolution_clock::now();    // TODO timer!
    if (libSelection)
        model->findPrimeNumbersParallel(this->A, this->B, this->threadSlider, "C++");
    else
        model->findPrimeNumbersParallel(this->A, this->B, this->threadSlider, "ASM");
    auto t2 = std::chrono::high_resolution_clock::now();
    this->duration = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    showProgress = false;
}

void Client::drawComputingFrame()
{
    viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    ImGui::Begin("Computing...", NULL, window_flags_imGui);
    //std::vector<double> times;

    std::string tmp0("Calling for ->\t");
    std::string tmp1("Time ->\t");
    std::string tmp2("");
    std::vector<char> animation({ '-', '\\', '|' , '/' });
    size_t i = 0;

    tmp0 += ( libSelection ? "C/C++" : "ASM");
    ImGui::Text(tmp0.c_str());
    ImGui::Separator();

    if (startCalculating)
    {
        animationDurationSum = 0.0;
        animationDuration = ImGui::GetTime();
        taskCalculating = std::thread(&Client::callComputing, this);
        taskCalculating.detach();
        startCalculating = false;
        showProgress = true;
    }
    //taskCalculating
    //taskCalculating.detach();

    //if (startCalculating)
    //{
    //    auto t1 = std::chrono::high_resolution_clock::now();    // TODO timer!
    //    if (libSelection)
    //        model->findPrimeNumbersParallel(this->A, this->B, this->threadSlider, "C++");
    //    else
    //        model->findPrimeNumbersParallel(this->A, this->B, this->threadSlider, "ASM");
    //    auto t2 = std::chrono::high_resolution_clock::now();
    //    duration = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    //    startCalculating = false;
    //}

    if (showProgress)
    {
        auto t1 = ImGui::GetTime();

        animationDuration = t1 - animationDuration;
        animationDurationSum += animationDuration;

        if (animationDurationSum >= 0.25)
        {
            tmp2 += animation.at(animationFrame++);
            animationDurationSum = 0.0;
        }
        else
            tmp2 += animation.at(animationFrame);

        if (animationFrame >= animation.size())
            animationFrame = 0;

        ImGui::Text("Calculating... Please wait."); // TODO procent bar?
        ImGui::SameLine();
        ImGui::Text(tmp2.c_str());

        animationDuration = t1;
    }
    else
    {
        //std::string tmp1("Time ->\t");
        tmp1 += std::to_string(duration);
        tmp1 += "[s]";
        ImGui::Text(tmp1.c_str());
    }

    if (ImGui::Button("Free threads"))
        startProcedure = false;

    ImGui::End();
    //times.push_back(std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count());

    //double sum = std::accumulate(times.begin(), times.end(), 0.0);
    //double average = (double)sum / (double)times.size();
    //std::cout << "\n AVERAGE ->\t" << average << "[s]" << std::endl;
}

void Client::loop()
{
    // Main loop
    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        
        // myWindow
        viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
        ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

        ImGui::Begin("Find Primes", NULL, window_flags_imGui);

        this->drawMenuBar();
        this->drawContent();

        if (ImGui::Button("FIND PRIMES!"))
            startCalculating = startProcedure = true;

        if (savePath)
        {
            ImGui::Separator();
            ImGui::InputTextWithHint("Output text file", "write here..", buf1, 64);
        }
        if (startProcedure)
            this->drawComputingFrame();
        if (exitProgram)
            done = true;
        ImGui::End();

        if (displayReadMe)
            this->drawReadmeFrame();
        //if (startProcedure)
        //    this->drawComputingFrame();
        
        //std::cout << this->A << "\t|\t" << this->B << std::endl;

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }
}

Client::~Client()
{
    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete model;
}