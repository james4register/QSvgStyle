/***************************************************************************
 *   Copyright (C) 2017 by Saïd LANKRI                                     *
 *   said.lankri@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SVGGEN_H
#define SVGGEN_H

#include <QSizeF>
#include <QPainterPath>
#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QSize>
#include <QVector>
#include <QDomElement>
#include <QString>
#include <QTimer>
#include <QColor>
#include <QBrush>
#include <QPen>

class QGraphicsScene;

class SvgGenInterior : public QGraphicsPathItem {
  public:
    enum FillType {
      FillTypeFirst,

      FillTypeFlat = FillTypeFirst,
      FillTypeGradient,
      FillTypeInvertedGradient,

      FillTypeMax = FillTypeInvertedGradient,
    };

    SvgGenInterior(QGraphicsItem *parent = Q_NULLPTR);
    ~SvgGenInterior();

    void setRoundInterior(bool hasIt);
    void setInteriorRoundness(qreal val);
    void setFillType(FillType type);
    void setFirstColor(const QColor &c);
    void setSecondColor(const QColor &c);
    void setSize(const QSizeF &sz);

    qreal interiorRoundness() const { return roundness; }
    FillType fillType() const { return fill; }
    QColor firstColor() const { return color1; }
    QColor secondColor() const { return color2; }

    QDomDocumentFragment toSvg(QDomDocument &doc);

  private:
    void calcInterior();
    void calcFill();

    qreal width, height, roundness;
    bool roundInterior;
    QColor color1, color2;
    FillType fill;
};

/**
 * @brief QGraphicsPathItem that represents a rounded subframe
 *
 * This class draws a subframe that has a specific width/height and corner
 * roundness.
 * A subframe is a union of its 8 elements
 * The size set with the @ref setSize() method, the position set by setX(), setY()
 * and the like functions, as well as the corner roundness set by @ref setCornerRoundness()
 * methods is applicable to the inner subframe. The outer subframe will
 * be computed by adding to it a margin of @ref subFrameWidth()
 * When the corner roundness is set to 0, the inner subframe will be a rectangle
 * The outer subframe roundness is @ref outerCornerRoundess()
 * and can be used as an initial roundness for the subsequent (above) subframes.
 */
class SvgGenSubFrame : public QObject, public QGraphicsItemGroup {
  Q_OBJECT

  public:
    enum FillType {
      FillTypeFirst,

      FillTypeFlat = FillTypeFirst,
      FillTypeGradient,
      FillTypeInvertedGradient,

      FillTypeMax = FillTypeInvertedGradient,
    };

    SvgGenSubFrame();
    SvgGenSubFrame(const SvgGenSubFrame *prev);
    SvgGenSubFrame(const QSizeF &sz, qreal sbwidth, qreal roundness = 0, bool round = true, bool split = true);
    virtual ~SvgGenSubFrame();

    void setSubFrameWidth(qreal val);
    void setCornerRoundness(qreal val);
    void setRoundCorners(bool val);
    void setSplitMode(bool val);
    void setSize(const QSizeF &sz);
    void setFillType(FillType type);
    void setFirstColor(const QColor &c);
    void setSecondColor(const QColor &c);
    void setHasTop(bool hasIt);
    void setHasBottom(bool hasIt);
    void setHasLeft(bool hasIt);
    void setHasRight(bool hasIt);

    /* corner roundness is for the inner subframe */
    qreal cornerRoundess() const { return roundness; }
    bool hasRoundCorners() const { return round; }
    qreal subFrameWidth() const { return sbwidth; }
    bool isSplit() const { return split; }
    qreal outerCornerRoundness() const { return roundness+sbwidth; }
    QColor firstColor() const { return color1; }
    QColor secondColor() const { return color2; }
    FillType fillType() const { return fill; }
    bool hasTop() const { return hastop; }
    bool hasBottom() const { return hasbottom; }
    bool hasLeft() const { return hasleft; }
    bool hasRight() const { return hasright; }
    qreal width() const { return m_width; }
    qreal height() const { return m_height; }

    virtual QRectF boundingRect() const;
    QDomDocumentFragment toSvg(QDomDocument doc, const QString &part);

  private:
    void calcSubFrame();
    void calcFill();

    qreal m_width, m_height, roundness, sbwidth;
    bool round, split;
    QGraphicsPathItem *top,*bottom,*left,*right;
    QGraphicsPathItem *topleft,*topright,*bottomleft,*bottomright;
    bool hastop,hasbottom,hasleft,hasright;
    QColor color1, color2;
    FillType fill;
};

class SvgGen : public QObject {
  Q_OBJECT

  public:
    /** Constructor. Takes ownership of the scene */
    SvgGen(QGraphicsScene *scene, QObject *parent = Q_NULLPTR);
    ~SvgGen();

