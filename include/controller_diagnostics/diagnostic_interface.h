//
// Created by matthew on 4/7/18.
//

#ifndef CONTROLLER_DIAGNOSTICS_DIAGNOSTIC_INTERFACE_H
#define CONTROLLER_DIAGNOSTICS_DIAGNOSTIC_INTERFACE_H

#include <hardware_interface/internal/hardware_resource_manager.h>
#include <diagnostic_msgs/DiagnosticStatus.h>
#include <map>
#include <utility>

namespace controller_diagnostics {

    struct DiagnosticHandleData {
        std::string message = "";
        unsigned char status = diagnostic_msgs::DiagnosticStatus::STALE;
        std::map<std::string, std::string> data;
        std::string hardwareID;
    };

    class DiagnosticHandle {
    public:
        DiagnosticHandle(std::string name, const DiagnosticHandleData *dhd) : name(name), dhd(dhd) {}

        std::string getName()         const {return this->name;}
        DiagnosticHandleData getDHD() const {return *this->dhd;}

    private:
        std::string name;
        const DiagnosticHandleData *dhd;
    };

    class DiagnosticStateInterface : public hardware_interface::HardwareResourceManager<DiagnosticHandle, hardware_interface::DontClaimResources> {};

}

#endif //CONTROLLER_DIAGNOSTICS_DIAGNOSTIC_INTERFACE_H
