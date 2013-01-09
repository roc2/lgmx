#include <cli_iface.h>
#include <iostream>
#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <debug.h>
#include <exception.h>
#include <view_manager.h>

//joanin


/**
 * Constructor.
 */

cli_iface::cli_iface(view_manager *parent) : QWidget(parent)
{
	cli_ = new lgmx::cli(parent);
	
	cmd_history_ = new std::list<QString>();
	curr_cmd_ = cmd_history_->begin();
	
	result_ = new QLabel("teste", this);
	
	result_->hide();
	input_ = new QLineEdit(this);
	
	input_->installEventFilter(this);
	
	layout_ = new QHBoxLayout(this);
	layout_->addWidget(input_);
	layout_->addWidget(result_);
	result_->setMaximumHeight (20);
	layout_->setContentsMargins(0, 0, 0, 0);
	layout_->setSpacing(0);
	
	
	this->setContentsMargins (0, 0, 0, 0);
	setLayout(layout_);
	this->setMaximumHeight (22);
	
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
{/*
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
	}*/

	return QWidget::event(event);
}

bool cli_iface::eventFilter(QObject *object, QEvent *event)
{
	if (object == input_ && event->type() == QEvent::KeyPress) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);

		QString cmd(input_->text());
		QString error;
		QString result;

		switch (ke->key()) {
		case Qt::Key_Return:
		case Qt::Key_Enter:
			
			if (cmd.isEmpty())
				return true;
			
			std::cout << "command issued: " << cmd.toStdString() << std::endl;
			// handle command
			if (cli_->execute(cmd, result, error)) {
				if (result.isEmpty())
					result_->hide();
				else {
					result_->setText("   " + result + "   " );
					result_->setStyleSheet("background-color: rgb(0, 0, 230);");
					result_->show();
				}
			} else {
				result_->setText("   " + error + "   ");
				result_->setStyleSheet("background-color: rgb(200, 0, 0);");
				result_->show();
			}
			
			// add to history
			cmd_history_->push_back(cmd);
			curr_cmd_ = cmd_history_->end();
			input_->clear();
			return true;
			
		case Qt::Key_Up:
			if (curr_cmd_ != cmd_history_->begin()) {
				curr_cmd_--;
				input_->setText(*curr_cmd_);
			}
			return true;
		
		case Qt::Key_Down:
			if (curr_cmd_ == cmd_history_->end() || ++curr_cmd_ == cmd_history_->end())
				input_->clear();
			else
				input_->setText(*curr_cmd_);

			return true;
		
		case Qt::Key_Escape:
			this->hide();
			return true;
		
		default:
			break;
		}
	}
	return false;
}


