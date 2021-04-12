#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QQmlContext>
#include "treeviewmodel.h"

#define FS_MODEL

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QAbstractItemModel* dataModel;
#ifdef FS_MODEL
    QFileSystemModel fileSystemModel;
    fileSystemModel.setRootPath(QDir::currentPath());
    TreeViewModel fileSystemTreeViewModel;
    SortFilterProxyModel sortFilterProxyModel;
    sortFilterProxyModel.setDynamicSortFilter(true);
    sortFilterProxyModel.sort(0);
    sortFilterProxyModel.setSourceModel(&fileSystemModel);
    fileSystemTreeViewModel.setSourceModel(&sortFilterProxyModel);
#elif

#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    //engine.rootContext()->setContextProperty("fileSystemModel", &fileSystemTreeViewModel);
    engine.load(url);

    return app.exec();
}
