//
// Created by matthew on 4/7/18.
//

#ifndef CONTROLLER_DIAGNOSTICS_DIAGNOSTIC_CONTROLLER_H
#define CONTROLLER_DIAGNOSTICS_DIAGNOSTIC_CONTROLLER_H

#include <controller_interface/controller.h>
#include <controller_diagnostics/diagnostic_interface.h>
#include <diagnostic_updater/diagnostic_updater.h>

namespace controller_diagnostics {

    class DiagnosticController : public controller_interface::Controller<controller_diagnostics::DiagnosticStateInterface> {
    public:
        virtual bool init(DiagnosticStateInterface *t, ros::NodeHandle &handle, ros::NodeHandle &nodeHandle);
        virtual void update(const ros::Time &time, const ros::Duration &period);

        void do_diag(diagnostic_updater::DiagnosticStatusWrapper &stat, std::string e);

        virtual ~DiagnosticController();

    private:
        std::map<std::string, diagnostic_updater::Updater *> updaters;
        DiagnosticStateInterface *dsi;
    };

}

#endif //CONTROLLER_DIAGNOSTICS_DIAGNOSTIC_CONTROLLER_H
