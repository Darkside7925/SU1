#include "su1/core.hpp"
#include "su1/display_server.hpp"
#include <csignal>
#include <iostream>
#include <thread>
#include <chrono>

namespace su1 {

static volatile bool running = true;
static volatile bool restart_requested = false;

void signal_handler(int sig) {
    switch (sig) {
        case SIGINT:
        case SIGTERM:
            running = false;
            break;
        case SIGHUP:
            restart_requested = true;
            break;
        case SIGUSR1:
            Logger::info("Received SIGUSR1 - reloading configuration");
            break;
        case SIGUSR2:
            Logger::info("Received SIGUSR2 - toggling debug mode");
            break;
        default:
            Logger::warning(String("Received unknown signal: ") + std::to_string(sig));
            break;
    }
}

void setup_signal_handlers() {
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
    sigaction(SIGHUP, &sa, nullptr);
    sigaction(SIGUSR1, &sa, nullptr);
    sigaction(SIGUSR2, &sa, nullptr);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGPIPE);
    sigprocmask(SIG_BLOCK, &mask, nullptr);
}

void setup_process_attributes() {
    if (setpriority(PRIO_PROCESS, 0, -20) == -1) {
        Logger::warning("Failed to set process priority");
    }

    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
        Logger::warning("Failed to lock memory");
    }

    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        Logger::warning("Failed to set real-time scheduling");
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        CPU_SET(i, &cpuset);
    }
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == -1) {
        Logger::warning("Failed to set CPU affinity");
    }
}

void setup_environment() {
    setenv("SU1_DISPLAY_SERVER", "1", 1);
    setenv("SU1_VERSION", "1.0.0", 1);
    setenv("SU1_BACKEND", "opengl", 1);
    setenv("SU1_RENDERER", "gles2", 1);
    setenv("SU1_COMPOSITOR", "liquid_glass", 1);
    setenv("SU1_WINDOW_MANAGER", "tiled", 1);
    setenv("SU1_THEME", "liquid_glass", 1);

    umask(022);

    if (chdir("/") == -1) {
        Logger::warning("Failed to change to root directory");
    }

    if (setsid() == -1) {
        Logger::warning("Failed to create new session");
    }

    std::freopen("/dev/null", "r", stdin);
    std::freopen("/var/log/su1.log", "a", stdout);
    std::freopen("/var/log/su1.log", "a", stderr);
}

bool check_requirements() {
    if (getuid() != 0) {
        Logger::error("SU1 must be run as root");
        return false;
    }

    struct utsname uname_data;
    if (uname(&uname_data) == -1) {
        Logger::error("Failed to get system information");
        return false;
    }

    Logger::info(String("Running on ") + uname_data.sysname + " " + uname_data.release);

    if (access("/dev/dri/card0", F_OK) == -1) {
        Logger::warning("No DRM device found, falling back to software rendering");
    }

    if (access("/dev/input/event0", F_OK) == -1) {
        Logger::warning("No input devices found");
    }

    return true;
}

void print_banner() {
    std::cout << R"(
    ____  _   _ ___ _     ____
   / ___|| | | |_ _| |   / ___|
   \___ \| | | || || |   \___ \
    ___) | |_| || || |___ ___) |
   |____/  \___/|___|_____|____/

    SU1 Display Server v1.0.0
    Liquid Glass Theme
    Extreme Performance & Graphics

    )" << std::endl;
}

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help              Show this help message\n";
    std::cout << "  -v, --version           Show version information\n";
    std::cout << "  -c, --config FILE       Use configuration file\n";
    std::cout << "  -d, --display DISPLAY   Set display number\n";
    std::cout << "  -b, --backend BACKEND   Set rendering backend (opengl, vulkan, software)\n";
    std::cout << "  -r, --renderer RENDERER Set renderer (gles2, gl3, vk)\n";
    std::cout << "  -t, --theme THEME       Set theme (liquid_glass, dark, light)\n";
    std::cout << "  -w, --window-manager WM Set window manager (tiled, floating, stacking)\n";
    std::cout << "  -l, --log-level LEVEL   Set log level (debug, info, warning, error)\n";
    std::cout << "  --no-vsync              Disable vertical synchronization\n";
    std::cout << "  --no-compositor         Disable compositor\n";
    std::cout << "  --software-rendering    Force software rendering\n";
    std::cout << "  --debug                 Enable debug mode\n";
    std::cout << "  --profile               Enable profiling\n";
    std::cout << "  --benchmark             Run benchmarks\n";
    std::cout << std::endl;
}

