#include <QApplication>
#include <QQmlApplicationEngine>

#include <gui/Object.hpp>

int
main(int argc, char* argv[])
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;

  qmlRegisterType<gui::Object>("lib", 1, 0, "Object");

  engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

  return app.exec();
}
