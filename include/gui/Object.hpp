#ifndef GUI_OBJECT_HPP
#define GUI_OBJECT_HPP

#include <lib/NormalCalculation.hpp>
#include <lib/Object.hpp>

#include <memory>

#include <QUrl>
#include <Qt3DCore/QComponent>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>

namespace gui {

class Object : public Qt3DCore::QComponent
{
  Q_OBJECT
  Q_PROPERTY(int nbVertices READ nbVertices NOTIFY objectChanged)
  Q_PROPERTY(int nbFaces READ nbFaces NOTIFY objectChanged)

public:
  typedef lib::NormalCalculation::Method CalculNormalMethod;
  Q_ENUM(CalculNormalMethod)

  Object(QNode* parent = nullptr);
  virtual ~Object();

  inline size_t nbVertices() const { return _object.vertices().size(); }
  inline size_t nbFaces() const { return _object.faces().size(); }

public slots:
  void calculateNormals(const CalculNormalMethod m);

  void readOFF(const QUrl& fileName);
  void writeOFF(const QUrl& fileName) const;
  void writeOBJ(const QUrl& fileName) const;

signals:
  void objectChanged();

private:
  lib::Object _object;
  std::unique_ptr<lib::NormalCalculation> _normalCalculation;
};

} // namespace gui

#endif
