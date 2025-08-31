#include "su1/configuration_registry.hpp"
#include "su1/core.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>

namespace SU1 {

class ConfigurationRegistry::Impl {
public:
    struct RegistryEntry {
        std::string key;
        std::string category;
        std::string value;
        std::string default_value;
        std::string type; // "string", "int", "float", "bool", "json"
        std::string description;
        bool requires_restart;
        bool is_modified;
        std::chrono::steady_clock::time_point last_modified;
        std::string validator_regex;
        std::function<bool(const std::string&)> custom_validator;
    };

    struct RegistrySection {
        std::string name;
        std::string description;
        std::unordered_map<std::string, RegistryEntry> entries;
        std::vector<std::string> entry_order;
        bool is_loaded;
        std::chrono::steady_clock::time_point last_saved;
    };

    std::unordered_map<std::string, RegistrySection> sections_;
    std::vector<std::string> section_order_;
    std::filesystem::path registry_path_;
    std::filesystem::path backup_path_;

    std::shared_mutex registry_mutex_;
    std::atomic<bool> auto_save_enabled_;
    std::atomic<int> auto_save_interval_seconds_;
    std::thread auto_save_thread_;
    std::atomic<bool> active_;

    // Performance and statistics
    int total_entries_;
    int modified_entries_;
    std::chrono::steady_clock::time_point last_save_time_;
    std::chrono::steady_clock::time_point last_load_time_;
    double average_save_time_ms_;
    double average_load_time_ms_;
    int save_count_;
    int load_count_;

public:
    Impl()
        : registry_path_("su1_registry.json")
        , backup_path_("su1_registry.backup.json")
        , auto_save_enabled_(true)
        , auto_save_interval_seconds_(60)
        , active_(true)
        , total_entries_(0)
        , modified_entries_(0)
        , average_save_time_ms_(0.0)
        , average_load_time_ms_(0.0)
        , save_count_(0)
        , load_count_(0) {

        initialize_default_registry();
        start_auto_save();
    }

    ~Impl() {
        active_ = false;
        if (auto_save_thread_.joinable()) {
            auto_save_thread_.join();
        }
        save_registry(); // Final save
    }

