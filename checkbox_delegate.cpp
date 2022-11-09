#include "checkbox_delegate.h"
#include <QCheckBox>
#include <QStylePainter>
#include <QPalette>
#include <QAbstractItemView>
#include <QDebug>

CChecBox_Delegate::CChecBox_Delegate( QObject *parent ) : QItemDelegate(parent)
{
    //m_sIconPath = "";
   // m_sText = "";
}

QWidget * CChecBox_Delegate::createEditor( QWidget * parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex&  index ) const
{
    QCheckBox * pEditor = new QCheckBox( parent );
    //pEditor->setText( m_sText );
    pEditor->setCheckable( true );
   // pEditor->setIcon( QIcon( m_sIconPath ) );

    pEditor -> installEventFilter( const_cast<CChecBox_Delegate*>( this ) );
    return pEditor;
}

void CChecBox_Delegate::setEditorData( QWidget * editor, const QModelIndex &index) const
{
    int value = index.model()->data( index, Qt::EditRole ).toInt();
    QCheckBox * cb = static_cast<QCheckBox*>( editor );

    cb->setChecked( value );
}

void CChecBox_Delegate::setModelData( QWidget * editor,
                                       QAbstractItemModel *model,
                                       const QModelIndex& index) const
{
    QCheckBox * cb = static_cast<QCheckBox*>( editor );
    model->setData( index, cb->isChecked() );
}

void CChecBox_Delegate::updateEditorGeometry( QWidget *editor,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex& index ) const
{
    editor->setGeometry( option.rect );
}

void CChecBox_Delegate::paint( QPainter * painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index ) const
{
    QWidget * w = dynamic_cast<QWidget *>( painter->device() );
    if ( w )
    {
        QStylePainter p( w );
        QStyleOptionButton opt;
        //opt.text = m_sText;
        //opt.icon = QIcon( m_sIconPath );
        //opt.iconSize = QSize( 24, 24 );
        opt.initFrom( w );
        opt.rect = option.rect;

        bool bChecked = index.data().toInt() > 0;
        if ( bChecked )
            opt.state |= QStyle::State_On;
        p.drawControl( QStyle::CE_CheckBox, opt );
    }
    QItemDelegate::paint(painter,option,index);

    //drawFocus( painter, option, option.rect );
}
