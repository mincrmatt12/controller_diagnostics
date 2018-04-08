//
// Created by matthew on 4/7/18.
//

#include <controller_diagnostics/diagnostic_interface.h>
#include <controller_interface/controller.h>
#include <controller_diagnostics/diagnostic_controller.h>
#include <pluginlib/class_list_macros.h>

namespace controller_diagnostics {

    bool controller_diagnostics::DiagnosticController::init(DiagnosticStateInterface *t, ros::NodeHandle &handle,
                                                            ros::NodeHandle &nodeHandle) {
        const std::vector<std::string>& names = t->getNames();
        ROS_DEBUG_STREAM("Got " << names.size() << " diagnostics");
        std::string name;
        if (!nodeHandle.getParam("name", name)) {
            ROS_FATAL_STREAM("Failed to start diagnostic_controller: couldn't read parameter name!");
            return false;
        }

        for (const auto &e : names) {
            auto u = new diagnostic_updater::Updater(nodeHandle, ros::NodeHandle(nodeHandle, e), name);
            u->setHardwareID(t->getHandle(e).getDHD().hardwareID);
            u->add(e, boost::bind(&DiagnosticController::do_diag, this, _1, e));
            this->updaters[e] = u;
        }

        this->dsi = t;

        return true;
    }

    DiagnosticController::~DiagnosticController() {
        for (auto e : this->updaters) {
            delete e.second;
        }
    }

    void controller_diagnostics::DiagnosticController::update(const ros::Time &time, const ros::Duration &period) {
        for (const auto &e : this->updaters) {
            e.second->update();
        }
    }

    void DiagnosticController::do_diag(diagnostic_updater::DiagnosticStatusWrapper &stat, std::string e) {
        DiagnosticHandleData hd = this->dsi->getHandle(e).getDHD();
        stat.summary(hd.status, hd.message);
        for (auto ele : hd.data) {
            stat.add(ele.first, ele.second);
        }
    }
}

PLUGINLIB_EXPORT_CLASS(controller_diagnostics::DiagnosticController, controller_interface::ControllerBase)