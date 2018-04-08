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

        for (const auto &e : names) {
            auto u = diagnostic_updater::Updater(nodeHandle, ros::NodeHandle(nodeHandle, e));
            u.setHardwareID(t->getHandle(e).getDHD().hardwareID);
            u.add(e, boost::bind(&DiagnosticController::do_diag, this, _1, e));
            this->updaters[e] = u;
        }

        this->dsi = t;
    }



    void controller_diagnostics::DiagnosticController::update(const ros::Time &time, const ros::Duration &period) {
        for (const auto &e : this->updaters) {
            e.second.update();
        }
    }

    DiagnosticController::~DiagnosticController() = default;

    void DiagnosticController::do_diag(diagnostic_updater::DiagnosticStatusWrapper &stat, std::string e) {
        DiagnosticHandleData hd = this->dsi->getHandle(e).getDHD();
        stat.summary(hd.status, hd.message);
        for (auto ele : hd.data) {
            stat.add(ele.first, ele.second);
        }
    }
}

PLUGINLIB_EXPORT_CLASS(controller_diagnostics::DiagnosticController, controller_interface::ControllerBase)