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

bool Client::checkInput()
{
    bool disableGui = false;
    unsigned countOperations = 0;

    for (int i = 0; i < args.size(); i++)
    {
        if (args.at(i) == "-begin" && (i + 1) <= args.size())
        {
            A = std::stol(args.at(i++ + 1));
            ++countOperations;
        }
        if (args.at(i) == "-end" && (i + 1) <= args.size())
        {
            B = std::stol(args.at(i++ + 1));
            ++countOperations;
        }
        if (args.at(i) == "-N" && (i + 1) <= args.size())
        {
            numberOfCalculations = std::stoi(args.at(i++ + 1));
            ++countOperations;
        }
        if (args.at(i) == "-dll" && (i + 1) <= args.size())
        {
            if (args.at(i + 1) == "C/C++")
            {
                libSelection = 1;
                ++countOperations;
            }
            else if (args.at(i + 1) == "ASM")
            {
                libSelection = 0;
                ++countOperations;
            }
        }
        if (args.at(i) == "-T" && (i + 1) <= args.size())
        {
            threadSlider = std::stoi(args.at(i++ + 1));
            ++countOperations;
        }
        if (args.at(i) == "-o" && (i + 1) <= args.size())
        {
            for (int j = 0; j < args.at(i + 1).size(); j++)
                buf1[j] = args.at(i + 1).at(j);
        }
        if (args.at(i) == "-x86")
            architectureSelection = 0;
        if (args.at(i) == "-x64")
            architectureSelection = 1;
    }

    return countOperations == 5 ? true : false;
}

Client::Client(char** _args, int _argc)
{
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    for (int i = 0; i < _argc; i++)
    {
        args.push_back(_args[i]);
    }
    //checkInput();
    buf1[64];
    windowName = "Find primes ";
    windowName += VERSION;
    windowW = 800;
    windowH = 600;
    libSelection = 0;
    architectureSelection = 0;
    A = 0;
    B = 100;
    animationFrame = 0;
    animationDuration = 0.0;
    animationDurationSum = 0.0;
    duration = 0.0;

    // New model for calculations
    model = new Model();

    threadSlider = model->getMaximumNumberOfThreads();

    try
    {
        useScriptVersion = checkInput();
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << "INPUT ERROR -> invalid argument: " << e.what() << std::endl;
    }
    catch (std::out_of_range& e)
    {
        std::cerr << "INPUT ERROR -> out of range: " << e.what() << std::endl;
    }

    if (!useScriptVersion)
    {
        // Demonstrate the various window flags. Typically you would just use the default!
        no_titlebar = true;
        no_scrollbar = true;
        no_menu = true;
        no_move = true;
        no_resize = true;
        no_collapse = true;
        no_close = false;
        no_nav = false;
        no_background = false;
        no_bring_to_front = false;
        no_docking = false;
        unsaved_document = false;
        savePath = false;
        exitProgram = false;
        displayHelp = false;
        displayReadMe = false;
        startProcedure = false;
        startCalculating = false;
        showProgress = false;

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
    }
    // Main loop
    done = false;
}

void Client::init()
{
    if (!useScriptVersion)
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
}

void Client::drawMenuBar()
{
    // Menu
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
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
    if (libSelection == 0)
    {
        ImGui::Text("Select proccesor architecture:");           ImGui::SameLine();
        ImGui::RadioButton("x86", &architectureSelection, 0);    ImGui::SameLine();
        ImGui::RadioButton("x64", &architectureSelection, 1);
    }
    ImGui::Separator();

    ImGui::Text("Select number of threads:");
    ImGui::SliderInt("slider int", &threadSlider, 1, model->getMaximumNumberOfThreads());
    ImGui::SameLine(); HelpMarker("CTRL+click to input value.!");
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
    projectInfo += "\nThe main goal of the project was to use the SIMD instructions";

    ImGui::Text(projectInfo.c_str());
    ImGui::Separator();
    ImGui::Text("User can also enter input with command line.");
    ImGui::Text("The list of switches is as follows:");
    ImGui::TextColored(ImVec4(0.00f, 0.0f, 0.9f, 1.0f), "-begin <integer>   "); ImGui::SameLine(); ImGui::Text("    <- begin of interval,");
    ImGui::TextColored(ImVec4(0.00f, 0.0f, 0.9f, 1.0f), "-end <integer>     "); ImGui::SameLine(); ImGui::Text("    <- end of interval,");
    ImGui::TextColored(ImVec4(0.00f, 0.0f, 0.9f, 1.0f), "-N <integer>       "); ImGui::SameLine(); ImGui::Text("    <- numbers of calculations,");
    ImGui::TextColored(ImVec4(0.00f, 0.0f, 0.9f, 1.0f), "-dll <C/C++ / ASM> "); ImGui::SameLine(); ImGui::Text("    <- Library selection,");
    ImGui::TextColored(ImVec4(0.00f, 0.0f, 0.9f, 1.0f), "-T <integer>       "); ImGui::SameLine(); ImGui::Text("    <- numbers of threads,");
    ImGui::Text("-o <outputPath>    ");    ImGui::SameLine();  ImGui::Text("    <- path to the output folder,");
    ImGui::Text("-x86               ");    ImGui::SameLine();  ImGui::Text("   <- architecture selection,");
    ImGui::Text("-x64               ");    ImGui::SameLine();  ImGui::Text("   <- architecture selection.");
    ImGui::Separator();
    ImGui::Text("Enumerations marked in blue are necessary to enable the script mode in which the graphic interface is disabled.");
    ImGui::Text("Example:");    ImGui::SameLine(); ImGui::TextColored(ImVec4(0.00f, 0.0f, 0.9f, 1.0f), "./primes.exe -begin 0 -end 10000000 -dll C/C++ -T 4 -N 10");

    if (ImGui::Button("Return"))
        displayReadMe = false;

    ImGui::End();
}

