#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QtWidgets>
#include <QPainter>


class ImageDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QVariant value = index.data(Qt::DecorationRole);
        if (value.isValid() && value.canConvert<QPixmap>())
        {
            QPixmap pixmap;
            pixmap.loadFromData(value.toByteArray());
            painter->drawPixmap(option.rect, pixmap.scaled(option.rect.size(), Qt::KeepAspectRatio));
        } else
        {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};
#endif // IMAGEDELEGATE_H
