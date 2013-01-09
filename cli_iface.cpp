#include <cli_iface.h>
#include <iostream>
#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <QLabel>
#include <debug.h>
#include <exception.h>
#include <view_manager.h>

//joanin


/**
 * Constructor.
 */

cli_iface::cli_iface(view_manager *parent) : QLineEdit(parent)
{
	cli_ = new lgmx::cli(parent);
	
	cmd_history_ = new std::list<QString>();
	curr_cmd_ = cmd_history_->begin();
	
	result_ = new QLabel(this);
}

/**
 * Destructor.
 */

cli_iface::~cli_iface()
{
	delete cmd_history_;
}

/**
 * CLI interface event handler. Handles special keys such as 
 * Enter, to issue a command, and Up and Down keys to repeat 
 * commands.
 */

bool cli_iface::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);

		QString cmd(text());

		switch (ke->key()) {
		case Qt::Key_Return:
		case Qt::Key_Enter:
			
			if (cmd.isEmpty())
				return true;
			
			std::cout << "command issued: " << cmd.toStdString() << std::endl;
			// handle command
			try {
				cli_->execute(cmd);
			} catch (lgmx::exception &excp) {
				debug(ERR, VIEW, excp.get_message());
			}
			
			// add to history
			cmd_history_->push_back(cmd);
			curr_cmd_ = cmd_history_->end();
			clear();
			return true;
			
		case Qt::Key_Up:
			if (curr_cmd_ != cmd_history_->begin()) {
				curr_cmd_--;
				setText(*curr_cmd_);
			}
			return true;
		
		case Qt::Key_Down:
			if (curr_cmd_ == cmd_history_->end() || ++curr_cmd_ == cmd_history_->end())
				clear();
			else
				setText(*curr_cmd_);

			return true;
		
		case Qt::Key_Escape:
			this->hide();
			return true;
		
		default:
			break;
		}
	}

	return QWidget::event(event);
}