struct CommandLineOptions {
    String config_file = "/etc/su1/su1.conf";
    String display = ":0";
    String backend = "opengl";
    String renderer = "gles2";
    String theme = "liquid_glass";
    String window_manager = "tiled";
    String log_level = "info";
    bool vsync = true;
    bool compositor = true;
    bool software_rendering = false;
    bool debug = false;
    bool profile = false;
    bool benchmark = false;
    bool help = false;
    bool version = false;
};

CommandLineOptions parse_command_line(int argc, char** argv) {
    CommandLineOptions options;

    for (int i = 1; i < argc; ++i) {
        String arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            options.help = true;
        } else if (arg == "-v" || arg == "--version") {
            options.version = true;
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                options.config_file = argv[++i];
            }
        } else if (arg == "-d" || arg == "--display") {
            if (i + 1 < argc) {
                options.display = argv[++i];
            }
        } else if (arg == "-b" || arg == "--backend") {
            if (i + 1 < argc) {
                options.backend = argv[++i];
            }
        } else if (arg == "-r" || arg == "--renderer") {
            if (i + 1 < argc) {
                options.renderer = argv[++i];
            }
        } else if (arg == "-t" || arg == "--theme") {
            if (i + 1 < argc) {
                options.theme = argv[++i];
            }
        } else if (arg == "-w" || arg == "--window-manager") {
            if (i + 1 < argc) {
                options.window_manager = argv[++i];
            }
        } else if (arg == "-l" || arg == "--log-level") {
            if (i + 1 < argc) {
                options.log_level = argv[++i];
            }
        } else if (arg == "--no-vsync") {
            options.vsync = false;
        } else if (arg == "--no-compositor") {
            options.compositor = false;
        } else if (arg == "--software-rendering") {
            options.software_rendering = true;
        } else if (arg == "--debug") {
            options.debug = true;
        } else if (arg == "--profile") {
            options.profile = true;
        } else if (arg == "--benchmark") {
            options.benchmark = true;
        }
    }

    return options;
}

int main(int argc, char** argv) {
    CommandLineOptions options = parse_command_line(argc, argv);

    if (options.help) {
        print_usage(argv[0]);
        return 0;
    }

    if (options.version) {
        std::cout << "SU1 Display Server v1.0.0" << std::endl;
        return 0;
    }

    print_banner();

    Logger::info("Starting SU1 Display Server...");

    if (!check_requirements()) {
        return 1;
    }

    setup_signal_handlers();
    setup_process_attributes();
    setup_environment();

    try {
        DisplayServer display_server(options);

        if (!display_server.initialize()) {
            Logger::error("Failed to initialize display server");
            return 1;
        }

        Logger::info("SU1 Display Server initialized successfully");

        while (running) {
            if (restart_requested) {
                Logger::info("Restart requested, shutting down...");
                display_server.shutdown();
                restart_requested = false;

                Logger::info("Restarting SU1 Display Server...");
                if (!display_server.initialize()) {
                    Logger::error("Failed to restart display server");
                    break;
                }
                Logger::info("SU1 Display Server restarted successfully");
            }

            display_server.run_once();

            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        Logger::info("Shutting down SU1 Display Server...");
        display_server.shutdown();
        Logger::info("SU1 Display Server shut down successfully");

    } catch (const std::exception& e) {
        Logger::error(String("Exception caught: ") + e.what());
        return 1;
    } catch (...) {
        Logger::error("Unknown exception caught");
        return 1;
    }

    return 0;
}

}
