#ifndef TESTS_TESTNORMALCALCULATION_HPP
#define TESTS_TESTNORMALCALCULATION_HPP

#include <lib/NormalCalculation.hpp>
#include <lib/Object.hpp>

#include <QtTest/QtTest>

namespace tests {

class TestNormalCalculation : public QObject
{
  Q_OBJECT

public:

    TestNormalCalculation(QObject * parent = nullptr);

private slots:
    void calculate_data();
    void calculate();
};

} // namespace tests

#endif
