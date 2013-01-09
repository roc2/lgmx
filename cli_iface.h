#ifndef CLI_IFACE_H
#define CLI_IFACE_H

#include <QLineEdit>
#include <list>
#include <cli.h>

class QEvent;
class QLabel;
class QHBoxLayout;
class view_manager;
//class lgmx::cli;

class cli_iface : public QWidget
{
	Q_OBJECT
	
public:
	cli_iface(view_manager* parent);
	~cli_iface();
	
	

private:
	bool event(QEvent *event);
	bool eventFilter(QObject *object, QEvent *event);

private:
	lgmx::cli *cli_;

	QLabel *result_;
	QLineEdit *input_;
	QHBoxLayout *layout_;
	std::list<QString>::iterator curr_cmd_;
	std::list<QString> *cmd_history_;
};



#endif