void Client::callComputing()
{
    this->duration = 0.0;
    std::string pathFromUser(buf1);
    if (pathFromUser.size())
        model->setOutputPath(pathFromUser);

    auto t1 = std::chrono::high_resolution_clock::now();
    if (libSelection)
        model->findPrimeNumbersParallel(this->A, this->B, this->threadSlider, "C++");
    else
    {
        model->setASMType(architectureSelection);
        model->findPrimeNumbersParallel(this->A, this->B, this->threadSlider, "ASM");
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    this->duration = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    showProgress = false;
}

double Client::scriptComputing()
{
    std::vector<char> animation({ '-', '\\', '|' , '/' });
    std::string tmpOutputPath(buf1);
    double sum = 0.0;
    bool endFlag = false;

    std::cout << "\n----------------\n";
    std::cout << "Calulating info:" << std::endl;
    std::cout << "Library:\t" << (this->libSelection ? "C++" : (this->architectureSelection ? "ASM x64" : "ASM x86")) << std::endl;
    std::cout << "Threads:\t" << this->threadSlider << std::endl;
    std::cout << "Interval:\t" << '(' << this->A << ',' << this->B << ')' << std::endl;
    std::cout << ( tmpOutputPath.empty() ? "No ouput path" : "Output path:\t" + tmpOutputPath ) << std::endl;
    std::cout << "\n----------------\n";

    for (int i = 0; i < numberOfCalculations; i++)
    {
        endFlag = false;    // TODO
        std::cout << "Task " << i + 1 << " from " << numberOfCalculations << " ->\t";
        //auto drawingTask = std::thread([this, &endFlag]
        //                                {
        //                                    drawWaitAnimation(std::cout, &endFlag);
        //                                });
        callComputing();
        endFlag = true;
        sum += this->duration;
        std::cout << this->duration << " [s]\n";
    }
    std::cout << "\n----------------\n";

    return sum / (double)this->numberOfCalculations;
}

void Client::drawWaitAnimation(std::ostream & _stream, bool * endFlag)
{
    std::vector<char> animation({ '-', '\\', '|' , '/' });
    double animationDuration = 0.0;
    double animationDurationSum = 0.0;
    char actualFrameCharacter = animation.at(0);
    int animationFrame = 0;
    while (*endFlag)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        
        _stream << actualFrameCharacter;

        if (animationDurationSum >= 0.25)
        {
            actualFrameCharacter = animation.at(animationFrame++);
            animationDurationSum = 0.0;
        }
        else
            actualFrameCharacter = animation.at(animationFrame);

        if (animationFrame >= animation.size())
            animationFrame = 0;
        auto t1 = std::chrono::high_resolution_clock::now();

        auto tmp = (t1 - t0);
        animationDuration = tmp.count();
        animationDurationSum += animationDuration;

        _stream << '\b';
    }
}

void Client::drawComputingFrame()
{
    viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    ImGui::Begin("Computing...", NULL, window_flags_imGui);

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

        ImGui::Text("Calculating... Please wait.");
        ImGui::SameLine();
        ImGui::Text(tmp2.c_str());

        animationDuration = t1;
    }
    else
    {
        tmp1 += std::to_string(duration);
        tmp1 += "[s]";
        ImGui::Text(tmp1.c_str());
    }

    // TODO trzeba dodaæ zabijanie poszczególnych w¹tków
    // W aktualnej wersji powo³ane w¹tki bêdê dzia³a³y w tle a¿ do zakoñczenia zadania
    if (ImGui::Button("Free threads"))
    {
        startProcedure = false;
        //if(taskCalculating.joinable())
        //    taskCalculating.~thread();
    }

    ImGui::End();
}

void Client::loop()
{
    if (!useScriptVersion)
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

            if ((this->A < this->B) && (threadSlider >= 1 && threadSlider <= 64))
            {
                if (ImGui::Button("FIND PRIMES!"))
                    startCalculating = startProcedure = true;
                if (threadSlider > model->getMaximumNumberOfThreads())
                    ImGui::TextColored(ImVec4(0.90f, 0.0f, 0.0f, 1.0f), "Note, the number of threads entered exceeds the number of logical threads provided by the processor!");
            }
            else
                ImGui::TextColored(ImVec4(0.90f, 0.0f, 0.0f, 1.0f), "Invalid Input!");

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

            // Rendering
            ImGui::Render();
            SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
            SDL_RenderClear(renderer);
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer);
        }
    }
    else
    {
        if (this->A < this->B)
        {
            auto averageTime = scriptComputing();
            std::cout << "Average time: " << averageTime << std::endl;
        }
        else
            std::cerr << "Begin is greater than End!" << std::endl;
    }
}

Client::~Client()
{
    if (!useScriptVersion)
    {
        // Cleanup
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    delete model;
}