    void setHasInterior(bool hasIt);
    void setHasFrame(bool hasIt);
    void setHasShadow(bool hasIt);
    void setRoundMode(bool hasIt);
    void setSplitMode(bool hasIt);
    void setHasTopFrame(bool hasIt);
    void setHasBottomFrame(bool hasIt);
    void setHasLeftFrame(bool hasIt);
    void setHasRightFrame(bool hasIt);
    void setHasTopShadow(bool hasIt);
    void setHasBottomShadow(bool hasIt);
    void setHasLeftShadow(bool hasIt);
    void setHasRightShadow(bool hasIt);

    bool hasFrame() const { return m_hasFrame; }
    bool hasInterior() const { return m_hasInterior; }
    bool hasShadow() const { return m_hasShadow; }
    bool roundMode() const { return m_roundMode; }
    bool splitMode() const { return m_splitMode; }
    bool isSquare() const { return width == height; }
    bool hasTopFrame() const { return m_hasTopFrame; }
    bool hasBottomFrame() const { return m_hasBottomFrame; }
    bool hasLeftFrame() const { return m_hasLeftFrame; }
    bool hasRightFrame() const { return m_hasRightFrame; }
    bool hasTopShadow() const { return m_hasTopShadow; }
    bool hasBottomShadow() const { return m_hasBottomShadow; }
    bool hasLeftShadow() const { return m_hasLeftShadow; }
    bool hasRightShadow() const { return m_hasRightShadow; }

    void setCenter(const QPointF &p);

    /* The size given to this method is the interior size */
    void setSize(const QSizeF &sz);

    void setFrameWidth(int width);
    void setSubFrameWidth(int idx, qreal width);
    /* roundness of iterior when object does not have a frame */
    void setInteriorRoundness(qreal val);
    void setSubFrameFillType(int idx, SvgGenSubFrame::FillType type);
    void setSubFrameFirstColor(int idx, const QColor &c);
    void setSubFrameSecondColor(int idx, const QColor &c);
    void setInteriorFillType(SvgGenInterior::FillType type);
    void setInteriorFirstColor(const QColor &c);
    void setInteriorSecondColor(const QColor &c);
    void setShadowWidth(qreal val);
    void setShadowFirstColor(const QColor &c);
    void setShadowSecondColor(const QColor &c);
    void setShadowFillType(SvgGenSubFrame::FillType type);

    void setBaseName(const QString &val) { m_basename = val; }
    void setVariant(const QString &val) { m_variant = val; }
    void setStatus(const QString &val) { m_status = val; }

    QString basename() const { return m_basename; }
    QString variant() const { return m_variant; }
    QString status() const { return m_status; }

    qreal interiorRoundness() const {
      if ( interior )
        return interior->interiorRoundness();
      else
        return 0.0;
    }

    qreal subFrameWidth(int n) const {
      if ( subFrames.count()-1 >= n )
        return subFrames[n]->subFrameWidth();
      else
        return -1;
    }
    int frameWidth() const { return framewidth; }
    QColor subFrameFirstColor(int n) const {
      if ( subFrames.count()-1 >= n )
        return subFrames[n]->firstColor();
      else
        return QColor();
    }
    QColor subFrameSecondColor(int n) const {
      if ( subFrames.count()-1 >= n )
        return subFrames[n]->secondColor();
      else
        return QColor();
    }
    SvgGenSubFrame::FillType subFrameFillType(int n) const {
      if ( subFrames.count()-1 >= n )
        return subFrames[n]->fillType();
      else
        return SvgGenSubFrame::FillTypeFirst;
    }
    SvgGenInterior::FillType interiorFillType() const {
      if ( interior )
        return interior->fillType();
      else
        return SvgGenInterior::FillTypeFirst;
    }
    QColor interiorFirstColor() const {
      if ( interior )
        return interior->firstColor();
      else
        return QColor();
    }
    QColor interiorSecondColor() const {
      if ( interior )
        return interior->secondColor();
      else
        return QColor();
    }
    qreal shadowWidth() const { return shadowwidth; }
    QColor shadowFirstColor() const {
      if ( shadow )
        return shadow->firstColor();
      else
        return QColor();
    }
    QColor shadowSecondColor() const {
      if ( shadow )
        return shadow->secondColor();
      else
        return QColor();
    }
    SvgGenSubFrame::FillType shadowFillType() const {
      if ( shadow )
        return shadow->fillType();
      else
        return SvgGenSubFrame::FillTypeFirst;
    }

    QDomDocument toSvg();

  private:
    void pushSubFrame();
    void popSubFrame();
    void rebuildShadow();

    qreal width,height,shadowwidth;
    int framewidth;
    bool m_hasShadow, m_hasInterior, m_hasFrame, m_roundMode, m_splitMode;
    bool m_hasTopFrame, m_hasBottomFrame, m_hasLeftFrame, m_hasRightFrame;
    bool m_hasTopShadow, m_hasBottomShadow, m_hasLeftShadow, m_hasRightShadow;
    QString m_basename, m_variant, m_status;
    QPointF center;
    QGraphicsScene *scene;
    QVector<SvgGenSubFrame *> subFrames;
    SvgGenInterior *interior;
    // shadows are like subframes, but with special treatment for colors
    SvgGenSubFrame *shadow;
    QGraphicsPathItem *cross;
};

#endif
