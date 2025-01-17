/**
 ******************************************************************************
 *
 * @file       configgadgetwidget.cpp
 * @author     E. Lafargue & The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @author     Tau Labs, http://taulabs.org, Copyright (C) 2013-2014
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup ConfigPlugin Config Plugin
 * @{
 * @brief The Configuration Gadget used to update settings in the firmware
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "configgadgetwidget.h"

#include "configvehicletypewidget.h"
#include "configinputwidget.h"
#include "configoutputwidget.h"
#include "configstabilizationwidget.h"
#include "configosdwidget.h"
#include "configmodulewidget.h"
#include "configautotunewidget.h"
#include "configcamerastabilizationwidget.h"
#include "configtxpidwidget.h"
#include "configattitudewidget.h"
#include "defaulthwsettingswidget.h"
#include "uavobjectutilmanager.h"

#include <QDebug>
#include <QStringList>
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>



ConfigGadgetWidget::ConfigGadgetWidget(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    ftw = new MyTabbedStackWidget(this, true, true);
    ftw->setIconSize(64);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(ftw);
    setLayout(layout);

    // *********************
    QWidget *qwd;

    QIcon *icon = new QIcon();
    icon->addFile(":/configgadget/images/hardware_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/hardware_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new DefaultHwSettingsWidget(this, false);
    ftw->insertTab(ConfigGadgetWidget::hardware, qwd, *icon, QString("Board"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/vehicle_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/vehicle_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigVehicleTypeWidget(this);
    ftw->insertTab(ConfigGadgetWidget::aircraft, qwd, *icon, QString("Vehicle"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/input_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/input_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigInputWidget(this);
    ftw->insertTab(ConfigGadgetWidget::input, qwd, *icon, QString("Input"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/output_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/output_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigOutputWidget(this);
    ftw->insertTab(ConfigGadgetWidget::output, qwd, *icon, QString("Output"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/ins_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/ins_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigAttitudeWidget(this);
    ftw->insertTab(ConfigGadgetWidget::sensors, qwd, *icon, QString("Attitude"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/stabilization_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/stabilization_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigStabilizationWidget(this);
    ftw->insertTab(ConfigGadgetWidget::stabilization, qwd, *icon, QString("Stabilization"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/osd_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/osd_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigOsdWidget(this);
    ftw->insertTab(ConfigGadgetWidget::osd, qwd, *icon, QString("OSD"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/modules_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/modules_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigModuleWidget(this);
    ftw->insertTab(ConfigGadgetWidget::modules, qwd, *icon, QString("Modules"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/autotune_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/autotune_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigAutotuneWidget(this);
    ftw->insertTab(ConfigGadgetWidget::autotune, qwd, *icon, QString("Autotune"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/camstab_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/camstab_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigCameraStabilizationWidget(this);
    ftw->insertTab(ConfigGadgetWidget::camerastabilization, qwd, *icon, QString("Camera Stab"));

    icon = new QIcon();
    icon->addFile(":/configgadget/images/txpid_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/txpid_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigTxPIDWidget(this);
    ftw->insertTab(ConfigGadgetWidget::txpid, qwd, *icon, QString("TxPID"));

    ftw->setCurrentIndex(ConfigGadgetWidget::hardware);
    // *********************
    // Listen to autopilot connection events

    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    TelemetryManager* telMngr = pm->getObject<TelemetryManager>();
    connect(telMngr, SIGNAL(connected()), this, SLOT(onAutopilotConnect()));
    connect(telMngr, SIGNAL(disconnected()), this, SLOT(onAutopilotDisconnect()));

    // And check whether by any chance we are not already connected
    if (telMngr->isConnected()) {
        onAutopilotConnect();    
    }

    help = 0;
    connect(ftw,SIGNAL(currentAboutToShow(int,bool*)),this,SLOT(tabAboutToChange(int,bool*)));//,Qt::BlockingQueuedConnection);
}

ConfigGadgetWidget::~ConfigGadgetWidget()
{
    // TODO: properly delete all the tabs in ftw before exiting
}

void ConfigGadgetWidget::startInputWizard()
{
    ftw->setCurrentIndex(ConfigGadgetWidget::input);
    ConfigInputWidget* inputWidget = dynamic_cast<ConfigInputWidget*>(ftw->getWidget(ConfigGadgetWidget::input));
    Q_ASSERT(inputWidget);
    inputWidget->startInputWizard();
}

void ConfigGadgetWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void ConfigGadgetWidget::onAutopilotDisconnect() {
    ftw->setCurrentIndex(ConfigGadgetWidget::hardware);
    ftw->removeTab(ConfigGadgetWidget::hardware);

    QIcon *icon = new QIcon();
    icon->addFile(":/configgadget/images/hardware_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/hardware_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    QWidget *qwd = new DefaultHwSettingsWidget(this, false);
    ftw->insertTab(ConfigGadgetWidget::hardware, qwd, *icon, QString("Hardware"));
    ftw->setCurrentIndex(ConfigGadgetWidget::hardware);

    emit autopilotDisconnected();
}

void ConfigGadgetWidget::onAutopilotConnect() {

    QIcon* icon;
    QWidget* qwd;

    qDebug()<<"ConfigGadgetWidget onAutopilotConnect";
    // First of all, check what Board type we are talking to, and
    // if necessary, remove/add tabs in the config gadget:
    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    UAVObjectUtilManager* utilMngr = pm->getObject<UAVObjectUtilManager>();
    if (utilMngr) {
        ftw->setCurrentIndex(ConfigGadgetWidget::hardware);
        ftw->removeTab(ConfigGadgetWidget::hardware);
        icon = new QIcon();
        icon->addFile(":/configgadget/images/hardware_normal.png", QSize(), QIcon::Normal, QIcon::Off);
        icon->addFile(":/configgadget/images/hardware_selected.png", QSize(), QIcon::Selected, QIcon::Off);

        // If the board provides a custom configuration widget then use it,
        // otherwise use the default which populates the fields from the
        // hardware UAVO
        Core::IBoardType *board = utilMngr->getBoardType();
        if (board == NULL) {
            QLabel *txt = new QLabel(this);
            txt->setText(tr("Board detected, but of unknown type. This could be because either your GCS or firmware is out of date."));
            qwd = txt;
        }
        else {
            qwd = board->getBoardConfiguration();
            if (qwd == NULL) {
                qwd = new DefaultHwSettingsWidget(this, true);
            } else {
            }
        }

        ftw->insertTab(ConfigGadgetWidget::hardware, qwd, *icon, QString(tr("Hardware")));
        ftw->setCurrentIndex(ConfigGadgetWidget::hardware);
    }

    //! Remove and recreate the attitude widget to refresh board capabilities
    ftw->removeTab(ConfigGadgetWidget::sensors);
    icon = new QIcon();
    icon->addFile(":/configgadget/images/ins_normal.png", QSize(), QIcon::Normal, QIcon::Off);
    icon->addFile(":/configgadget/images/ins_selected.png", QSize(), QIcon::Selected, QIcon::Off);
    qwd = new ConfigAttitudeWidget(this);
    ftw->insertTab(ConfigGadgetWidget::sensors, qwd, *icon, QString("Attitude"));

    emit autopilotConnected();
}

void ConfigGadgetWidget::tabAboutToChange(int i, bool * proceed)
{
    Q_UNUSED(i);
    *proceed = true;
    ConfigTaskWidget * wid = qobject_cast<ConfigTaskWidget *>(ftw->currentWidget());
    if(!wid) {
        return;
    }

    // Check if widget is dirty (i.e. has unsaved changes)
    if(wid->isDirty() && wid->isAutopilotConnected())
    {
        int ans=QMessageBox::warning(this,tr("Unsaved changes"),tr("The tab you are leaving has unsaved changes,"
                                                           "if you proceed they may be lost."
                                                           "Do you still want to proceed?"),QMessageBox::Yes,QMessageBox::No);
        if(ans==QMessageBox::No) {
            *proceed=false;
        } else {
            wid->setDirty(false);
        }
    }
}
