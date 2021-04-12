#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QFileSystemModel>
//#include "TreeViewModel.h"
#include <QtCore/QSortFilterProxyModel>
#include <QQmlContext>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardItem>
#include <QDebug>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include "TreeViewModel.h"

class SortFilterProxyModel : public QSortFilterProxyModel
{
public:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        // If sorting by file names column
        if (sortColumn() == 0) {
            QFileSystemModel *fsm = qobject_cast<QFileSystemModel*>(sourceModel());
            bool asc = sortOrder() == Qt::AscendingOrder ? true : false;

            QFileInfo leftFileInfo  = fsm->fileInfo(left);
            QFileInfo rightFileInfo = fsm->fileInfo(right);


            // If DotAndDot move in the beginning
            if (sourceModel()->data(left).toString() == "..")
                return asc;
            if (sourceModel()->data(right).toString() == "..")
                return !asc;

            // Move dirs upper
            if (!leftFileInfo.isDir() && rightFileInfo.isDir()) {
                return !asc;
            }
            if (leftFileInfo.isDir() && !rightFileInfo.isDir()) {
                return asc;
            }
        }

        return QSortFilterProxyModel::lessThan(left, right);
    }
};

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QFileSystemModel fileSystemModel;
    fileSystemModel.setRootPath(QDir::currentPath());
    TreeViewModel fileSystemTreeViewModel;
    SortFilterProxyModel sortFilterProxyModel;
    sortFilterProxyModel.setDynamicSortFilter(true);
    sortFilterProxyModel.sort(0);
    sortFilterProxyModel.setSourceModel(&fileSystemModel);

    fileSystemTreeViewModel.setSourceModel(&sortFilterProxyModel);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("fileSystemModel", &fileSystemTreeViewModel);
    engine.load(url);

    return app.exec();
}
