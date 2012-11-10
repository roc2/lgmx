#ifndef C_LABEL_H
#define C_LABEL_H

#include <QLabel>

/**
 * Clickable label.
 */

class c_label : public QLabel
{
	Q_OBJECT

public:
	c_label(const QString &text, QWidget *parent = NULL);
	~c_label();
 
signals:
	void clicked();
 
public slots:
	void slotClicked();
 
protected:
	void mousePressEvent(QMouseEvent *event);
};

#endif