    void initialize_default_registry() {
        // System Settings
        create_section("system", "Core system settings");
        set_entry("system", "version", "1.0.0", "SU1 version", "string", false);
        set_entry("system", "install_date", std::to_string(std::chrono::system_clock::now().time_since_epoch().count()), "Installation timestamp", "string", false);
        set_entry("system", "language", "en", "System language", "string", true);
        set_entry("system", "theme", "dark", "Default theme", "string", false);
        set_entry("system", "auto_update", "true", "Enable automatic updates", "bool", false);

        // Performance Settings
        create_section("performance", "Performance and optimization settings");
        set_entry("performance", "target_fps", "540", "Target frame rate", "int", false);
        set_entry("performance", "vsync", "false", "Enable vertical synchronization", "bool", false);
        set_entry("performance", "gpu_acceleration", "true", "Enable GPU acceleration", "bool", true);
        set_entry("performance", "adaptive_quality", "true", "Enable adaptive quality", "bool", false);
        set_entry("performance", "max_animations", "1000", "Maximum animations per frame", "int", false);
        set_entry("performance", "texture_quality", "high", "Texture quality level", "string", false);
        set_entry("performance", "shadow_quality", "high", "Shadow rendering quality", "string", false);
        set_entry("performance", "particle_limit", "10000", "Maximum particle count", "int", false);
        set_entry("performance", "memory_limit_mb", "4096", "GPU memory limit in MB", "int", false);

        // Display Settings
        create_section("display", "Display and visual settings");
        set_entry("display", "resolution_width", "1920", "Display width", "int", true);
        set_entry("display", "resolution_height", "1080", "Display height", "int", true);
        set_entry("display", "refresh_rate", "540", "Display refresh rate", "int", true);
        set_entry("display", "color_depth", "32", "Color depth in bits", "int", true);
        set_entry("display", "hdr_enabled", "true", "Enable HDR rendering", "bool", true);
        set_entry("display", "fullscreen", "false", "Enable fullscreen mode", "bool", false);
        set_entry("display", "borderless", "false", "Enable borderless windowed mode", "bool", false);
        set_entry("display", "vsync_method", "adaptive", "V-sync implementation method", "string", false);

        // Audio Settings
        create_section("audio", "Audio and sound settings");
        set_entry("audio", "master_volume", "0.8", "Master volume level", "float", false);
        set_entry("audio", "sound_effects_volume", "0.7", "Sound effects volume", "float", false);
        set_entry("audio", "music_volume", "0.6", "Background music volume", "float", false);
        set_entry("audio", "voice_volume", "0.9", "Voice audio volume", "float", false);
        set_entry("audio", "mute_all", "false", "Mute all audio", "bool", false);
        set_entry("audio", "audio_device", "default", "Selected audio device", "string", false);
        set_entry("audio", "spatial_audio", "true", "Enable spatial audio", "bool", false);

        // Input Settings
        create_section("input", "Input device and control settings");
        set_entry("input", "mouse_sensitivity", "1.0", "Mouse sensitivity multiplier", "float", false);
        set_entry("input", "mouse_acceleration", "false", "Enable mouse acceleration", "bool", false);
        set_entry("input", "keyboard_repeat_delay", "300", "Keyboard repeat delay (ms)", "int", false);
        set_entry("input", "keyboard_repeat_rate", "30", "Keyboard repeat rate (Hz)", "int", false);
        set_entry("input", "gamepad_enabled", "true", "Enable gamepad support", "bool", false);
        set_entry("input", "touch_enabled", "true", "Enable touch input", "bool", false);
        set_entry("input", "gesture_recognition", "true", "Enable gesture recognition", "bool", false);

        // Accessibility Settings
        create_section("accessibility", "Accessibility and usability settings");
        set_entry("accessibility", "high_contrast", "false", "Enable high contrast mode", "bool", false);
        set_entry("accessibility", "large_text", "false", "Use large text size", "bool", false);
        set_entry("accessibility", "screen_reader", "false", "Enable screen reader support", "bool", false);
        set_entry("accessibility", "color_blind_mode", "none", "Color blindness correction", "string", false);
        set_entry("accessibility", "reduced_motion", "false", "Reduce motion and animations", "bool", false);
        set_entry("accessibility", "focus_indicators", "true", "Show keyboard focus indicators", "bool", false);
        set_entry("accessibility", "zoom_factor", "1.0", "Screen zoom factor", "float", false);

        // Network Settings
        create_section("network", "Network and connectivity settings");
        set_entry("network", "auto_connect", "true", "Automatically connect to networks", "bool", false);
        set_entry("network", "proxy_enabled", "false", "Enable proxy server", "bool", false);
        set_entry("network", "proxy_host", "", "Proxy server hostname", "string", false);
        set_entry("network", "proxy_port", "8080", "Proxy server port", "int", false);
        set_entry("network", "bandwidth_limit", "0", "Bandwidth limit in KB/s (0 = unlimited)", "int", false);
        set_entry("network", "sync_settings", "true", "Sync settings across devices", "bool", false);

        // Privacy Settings
        create_section("privacy", "Privacy and security settings");
        set_entry("privacy", "telemetry_enabled", "false", "Enable telemetry and analytics", "bool", false);
        set_entry("privacy", "crash_reporting", "true", "Enable automatic crash reporting", "bool", false);
        set_entry("privacy", "location_services", "false", "Enable location services", "bool", false);
        set_entry("privacy", "data_collection", "minimal", "Data collection level", "string", false);
        set_entry("privacy", "sync_personal_data", "false", "Sync personal data", "bool", false);

        // Development Settings
        create_section("development", "Developer and debugging settings");
        set_entry("development", "debug_mode", "false", "Enable debug mode", "bool", true);
        set_entry("development", "log_level", "info", "Logging verbosity level", "string", false);
        set_entry("development", "performance_overlay", "false", "Show performance overlay", "bool", false);
        set_entry("development", "frame_time_graph", "false", "Show frame time graph", "bool", false);
        set_entry("development", "memory_profiler", "false", "Enable memory profiling", "bool", false);
        set_entry("development", "gpu_profiler", "false", "Enable GPU profiling", "bool", false);

        update_total_entries_count();
    }

