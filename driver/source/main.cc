#include "main.hpp"

Lidar::Lidar(const char *path):
    m_Lidar(rplidar::RPlidarDriver::CreateDriver())
{
    if (!IS_OK(m_Lidar->connect(path, 115200))) {
        throw std::runtime_error("Failed to connect to Lidar.");
    }
}

void Lidar::SelectScanMode(const char *mode) {
    std::vector<rplidar::RplidarScanMode> modes = {};
    m_Lidar->getAllSupportedScanModes(modes);

    // Prints all the available modes, as debug info ...
    //  Although I highly doubt if it ever will change.
    std::cout << "Available Modes:" << std::endl;
    std::for_each(modes.begin(), modes.end(), [](const rplidar::RplidarScanMode &mode) {
        std::cout << "\t" << mode.id << ": " << mode.us_per_sample << "us, " << mode.max_distance << "m, " << mode.scan_mode << std::endl;
    });

    // Attempts to get the scan-mode specified in the argument
    //  if not, throw runtime error.
    std::vector<rplidar::RplidarScanMode>::iterator it = std::find_if(
        modes.begin(), modes.end(),
        [&](const rplidar::RplidarScanMode &c) {
            return strcmp(mode, c.scan_mode) == 0;
        }
    );

    if (it == modes.end()) {
        throw std::runtime_error("Scan mode does not exist!");
    }

    std::cout << "Scan-mode " << it->id << " selected!" << std::endl;
    m_ScanMode = *it;
}

void Lidar::StartScan() {
    assert(m_ScanMode);

    if (!IS_OK(m_Lidar->startMotor())) {
        throw std::runtime_error("Failed to start motor!");
    } else if (!IS_OK(m_Lidar->startScanExpress(false, m_ScanMode->id))) {
        throw std::runtime_error("Failed to start scan!");
    }
}

std::size_t Lidar::ScanData(rplidar_response_measurement_node_hq_t *res, std::size_t size) {
    std::cout << "scanning" << std::endl;
    u_result err = m_Lidar->grabScanDataHq(res, size);
    if (!IS_OK(err)) {
        throw std::runtime_error("Failed to get scan data");
    }

    return size;
}

void Lidar::StopScan() {
    if (!IS_OK(m_Lidar->startMotor())) {
        throw std::runtime_error("Failed to start motor!");
    }

    m_Lidar->stop();
}

Lidar::~Lidar() {
    if (m_Lidar->isConnected()) {
        m_Lidar->disconnect();
    }

    rplidar::RPlidarDriver::DisposeDriver(m_Lidar);
}

#define MAX_NODE_COUNT 8192



int32_t main(int32_t argc, char **argv) {
    const char *path = "/dev/ttyUSB0";
    if (argc > 1) {
        path = argv[1];
    }

    Lidar main(path);
    main.SelectScanMode("Boost");
    main.StartScan();

    std::array<rplidar_response_measurement_node_hq_t, MAX_NODE_COUNT> nodes;
    while (true) {
        try {
            std::size_t count = main.ScanData(&nodes[0], MAX_NODE_COUNT);
            for (std::size_t i = 0; i < count; ++i) {
                if (nodes[i].dist_mm_q2 == 0) continue;

                std::cout << nodes[i].dist_mm_q2 << ", " << nodes[i].angle_z_q14 << std::endl;
            }
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}