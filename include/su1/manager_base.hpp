#pragma once

#include "su1/core.hpp"

namespace su1 {

class Manager {
public:
    Manager() = default;
    virtual ~Manager() = default;

    virtual bool initialize() { return true; }
    virtual void shutdown() {}
    virtual void update() {}

    bool is_initialized() const { return initialized_; }

protected:
    bool initialized_ = false;
};

// Common manager typedefs
using ManagerPtr = std::shared_ptr<Manager>;

// Forward declarations for missing managers
class InputManager;
class Compositor;
class Theme;
class Config;
class NotificationManager;
class PowerManager;
class SecurityManager;
class AccessibilityManager;
class HotkeyManager;
class AnimationEngine;
class EffectEngine;
class SessionManager;
class NetworkManager;
class AudioManager;
class VideoManager;
class CameraManager;
class SensorManager;
class BluetoothManager;
class WifiManager;
class CellularManager;
class GPSManager;
class NFCManager;
class RFIDManager;
class BarcodeManager;
class FingerprintManager;
class FaceManager;
class IrisManager;
class VoiceManager;
class GestureManager;
class HapticManager;
class ForceManager;
class PressureManager;
class TemperatureManager;
class HumidityManager;
class LightManager;
class ColorManager;
class ProximityManager;
class AccelerationManager;
class GyroscopeManager;
class MagnetometerManager;
class BarometerManager;
class AltimeterManager;
class CompassManager;
class PedometerManager;
class StepManager;
class ActivityManager;
class HeartRateManager;
class BloodPressureManager;
class GlucoseManager;
class OxygenManager;
class ECGManager;
class EMGManager;
class EEGManager;
class EOGManager;
class GSRManager;
class RespirationManager;
class BodyTemperatureManager;
class SkinTemperatureManager;
class AmbientTemperatureManager;
class UVManager;
class RadiationManager;
class ChemicalManager;
class BiologicalManager;
class GeneticManager;
class NeuralManager;
class QuantumManager;
class RelativisticManager;
class SubatomicManager;
class NanoscopicManager;
class MicroscopicManager;
class MacroscopicManager;
class CosmicManager;
class UniversalManager;
class MultiversalManager;
class OmniversalManager;

} // namespace su1