    void create_section(const std::string& name, const std::string& description) {
        RegistrySection section;
        section.name = name;
        section.description = description;
        section.is_loaded = true;
        section.last_saved = std::chrono::steady_clock::now();

        sections_[name] = section;
        section_order_.push_back(name);
    }

    void set_entry(const std::string& section, const std::string& key, const std::string& value,
                   const std::string& description, const std::string& type, bool requires_restart) {
        std::unique_lock<std::shared_mutex> lock(registry_mutex_);

        if (sections_.find(section) == sections_.end()) {
            create_section(section, "");
        }

        RegistryEntry entry;
        entry.key = key;
        entry.category = section;
        entry.value = value;
        entry.default_value = value;
        entry.type = type;
        entry.description = description;
        entry.requires_restart = requires_restart;
        entry.is_modified = false;
        entry.last_modified = std::chrono::steady_clock::now();

        sections_[section].entries[key] = entry;
        sections_[section].entry_order.push_back(key);
    }

    std::string get_entry(const std::string& section, const std::string& key) const {
        std::shared_lock<std::shared_mutex> lock(registry_mutex_);

        auto section_it = sections_.find(section);
        if (section_it != sections_.end()) {
            auto entry_it = section_it->second.entries.find(key);
            if (entry_it != section_it->second.entries.end()) {
                return entry_it->second.value;
            }
        }
        return "";
    }

    void set_entry_value(const std::string& section, const std::string& key, const std::string& value) {
        std::unique_lock<std::shared_mutex> lock(registry_mutex_);

        auto section_it = sections_.find(section);
        if (section_it != sections_.end()) {
            auto entry_it = section_it->second.entries.find(key);
            if (entry_it != section_it->second.entries.end()) {
                if (entry_it->second.value != value) {
                    entry_it->second.value = value;
                    entry_it->second.is_modified = true;
                    entry_it->second.last_modified = std::chrono::steady_clock::now();
                    modified_entries_++;
                }
            }
        }
    }

    void start_auto_save() {
        auto_save_thread_ = std::thread([this]() {
            while (active_) {
                std::this_thread::sleep_for(std::chrono::seconds(auto_save_interval_seconds_));

                if (auto_save_enabled_ && modified_entries_ > 0) {
                    save_registry();
                }
            }
        });
    }

