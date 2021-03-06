#include <lib/SequentialNormalCalculation.hpp>
#include <tests/TestNormalCalculation.hpp>

#include <QDebug>
#include <fstream>
#include <limits>
#include <memory>
#include <string>

using namespace tests;
using namespace std::string_literals;

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(lib::NormalCalculation::Method)

TestNormalCalculation::TestNormalCalculation(QObject* parent)
  : QObject(parent)
{
}

void
TestNormalCalculation::calculate_data()
{
  QTest::addColumn<std::string>("fileName");
  QTest::addColumn<lib::NormalCalculation::Method>("method");

  QTest::newRow("bouddha1m.off/OpenMP")
    << "bouddha1m.off"s << lib::NormalCalculation::Method::OpenMP;
  QTest::newRow("bouddha1m.off/pThread")
    << "bouddha1m.off"s << lib::NormalCalculation::Method::pThread;
}

void
TestNormalCalculation::calculate()
{
  QFETCH(std::string, fileName);
  QFETCH(lib::NormalCalculation::Method, method);

  int precision = std::numeric_limits<long double>::max_digits10;

  std::ifstream file(fileName);
  file.precision(precision);
  lib::Object object = lib::Object::readOFF(file);

  lib::SequentialNormalCalculation seq(object);
  std::unique_ptr<lib::NormalCalculation> nc(
    lib::NormalCalculation::factory(method, object));

  seq.calculate();
  nc->calculate();

  QCOMPARE(seq.faceNormal().size(), nc->faceNormal().size());
  QCOMPARE(seq.vertexNormal().size(), nc->vertexNormal().size());

  for (size_t i = 0; i < seq.faceNormal().size(); ++i) {
    QCOMPARE(1.0 + seq.faceNormal()[i].x, 1.0 + nc->faceNormal()[i].x);
    QCOMPARE(1.0 + seq.faceNormal()[i].y, 1.0 + nc->faceNormal()[i].y);
    QCOMPARE(1.0 + seq.faceNormal()[i].z, 1.0 + nc->faceNormal()[i].z);
  }

  for (size_t i = 0; i < seq.vertexNormal().size(); ++i) {
    QCOMPARE(1.0 + seq.vertexNormal()[i].x, 1.0 + nc->vertexNormal()[i].x);
    QCOMPARE(1.0 + seq.vertexNormal()[i].y, 1.0 + nc->vertexNormal()[i].y);
    QCOMPARE(1.0 + seq.vertexNormal()[i].z, 1.0 + nc->vertexNormal()[i].z);
  }
}

QTEST_MAIN(TestNormalCalculation)
//#include "testqstring.moc"
