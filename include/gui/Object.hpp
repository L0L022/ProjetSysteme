#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <QObject>

namespace gui {

class Object : public QObject
{
public:
  Object(QObject *parent = nullptr);
  virtual ~Object() = default;

private:
};

}

#endif
