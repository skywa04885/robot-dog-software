#pragma once

#include <cstdint>
#include <iostream>
#include <algorithm>
#include <optional>
#include <array>
#include <assert.h>

#include <rplidar.h>
#include <string.h>

#include <panda3d/panda.h>
#include <panda3d/pandaFramework.h>

using namespace rp::standalone;

class Lidar {
public:
    Lidar(const char *path);

    void SelectScanMode(const char *mode);
    void StartScan();
    void StopScan();

    std::size_t ScanData(rplidar_response_measurement_node_hq_t *res, std::size_t size);

    ~Lidar();
private:
    rplidar::RPlidarDriver *m_Lidar;
    std::optional<rplidar::RplidarScanMode> m_ScanMode;
};

int32_t main(int32_t argc, char **argv);