    bool save_registry() {
        auto start_time = std::chrono::high_resolution_clock::now();

        try {
            std::unique_lock<std::shared_mutex> lock(registry_mutex_);

            // Create backup
            if (std::filesystem::exists(registry_path_)) {
                std::filesystem::copy_file(registry_path_, backup_path_,
                                         std::filesystem::copy_options::overwrite_existing);
            }

            std::ofstream file(registry_path_);
            if (!file.is_open()) return false;

            file << "{\n";
            file << "  \"metadata\": {\n";
            file << "    \"version\": \"1.0\",\n";
            file << "    \"total_sections\": " << sections_.size() << ",\n";
            file << "    \"total_entries\": " << total_entries_ << ",\n";
            file << "    \"modified_entries\": " << modified_entries_ << ",\n";
            file << "    \"last_saved\": " << std::chrono::system_clock::now().time_since_epoch().count() << "\n";
            file << "  },\n";
            file << "  \"sections\": {\n";

            for (size_t i = 0; i < section_order_.size(); ++i) {
                const auto& section_name = section_order_[i];
                const auto& section = sections_[section_name];

                file << "    \"" << section_name << "\": {\n";
                file << "      \"description\": \"" << section.description << "\",\n";
                file << "      \"entries\": {\n";

                for (size_t j = 0; j < section.entry_order.size(); ++j) {
                    const auto& entry_key = section.entry_order[j];
                    const auto& entry = section.entries.at(entry_key);

                    file << "        \"" << entry_key << "\": {\n";
                    file << "          \"value\": ";

                    // Handle different value types
                    if (entry.type == "string") {
                        file << "\"" << escape_json_string(entry.value) << "\"";
                    } else if (entry.type == "bool") {
                        file << (entry.value == "true" ? "true" : "false");
                    } else {
                        file << entry.value;
                    }

                    file << ",\n";
                    file << "          \"type\": \"" << entry.type << "\",\n";
                    file << "          \"description\": \"" << entry.description << "\",\n";
                    file << "          \"default_value\": ";

                    if (entry.type == "string") {
                        file << "\"" << escape_json_string(entry.default_value) << "\"";
                    } else if (entry.type == "bool") {
                        file << (entry.default_value == "true" ? "true" : "false");
                    } else {
                        file << entry.default_value;
                    }

                    file << ",\n";
                    file << "          \"requires_restart\": " << (entry.requires_restart ? "true" : "false") << ",\n";
                    file << "          \"is_modified\": " << (entry.is_modified ? "true" : "false") << "\n";
                    file << "        }";

                    if (j < section.entry_order.size() - 1) {
                        file << ",";
                    }
                    file << "\n";
                }

                file << "      }\n    }";

                if (i < section_order_.size() - 1) {
                    file << ",";
                }
                file << "\n";
            }

            file << "  }\n}\n";

            auto end_time = std::chrono::high_resolution_clock::now();
            auto save_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

            last_save_time_ = std::chrono::steady_clock::now();
            save_count_++;

            // Update average save time
            average_save_time_ms_ = (average_save_time_ms_ * (save_count_ - 1) + save_duration.count() / 1000.0) / save_count_;

            modified_entries_ = 0;
            return true;

        } catch (const std::exception&) {
            return false;
        }
    }

    bool load_registry() {
        auto start_time = std::chrono::high_resolution_clock::now();

        try {
            if (!std::filesystem::exists(registry_path_)) {
                return false;
            }

            std::ifstream file(registry_path_);
            if (!file.is_open()) return false;

            // Simple JSON parsing (in a real implementation, use a proper JSON library)
            std::string content((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());

            // For now, just mark as loaded
            for (auto& section : sections_) {
                section.second.is_loaded = true;
                section.second.last_saved = std::chrono::steady_clock::now();
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            auto load_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

            last_load_time_ = std::chrono::steady_clock::now();
            load_count_++;

            // Update average load time
            average_load_time_ms_ = (average_load_time_ms_ * (load_count_ - 1) + load_duration.count() / 1000.0) / load_count_;

            return true;

        } catch (const std::exception&) {
            return false;
        }
    }

    std::string escape_json_string(const std::string& str) const {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default: result += c; break;
            }
        }
        return result;
    }

    void update_total_entries_count() {
        total_entries_ = 0;
        for (const auto& section : sections_) {
            total_entries_ += section.second.entries.size();
        }
    }

    std::vector<std::string> get_sections() const {
        std::shared_lock<std::shared_mutex> lock(registry_mutex_);
        return section_order_;
    }

    std::vector<std::string> get_entries_in_section(const std::string& section) const {
        std::shared_lock<std::shared_mutex> lock(registry_mutex_);

        auto section_it = sections_.find(section);
        if (section_it != sections_.end()) {
            return section_it->second.entry_order;
        }
        return {};
    }

