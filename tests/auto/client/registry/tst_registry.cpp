/****************************************************************************
 * This file is part of Liri.
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
#include <QtQuick/QQuickItem>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandSeat>
#include <QtWaylandCompositor/QWaylandOutput>
#include <QtWaylandCompositor/private/qwaylandcompositor_p.h>

#include <LiriWaylandClient/Buffer>
#include <LiriWaylandClient/ClientConnection>
#include <LiriWaylandClient/Compositor>
#include <LiriWaylandClient/FullScreenShell>
#include <LiriWaylandClient/Output>
#include <LiriWaylandClient/Registry>
#include <LiriWaylandClient/Region>
#include <LiriWaylandClient/Screencaster>
#include <LiriWaylandClient/Screenshooter>
#include <LiriWaylandClient/Seat>
#include <LiriWaylandClient/Shm>
#include <LiriWaylandClient/ShmPool>
#include <LiriWaylandClient/Surface>
#include <LiriWaylandServer/Screencaster>
#include <LiriWaylandServer/Screenshooter>

using namespace Liri;

static const QString s_socketName = QStringLiteral("liri-test-0");

class TestRegistry : public QObject
{
    Q_OBJECT
public:
    TestRegistry(QObject *parent = nullptr)
        : QObject(parent)
        , m_compositor(nullptr)
    {
    }

private:
    QWaylandCompositor *m_compositor;

private Q_SLOTS:
    void init()
    {
        delete m_compositor;

        m_compositor = new QWaylandCompositor(this);
        m_compositor->setSocketName(s_socketName.toUtf8());

        new QWaylandOutput(m_compositor, nullptr);
        new WaylandServer::Screencaster(m_compositor);
        new WaylandServer::Screenshooter(m_compositor);

        m_compositor->create();
    }

    void cleanup()
    {
        delete m_compositor;
        m_compositor = nullptr;
    }

    void testSetup()
    {
        WaylandClient::ClientConnection *display(new WaylandClient::ClientConnection());
        display->setSocketName(s_socketName);

        QThread *thread = new QThread(this);
        display->moveToThread(thread);
        thread->start();

        QSignalSpy connectedSpy(display, SIGNAL(connected()));
        QSignalSpy failedSpy(display, SIGNAL(failed()));
        display->initializeConnection();
        QVERIFY(connectedSpy.wait());
        QCOMPARE(connectedSpy.count(), 1);
        QCOMPARE(failedSpy.count(), 0);
        QVERIFY(display->display());

        WaylandClient::Registry registry;
        registry.create(display->display());
        registry.setup();

        display->deleteLater();
        thread->quit();
        thread->wait();
        delete thread;
    }

#define TEST_CREATE_BEGIN() \
    WaylandClient::ClientConnection *display(new WaylandClient::ClientConnection()); \
    display->setSocketName(s_socketName); \
    \
    QThread *thread = new QThread(this); \
    display->moveToThread(thread); \
    thread->start(); \
    \
    QSignalSpy connectedSpy(display, SIGNAL(connected())); \
    QSignalSpy failedSpy(display, SIGNAL(failed())); \
    display->initializeConnection(); \
    QVERIFY(connectedSpy.wait()); \
    QCOMPARE(connectedSpy.count(), 1); \
    QCOMPARE(failedSpy.count(), 0); \
    QVERIFY(display->display())

#define TEST_CREATE_BODY(signalSignature, createMethod) \
    WaylandClient::Registry registry; \
    registry.create(display->display()); \
    QSignalSpy announced(&registry, signalSignature); \
    QVERIFY(announced.isValid()); \
    registry.setup(); \
    display->flushRequests(); \
    QVERIFY(announced.wait()); \
    const quint32 name = announced.first().first().value<quint32>(); \
    const quint32 version = announced.first().last().value<quint32>(); \
    auto *o = registry.createMethod(name, version, this); \
    QVERIFY(o); \
    o->deleteLater()

#define TEST_CREATE_END() \
    display->deleteLater(); \
    thread->quit(); \
    thread->wait(); \
    delete thread

    void testCompositor()
    {
        TEST_CREATE_BEGIN();

        TEST_CREATE_BODY(SIGNAL(compositorAnnounced(quint32,quint32)), createCompositor);

        WaylandClient::Surface *surface = o->createSurface(this);
        QVERIFY(surface);
        WaylandClient::Region *region = o->createRegion(this);
        QVERIFY(region);

        TEST_CREATE_END();

        delete surface;
        delete region;
    }

    /*
    void testFullScreenShell()
    {
        TEST_CREATE_BEGIN();
        TEST_CREATE_BODY(SIGNAL(fullScreenShellAnnounced(quint32,quint32)), createFullScreenShell);
        TEST_CREATE_END();
    }
    */

    void testOutput()
    {
        TEST_CREATE_BEGIN();
        TEST_CREATE_BODY(SIGNAL(outputAnnounced(quint32,quint32)), createOutput);
        TEST_CREATE_END();
    }

    void testSeat()
    {
        TEST_CREATE_BEGIN();

        TEST_CREATE_BODY(SIGNAL(seatAnnounced(quint32,quint32)), createSeat);

        QSignalSpy keyboardSpy(o, SIGNAL(keyboardAdded()));
        QVERIFY(keyboardSpy.isValid());
        QCOMPARE(keyboardSpy.count(), 0);

        QSignalSpy pointerSpy(o, SIGNAL(pointerAdded()));
        QVERIFY(pointerSpy.isValid());
        QCOMPARE(pointerSpy.count(), 0);

        QSignalSpy touchSpy(o, SIGNAL(touchAdded()));
        QVERIFY(touchSpy.isValid());
        QCOMPARE(touchSpy.count(), 0);

        new QWaylandSeat(m_compositor);

        display->flushRequests();
        QVERIFY(keyboardSpy.wait());
        //QVERIFY(pointerSpy.wait());
        //QVERIFY(touchSpy.wait());
        display->flushRequests();
        QCOMPARE(keyboardSpy.count(), 1);
        QCOMPARE(pointerSpy.count(), 1);
        QCOMPARE(touchSpy.count(), 1);

        TEST_CREATE_END();
    }

    void testShm()
    {
        TEST_CREATE_BEGIN();

        TEST_CREATE_BODY(SIGNAL(shmAnnounced(quint32,quint32)), createShm);

        WaylandClient::ShmPool *pool = o->createPool(1024);
        QVERIFY(pool);
        WaylandClient::BufferPtr buffer = pool->createBuffer(QSize(10, 10), 10 * 4);
        QVERIFY(!buffer.isNull());
        WaylandClient::BufferPtr buffer2 = pool->createBuffer(QSize(10, 10), 10 * 4, buffer.data()->address());
        QVERIFY(!buffer2.isNull());

        TEST_CREATE_END();

        delete pool;
    }

    void testScreencaster()
    {
        TEST_CREATE_BEGIN();

        WaylandClient::Registry registry;
        registry.create(display->display());

        QSignalSpy shmAnnounced(&registry, SIGNAL(shmAnnounced(quint32,quint32)));
        QSignalSpy announced(&registry, SIGNAL(screencasterAnnounced(quint32,quint32)));
        QVERIFY(shmAnnounced.isValid());
        QVERIFY(announced.isValid());

        registry.setup();
        display->flushRequests();

        QVERIFY(shmAnnounced.wait());
        //QVERIFY(announced.wait());

        const quint32 shmName = shmAnnounced.first().first().value<quint32>();
        const quint32 shmVersion = shmAnnounced.first().last().value<quint32>();
        WaylandClient::Shm *shm = registry.createShm(shmName, shmVersion, this);
        QVERIFY(shm);

        const quint32 name = announced.first().first().value<quint32>();
        const quint32 version = announced.first().last().value<quint32>();
        WaylandClient::Screencaster *screencaster = registry.createScreencaster(shm, name, version, this);
        QVERIFY(screencaster);

        delete screencaster;
        delete shm;

        TEST_CREATE_END();
    }

    void testScreenshooter()
    {
        TEST_CREATE_BEGIN();

        WaylandClient::Registry registry;
        registry.create(display->display());

        QSignalSpy shmAnnounced(&registry, SIGNAL(shmAnnounced(quint32,quint32)));
        QSignalSpy announced(&registry, SIGNAL(screenshooterAnnounced(quint32,quint32)));
        QVERIFY(shmAnnounced.isValid());
        QVERIFY(announced.isValid());

        registry.setup();
        display->flushRequests();

        QVERIFY(shmAnnounced.wait());
        //QVERIFY(announced.wait());

        const quint32 shmName = shmAnnounced.first().first().value<quint32>();
        const quint32 shmVersion = shmAnnounced.first().last().value<quint32>();
        WaylandClient::Shm *shm = registry.createShm(shmName, shmVersion, this);
        QVERIFY(shm);

        const quint32 name = announced.first().first().value<quint32>();
        const quint32 version = announced.first().last().value<quint32>();
        WaylandClient::Screenshooter *screenshooter = registry.createScreenshooter(shm, name, version, this);
        QVERIFY(screenshooter);

        delete screenshooter;
        delete shm;

        TEST_CREATE_END();
    }
};

QTEST_MAIN(TestRegistry)

#include "tst_registry.moc"
