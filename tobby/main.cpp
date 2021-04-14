#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardItemModel>
#include "treeviewmodel.h"
#include "systemmodel.h"

#define FS_MODEL

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QString strMainQML = QStringLiteral("qrc:/main.qml");
    const QUrl url(strMainQML);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    /* DATA MODEL FOR TREEVIEW */
    QStandardItemModel dataModel;
    QStandardItem root("Root");
    root.setIcon(QIcon("qrc:/icons/sockchrist.png"));
    QStandardItem child1("A1");
    QStandardItem child2("A2");
    QStandardItem child3("A3");
    QStandardItem child1OfChild1("B1");
    QStandardItem child2OfChild1("B1");
    QStandardItem child1OfChild2("B2");
    child1.appendRow(&child1OfChild1);
    child1.appendRow(&child2OfChild1);
    child2.appendRow(&child1OfChild2);
    root.appendRow(&child1);
    root.appendRow(&child2);
    root.appendRow(&child3);
    dataModel.appendRow(&root);
    TreeViewModel standardItemTreeViewModel;
    standardItemTreeViewModel.setSourceModel(&dataModel);

    /* DATA MODEL FOR SYSTEM */
    SystemModel sm;
    engine.rootContext()->setContextProperty("systemModel", &sm);
    engine.rootContext()->setContextProperty("dataModel", &standardItemTreeViewModel);
    engine.load(url);
    return app.exec();
}