    void reset_entry_to_default(const std::string& section, const std::string& key) {
        std::unique_lock<std::shared_mutex> lock(registry_mutex_);

        auto section_it = sections_.find(section);
        if (section_it != sections_.end()) {
            auto entry_it = section_it->second.entries.find(key);
            if (entry_it != section_it->second.entries.end()) {
                entry_it->second.value = entry_it->second.default_value;
                entry_it->second.is_modified = false;
                entry_it->second.last_modified = std::chrono::steady_clock::now();
            }
        }
    }

    void reset_section_to_defaults(const std::string& section) {
        std::unique_lock<std::shared_mutex> lock(registry_mutex_);

        auto section_it = sections_.find(section);
        if (section_it != sections_.end()) {
            for (auto& entry : section_it->second.entries) {
                entry.second.value = entry.second.default_value;
                entry.second.is_modified = false;
                entry.second.last_modified = std::chrono::steady_clock::now();
            }
        }
    }

    void reset_all_to_defaults() {
        std::unique_lock<std::shared_mutex> lock(registry_mutex_);

        for (auto& section : sections_) {
            for (auto& entry : section.second.entries) {
                entry.second.value = entry.second.default_value;
                entry.second.is_modified = false;
                entry.second.last_modified = std::chrono::steady_clock::now();
            }
        }
        modified_entries_ = 0;
    }

    // Convenience methods for different data types
    int get_int(const std::string& section, const std::string& key) const {
        std::string value = get_entry(section, key);
        return value.empty() ? 0 : std::stoi(value);
    }

    float get_float(const std::string& section, const std::string& key) const {
        std::string value = get_entry(section, key);
        return value.empty() ? 0.0f : std::stof(value);
    }

    bool get_bool(const std::string& section, const std::string& key) const {
        std::string value = get_entry(section, key);
        return value == "true";
    }

    void set_int(const std::string& section, const std::string& key, int value) {
        set_entry_value(section, key, std::to_string(value));
    }

    void set_float(const std::string& section, const std::string& key, float value) {
        set_entry_value(section, key, std::to_string(value));
    }

    void set_bool(const std::string& section, const std::string& key, bool value) {
        set_entry_value(section, key, value ? "true" : "false");
    }
};

ConfigurationRegistry::ConfigurationRegistry() : impl_(std::make_unique<Impl>()) {}

ConfigurationRegistry::~ConfigurationRegistry() = default;

bool ConfigurationRegistry::initialize() {
    return impl_->load_registry();
}

void ConfigurationRegistry::shutdown() {
    impl_->save_registry();
}

bool ConfigurationRegistry::save_registry() {
    return impl_->save_registry();
}

bool ConfigurationRegistry::load_registry() {
    return impl_->load_registry();
}

std::string ConfigurationRegistry::get_string(const std::string& section, const std::string& key) const {
    return impl_->get_entry(section, key);
}

int ConfigurationRegistry::get_int(const std::string& section, const std::string& key) const {
    return impl_->get_int(section, key);
}

float ConfigurationRegistry::get_float(const std::string& section, const std::string& key) const {
    return impl_->get_float(section, key);
}

bool ConfigurationRegistry::get_bool(const std::string& section, const std::string& key) const {
    return impl_->get_bool(section, key);
}

void ConfigurationRegistry::set_string(const std::string& section, const std::string& key, const std::string& value) {
    impl_->set_entry_value(section, key, value);
}

void ConfigurationRegistry::set_int(const std::string& section, const std::string& key, int value) {
    impl_->set_int(section, key, value);
}

void ConfigurationRegistry::set_float(const std::string& section, const std::string& key, float value) {
    impl_->set_float(section, key, value);
}

void ConfigurationRegistry::set_bool(const std::string& section, const std::string& key, bool value) {
    impl_->set_bool(section, key, value);
}

} // namespace SU1

