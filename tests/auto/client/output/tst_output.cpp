/****************************************************************************
 * This file is part of Hawaii.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtCore/QThread>
#include <QtTest/QtTest>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandOutput>

#include <LiriWaylandClient/ClientConnection>
#include <LiriWaylandClient/Compositor>
#include <LiriWaylandClient/Output>
#include <LiriWaylandClient/Registry>

using namespace Liri::WaylandClient;

static const QString s_socketName = QStringLiteral("liri-test-0");

class TestOutput : public QObject
{
    Q_OBJECT
public:
    TestOutput(QObject *parent = nullptr)
        : QObject(parent)
        , m_compositor(nullptr)
        , m_output(nullptr)
        , m_thread(nullptr)
        , m_display(nullptr)
    {
    }

private:
    QWaylandCompositor *m_compositor;
    QWaylandOutput *m_output;
    QThread *m_thread;
    ClientConnection *m_display;

private Q_SLOTS:
    void init()
    {
        delete m_compositor;
        m_compositor = new QWaylandCompositor(this);
        m_compositor->setSocketName(s_socketName.toUtf8());

        m_output = new QWaylandOutput(m_compositor, nullptr);
        m_output->addMode(QWaylandOutputMode(QSize(800, 600), 60000), true);
        QWaylandOutputMode currentMode(QSize(1024, 768), 60000);
        m_output->addMode(currentMode);
        m_output->addMode(QWaylandOutputMode(QSize(1920, 1080), 60000));
        m_output->setCurrentMode(currentMode);
        m_output->setPosition(QPoint(0, 0));
        m_output->setPhysicalSize(QSize(350, 420));
        m_compositor->create();

        delete m_display;
        m_display = new ClientConnection();
        m_display->setSocketName(s_socketName);

        delete m_thread;
        m_thread = new QThread(this);
        m_display->moveToThread(m_thread);
        m_thread->start();

        QSignalSpy connectedSpy(m_display, SIGNAL(connected()));
        QSignalSpy failedSpy(m_display, SIGNAL(failed()));
        m_display->initializeConnection();
        QVERIFY(connectedSpy.wait());
        QCOMPARE(connectedSpy.count(), 1);
        QCOMPARE(failedSpy.count(), 0);
        QVERIFY(m_display->display());
    }

    void cleanup()
    {
        delete m_output;
        m_output = nullptr;

        delete m_compositor;
        m_compositor = nullptr;

        if (m_thread) {
            m_thread->quit();
            m_thread->wait();
            delete m_thread;
            m_thread = nullptr;
        }

        delete m_display;
        m_display = nullptr;
    }

    void testAnnounce()
    {
        Registry registry;
        registry.create(m_display->display());
        QSignalSpy announced(&registry, SIGNAL(outputAnnounced(quint32,quint32)));
        QVERIFY(announced.isValid());
        registry.setup();

        QVERIFY(announced.wait());

        const quint32 name = announced.first().first().value<quint32>();
        const quint32 version = announced.first().last().value<quint32>();
        Output *output = registry.createOutput(name, version, this);
        QVERIFY(output);

        // Output holds an invalid data at the beginning
        QCOMPARE(output->manufacturer(), QString());
        QCOMPARE(output->model(), QString());
        QCOMPARE(output->physicalSize(), QSize());
        QCOMPARE(output->position(), QPoint(0, 0));
        QCOMPARE(output->size(), QSize(-1, -1));
        QVERIFY(output->geometry().isEmpty());
        QVERIFY(output->geometry().isNull());
        QCOMPARE(output->scale(), 1);
        QCOMPARE(output->transform(), Output::TransformNormal);
        QCOMPARE(output->subpixel(), Output::SubpixelUnknown);

        // Then we should receive the current geometry
        QSignalSpy outputSpy(output, SIGNAL(outputChanged()));
        QVERIFY(outputSpy.isValid());
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->geometry(), QRect(0, 0, 1024, 768));
        QCOMPARE(output->physicalSize(), QSize(350, 420));
    }

    void testChanged()
    {
        Registry registry;
        registry.create(m_display->display());
        QSignalSpy announced(&registry, SIGNAL(outputAnnounced(quint32,quint32)));
        QVERIFY(announced.isValid());
        registry.setup();

        QVERIFY(announced.wait());

        const quint32 name = announced.first().first().value<quint32>();
        const quint32 version = announced.first().last().value<quint32>();
        Output *output = registry.createOutput(name, version, this);
        QVERIFY(output);

        QSignalSpy outputSpy(output, SIGNAL(outputChanged()));
        QVERIFY(outputSpy.isValid());

        // Manufacturer
        m_output->setManufacturer(QStringLiteral("Manufacturer"));
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->manufacturer(), QStringLiteral("Manufacturer"));

        // Model
        m_output->setModel(QStringLiteral("Model"));
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->model(), QStringLiteral("Model"));

        // Position
        m_output->setPosition(QPoint(1024, 0));
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->position(), QPoint(1024, 0));

        // Physical size
        m_output->setPhysicalSize(QSize(400, 400));
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->physicalSize(), QSize(400, 400));

        // Scale
        m_output->setScaleFactor(2);
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->scale(), 2);

        // Subpixel
        m_output->setSubpixel(QWaylandOutput::SubpixelHorizontalBgr);
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->subpixel(), Output::SubpixelHorizontalBgr);

        // Transform
        m_output->setTransform(QWaylandOutput::Transform90);
        QVERIFY(outputSpy.wait());
        QCOMPARE(output->transform(), Output::Transform90);
    }

    void testMode()
    {
        Registry registry;
        registry.create(m_display->display());
        QSignalSpy announced(&registry, SIGNAL(outputAnnounced(quint32,quint32)));
        QVERIFY(announced.isValid());
        registry.setup();

        QVERIFY(announced.wait());

        const quint32 name = announced.first().first().value<quint32>();
        const quint32 version = announced.first().last().value<quint32>();
        Output *output = registry.createOutput(name, version, this);
        QVERIFY(output);

        QSignalSpy outputSpy(output, SIGNAL(outputChanged()));
        QVERIFY(outputSpy.isValid());
        QSignalSpy modeAddedSpy(output, SIGNAL(modeAdded(Output::Mode)));
        QVERIFY(modeAddedSpy.isValid());
        QSignalSpy modeChangedSpy(output, SIGNAL(modeChanged(Output::Mode)));
        QVERIFY(modeChangedSpy.isValid());
        QSignalSpy sizeChangedSpy(output, SIGNAL(sizeChanged()));
        QVERIFY(sizeChangedSpy.isValid());
        QSignalSpy geometryChangedSpy(output, SIGNAL(geometryChanged()));
        QVERIFY(geometryChangedSpy.isValid());
        QSignalSpy refreshRateChangedSpy(output, SIGNAL(refreshRateChanged()));
        QVERIFY(refreshRateChangedSpy.isValid());

        QVERIFY(outputSpy.wait());

        QCOMPARE(modeAddedSpy.count(), 3);
        QCOMPARE(modeAddedSpy.at(0).first().value<Output::Mode>().flags, Output::PreferredMode);
        QCOMPARE(modeAddedSpy.at(0).first().value<Output::Mode>().size, QSize(800, 600));
        QCOMPARE(modeAddedSpy.at(0).first().value<Output::Mode>().refreshRate, qreal(60));
        QCOMPARE(modeAddedSpy.at(1).first().value<Output::Mode>().flags, Output::CurrentMode);
        QCOMPARE(modeAddedSpy.at(1).first().value<Output::Mode>().size, QSize(1024, 768));
        QCOMPARE(modeAddedSpy.at(1).first().value<Output::Mode>().refreshRate, qreal(60));
        QCOMPARE(modeAddedSpy.at(2).first().value<Output::Mode>().flags, 0);
        QCOMPARE(modeAddedSpy.at(2).first().value<Output::Mode>().size, QSize(1920, 1080));
        QCOMPARE(modeAddedSpy.at(2).first().value<Output::Mode>().refreshRate, qreal(60));

        QList<Output::Mode> modes = output->modes();
        QCOMPARE(modes.count(), 3);
        QCOMPARE(modes.at(0).size, QSize(800, 600));
        QCOMPARE(modes.at(0).flags, Output::PreferredMode);
        QCOMPARE(modes.at(0).refreshRate, qreal(60));
        QCOMPARE(modes.at(1).size, QSize(1024, 768));
        QCOMPARE(modes.at(1).flags, Output::CurrentMode);
        QCOMPARE(modes.at(1).refreshRate, qreal(60));
        QCOMPARE(modes.at(2).size, QSize(1920, 1080));
        QCOMPARE(modes.at(2).flags, 0);
        QCOMPARE(modes.at(2).refreshRate, qreal(60));

        m_output->setCurrentMode(QWaylandOutputMode(QSize(1920, 1080), 60000));
        QVERIFY(modeChangedSpy.wait());
        QCOMPARE(sizeChangedSpy.count(), 5);
        QCOMPARE(geometryChangedSpy.count(), 5);
        QCOMPARE(refreshRateChangedSpy.count(), 5);
        QCOMPARE(modeChangedSpy.count(), 2);
        QCOMPARE(modeChangedSpy.at(1).first().value<Output::Mode>().flags, Output::CurrentMode);
        QCOMPARE(modeChangedSpy.at(1).first().value<Output::Mode>().size, QSize(1920, 1080));
        QCOMPARE(modeChangedSpy.at(1).first().value<Output::Mode>().refreshRate, qreal(60));

        modes = output->modes();
        QCOMPARE(modes.count(), 3);
        QCOMPARE(modes.at(2).size, QSize(1920, 1080));
        QCOMPARE(modes.at(2).flags, Output::CurrentMode);
        QCOMPARE(modes.at(2).refreshRate, qreal(60));
    }
};

QTEST_MAIN(TestOutput)

#include "tst_output.moc"
