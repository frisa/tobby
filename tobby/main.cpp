#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardItemModel>
#include "treeviewmodel.h"

#define FS_MODEL

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    /* DATA MODEL */
    QStandardItemModel dataModel;
    QStandardItem root("Root");
    //root.setIcon(qtApp.style()->standardIcon(QStyle::SP_DesktopIcon));
    QStandardItem child1("Child 1");
    QStandardItem child2("Child 2");
    QStandardItem child3("Child 3");
    QStandardItem child1OfChild1("Child 1 of Child 1");
    QStandardItem child2OfChild1("Child 2 of Child 1");
    QStandardItem child1OfChild2("Child 1 of Child 2");
    child1.appendRow(&child1OfChild1);
    child1.appendRow(&child2OfChild1);
    child2.appendRow(&child1OfChild2);
    root.appendRow(&child1);
    root.appendRow(&child2);
    root.appendRow(&child3);
    dataModel.appendRow(&root);
    TreeViewModel standardItemTreeViewModel;
    standardItemTreeViewModel.setSourceModel(&dataModel);

    engine.rootContext()->setContextProperty("dataModel", &standardItemTreeViewModel);
    engine.load(url);

    return app.exec();
}
