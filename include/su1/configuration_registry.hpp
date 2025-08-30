#pragma once

#include "su1/core.hpp"
#include <vector>
#include <string>
#include <memory>

namespace SU1 {

class ConfigurationRegistry {
public:
    ConfigurationRegistry();
    ~ConfigurationRegistry();

    bool initialize();
    void shutdown();

    // Registry management
    bool save_registry();
    bool load_registry();

    // Getters for different data types
    std::string get_string(const std::string& section, const std::string& key) const;
    int get_int(const std::string& section, const std::string& key) const;
    float get_float(const std::string& section, const std::string& key) const;
    bool get_bool(const std::string& section, const std::string& key) const;

    // Setters for different data types
    void set_string(const std::string& section, const std::string& key, const std::string& value);
    void set_int(const std::string& section, const std::string& key, int value);
    void set_float(const std::string& section, const std::string& key, float value);
    void set_bool(const std::string& section, const std::string& key, bool value);

    // Registry structure
    std::vector<std::string> get_sections() const;
    std::vector<std::string> get_entries_in_section(const std::string& section) const;

    // Reset functions
    void reset_entry_to_default(const std::string& section, const std::string& key);
    void reset_section_to_defaults(const std::string& section);
    void reset_all_to_defaults();

    // Entry information
    std::string get_entry_description(const std::string& section, const std::string& key) const;
    std::string get_entry_type(const std::string& section, const std::string& key) const;
    bool entry_requires_restart(const std::string& section, const std::string& key) const;
    bool entry_is_modified(const std::string& section, const std::string& key) const;

    // Auto-save settings
    void set_auto_save_enabled(bool enabled);
    void set_auto_save_interval(int seconds);

    // Statistics
    int get_total_entries_count() const;
    int get_modified_entries_count() const;
    double get_average_save_time_ms() const;
    double get_average_load_time_ms() const;
    int get_save_count() const;
    int get_load_count() const;

    // Backup and restore
    bool create_backup();
    bool restore_from_backup();

    // Import/Export
    bool export_to_file(const std::string& filename);
    bool import_from_file(const std::string& filename);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace SU1
